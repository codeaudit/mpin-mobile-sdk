//
//  AddIdentityViewController.h
//  MPinApp
//
//  Created by Georgi Georgiev on 11/20/14.
//  Copyright (c) 2014 Certivox. All rights reserved.
//

#import "MPin+AsyncOperations.h"

@class BackButton;

@interface AddIdentityViewController : UIViewController <UITextFieldDelegate, MPinSDKDelegate>

@property (nonatomic, weak) IBOutlet BackButton* btnBack;
@property (nonatomic, weak) IBOutlet UITextField* txtIdentity;
@property (nonatomic, weak) IBOutlet UITextField* txtDevName;
@property (nonatomic, weak) IBOutlet UILabel* lblIdentity;
@property (nonatomic, weak) IBOutlet UILabel* lblDevName;

@end
