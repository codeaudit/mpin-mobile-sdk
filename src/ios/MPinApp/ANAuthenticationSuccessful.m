//
//  ANAuthenticationSuccessful.m
//  MPinApp
//
//  Created by Tihomir Ganev on 29.May.15.
//  Copyright (c) 2015 Certivox. All rights reserved.
//

#import "ANAuthenticationSuccessful.h"
#import "ThemeManager.h"

@implementation ANAuthenticationSuccessful

- ( void ) viewDidLoad
{
    [super viewDidLoad];
    [[ThemeManager sharedManager] beautifyViewController:self];
}

- ( void ) viewWillAppear:( BOOL )animated
{
    [super viewWillAppear:animated];
    _lblMessage.text = NSLocalizedString(@"HUD_AUTH_SUCCESS", @"Authentication successful");
}

- ( IBAction )back:( UIBarButtonItem * )sender
{
    [self.navigationController popToRootViewControllerAnimated:YES];
}

@end
