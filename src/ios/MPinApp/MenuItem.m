//
//  MenuItem.m
//  MPinApp
//
//  Created by Georgi Georgiev on 9/2/15.
//  Copyright (c) 2015 Certivox. All rights reserved.
//

#import "MenuItem.h"

@implementation MenuItem
-(id) initWith:(NSString *) title controller:(UIViewController * ) controller {
    self = [super init];
    if ( self )
    {
        self.title = title;
        self.controller = controller;
    }
    return self;}

@end
