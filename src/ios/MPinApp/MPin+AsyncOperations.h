//
//  MPin+AsyncOperations.h
//  MPinApp
//
//  Created by Georgi Georgiev on 3/19/15.
//  Copyright (c) 2015 Certivox. All rights reserved.
//

#import "MPin.h"

// ERROR User info structure
static NSString *const kMPinSatus = @"MPinStatus";
static NSString *const kUSER = @"currentUser";


@protocol MPinSDKDelegate
@optional

- ( void ) OnInitCompleted:( id ) sender;
- ( void ) OnInitError:( id ) sender error:( NSError * ) error;

- ( void ) OnTestBackendCompleted:( id ) sender;
- ( void ) OnTestBackendError:( id ) sender error:( NSError * ) error;

- ( void ) OnSetBackendCompleted:( id ) sender;
- ( void ) OnSetBackendError:( id ) sender error:( NSError * ) error;

- ( void ) OnRegisterNewUserCompleted:( id ) sender user:( const id<IUser>) user;
- ( void ) OnRegisterNewUserError:( id ) sender error:( NSError * ) error;

- ( void ) OnRestartRegistrationCompleted:( id ) sender user:( const id<IUser>) user;
- ( void ) OnRestartRegistrationError:( id ) sender error:( NSError * ) error;

- ( void ) OnFinishRegistrationCompleted:( id ) sender user:( const id<IUser>) user;
- ( void ) OnFinishRegistrationError:( id ) sender error:( NSError * ) error;

- ( void ) OnAuthenticateCompleted:( id ) sender user:( const id<IUser>) user;
- ( void ) OnAuthenticateError:( id ) sender error:( NSError * ) error;

- ( void ) OnAuthenticateAuthResultCompleted:( id ) sender user:( id<IUser>) user authResultData:( NSString * )authResultData;
- ( void ) OnAuthenticateAuthResultError:( id ) sender error:( NSError * ) error;

- ( void ) OnAuthenticateOTPCompleted:( id ) sender user:( id<IUser>) user otp:( OTP * )otp;
- ( void ) OnAuthenticateOTPError:( id ) sender error:( NSError * ) error;

- ( void ) OnAuthenticateAccessNumberCompleted:( id ) sender user:( id<IUser>) user;
- ( void ) OnAuthenticateAccessNumberError:( id ) sender error:( NSError * ) error;

- ( void ) OnLogoutCompleted:( id ) sender isSuccessful:( Boolean ) isSuccessful;

- ( void ) OnAuthenticateCanceled;
@end


@interface MPin ( AsyncOperations )

@property( nonatomic, strong ) id<MPinSDKDelegate> delegate;

+ ( BOOL ) isInitialized;
- ( void ) initSDK:( NSDictionary * )config;
- ( void ) TestBackend:( const NSString * ) url rpsPrefix:( NSString * ) rpsPrefix;
- ( void ) SetBackend:( const NSString * ) url rpsPrefix:( NSString * ) rpsPrefix;
- ( void ) RegisterNewUser:( NSString * ) userName devName:( NSString * ) devName;
- ( void ) RegisterNewUser:( NSString * ) userName devName:( NSString * ) devName userData:( NSString * ) userData;
- ( void ) RestartRegistration:( const id<IUser>) user;
- ( void ) RestartRegistration:( const id<IUser>)user userData:( NSString * ) userData;
- ( void ) FinishRegistration:( const id<IUser>) user;
- ( void ) Authenticate:( const id<IUser>) user askForFingerprint:( BOOL )boolAskForFingerprint;
- ( void ) AuthenticateOTP:( id<IUser>) user askForFingerprint:( BOOL )boolAskForFingerprint;
- ( void ) AuthenticateAN:( id<IUser>) user accessNumber:( NSString * ) an askForFingerprint:( BOOL )boolAskForFingerprint;

+ ( Boolean ) isDeviceName;

- ( void ) Logout:( const id<IUser>) user;

@end
