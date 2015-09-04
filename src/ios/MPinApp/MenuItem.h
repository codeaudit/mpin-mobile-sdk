//
//  MenuItem.h
//  MPinApp
//
//  Created by Georgi Georgiev on 9/2/15.
//  Copyright (c) 2015 Certivox. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface MenuItem : NSObject
@property(nonatomic, retain) NSString *title;
@property(nonatomic, retain) UIViewController * controller;

-(id) initWith:(NSString *) title controller:(UIViewController * ) controller;
@end
