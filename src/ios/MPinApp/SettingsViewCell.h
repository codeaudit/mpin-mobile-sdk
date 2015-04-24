//
//  SettingsViewCell.h
//  MPinApp
//
//  Created by Georgi Georgiev on 1/20/15.
//  Copyright (c) 2015 Certivox. All rights reserved.
//

@interface SettingsViewCell : UITableViewCell
@property (nonatomic, readwrite, retain) IBOutlet UILabel * url;
@property(nonatomic,weak) IBOutlet UISwitch * otp;
@property(nonatomic,weak) IBOutlet UISwitch * an;
@property(nonatomic,weak) IBOutlet UISwitch * devName;


- (void) invalidate:(NSMutableArray *) settings atIndex:(NSInteger) index;

-(IBAction)onOTPValueChanged:(id)sender;
-(IBAction)onAccessNumberValueChanged:(id)sender;
-(IBAction)onDeviceNameValueChanged:(id)sender;

@end
