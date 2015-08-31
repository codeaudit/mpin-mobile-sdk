//
//  Certivox_MPin_Tests.m
//  Certivox MPin Tests
//
//  Created by Tihomir Ganev on 6.Aug.15.
//  Copyright (c) 2015 Certivox. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <XCTest/XCTest.h>
#import "UserListViewController.h"
#import "AppDelegate.h"

@interface Certivox_MPin_Tests : XCTestCase

@property ( nonatomic ) UserListViewController *vcToTest;
@property ( nonatomic ) AppDelegate *appDelegate;

@end

@implementation Certivox_MPin_Tests

- ( void )setUp
{
    [super setUp];
    // Put setup code here. This method is called before the invocation of each test method in the class.
    _vcToTest = [[UserListViewController alloc] init];
}

- ( void )tearDown
{
    // Put teardown code here. This method is called after the invocation of each test method in the class.
    [super tearDown];
}

- ( void ) testAppDelegateConnectionDown
{
    [self.appDelegate connectionDown];
}

- ( void ) testAppDelegateConnectionUp
{
    [self.appDelegate connectionUp];
}

- ( void )testShowLeftMenu
{
    [self.vcToTest showLeftMenuPressed:nil];
}

@end
