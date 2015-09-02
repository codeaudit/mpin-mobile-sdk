/*
 Copyright (c) 2012-2015, Certivox
 All rights reserved.
 
 Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 
 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 
 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 
 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 
 For full details regarding our CertiVox terms of service please refer to
 the following links:
 * Our Terms and Conditions -
 http://www.certivox.com/about-certivox/terms-and-conditions/
 * Our Security and Privacy -
 http://www.certivox.com/about-certivox/security-privacy/
 * Our Statement of Position and Our Promise on Software Patents -
 http://www.certivox.com/about-certivox/patents/
 */

#import "MPin.h"

// ERROR User info structure
static NSString *const kMPinSatus = @"MPinStatus";
static NSString *const kUSER = @"currentUser";


@protocol MPinSDKDelegate
@optional

- ( void ) OnTestBackendCompleted:( id ) sender;
- ( void ) OnTestBackendError:( id ) sender error:( NSError * ) error;

- ( void ) OnSetBackendCompleted:( id ) sender;
- ( void ) OnSetBackendError:( id ) sender error:( NSError * ) error;

- ( void ) OnRegisterNewUserCompleted:( id ) sender user:( const id<IUser>) user;
- ( void ) OnRegisterNewUserError:( id ) sender error:( NSError * ) error;

- ( void ) OnRestartRegistrationCompleted:( id ) sender user:( const id<IUser>) user;
- ( void ) OnRestartRegistrationError:( id ) sender error:( NSError * ) error;

- ( void ) OnActivateUserRegisteredBySMSCompleted:( id ) sender;
- ( void ) OnActivateUserRegisteredBySMSError:( id ) sender error:( NSError * ) error;

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

+ ( BOOL ) isConfigLoadSuccessfully;
- ( void ) TestBackend:( const NSString * ) url rpsPrefix:( NSString * ) rpsPrefix;
- ( void ) SetBackend:( const NSString * ) url rpsPrefix:( NSString * ) rpsPrefix;
- ( void ) SetBackend:( const NSDictionary * ) config;
- ( void ) RegisterNewUser:( NSString * ) userName devName:( NSString * ) devName;
- ( void ) RegisterNewUser:( NSString * ) userName devName:( NSString * ) devName userData:( NSString * ) userData;
- ( void ) RestartRegistration:( const id<IUser>) user;
- ( void ) RestartRegistration:( const id<IUser>)user userData:( NSString * ) userData;
- ( void ) FinishRegistration:( const id<IUser>) user;
- ( void ) RegisterUserBySMS:(NSString* ) mpinId activationKey:(NSString *) activationKey;
- ( void ) Authenticate:( const id<IUser>) user askForFingerprint:( BOOL )boolAskForFingerprint;
- ( void ) AuthenticateOTP:( id<IUser>) user askForFingerprint:( BOOL )boolAskForFingerprint;
- ( void ) AuthenticateAN:( id<IUser>) user accessNumber:( NSString * ) an askForFingerprint:( BOOL )boolAskForFingerprint;

+ ( Boolean ) isDeviceName;

- ( void ) Logout:( const id<IUser>) user;

@end
