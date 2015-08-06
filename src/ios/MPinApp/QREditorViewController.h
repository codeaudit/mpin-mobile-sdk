//
//  QREditorViewController.h
//  MPinApp
//
//  Created by Tihomir Ganev on 21.Jul.15.
//  Copyright (c) 2015 Certivox. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface QREditorViewController : UIViewController <UITableViewDataSource, UITableViewDelegate, UIAlertViewDelegate>

@property ( nonatomic, strong, setter = setArrConfigs : ) NSArray *arrQRConfigs;
@property ( nonatomic, weak ) IBOutlet UITableView *tblView;
@property ( nonatomic, weak ) IBOutlet UILabel *lblMessage;

@end
