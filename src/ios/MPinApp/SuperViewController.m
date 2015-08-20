//
//  SuperViewController.m
//  MPinApp
//
//  Created by Tihomir Ganev on 17.Aug.15.
//  Copyright (c) 2015 Certivox. All rights reserved.
//

#import "SuperViewController.h"
#import "ThemeManager.h"


@interface SuperViewController ( )
{}
@end

@implementation SuperViewController

- ( void )viewDidLoad
{
    [super viewDidLoad];
}

- ( void )didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- ( void )viewWillAppear:( BOOL )animated    // Called when the view is about to made visible. Default does nothing
{
    [super viewWillAppear:animated];
}

- ( void )viewDidAppear:( BOOL )animated     // Called when the view has been fully transitioned onto the screen. Default does nothing
{
    [super viewDidAppear:animated];
}

- ( void )viewWillDisappear:( BOOL )animated // Called when the view is dismissed, covered or otherwise hidden. Default does nothing
{
    [super viewWillDisappear:animated];
}


@end
