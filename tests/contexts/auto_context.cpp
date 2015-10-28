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
 * MPinSDK::IContext and all related interfaces implementation for command line test client
 */

#include "auto_context.h"
#include "../common/http_request.h"
#include "../common/memory_storage.h"

#include <iostream>
#include <fstream>

typedef MPinSDK::String String;
typedef MPinSDK::IHttpRequest IHttpRequest;
typedef MPinSDK::IPinPad IPinPad;
typedef MPinSDK::CryptoType CryptoType;
typedef MPinSDK::UserPtr UserPtr;

/*
 * Pinpad class impl
 */

class AutoPinpad : public MPinSDK::IPinPad
{
public:
    void SetPin(const String& pin)
    {
        m_pin = pin;
    }

    virtual String Show(UserPtr user, Mode mode)
    {
        return m_pin;
    }

private:
    String m_pin;
};

/*
 * Context class impl
 */

AutoContext::AutoContext()
{
    m_nonSecureStorage = new MemoryStorage();
    m_secureStorage = new MemoryStorage();
    m_pinpad = new AutoPinpad();
}

AutoContext::~AutoContext()
{
    delete m_nonSecureStorage;
    delete m_secureStorage;
    delete m_pinpad;
}

IHttpRequest * AutoContext::CreateHttpRequest() const
{
    return new HttpRequest();
}

void AutoContext::ReleaseHttpRequest(IN IHttpRequest *request) const
{
    delete request;
}

MPinSDK::IStorage * AutoContext::GetStorage(IStorage::Type type) const
{
    if(type == IStorage::SECURE)
    {
        return m_secureStorage;
    }

    return m_nonSecureStorage;
}

IPinPad * AutoContext::GetPinPad() const
{
    return m_pinpad;
}

CryptoType AutoContext::GetMPinCryptoType() const
{
    return MPinSDK::CRYPTO_NON_TEE;
}

void AutoContext::SetPin(const String& pin)
{
    ((AutoPinpad *) m_pinpad)->SetPin(pin);
}
