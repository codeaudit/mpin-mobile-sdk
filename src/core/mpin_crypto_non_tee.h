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
 * Internal M-Pin Crypto interface Non-TEE implementation
 */

#ifndef _MPIN_CRYPTO_NON_TEE_H_
#define _MPIN_CRYPTO_NON_TEE_H_

#include "mpin_crypto.h"
extern "C"
{
#include "crypto/mpin.h"
}

class MPinCryptoNonTee : public IMPinCrypto
{
public:
    typedef MPinSDK::IPinPad IPinPad;
    typedef MPinSDK::IStorage IStorage;
    typedef util::JsonObject JsonObject;
    typedef MPinSDK::UserPtr UserPtr;

    MPinCryptoNonTee();
    ~MPinCryptoNonTee();

    Status Init(IN IPinPad *pinpad, IN IStorage *storage);
    void Destroy();

    virtual Status OpenSession();
    virtual void CloseSession();
    virtual Status Register(IN UserPtr user, IN std::vector<String>& clientSecretShares);
    virtual Status AuthenticatePass1(IN UserPtr user, IN std::vector<String>& timePermitShares, OUT String& commitmentU, OUT String& commitmentUT);
    virtual Status AuthenticatePass2(IN UserPtr user, const String& challenge, OUT String& validator);
    virtual void DeleteToken(const String& mpinId);

	virtual Status SaveRegOTT(const String& mpinId, const String& regOTT);
    virtual Status LoadRegOTT(const String& mpinId, OUT String& regOTT);
    virtual Status DeleteRegOTT(const String& mpinId);

private:
    bool StoreToken(const String& mpinId, const String& token);
    String GetToken(const String& mpinId);
    void SaveDataForPass2(const String& mpinId, const String& clientSecret, const String& x);
    void ForgetPass2Data();
    static void GenerateRandomSeed(OUT char *buf, size_t len);

private:
    IPinPad *m_pinPad;
    IStorage *m_storage;
    bool m_initialized;
    bool m_sessionOpened;
    String m_mpinId;
    String m_clientSecret;
    String m_x;
    JsonObject m_tokens;
};


#endif // _MPIN_CRYPTO_NON_TEE_H_
