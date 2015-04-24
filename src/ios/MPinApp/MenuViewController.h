//
//  MenuViewController.h
//  MPinApp
//
//  Created by Tihomir Ganev on 6.февр..15.
//  Copyright (c) 2015 г. Certivox. All rights reserved.
//

@interface MenuViewController : UIViewController <UITableViewDelegate, UITableViewDataSource>

@property (nonatomic, weak) IBOutlet UITableView* tblMenu;
@property (nonatomic, weak) IBOutlet UILabel* lblAppVersion;
@property (nonatomic, weak) IBOutlet UILabel* lblConfigurationName;
@property (nonatomic, weak) IBOutlet UILabel* lblConfigurationURL;

-(void) setCenterWithID: (int)vcId;
- (void) setConfiguration;

@end
