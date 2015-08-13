//
//  UserListViewControllerTests.m
//  MPinApp
//
//  Created by Tihomir Ganev on 13.Aug.15.
//  Copyright (c) 2015 Certivox. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <XCTest/XCTest.h>
#import <OCMock/OCMock.h>
#import "UserListViewController.h"

@interface UserListViewControllerTests : XCTestCase

@property ( nonatomic, strong ) UserListViewController *vc;
@end

@implementation UserListViewControllerTests

- ( void )setUp
{
    [super setUp];
    // Put setup code here. This method is called before the invocation of each test method in the class.
    self.vc = [[UserListViewController alloc] init];
}

- ( void )tearDown
{
    // Put teardown code here. This method is called after the invocation of each test method in the class.
    [super tearDown];
}

- ( void ) testInvalidate
{
    [self.vc invalidate];
}

- ( void ) testshowLeftMenuPressed:( id )sender
{
    [self.vc showLeftMenuPressed:nil];
}


@end
