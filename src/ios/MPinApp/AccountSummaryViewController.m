//
//  AccountSummaryViewController.m
//  MPinApp
//
//  Created by Tihomir Ganev on 25.февр..15.
//  Copyright (c) 2015 г. Certivox. All rights reserved.
//

#import "AccountSummaryViewController.h"
#import "MenuViewController.h"
#import "MFSideMenu.h"

@interface AccountSummaryViewController ()

@property(nonatomic,weak) IBOutlet UILabel *lblEmail;

-(IBAction)back:(id)sender;
-(IBAction)logout:(id)sender;
@end

@implementation AccountSummaryViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
}

-(void) viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    _lblEmail.text = _strEmail;
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

-(IBAction)back:(id)sender
{
    [self.menuContainerViewController toggleLeftSideMenuCompletion:nil];
}

-(IBAction)logout:(id)sender
{
    [self.navigationController popToRootViewControllerAnimated:YES];
}
@end
