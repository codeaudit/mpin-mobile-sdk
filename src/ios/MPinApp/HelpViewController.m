//
//  HelpViewController.m
//  MPinApp
//
//  Created by Georgi Georgiev on 8/31/15.
//  Copyright (c) 2015 Certivox. All rights reserved.
//

#import "HelpViewController.h"
#import "Constants.h"


@interface NSArray(Helper)
+(Boolean) isEmpty:(NSArray *) arr;
@end

@implementation NSString(Helper)
+(Boolean) isEmpty:(NSArray *) arr{
    if(arr == nil ) return YES;
    return ([arr count] == 0);
}
@end


@interface HelpViewController () {
int  listIterator;
}

- ( void ) loadData:(NSDictionary *) data;

@end

@implementation HelpViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    listIterator = 0;
    if([NSArray isEmpty:self.dataSource]) return;
    
    self.skip.hidden = NO;
    self.next.hidden = ([self.dataSource count] <= 1);
    [self loadData:[self.dataSource objectAtIndex:listIterator++]];
}

- ( void ) loadData:(NSDictionary *) data {
    if(data == nil) return;
    
    self.mainTitle.text = [data objectForKey:kHelpTitle];
    self.art.image = [UIImage imageNamed:[data objectForKey:kHelpImage]];
    self.subTitle.text = [data objectForKey:kHelpSubTitle];
    self.desc.text = [data objectForKey:kHelpDescription];
}

- ( IBAction )nextItem:( id )sender{
    [self loadData:[self.dataSource objectAtIndex:listIterator++]];
    self.next.hidden = ((listIterator + 1)>=[self.dataSource count]);
}

- ( IBAction )back:( id )sender{
    [self dismissViewControllerAnimated:NO completion:nil];
}

@end
