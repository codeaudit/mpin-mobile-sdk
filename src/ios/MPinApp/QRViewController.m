/*
 Copyright (c) 2012-2015, Certivox
 All rights reserved.
 
 Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 
 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 
 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 
 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 
 For full details regarding our CertiVox terms of service please refer to
 the following links:
 * Our Terms and Conditions -
 http://www.certivox.com/about-certivox/terms-and-conditions/
 * Our Security and Privacy -
 http://www.certivox.com/about-certivox/security-privacy/
 * Our Statement of Position and Our Promise on Software Patents -
 http://www.certivox.com/about-certivox/patents/
 */


#import "QRViewController.h"
#import "MFSideMenu.h"
#import "NSString+Helper.h"
#import "Constants.h"
#import "ConfigurationManager.h"
#import "Utilities.h"
#import "QREditorViewController.h"
#import <AudioToolbox/AudioToolbox.h>

static NSInteger constIntTimeoutInterval = 30;

@interface QRViewController ( )
{
    
}
@property SystemSoundID soundID;
@property( nonatomic ) BOOL isReading;
@property ( nonatomic, strong ) AVCaptureSession *captureSession;
@property ( nonatomic, strong ) AVCaptureVideoPreviewLayer *videoPreviewLayer;
@property ( nonatomic, weak ) IBOutlet UIImageView *imgViewRectangle;
@property ( nonatomic, weak ) IBOutlet UILabel *lblMessage;
@property ( nonatomic, weak ) IBOutlet UIActivityIndicatorView *activityIndicator;

-( BOOL ) startReading;
-( void ) stopReading;
- ( void ) loadConfigurations:( NSString * ) url;
@end

@implementation QRViewController

- ( void )viewDidLoad
{
    [super viewDidLoad];
    _isReading = NO;
    _captureSession = nil;
    NSURL *url = [NSURL URLWithString:@"/System/Library/Audio/UISounds/sms-received2.caf"];
    CFURLRef u = (__bridge_retained CFURLRef)(url);
    AudioServicesCreateSystemSoundID(u,&_soundID );
    CFRelease(u);
    
}

- (void) viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    
}

- (void) dealloc
{
    AudioServicesDisposeSystemSoundID ( _soundID );
}

- ( void )viewWillAppear:( BOOL )animated
{
    [super viewWillAppear:animated];
    
    _imgViewRectangle.hidden = YES;
    _lblMessage.hidden = YES;
    _lblMessage.text = NSLocalizedString(@"QR_MESSAGE", @"Place  the QR code in the centre of the screen. It will be scanned automatically.");
}

- ( void ) viewDidAppear:( BOOL )animated
{
    [super viewDidAppear:animated];

    if ( !( _isReading = [self startReading] ) )
    {
        [[ErrorHandler sharedManager] presentMessageInViewController:self
         errorString:NSLocalizedString(@"ERROR_UNABLE_TO_SCAN_QR", @"Unable to scan QR code!")
         addActivityIndicator:NO
         minShowTime:3];
    }
}

-( BOOL ) startReading
{
    AVCaptureDevice *captureDevice = [AVCaptureDevice defaultDeviceWithMediaType:AVMediaTypeVideo];
    if ( captureDevice == nil )
    {
        return NO;
    }

    dispatch_async(dispatch_get_main_queue(), ^ (void) {
        NSError *error;



        AVCaptureDeviceInput *input = [AVCaptureDeviceInput deviceInputWithDevice:captureDevice error:&error];
        if ( !input )
        {
            [[ErrorHandler sharedManager] presentMessageInViewController:self
             errorString:[error localizedDescription]
             addActivityIndicator:NO
             minShowTime:3];
        }

        _captureSession = [[AVCaptureSession alloc] init];
        [_captureSession addInput:input];

        AVCaptureMetadataOutput *captureMetadataOutput = [[AVCaptureMetadataOutput alloc] init];
        [_captureSession addOutput:captureMetadataOutput];

        dispatch_queue_t dispatchQueue;
        dispatchQueue = dispatch_queue_create("myQueue", NULL);
        [captureMetadataOutput setMetadataObjectsDelegate:self queue:dispatchQueue];
        [captureMetadataOutput setMetadataObjectTypes:[NSArray arrayWithObject:AVMetadataObjectTypeQRCode]];

        _videoPreviewLayer = [[AVCaptureVideoPreviewLayer alloc] initWithSession:_captureSession];
        [_videoPreviewLayer setVideoGravity:AVLayerVideoGravityResizeAspectFill];
        [_videoPreviewLayer setFrame:_viewPreview.layer.bounds];
        [_viewPreview.layer insertSublayer:_videoPreviewLayer atIndex:0];
        [_captureSession startRunning];
        _imgViewRectangle.hidden = NO;
        _lblMessage.hidden = NO;
    });



    return YES;
}

-( void ) stopReading
{
    dispatch_async(dispatch_get_main_queue(), ^ (void) {
        _imgViewRectangle.hidden = YES;
        _lblMessage.hidden = YES;
        [_captureSession stopRunning];
        _captureSession = nil;

        [_videoPreviewLayer removeFromSuperlayer];
        _isReading = NO;
    });
}

