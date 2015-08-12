//
//  ConfigurationManagerTests.m
//  MPinApp
//
//  Created by Tihomir Ganev on 12.Aug.15.
//  Copyright (c) 2015 Certivox. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <XCTest/XCTest.h>
#import "ConfigurationManager.h"

@interface ConfigurationManagerTests : XCTestCase

@property ( nonatomic ) ConfigurationManager *cm;

@end

@implementation ConfigurationManagerTests

- ( void )setUp
{
    [super setUp];
    // Put setup code here. This method is called before the invocation of each test method in the class.
    self.cm = [ConfigurationManager sharedManager];
}

- ( void )tearDown
{
    // Put teardown code here. This method is called after the invocation of each test method in the class.
    [super tearDown];
}

- ( void ) testGetConfigurationAtIndex
{
    for ( int i = -100; i < 100; i++ )
    {
        [self.cm getConfigurationAtIndex:i];
        NSLog(@"%d", i);
    }
}

@end
