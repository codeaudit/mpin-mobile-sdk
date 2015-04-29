//
//  MPin+AsyncOperations.m
//  MPinApp
//
//  Created by Georgi Georgiev on 3/19/15.
//  Copyright (c) 2015 Certivox. All rights reserved.
//

#import "MPin+AsyncOperations.h"
#import "Constants.h"
#import <objc/runtime.h>
@import LocalAuthentication;

static char const* const delegateKey = "delegateKey";

@implementation MPin (AsyncOperations)

@dynamic delegate;

- (id<MPinSDKDelegate>)delegate
{
    return (id<MPinSDKDelegate>)objc_getAssociatedObject(self, delegateKey);
}

- (void)setDelegate:(id<MPinSDKDelegate>)delegate
{
    objc_setAssociatedObject(self, delegateKey, (id)delegate, OBJC_ASSOCIATION_ASSIGN);
}

- (void)TestBackend:(const NSString*)url rpsPrefix:(NSString*)rpsPrefix
{

    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        
        MpinStatus *mpinStatus = [MPin TestBackend:url rpsPrefix:rpsPrefix];
        
        dispatch_async(dispatch_get_main_queue(), ^(void) {
            
            if(self.delegate == nil) return;
            
            if(mpinStatus.status == OK) {
                if ([(NSObject *)self.delegate respondsToSelector:@selector(OnTestBackendCompleted:)]) {
                    [self.delegate OnTestBackendCompleted:self];
                }
            } else {
                if ([(NSObject *)self.delegate respondsToSelector:@selector(OnTestBackendError:error:)]) {
                    [self.delegate OnTestBackendError:self error:[NSError errorWithDomain:@"SDK" code:mpinStatus.status userInfo:@{kMPinSatus: mpinStatus}]];
                }
            }
        });
    });
}

- (void)SetBackend:(const NSString*)url rpsPrefix:(NSString*)rpsPrefix
{

    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        
        MpinStatus *mpinStatus = [MPin SetBackend:url rpsPrefix:rpsPrefix];
        
        dispatch_async(dispatch_get_main_queue(), ^(void) {
            
            if(self.delegate == nil) return;
            
            if(mpinStatus.status == OK) {
                if ([(NSObject *)self.delegate respondsToSelector:@selector(OnSetBackendCompleted:)]) {
                    [self.delegate OnSetBackendCompleted:self];
                }
            } else {
                if ([(NSObject *)self.delegate respondsToSelector:@selector(OnSetBackendError:error:)]) {
                    [self.delegate OnSetBackendError:self error:[NSError errorWithDomain:@"SDK" code:mpinStatus.status userInfo:@{kMPinSatus: mpinStatus}]];
                }
            }
        });
    });
}

- (void)RegisterNewUser:(NSString*)userName devName:(NSString*)devName userData:(NSString*)userData
{

    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        
        id<IUser> user;
        if ([userName isEqualToString:kEmptyStr] || [devName isEqualToString:kDevName]) {
            user= [MPin MakeNewUser:userName];
        } else {
            user= [MPin MakeNewUser:userName deviceName:devName];
        }
        
        MpinStatus* mpinStatus = [MPin StartRegistration:user userData:userData];
        
        dispatch_async(dispatch_get_main_queue(), ^(void) {
            if(self.delegate == nil) return;
            
            if(mpinStatus.status == OK) {
                if ([(NSObject *)self.delegate respondsToSelector:@selector(OnRegisterNewUserCompleted:user:)]) {
                    [self.delegate OnRegisterNewUserCompleted:self user:user];
                }
            } else {
                if ([(NSObject *)self.delegate respondsToSelector:@selector(OnRegisterNewUserError:error:)]) {
                    [self.delegate OnRegisterNewUserError:self
                                                    error:[NSError errorWithDomain:@"SDK"
                                                                              code:mpinStatus.status
                                                                          userInfo:@{kMPinSatus: mpinStatus,kUSER: user}
                                                           ]
                     ];
                }
            }
        });
    });
}

- (void)RestartRegistration:(const id<IUser>)user userData:(NSString*)userData
{

    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        
        MpinStatus *mpinStatus = [MPin RestartRegistration:user userData:userData];
        
        dispatch_async(dispatch_get_main_queue(), ^(void) {
            
            if(self.delegate == nil) return;
            
            if(mpinStatus.status == OK) {
                if ([(NSObject *)self.delegate respondsToSelector:@selector(OnRestartRegistrationCompleted: user:)]) {
                    [self.delegate OnRestartRegistrationCompleted:self user:user];
                }
            } else {
                if ([(NSObject *)self.delegate respondsToSelector:@selector(OnRestartRegistrationError:error:)]) {
                    [self.delegate OnRestartRegistrationError:self
                                                        error:[NSError errorWithDomain:@"SDK"
                                                                                  code:mpinStatus.status
                                                                                   userInfo:@{kMPinSatus: mpinStatus,kUSER: user}
                                                                          ]
                     ];
                }
            }
        });
    });
}

