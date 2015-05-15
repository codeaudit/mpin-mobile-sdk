//
//  AddSettingViewController.h
//  MPinApp
//
//  Created by Georgi Georgiev on 1/20/15.
//  Copyright (c) 2015 Certivox. All rights reserved.
//

#import "MPin+AsyncOperations.h"

@interface AddSettingViewController : UIViewController <UITextFieldDelegate,
                                                        UITableViewDataSource,
                                                        UITableViewDelegate,
                                                        MPinSDKDelegate>
@property (nonatomic, weak) IBOutlet UIBarButtonItem *btnDone;
@property (nonatomic, weak) IBOutlet UITableView* tblView;
@property (nonatomic, weak) IBOutlet UIButton *btnTestConfig;
@property (nonatomic) bool isEdit;
@property (nonatomic) NSInteger selectedIndex;

- (IBAction)onSave:(id)sender;

@end
