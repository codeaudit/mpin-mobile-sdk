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

#include "cmdline_context_v2.h"
#include "http_request.h"

#include <iostream>
#include <fstream>

typedef MPinSDKv2::String String;
typedef MPinSDKv2::IHttpRequest IHttpRequest;
typedef MPinSDKv2::CryptoType CryptoType;
typedef MPinSDKv2::UserPtr UserPtr;
using namespace std;

/*
 * Storage class impl
 */

class FileStorageV2 : public MPinSDKv2::IStorage
{
public:
    FileStorageV2(const String& fileName) : m_fileName(fileName) {}

    virtual bool SetData(const String& data)
    {
        fstream file(m_fileName.c_str(), fstream::out);
        file.clear();
        file.seekp(fstream::beg);
        file << data;
        file.close();

        return true;
    }

    virtual bool GetData(String &data)
    {
        fstream file(m_fileName.c_str(), fstream::in);
        stringbuf buf;
        file >> &buf;
        data = buf.str();
        file.close();

        return true;
    }

    virtual const String& GetErrorMessage() const
    {
        return m_errorMessage;
    }

private:
    String m_fileName;
    String m_errorMessage;
};

/*
 * Context class impl
 */

CmdLineContextV2::CmdLineContextV2(const String& usersFile, const String& tokensFile)
{
    m_nonSecureStorage = new FileStorageV2(usersFile);
    m_secureStorage = new FileStorageV2(tokensFile);
}

CmdLineContextV2::~CmdLineContextV2()
{
    delete m_nonSecureStorage;
    delete m_secureStorage;
}

IHttpRequest * CmdLineContextV2::CreateHttpRequest() const
{
    return new HttpRequest();
}

void CmdLineContextV2::ReleaseHttpRequest(IN IHttpRequest *request) const
{
    delete request;
}

MPinSDK::IStorage * CmdLineContextV2::GetStorage(IStorage::Type type) const
{
    if(type == IStorage::SECURE)
    {
        return m_secureStorage;
    }

    return m_nonSecureStorage;
}

CryptoType CmdLineContextV2::GetMPinCryptoType() const
{
    return MPinSDK::CRYPTO_NON_TEE;
}
