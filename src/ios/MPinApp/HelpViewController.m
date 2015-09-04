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

#import "HelpViewController.h"
#import "Constants.h"
#import "MenuViewController.h"
#import "MFSideMenu.h"


@interface NSArray(Helper)
+(Boolean) isEmpty:(NSArray *) arr;
@end

@implementation NSArray(Helper)
+(Boolean) isEmpty:(NSArray *) arr{
    if(arr == nil ) return YES;
    return ([arr count] == 0);
}
@end


@interface HelpViewController () {
int  listIterator;
}

- ( void ) loadData:(NSDictionary *) data;
- ( void ) hideNavButtons:(BOOL) hide;
- ( void ) invalidate;
@end

@implementation HelpViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    [self invalidate];
}

- ( IBAction )nextItem:( id )sender {
    [self loadData:[self.dataSource objectAtIndex:listIterator++]];
    [self hideNavButtons:((listIterator + 1)>=[self.dataSource count])];
}

- ( IBAction )skip:( id )sender {
    
    if (self.presentingViewController != nil) {
        [self dismissViewControllerAnimated:NO completion:nil];
        return;
    }
    
    [self invalidate];
    [self.menuContainerViewController  toggleLeftSideMenuCompletion:nil];
}

- ( IBAction )done:(id)sender {
    
    if (self.presentingViewController != nil) {
        [self dismissViewControllerAnimated:NO completion:nil];
        return;
    }
    
    [self invalidate];
    [self.menuContainerViewController toggleLeftSideMenuCompletion:nil];
}

- ( void ) loadData:(NSDictionary *) data {
    if(data == nil) return;
    
    self.mainTitle.text = [data objectForKey:kHelpTitle];
    self.art.image = [UIImage imageNamed:[data objectForKey:kHelpImage]];
    self.subTitle.text = [data objectForKey:kHelpSubTitle];
    self.desc.text = [data objectForKey:kHelpDescription];
}

- ( void ) hideNavButtons:(BOOL) hide {
    self.skip.hidden = hide;
    self.next.hidden = hide;
    self.done.hidden = !hide;
}

- ( void ) invalidate {
    listIterator = 0;
    if([NSArray isEmpty:self.dataSource]) return;
    [self hideNavButtons:([self.dataSource count] <= 1)];
    [self loadData:[self.dataSource objectAtIndex:listIterator++]];
}

@end
