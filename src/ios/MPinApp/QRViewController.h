//
//  QRViewController.h
//  MPinApp
//
//  Created by Georgi Georgiev on 6/24/15.
//  Copyright (c) 2015 Certivox. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <AVFoundation/AVFoundation.h>

@interface QRViewController : UIViewController <AVCaptureMetadataOutputObjectsDelegate>

@property (weak, nonatomic) IBOutlet UIView * viewPreview;
@property (weak, nonatomic) IBOutlet UILabel * lblStatus;
@property (weak, nonatomic) IBOutlet UIBarButtonItem * bbItemStart;

- (IBAction)startStopReading:(id)sender;
- (IBAction) back:(id)sender;

@end
