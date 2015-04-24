//
//  SettingsViewController.h
//  MPinApp
//
//  Created by Georgi Georgiev on 1/19/15.
//  Copyright (c) 2015 Certivox. All rights reserved.
//

#import "MPin+AsyncOperations.h"

@interface SettingsViewController : UIViewController <UITableViewDataSource, UITableViewDelegate, UITabBarDelegate, MPinSDKDelegate, UIAlertViewDelegate>

@property (nonatomic, weak) IBOutlet UITableView* tableView;
@property (nonatomic, weak) IBOutlet UIButton* btnSignIn;
@property (nonatomic, weak) IBOutlet UIButton* btnEditConfiguration;
@property (nonatomic, weak) IBOutlet UIButton* btnDeleteConfiguration;
@property (nonatomic, weak) IBOutlet UIView* viewButtons;

- (IBAction)add:(id)sender;
- (IBAction)edit:(id)sender;

@end
