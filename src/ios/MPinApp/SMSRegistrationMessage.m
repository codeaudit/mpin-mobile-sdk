//
//  SMSNotificationService.m
//  MPinApp
//
//  Created by Georgi Georgiev on 9/10/15.
//  Copyright (c) 2015 Certivox. All rights reserved.
//

#import "SMSRegistrationMessage.h"
#import "Utilities.h"

@implementation SMSRegistrationMessage

static NSString *kmpinid = @"mpinId";
static NSString *kActivateKey = @"activateKey";
static NSString *kUserId = @"userID";

- ( id ) initWith:( NSURL * ) url {
    
    if ( self = [super init] )
    {
        if (url == nil) {
            self.error = [NSError errorWithDomain:@"SMSRegMessage: Invalid URL provided!" code:-1 userInfo:nil];
            return self;
        }
        
        NSDictionary * urlParams = [Utilities urlQueryParamsToDictianary:[url query]];
        if (urlParams == nil) {
            self.error = [NSError errorWithDomain:[NSString stringWithFormat:@"SMSRegMessage: bad url query parameters: %@", [url query] ] code:-1 userInfo:nil];
            return self;
        }
        
        NSString * hexMpinId = [urlParams objectForKey:kmpinid];
        NSString * activateKey = [urlParams objectForKey:kActivateKey];
        NSString * hash_user_id = [urlParams objectForKey:kHashUserId];
        
        if ((hexMpinId == nil) || (activateKey ==  nil) || ( hash_user_id == nil )) {
            self.error = [NSError errorWithDomain:[NSString stringWithFormat:@"SMSRegMessage: Missing one or more of the parameters! -  %@", [url query] ] code:-1 userInfo:nil];
            return self;
        }
        
        NSString * jsonMpinId = [Utilities stringFromHexString:hexMpinId];
        NSError *error = nil;
        NSDictionary *dictMpinId = [NSJSONSerialization JSONObjectWithData:[jsonMpinId dataUsingEncoding:NSUTF8StringEncoding] options:kNilOptions error:&error];
        if(error != nil) {
            self.error = error;
            return self;
        }
        
        NSString * userID = dictMpinId[kUserId];
        if( ![self setUserID:userID forHashValue:hash_user_id]) return self;
        
        self.mpinId = hexMpinId;
        self.activateKey = activateKey;
    }
    
    return self;
}



@end