- (void)RegisterNewUser:(NSString*)userName devName:(NSString*)devName
{
    [self RegisterNewUser:userName devName:devName userData:@""];
}
- (void)RestartRegistration:(const id<IUser>)user
{
    [self RestartRegistration:user userData:@""];
}

- (void)FinishRegistration:(const id<IUser>)user
{

    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        
        MpinStatus *mpinStatus = [MPin FinishRegistration:user];
        
        dispatch_async(dispatch_get_main_queue(), ^(void) {
            
            if(self.delegate == nil) return;
            
            if(mpinStatus.status == OK) {
                if ([(NSObject *)self.delegate respondsToSelector:@selector(OnFinishRegistrationCompleted: user:)]) {
                    [self.delegate OnFinishRegistrationCompleted:self user:user];
                }
            } else {
                if ([(NSObject *)self.delegate respondsToSelector:@selector(OnFinishRegistrationError:error:)]) {
                    [self.delegate OnFinishRegistrationError:self
                                                       error:[NSError errorWithDomain:@"SDK"
                                                                                 code:mpinStatus.status
                                                                             userInfo:@{kMPinSatus: mpinStatus,kUSER: user}
                                                              ]
                     ];
                }
            }
        });
    });
}

- (void)Authenticate:(const id<IUser>)user
{
    void (^touchIDBlock)(BOOL success, NSError *error) = ^void(BOOL success, NSError *error)
    {
        if (success)
        {
            dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
                
                MpinStatus * mpinStatus = [MPin Authenticate:user];
                
                dispatch_async(dispatch_get_main_queue(), ^(void) {
                    
                    if(self.delegate == nil) return;
                    
                    if(mpinStatus.status == OK) {
                        if ([(NSObject *)self.delegate respondsToSelector:@selector(OnAuthenticateCompleted:user:)]) {
                            [self.delegate OnAuthenticateCompleted:self user:user];
                        }
                    } else {
                        if ([(NSObject *)self.delegate respondsToSelector:@selector(OnAuthenticateError:error:)]) {
                            [self.delegate OnAuthenticateError:self
                                                         error:[NSError errorWithDomain:@"SDK"
                                                                                   code:mpinStatus.status
                                                                               userInfo:@{kMPinSatus: mpinStatus,kUSER: user}
                                                                ]
                             ];
                        }
                    }
                });
            });
        }
        else
        {
            dispatch_async(dispatch_get_main_queue(), ^(void) {
                
                [self.delegate OnAuthenticateCanceled];
            });
            
        }
    };
    
    void (^authenticateBlock)() = ^void() {
        LAContext *context = [[LAContext alloc] init];
        NSError *error;
        if ([context canEvaluatePolicy:LAPolicyDeviceOwnerAuthenticationWithBiometrics
                                 error:&error])
        {
            [context evaluatePolicy:LAPolicyDeviceOwnerAuthenticationWithBiometrics
                    localizedReason:@"Please verify fingerprint"
                              reply:touchIDBlock];
        }
        else
        {
            dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
                
                MpinStatus * mpinStatus = [MPin Authenticate:user];
                
                dispatch_async(dispatch_get_main_queue(), ^(void) {
                    
                    if(self.delegate == nil) return;
                    
                    if(mpinStatus.status == OK) {
                        if ([(NSObject *)self.delegate respondsToSelector:@selector(OnAuthenticateCompleted:user:)]) {
                            [self.delegate OnAuthenticateCompleted:self user:user];
                        }
                    } else {
                        if ([(NSObject *)self.delegate respondsToSelector:@selector(OnAuthenticateError:error:)]) {
                            [self.delegate OnAuthenticateError:self
                                                         error:[NSError errorWithDomain:@"SDK"
                                                                                   code:mpinStatus.status
                                                                               userInfo:@{kMPinSatus: mpinStatus,kUSER: user}
                                                                ]
                             ];
                        }
                    }
                });
            });
        }
        
    };
    
    dispatch_async(dispatch_get_main_queue(), authenticateBlock);
}

