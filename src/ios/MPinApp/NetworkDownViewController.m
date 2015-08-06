//
//  NetworkDownViewController.m
//  MPinApp
//
//  Created by Tihomir Ganev on 6.Aug.15.
//  Copyright (c) 2015 Certivox. All rights reserved.
//

#import "NetworkDownViewController.h"
#import "ThemeManager.h"

@interface NetworkDownViewController ()

@end

@implementation NetworkDownViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    [[ThemeManager sharedManager] beautifyViewController:self];
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

@end
