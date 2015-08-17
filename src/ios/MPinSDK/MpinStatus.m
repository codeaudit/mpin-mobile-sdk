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


#import "MpinStatus.h"

@implementation MpinStatus

-(id) initWith:(MPinStatus)status errorMessage:(NSString*) error {
    self = [super init];
    if (self) {
        self.status = status;
        self.errorMessage = error;
    }
    return self;

}

- (NSString *) getStatusCodeAsString {
    NSString * result = @"";
    switch (self.status) {
        case OK:
            result = @"KEY_BTNOK";
            break;
        case PIN_INPUT_CANCELED:
            result = @"PIN_INPUT_CANCELED";
            break;
        case CRYPTO_ERROR:
            result = @"CRYPTO_ERROR";
            break;
        case STORAGE_ERROR:
            result = @"STORAGE_ERROR";
            break;
        case NETWORK_ERROR:
            result = @"NETWORK_ERROR";
            break;
        case RESPONSE_PARSE_ERROR:
            result = @"RESPONSE_PARSE_ERROR";
            break;
        case FLOW_ERROR:
            result = @"FLOW_ERROR";
            break;
        case IDENTITY_NOT_AUTHORIZED:
            result = @"IDENTITY_NOT_AUTHORIZED";
            break;
        case IDENTITY_NOT_VERIFIED:
            result = @"IDENTITY_NOT_VERIFIED";
            break;
        case REQUEST_EXPIRED:
            result = @"REQUEST_EXPIRED";
            break;
        case REVOKED:
            result = @"REVOKED";
            break;
        case INCORRECT_PIN:
            result = @"INCORRECT_PIN";
            break;
        case INCORRECT_ACCESS_NUMBER:
            result = @"INCORRECT_ACCESS_NUMBER";
            break;
        case HTTP_SERVER_ERROR:
            result = @"HTTP_SERVER_ERROR";
            break;
        case HTTP_REQUEST_ERROR:
            result = @"HTTP_REQUEST_ERROR";
            break;
        default:
            break;
    }
    return result;
}

@end
