//
//  QRViewController.m
//  MPinApp
//
//  Created by Georgi Georgiev on 6/24/15.
//  Copyright (c) 2015 Certivox. All rights reserved.
//

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
    SystemSoundID soundID;
}
@property( nonatomic ) BOOL isReading;
@property ( nonatomic, strong ) AVCaptureSession *captureSession;
@property ( nonatomic, strong ) AVCaptureVideoPreviewLayer *videoPreviewLayer;
@property ( nonatomic, weak ) IBOutlet UIImageView *imgViewRectangle;

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

    NSURL *fileURL = [NSURL URLWithString:@"/System/Library/Audio/UISounds/sms-received2.caf"];
    AudioServicesCreateSystemSoundID((__bridge_retained CFURLRef)fileURL,&soundID);

}

- ( void ) viewDidAppear:( BOOL )animated
{
    [super viewDidAppear:animated];

    if ( !( _isReading = [self startReading] ) )
    {
        [[ErrorHandler sharedManager] presentMessageInViewController:self
         errorString:@"Uanble to load camera and scan QR code!"
         addActivityIndicator:NO
         minShowTime:3];
    }
}

- ( void ) viewWillDisappear:( BOOL )animated
{
    [super viewWillDisappear:animated];
}

-( BOOL ) startReading
{
    NSError *error;

    AVCaptureDevice *captureDevice = [AVCaptureDevice defaultDeviceWithMediaType:AVMediaTypeVideo];

    AVCaptureDeviceInput *input = [AVCaptureDeviceInput deviceInputWithDevice:captureDevice error:&error];
    if ( !input )
    {
        [[ErrorHandler sharedManager] presentMessageInViewController:self
         errorString:[error localizedDescription]
         addActivityIndicator:NO
         minShowTime:3];

        return NO;
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

    return YES;
}

-( void ) stopReading
{
    [_captureSession stopRunning];
    _captureSession = nil;

    [_videoPreviewLayer removeFromSuperlayer];
    _isReading = NO;
}


- ( void ) loadConfigurations:( NSString * ) url
{
    if ( ![NSString isValidURL:url] )
    {
        [[ErrorHandler sharedManager] presentMessageInViewController:self
         errorString:@"Invalid URL!"
         addActivityIndicator:NO
         minShowTime:3];

        return;
    }

    [[ErrorHandler sharedManager] presentMessageInViewController:self
                                                     errorString:@"Loading URL"
                                            addActivityIndicator:YES
                                                     minShowTime:0];
    
    [self performSelectorOnMainThread:@selector( stopReading ) withObject:nil waitUntilDone:NO];

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
            [[ErrorHandler sharedManager] hideMessage];
            NSArray *configs = [NSJSONSerialization JSONObjectWithData:ConfigJSONdata options:kNilOptions error:&error];
            dispatch_async(dispatch_get_main_queue(), ^ (void) {
                UIStoryboard *storyboard = [UIStoryboard storyboardWithName:@"Main_iPhone" bundle:nil];
                QREditorViewController *vcQREditor = [storyboard instantiateViewControllerWithIdentifier:@"QREditorViewController"];
                vcQREditor.arrQRConfigs = [configs copy];
                [self.navigationController pushViewController:vcQREditor animated:YES];
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
                        errorMessage = @"Connection timeout!";
                        break;

                    case -1012:
                        errorMessage = @"Unauthorized Access! Please check your e-mail and confirm the activation link!";
                        break;

                    default:
                        errorMessage = error.localizedDescription;
                        break;
                    }
                    [[ErrorHandler sharedManager] updateMessage:errorMessage addActivityIndicator:NO hideAfter:3];
                    double delayInSeconds = 3.0;
                    dispatch_time_t popTime = dispatch_time(DISPATCH_TIME_NOW, delayInSeconds * NSEC_PER_SEC);
                    dispatch_after(popTime, dispatch_get_main_queue(), ^(void){
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
            [self performSelectorOnMainThread:@selector( loadConfigurations: ) withObject:[metadataObj stringValue] waitUntilDone:NO];
            AudioServicesPlaySystemSound(soundID);
        }
    }
}

-( IBAction )close:( id )sender
{
    [self.navigationController popViewControllerAnimated:YES];
}

@end