- ( void ) loadConfigurations:( NSString * ) url
{
    if ( ![NSString isValidURL:url] )
    {
        dispatch_async(dispatch_get_main_queue(), ^ (void) {
            [[ErrorHandler sharedManager] presentMessageInViewController:self
             errorString:NSLocalizedString(@"ERROR_INVALID_URL", @"Invalid URL!")
             addActivityIndicator:NO
             minShowTime:3];
            double delayInSeconds = 3.0;
            dispatch_time_t popTime = dispatch_time(DISPATCH_TIME_NOW, delayInSeconds * NSEC_PER_SEC);
            dispatch_after(popTime, dispatch_get_main_queue(), ^ (void){
                [self performSelectorOnMainThread:@selector( startReading ) withObject:nil waitUntilDone:NO];
            });
        });

        return;
    }

    [[ErrorHandler sharedManager] presentMessageInViewController:self
     errorString:NSLocalizedString(@"LOADING", @"Loading URL")
     addActivityIndicator:YES
     minShowTime:0];

    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^ {
        NSURL *theUrl = [NSURL URLWithString:url];
        NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:theUrl cachePolicy:NSURLRequestReloadIgnoringCacheData timeoutInterval:constIntTimeoutInterval];
        [request setTimeoutInterval:constIntTimeoutInterval];
        request.HTTPMethod = @"GET";

        NSHTTPURLResponse *response = nil;
        NSError *error = nil;
        NSData *ConfigJSONdata = [NSURLConnection sendSynchronousRequest:request returningResponse:&response error:&error];

        // parse json data
        if ( ConfigJSONdata != nil )
        {
            NSArray *configs = [NSJSONSerialization JSONObjectWithData:ConfigJSONdata options:kNilOptions error:&error];
            dispatch_async(dispatch_get_main_queue(), ^ (void) {
                UIStoryboard *storyboard = [UIStoryboard storyboardWithName:@"Main_iPhone" bundle:nil];
                QREditorViewController *vcQREditor = [storyboard instantiateViewControllerWithIdentifier:@"QREditorViewController"];
                vcQREditor.arrQRConfigs = [configs copy];
                if ( configs.count )
                {
                    [[ErrorHandler sharedManager] hideMessage];
                    [self.navigationController pushViewController:vcQREditor animated:YES];
                }
                else
                {
                    [[ErrorHandler sharedManager] updateMessage:@"Cannot parse the response" addActivityIndicator:NO hideAfter:3];
                    double delayInSeconds = 3.0;
                    dispatch_time_t popTime = dispatch_time(DISPATCH_TIME_NOW, delayInSeconds * NSEC_PER_SEC);
                    dispatch_after(popTime, dispatch_get_main_queue(), ^ (void){
                        [self performSelectorOnMainThread:@selector( startReading ) withObject:nil waitUntilDone:NO];
                    });
                }
            });
        }
        else
        {
            dispatch_async(dispatch_get_main_queue(), ^ (void)
            {
                if ( error != nil )
                {
                    NSString *errorMessage = @"";
                    switch ( error.code )
                    {
                    case -1001:         //Connection timeout
                        errorMessage = NSLocalizedString(@"ERROR_CONNECTION_TIMEOUT", @"Connection timeout!");
                        break;

                    case -1012:
                            errorMessage = NSLocalizedString(@"ERROR_QR_UNAUTHORIZED_ACCESS", @"Unauthorized Access! Please check your e-mail and confirm the activation link!");
                        break;

                    default:
                        errorMessage = error.localizedDescription;
                        break;
                    }
                    [[ErrorHandler sharedManager] updateMessage:errorMessage addActivityIndicator:NO hideAfter:3];
                    double delayInSeconds = 3.0;
                    dispatch_time_t popTime = dispatch_time(DISPATCH_TIME_NOW, delayInSeconds * NSEC_PER_SEC);
                    dispatch_after(popTime, dispatch_get_main_queue(), ^ (void){
                        [self performSelectorOnMainThread:@selector( startReading ) withObject:nil waitUntilDone:NO];
                    });
                }
            });
        }
    });
}

-( void )captureOutput:( AVCaptureOutput * )captureOutput didOutputMetadataObjects:( NSArray * )metadataObjects fromConnection:( AVCaptureConnection * )connection
{
    if ( metadataObjects != nil && [metadataObjects count] > 0 )
    {
        AVMetadataMachineReadableCodeObject *metadataObj = [metadataObjects objectAtIndex:0];
        if ( [[metadataObj type] isEqualToString:AVMetadataObjectTypeQRCode] )
        {
            [self performSelectorOnMainThread:@selector( stopReading ) withObject:nil waitUntilDone:YES];
            [self performSelectorOnMainThread:@selector( loadConfigurations: ) withObject:[metadataObj stringValue] waitUntilDone:YES];
            AudioServicesPlaySystemSound(_soundID);
        }
    }
}

-( IBAction )close:( id )sender
{
    [self.navigationController popViewControllerAnimated:YES];
}

@end
