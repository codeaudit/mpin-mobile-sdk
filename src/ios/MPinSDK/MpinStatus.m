//
//  MpinStatus.m
//  MPinSDK
//
//  Created by Georgi Georgiev on 11/24/14.
//  Copyright (c) 2014 Certivox. All rights reserved.
//

#import "MpinStatus.h"

@implementation MpinStatus

-(id) initWith:(MPinStatus)status errorMessage:(NSString*) error {
    self = [super init];
    if (self) {
        self.status = status;
        self.errorMessage = error;
    }
    return self;

}

- (NSString *) getStatusCodeAsString {
    NSString * result = @"";
    switch (self.status) {
        case OK:
            result = @"KEY_BTNOK";
            break;
        case PIN_INPUT_CANCELED:
            result = @"PIN_INPUT_CANCELED";
            break;
        case CRYPTO_ERROR:
            result = @"CRYPTO_ERROR";
            break;
        case STORAGE_ERROR:
            result = @"STORAGE_ERROR";
            break;
        case NETWORK_ERROR:
            result = @"NETWORK_ERROR";
            break;
        case RESPONSE_PARSE_ERROR:
            result = @"RESPONSE_PARSE_ERROR";
            break;
        case FLOW_ERROR:
            result = @"FLOW_ERROR";
            break;
        case IDENTITY_NOT_AUTHORIZED:
            result = @"IDENTITY_NOT_AUTHORIZED";
            break;
        case IDENTITY_NOT_VERIFIED:
            result = @"IDENTITY_NOT_VERIFIED";
            break;
        case REQUEST_EXPIRED:
            result = @"REQUEST_EXPIRED";
            break;
        case REVOKED:
            result = @"REVOKED";
            break;
        case INCORRECT_PIN:
            result = @"INCORRECT_PIN";
            break;
        case INCORRECT_ACCESS_NUMBER:
            result = @"INCORRECT_ACCESS_NUMBER";
            break;
        case HTTP_SERVER_ERROR:
            result = @"HTTP_SERVER_ERROR";
            break;
        case HTTP_REQUEST_ERROR:
            result = @"HTTP_REQUEST_ERROR";
            break;
        default:
            break;
    }
    return result;
}

@end
