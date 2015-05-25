//
//  MpinStatus.h
//  MPinSDK
//
//  Created by Georgi Georgiev on 11/24/14.
//  Copyright (c) 2014 Certivox. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSInteger, MPinStatus) {
    OK = 0, /* = MPinSDK::Status::OK*/
    PIN_INPUT_CANCELED, // Local error, returned when user cancels pin entering
    CRYPTO_ERROR /* = MPinSDK::Status::CRYPTO_ERROR*/, // Local error in crypto functions
    STORAGE_ERROR, // Local storage related error
    NETWORK_ERROR, // Local error - cannot connect to remote server (no internet, or invalid server/port)
    RESPONSE_PARSE_ERROR, // Local error - cannot parse json response from remote server (invalid json or unexpected json structure)
    FLOW_ERROR, // Local error - unproper MPinSDK class usage
    IDENTITY_NOT_AUTHORIZED, // Remote error - the remote server refuses user registration
    IDENTITY_NOT_VERIFIED, // Remote error - the remote server refuses user registration because identity is not verified
    REQUEST_EXPIRED, // Remote error - the register/authentication request expired
    REVOKED, // Remote error - cannot get time permit (propably the user is temporary suspended)
    INCORRECT_PIN, // Remote error - user entered wrong pin
    INCORRECT_ACCESS_NUMBER, // Remote/local error - wrong access number (checksum failed or RPS returned 412)
    HTTP_SERVER_ERROR, // Remote error, that was not reduced to one of the above - the remote server returned internal server error status (5xx)
    HTTP_REQUEST_ERROR // Remote error, that was not reduced to one of the above - invalid data sent to server, the remote server returned 4xx error status
};

@interface MpinStatus : NSObject

@property (nonatomic, readwrite) MPinStatus status;
@property (nonatomic, retain, readwrite) NSString* errorMessage;

- (instancetype)initWith:(MPinStatus)status errorMessage:(NSString*)error;
@property (NS_NONATOMIC_IOSONLY, getter=getStatusCodeAsString, readonly, copy) NSString* statusCodeAsString;

@end
