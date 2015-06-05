//
//  ANAuthenticationSuccessful.h
//  MPinApp
//
//  Created by Tihomir Ganev on 29.May.15.
//  Copyright (c) 2015 Certivox. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "IUser.h"

@interface ANAuthenticationSuccessful : UIViewController

- ( IBAction )back:( UIBarButtonItem * )sender;

@property( nonatomic,strong ) id<IUser> currentUser;
@property ( nonatomic, weak ) IBOutlet UILabel *lblMessage;

@end
