//
//  Utilities.h
//  MPinApp
//
//  Created by Georgi Georgiev on 7/13/15.
//  Copyright (c) 2015 Certivox. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Constants.h"

@interface Utilities : NSObject

+(enum SERVICES) ServerJSONConfigTypeToService_type:(NSString*) jsonTypeName;

@end
