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

/*
 * Internal M-Pin Crypto interface
 */

#ifndef _MPIN_CRYPTO_H_
#define _MPIN_CRYPTO_H_

#include <string>
#include <vector>

#include "mpin_sdk.h"


class IMPinCrypto
{
public:
    typedef MPinSDK::String String;
    typedef MPinSDK::Status Status;
    typedef MPinSDK::UserPtr UserPtr;

    virtual ~IMPinCrypto() {}

    virtual Status OpenSession() = 0;
    virtual void CloseSession() = 0;
    virtual Status Register(IN UserPtr user, IN std::vector<String>& clientSecretShares) = 0;
    virtual Status AuthenticatePass1(IN UserPtr user, IN std::vector<String>& timePermitShares, OUT String& commitmentU, OUT String& commitmentUT) = 0;
    virtual Status AuthenticatePass2(IN UserPtr user, const String& challenge, OUT String& validator) = 0;
    virtual void DeleteToken(const String& mpinId) = 0;
    
    virtual Status SaveRegOTT(const String& mpinId, const String& regOTT) = 0;
    virtual Status LoadRegOTT(const String& mpinId, OUT String& regOTT) = 0;
    virtual Status DeleteRegOTT(const String& mpinId) = 0;
};


#endif // _MPIN_CRYPTO_H_
