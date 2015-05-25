//
//  MenuViewController.m
//  MPinApp
//
//  Created by Tihomir Ganev on 6.февр..15.
//  Copyright (c) 2015 г. Certivox. All rights reserved.
//

#import "MenuViewController.h"
#import "MFSideMenu.h"
#import "AboutViewController.h"
#import "SettingsViewController.h"
#import "UserListViewController.h"
#import "AppDelegate.h"
#import "MenuTableViewCell.h"
#import "ThemeManager.h"

#define USER_LIST 0
#define SETTINGS 1
#define ABOUT 2
@interface MenuViewController () {
    AboutViewController* vcAbout;
    SettingsViewController* vcSettings;
    UserListViewController* vcUserList;
}

@end

@implementation MenuViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    [[ThemeManager sharedManager] beautifyViewController:self];
    vcAbout = [self.storyboard instantiateViewControllerWithIdentifier:@"AboutViewController"];

    AppDelegate* appDelegate = [UIApplication sharedApplication].delegate;

    vcAbout = [self.storyboard instantiateViewControllerWithIdentifier:@"AboutViewController"];
    vcUserList = appDelegate.vcUserList;
    vcSettings = [self.storyboard instantiateViewControllerWithIdentifier:@"SettingsViewController"];
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    [self setConfiguration];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

#pragma mark - UITableViewDataSource -

- (CGFloat)tableView:(UITableView*)tableView heightForRowAtIndexPath:(NSIndexPath*)indexPath
{
    return 60.f;
}

- (NSInteger)numberOfSectionsInTableView:(UITableView*)tableView
{
    return 1;
}

- (NSInteger)tableView:(UITableView*)tableView numberOfRowsInSection:(NSInteger)section
{
    return 3;
}

- (UITableViewCell*)tableView:(UITableView*)tableView cellForRowAtIndexPath:(NSIndexPath*)indexPath
{

    static NSString* userListTableIdentifier = @"MenuTableViewCell";
    MenuTableViewCell* cell = [tableView dequeueReusableCellWithIdentifier:userListTableIdentifier];
    if (cell == nil)
        cell = [[MenuTableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:userListTableIdentifier];
    [[ThemeManager sharedManager] customiseMenuCell:cell];
    return cell;
}

- (void)tableView:(UITableView*)tableView willDisplayCell:(UITableViewCell*)cell forRowAtIndexPath:(NSIndexPath*)indexPath
{
    switch (indexPath.row) {
    case USER_LIST:
        ((MenuTableViewCell*)cell).lblMenuID.text = @"IDENTITY LIST";
        break;

    case SETTINGS:
        ((MenuTableViewCell*)cell).lblMenuID.text = @"CONFIGURATION LIST";
        break;

    case ABOUT:
        ((MenuTableViewCell*)cell).lblMenuID.text = @"ABOUT";
        break;
    }
}

- (void)tableView:(UITableView*)tableView didSelectRowAtIndexPath:(NSIndexPath*)indexPath
{
    UIViewController* vc = vcUserList;

    [tableView deselectRowAtIndexPath:indexPath animated:YES];

    switch (indexPath.row) {
    case USER_LIST:
        vc = vcUserList;
        break;

    case SETTINGS:
        vc = vcSettings;
        break;

    case ABOUT:
        vc = vcAbout;
        break;
    }

    UINavigationController* navigationController = self.menuContainerViewController.centerViewController;
    NSArray* controllers = @[ vc ];
    navigationController.viewControllers = controllers;
    [self.menuContainerViewController setMenuState:MFSideMenuStateClosed];
}

- (void)setCenterWithID:(int)vcId
{
    UIViewController* vc = vcUserList;
    switch (vcId) {
    case USER_LIST:
        vc = vcUserList;
        break;

    case SETTINGS:
        vc = vcSettings;
        break;

    case ABOUT:
        vc = vcAbout;
        break;
    }

    UINavigationController* navigationController = self.menuContainerViewController.centerViewController;
    NSArray* controllers = @[ vc ];
    navigationController.viewControllers = controllers;
    [self.menuContainerViewController setMenuState:MFSideMenuStateClosed];
}

- (void)setConfiguration
{
    NSArray* settings = [[NSUserDefaults standardUserDefaults] objectForKey:@"settings"];
    NSInteger intSelectedConfiguration = [[NSUserDefaults standardUserDefaults] integerForKey:@"currentSelectionIndex"];
    NSDictionary* dictConfiguration = settings[intSelectedConfiguration];
    _lblConfigurationName.text = dictConfiguration[@"CONFIG_NAME"];
    _lblConfigurationURL.text = dictConfiguration[@"backend"];
}

@end
