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

#include "cmdline_context.h"
#include "../common/http_request.h"
#include "../common/file_storage.h"

#include <iostream>
#include <fstream>

typedef MPinSDK::String String;
typedef MPinSDK::IHttpRequest IHttpRequest;
typedef MPinSDK::IPinPad IPinPad;
typedef MPinSDK::CryptoType CryptoType;
typedef MPinSDK::UserPtr UserPtr;
using namespace std;

/*
 * Pinpad class impl
 */

class CmdLinePinpad : public MPinSDK::IPinPad
{
public:
    virtual String Show(UserPtr user, Mode mode)
    {
        String pin;
        cout << "Enter pin: ";
        cin >> pin;

        // Special character to simulate PIN_INPUT_CANCELED
        if(pin == "c")
        {
            pin.clear();
        }

        return pin;
    }
};

/*
 * Context class impl
 */

CmdLineContext::CmdLineContext(const String& usersFile, const String& tokensFile)
{
    m_nonSecureStorage = new FileStorage(usersFile);
    m_secureStorage = new FileStorage(tokensFile);
    m_pinpad = new CmdLinePinpad();
}

CmdLineContext::~CmdLineContext()
{
    delete m_nonSecureStorage;
    delete m_secureStorage;
    delete m_pinpad;
}

IHttpRequest * CmdLineContext::CreateHttpRequest() const
{
    return new HttpRequest();
}

void CmdLineContext::ReleaseHttpRequest(IN IHttpRequest *request) const
{
    delete request;
}

MPinSDK::IStorage * CmdLineContext::GetStorage(IStorage::Type type) const
{
    if(type == IStorage::SECURE)
    {
        return m_secureStorage;
    }

    return m_nonSecureStorage;
}

IPinPad * CmdLineContext::GetPinPad() const
{
    return m_pinpad;
}

CryptoType CmdLineContext::GetMPinCryptoType() const
{
    return MPinSDK::CRYPTO_NON_TEE;
}
