//
//  Certivox_MPin_Tests.m
//  Certivox MPin Tests
//
//  Created by Georgi Georgiev on 7/13/15.
//  Copyright (c) 2015 Certivox. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <XCTest/XCTest.h>
#import "Constants.h"

static NSInteger constIntTimeoutInterval = 30;

@interface Certivox_MPin_Tests : XCTestCase

@end

@implementation Certivox_MPin_Tests

- (void)setUp {
    [super setUp];
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
    [super tearDown];
}

- (void)testExample {
    // Parsing JSON
    
    NSURL * theUrl = [NSURL URLWithString:@"http://192.168.10.75:8080/config.json"];
    NSMutableURLRequest * request = [NSMutableURLRequest requestWithURL:theUrl cachePolicy:NSURLRequestUseProtocolCachePolicy timeoutInterval:constIntTimeoutInterval];
    [request setTimeoutInterval:constIntTimeoutInterval];
    request.HTTPMethod = @"GET";
    
    NSHTTPURLResponse * response = nil;
    NSError * error = nil;
    NSData * ConfigJSONdata = [NSURLConnection sendSynchronousRequest:request returningResponse:&response error:&error];
    

    if(ConfigJSONdata != nil) {
        // parse json data
        NSArray *configs = [NSJSONSerialization JSONObjectWithData:ConfigJSONdata options:kNilOptions error:&error];
        if (error == nil) {
            for (int i = 0; i<[configs count]; i++) {
                NSLog(@"%@",[[configs objectAtIndex:i] valueForKey:kJSON_NAME]);
                NSLog(@"%@",[[configs objectAtIndex:i] valueForKey:kJSON_TYPE]);
                NSLog(@"%@",[[configs objectAtIndex:i] valueForKey:kJSON_URL]);
                NSLog(@"%@",[[configs objectAtIndex:i] valueForKey:kJSON_PREFIX]);
                


            }
        }
        
}
    
    
    
    
}

- (void)testPerformanceExample {
    // This is an example of a performance test case.
    [self measureBlock:^{
        // Put the code you want to measure the time of here.
    }];
}

@end
