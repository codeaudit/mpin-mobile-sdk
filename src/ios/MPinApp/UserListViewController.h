//
//  UserListViewController.h
//  MPinApp
//
//  Created by Georgi Georgiev on 11/19/14.
//  Copyright (c) 2014 Certivox. All rights reserved.
//

#import "AccessNumberViewController.h"
#import "MPin+AsyncOperations.h"

@interface UserListTableViewCell : UITableViewCell


@property(nonatomic, strong) IBOutlet UILabel            *lblUserID;
@property(nonatomic, strong) IBOutlet UIImageView        *imgViewUser;
@property(nonatomic, strong) IBOutlet UIImageView        *imgViewSelected;

@end

@interface UserListViewController : UIViewController <UITableViewDataSource, UITableViewDelegate, AccessNumberDelegate, MPinSDKDelegate>


- (IBAction)showLeftMenuPressed:(id)sender;

@property(nonatomic, weak) IBOutlet UIView             *viewButtonsContainer;
@property(nonatomic, weak) IBOutlet UITableView        *table;
@property(nonatomic, weak) IBOutlet UIButton           *btnAdd;
@property(nonatomic, weak) IBOutlet UIButton           *btnDelete;
@property(nonatomic, weak) IBOutlet UIButton           *btnAuthenticate;
@property (nonatomic, strong) NSMutableArray *users;

@end
