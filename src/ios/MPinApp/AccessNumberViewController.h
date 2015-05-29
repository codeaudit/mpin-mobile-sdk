//
//  AccessNumberViewController.h
//  MPinApp
//
//  Created by Georgi Georgiev on 1/22/15.
//  Copyright (c) 2015 Certivox. All rights reserved.
//

#import "PinPadViewController.h"
#import "MPin+AsyncOperations.h"

@protocol AccessNumberDelegate <NSObject>
-( void ) onAccessNumber:( NSString * ) an;
@end

@interface AccessNumberViewController : DigitPadViewController <MPinSDKDelegate>

@property( nonatomic,strong ) NSString *strEmail;
@property ( weak ) id <AccessNumberDelegate> delegate;

@property( nonatomic,weak ) IBOutlet UILabel *lblEmail;
@property( nonatomic,weak ) IBOutlet UILabel *lblNote;
@property( nonatomic,weak ) IBOutlet UITextField *txtAN;
@property( nonatomic,weak ) IBOutlet UIButton *btnLogin;

@property( nonatomic,strong ) id<IUser> currentUser;
@end