- (void)AuthenticateOTP:(id<IUser>)user
{
    dispatch_async(dispatch_get_main_queue(), ^(void){
        LAContext *context = [[LAContext alloc] init];
        NSError *error;
        if ([context canEvaluatePolicy:LAPolicyDeviceOwnerAuthenticationWithBiometrics
                                 error:&error])
        {
            [context evaluatePolicy:LAPolicyDeviceOwnerAuthenticationWithBiometrics
                    localizedReason:@"Please verify fingerprint"
                              reply:^(BOOL success, NSError *authenticationError)
             {
                 if (success)
                 {
                     dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
                         
                         OTP *otp = nil;
                         MpinStatus * mpinStatus = [MPin AuthenticateOTP:user otp:&otp];
                         
                         dispatch_async(dispatch_get_main_queue(), ^(void) {
                             
                             if(self.delegate == nil) return;
                             
                             if(mpinStatus.status == OK) {
                                 if ([(NSObject *)self.delegate respondsToSelector:@selector(OnAuthenticateOTPCompleted:user:otp:)]) {
                                     [self.delegate OnAuthenticateOTPCompleted:self user:user otp:otp];
                                 }
                             } else {
                                 if ([(NSObject *)self.delegate respondsToSelector:@selector(OnAuthenticateOTPError:error:)]) {
                                     [self.delegate OnAuthenticateOTPError:self
                                                                     error:[NSError errorWithDomain:@"SDK"
                                                                                               code:mpinStatus.status
                                                                                           userInfo:@{kMPinSatus: mpinStatus,kUSER: user}
                                                                            ]
                                      ];
                                 }
                             }
                         });
                     });
                 }
                 else
                 {
                     dispatch_async(dispatch_get_main_queue(), ^(void) {
                         if ([(NSObject *)self.delegate respondsToSelector:@selector(OnAuthenticateCanceled)])
                         {
                             [self.delegate OnAuthenticateCanceled];
                         }
                     });
                 }
             }];
        }
        else
        {
            dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
                
                OTP *otp = nil;
                MpinStatus * mpinStatus = [MPin AuthenticateOTP:user otp:&otp];
                
                dispatch_async(dispatch_get_main_queue(), ^(void) {
                    
                    if(self.delegate == nil) return;
                    
                    if(mpinStatus.status == OK) {
                        if ([(NSObject *)self.delegate respondsToSelector:@selector(OnAuthenticateOTPCompleted:user:otp:)]) {
                            [self.delegate OnAuthenticateOTPCompleted:self user:user otp:otp];
                        }
                    } else {
                        if ([(NSObject *)self.delegate respondsToSelector:@selector(OnAuthenticateOTPError:error:)]) {
                            [self.delegate OnAuthenticateOTPError:self
                                                            error:[NSError errorWithDomain:@"SDK"
                                                                                      code:mpinStatus.status
                                                                                  userInfo:@{kMPinSatus: mpinStatus,kUSER: user}
                                                                   ]
                             ];
                        }
                    }
                });
            });
        }
    });
}




- (void) AuthenticateAN:(id<IUser>) user  accessNumber:(NSString *) an
{
    dispatch_async(dispatch_get_main_queue(), ^(void)
    {
        LAContext *context = [[LAContext alloc] init];
        NSError *error;
        if ([context canEvaluatePolicy: LAPolicyDeviceOwnerAuthenticationWithBiometrics error:&error])
        {
            [context evaluatePolicy:LAPolicyDeviceOwnerAuthenticationWithBiometrics
                    localizedReason:@"Please verify fingerprint"
                              reply:^(BOOL success, NSError *authenticationError)
             {
                 if (success) {
                     dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
                         
                         MpinStatus* mpinStatus = [MPin AuthenticateAN:user accessNumber:an];
                         
                         dispatch_async(dispatch_get_main_queue(), ^(void) {
                             if(self.delegate == nil) return;
                             
                             if(mpinStatus.status == OK) {
                                 if ([(NSObject *)self.delegate respondsToSelector:@selector(OnAuthenticateAccessNumberCompleted:user:)]) {
                                     [self.delegate OnAuthenticateAccessNumberCompleted:self user:user];
                                 }
                             } else {
                                 if ([(NSObject *)self.delegate respondsToSelector:@selector(OnAuthenticateAccessNumberError:error:)]) {
                                     [self.delegate OnAuthenticateAccessNumberError:self
                                                                              error:[NSError errorWithDomain:@"SDK"
                                                                                                        code:mpinStatus.status
                                                                                                    userInfo:@{kMPinSatus: mpinStatus,kUSER: user}
                                                                                     ]
                                      ];
                                 }
                             }
                         });
                     });
                 }
             }];
        }
        else
        {
            dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
                
                MpinStatus* mpinStatus = [MPin AuthenticateAN:user accessNumber:an];
                
                dispatch_async(dispatch_get_main_queue(), ^(void) {
                    if(self.delegate == nil) return;
                    
                    if(mpinStatus.status == OK) {
                        if ([(NSObject *)self.delegate respondsToSelector:@selector(OnAuthenticateAccessNumberCompleted:user:)]) {
                            [self.delegate OnAuthenticateAccessNumberCompleted:self user:user];
                        }
                    } else {
                        if ([(NSObject *)self.delegate respondsToSelector:@selector(OnAuthenticateAccessNumberError:error:)]) {
                            [self.delegate OnAuthenticateAccessNumberError:self
                                                                     error:[NSError errorWithDomain:@"SDK"
                                                                                               code:mpinStatus.status
                                                                                           userInfo:@{kMPinSatus: mpinStatus,kUSER: user}
                                                                            ]
                             ];
                        }
                    }
                });
            });
        }
    });
}

+ (Boolean) isDeviceName {
    NSString * value  = [MPin GetClientParam:kDeviceName];
    if (value == nil) return false;
    return [value isEqualToString:@"true"];
}

- (void)Logout:(const id<IUser>)user
{
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        
        BOOL isSuccessful = [MPin Logout:user];
        
        dispatch_async(dispatch_get_main_queue(), ^(void) {
            
            if(self.delegate == nil) return;
            if ([(NSObject *)self.delegate respondsToSelector:@selector(OnLogoutCompleted:isSuccessful:)]) {
                [self.delegate OnLogoutCompleted:self isSuccessful:isSuccessful];
            }
        });
    });
}

@end
