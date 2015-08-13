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

@end

@implementation UserListViewControllerTests

- ( void )setUp
{
    [super setUp];
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- ( void )tearDown
{
    // Put teardown code here. This method is called after the invocation of each test method in the class.
    [super tearDown];
}

- ( void )testExample
{
    // This is an example of a functional test case.
    XCTAssert(YES, @"Pass");
}

- ( void )testPerformanceExample
{
    // This is an example of a performance test case.
    [self measureBlock: ^ {
        // Put the code you want to measure the time of here.
    }];
}

@end
