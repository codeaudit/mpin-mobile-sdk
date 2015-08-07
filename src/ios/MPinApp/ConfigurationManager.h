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


#import <Foundation/Foundation.h>

#define NOT_SELECTED -1

@interface ConfigurationManager : NSObject

+ ( ConfigurationManager * )sharedManager;

@property ( nonatomic ) NSInteger intSelectedConfiguration;
@property ( NS_NONATOMIC_IOSONLY, getter = getSelectedUserIndexforSelectedConfiguration, readonly ) NSInteger selectedUserIndexforSelectedConfiguration;
@property ( NS_NONATOMIC_IOSONLY, getter = getSelectedConfigurationIndex, readonly ) NSInteger selectedConfigurationIndex;
@property ( NS_NONATOMIC_IOSONLY, getter = getConfigurationsCount, readonly ) NSInteger configurationsCount;


- ( void )addConfiguration:( NSString * )url serviceType:( int )serviceType name:( NSString * )configurationName prefixName:( NSString * ) prefixName;
- ( void )addConfigurationWithURL:( NSString * )url serviceType:( int )serviceType name:( NSString * )configurationName;
- ( void )addConfigurationWithURL:( NSString * )url serviceType:( int )serviceType name:( NSString * )configurationName prefixName:( NSString * ) prefixName;
- ( void )saveConfigurations;
- ( BOOL )saveConfigurationAtIndex:( NSInteger )index url:( NSString * )url serviceType:( int )serviceType name:( NSString * )configurationName;
- ( BOOL )saveConfigurationAtIndex:( NSInteger )index url:( NSString * )url serviceType:( int )serviceType name:( NSString * )configurationName prefixName:( NSString * ) prefixName;
- ( BOOL )deleteConfigurationAtIndex:( NSInteger )index;
- ( BOOL )setSelectedConfiguration:( NSInteger )index;
- ( BOOL )setSelectedUserForCurrentConfiguration:( NSInteger )userIndex;
- ( BOOL )isEmpty;
- ( NSInteger ) defaultConfigCount;

- ( NSString * )getURLAtIndex:( NSInteger )index;
- ( NSString * )getNameAtIndex:( NSInteger )index;
- ( NSString * )getPrefixAtIndex:( NSInteger )index;
- ( BOOL )getIsDeviceName:( NSInteger )index;
- ( NSInteger )getConfigurationTypeAtIndex:( NSInteger )index;
- ( NSDictionary * )getSelectedConfiguration;
- ( NSDictionary * )getConfigurationAtIndex:( NSInteger ) index;

-( NSString * ) getDeviceName;
-( void ) setDeviceName:( NSString * ) devName;

-( NSInteger ) configurationExists: ( NSDictionary * ) configuration;
@end
