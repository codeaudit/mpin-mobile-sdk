//
//  MPinSDKTests.m
//  MPinSDKTests
//
//  Created by Georgi Georgiev on 11/17/14.
//  Copyright (c) 2014 Certivox. All rights reserved.
//

#import <XCTest/XCTest.h>
#import "Context.h"
#include "Storage.h"

#define TEST_MESSAGE "Test Message!"

#import "MPin.h"
#import "mpin_sdk.h"
#import "def.h"
#import "Context.h"
#include <vector>

typedef MPinSDK::UserPtr UserPtr;
typedef MPinSDK::Status Status;

@interface MPinSDKTests : XCTestCase

@end

@implementation MPinSDKTests

- (void)setUp
{
    [super setUp];
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown
{
    // Put teardown code here. This method is called after the invocation of each test method in the class.
    [super tearDown];
}

//- (void)testIHTTPRequest
//{
//    IContext  * pIContext = sdk::Context::Instance();
//	IHttpRequest* pIHTTPRequest = pIContext->CreateHttpRequest();
//	pIHTTPRequest->Execute(IHttpRequest::GET, "http://api.geonames.org/earthquakesJSON?north=44.1&south=-9.9&east=-22.4&west=55.2&username=aporter");
//	MPinSDK::String response = pIHTTPRequest->GetResponseData();
//	pIContext->ReleaseHttpRequest(pIHTTPRequest);
//    
//    XCTAssertTrue(!response.empty(), @"result %s", response.c_str());
//}
//
//- (void)testIStore
//{
//    IContext  * pIContext = sdk::Context::Instance();
//	IStorage *  pIStorage = pIContext->GetStorage(IStorage::NONSECURE);
//	pIStorage->SetData(TEST_MESSAGE);
//	String str = "";
//	pIStorage->GetData(str);
//    XCTAssertTrue( (str == TEST_MESSAGE), @"result %s", TEST_MESSAGE);
//}



- (void)testIStore
{
    IStorage* pSecure = new store::Storage(TRUE);
    IStorage* pNonSecure = new store::Storage(FALSE);
    pSecure->SetData(TEST_MESSAGE);
    pNonSecure->SetData(TEST_MESSAGE);
    RELEASE(pSecure)
    RELEASE(pNonSecure)
    
    pSecure = new store::Storage(TRUE);
    pNonSecure = new store::Storage(FALSE);
    
    String str = "";
    pSecure->GetData(str);
    XCTAssertTrue( (str == TEST_MESSAGE), @"result %s", TEST_MESSAGE);
    str = "";
    pNonSecure->GetData(str);
    XCTAssertTrue( (str == TEST_MESSAGE), @"result %s", TEST_MESSAGE);
}


//- (void) testMPinSDKRegisterNewUser
//{
//    MPinSDK mpin;
//    StringMap sm;
//    sm["RPA_server"] = "https://mpindemo-qa-v3.certivox.org";
//    Status s =  mpin.Init(sm, sdk::Context::Instance());
//    XCTAssertTrue( (s.GetStatusCode() == MPinSDK::Status::OK), @"result %s", s.GetErrorMessage().c_str());
//    UserPtr user = mpin.MakeNewUser("gdg79@mail.bg");
//    s = mpin.StartRegistration(user);
//    XCTAssertTrue( (s.GetStatusCode() == MPinSDK::Status::OK), @"result %s", s.GetErrorMessage().c_str());
//}
//

//- (void) testMPinSDKSetupNewPin
//{
//    MPinSDK mpin;
//    StringMap sm;
//    sm["RPA_server"] = "https://mpindemo-qa-v3.certivox.org";
//    Status s =  mpin.Init(sm, sdk::Context::Instance());
//    UserPtr user = mpin.MakeNewUser("gdg79@mail.bg");
//    mpin.MakeNewUser("gdg11@mail.bg");
//    mpin.MakeNewUser("gdg12@mail.bg");
//    mpin.MakeNewUser("gdg13@mail.bg");
//    mpin.MakeNewUser("gdg14@mail.bg");
//    
//    NSDictionary * config = [NSDictionary dictionaryWithObject:@"https://mpindemo-qa-v3.certivox.org" forKey:@"RPA_server"];
//    [MPin initWithConfig:config];
//    NSArray *ar = [MPin listUsers];
//    
//    
//    XCTAssertTrue( ar.count, @"result fjdkjf");
//    
//}





@end
