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

static NSInteger constIntTimeoutInterval = 30;

@interface QRViewController ( )
@property( nonatomic ) BOOL isReading;
@property ( nonatomic, strong ) AVCaptureSession *captureSession;
@property ( nonatomic, strong ) AVCaptureVideoPreviewLayer *videoPreviewLayer;
@property ( nonatomic, strong ) AVAudioPlayer *audioPlayer;

-( void )loadBeepSound;
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

    [self loadBeepSound];
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
    if ( _isReading )
        [self stopReading];
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
    [_viewPreview.layer addSublayer:_videoPreviewLayer];

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

- ( IBAction )startStopReading:( id )sender
{
    if ( !_isReading )
    {
        if ( [self startReading] )
        {
            [_bbItemStart setTitle:@"Stop"];
        }
    }
    else
    {
        [self stopReading];
        [_bbItemStart setTitle:@"Start!"];
    }
    _isReading = !_isReading;
}

-( void )loadBeepSound
{
    NSString *beepFilePath = [[NSBundle mainBundle] pathForResource:@"beep" ofType:@"mp3"];
    NSURL *beepURL = [NSURL URLWithString:beepFilePath];
    NSError *error;

    _audioPlayer = [[AVAudioPlayer alloc] initWithContentsOfURL:beepURL error:&error];
    if ( error )
    {
        NSLog(@"Could not play beep file.");
        NSLog(@"%@", [error localizedDescription]);
    }
    else
    {
        [_audioPlayer prepareToPlay];
    }
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

    // TODO: show some loading functionality !!!

    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^ {
        NSURL *theUrl = [NSURL URLWithString:url];
        NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:theUrl cachePolicy:NSURLRequestUseProtocolCachePolicy timeoutInterval:constIntTimeoutInterval];
        [request setTimeoutInterval:constIntTimeoutInterval];
        request.HTTPMethod = @"GET";

        NSHTTPURLResponse *response = nil;
        NSError *error = nil;
        NSData *ConfigJSONdata = [NSURLConnection sendSynchronousRequest:request returningResponse:&response error:&error];

        // parse json data
        if ( ConfigJSONdata != nil )
        {
            NSArray *configs = [NSJSONSerialization JSONObjectWithData:ConfigJSONdata options:kNilOptions error:&error];
            if ( error == nil )
            {
                for ( int i = 0; i < [configs count]; i++ )
                {
                    BOOL isExisting = NO;
                    int indexOfExisting = -1;
                    for ( int j = 0; j < [ConfigurationManager sharedManager].configurationsCount; j++ )
                    {
                        if ( [[[ConfigurationManager sharedManager] getNameAtIndex:j] isEqualToString:[[configs objectAtIndex:i] valueForKey:kJSON_NAME]] )
                        {
                            isExisting = YES;
                            indexOfExisting = j;
                            NSLog(@"Configuratoin exists at index: %d", j);
                            break;
                        }
                    }
                    if (isExisting)
                    {
                        [[ConfigurationManager sharedManager] saveConfigurationAtIndex:indexOfExisting
                                                                                   url:[[configs objectAtIndex:i] valueForKey:kJSON_URL]
                                                                           serviceType:[Utilities ServerJSONConfigTypeToService_type:[[configs objectAtIndex:i] valueForKey:kJSON_TYPE]]
                                                                                  name:[[configs objectAtIndex:i] valueForKey:kJSON_NAME]];
                    }
                    else
                    {
                        [[ConfigurationManager sharedManager] addConfiguration:[[configs objectAtIndex:i] valueForKey:kJSON_URL]
                                                                   serviceType:[Utilities ServerJSONConfigTypeToService_type:[[configs objectAtIndex:i] valueForKey:kJSON_TYPE]]
                                                                          name:[[configs objectAtIndex:i] valueForKey:kJSON_NAME]
                                                                    prefixName:[[configs objectAtIndex:i] valueForKey:kJSON_PREFIX]
                         ];
                    }
                }
                [[ConfigurationManager sharedManager] saveConfigurations];
            }
        }

        dispatch_async(dispatch_get_main_queue(), ^ (void) {
            // TODO: hide loading functionality

            if ( error != nil )
            {
                NSString *errorMessage = @"";
                switch ( error.code )
                {
                case -1001:     //Connection timeout
                    errorMessage = @"Connection timeout!";
                    break;

                case -1012:
                    errorMessage = @"Unauthorized Access! Please check your e-mail and confirm the activation link!";
                    break;

                default:
                    errorMessage = error.localizedDescription;
                    break;
                }

                [[ErrorHandler sharedManager] presentMessageInViewController:self
                 errorString:errorMessage
                 addActivityIndicator:NO
                 minShowTime:3];
            }
            else
            {
                [self close:nil];
            }
        });
    });
}

-( void )captureOutput:( AVCaptureOutput * )captureOutput didOutputMetadataObjects:( NSArray * )metadataObjects fromConnection:( AVCaptureConnection * )connection
{
    if ( metadataObjects != nil && [metadataObjects count] > 0 )
    {
        AVMetadataMachineReadableCodeObject *metadataObj = [metadataObjects objectAtIndex:0];
        if ( [[metadataObj type] isEqualToString:AVMetadataObjectTypeQRCode] )
        {
            [self performSelectorOnMainThread:@selector( stopReading ) withObject:nil waitUntilDone:NO];
            [self performSelectorOnMainThread:@selector( loadConfigurations: ) withObject:[metadataObj stringValue] waitUntilDone:NO];

            if ( _audioPlayer )
            {
                [_audioPlayer play];
            }
        }
    }
}

-( IBAction )close:( id )sender
{
    [self dismissViewControllerAnimated:NO completion:nil];
}

@end
