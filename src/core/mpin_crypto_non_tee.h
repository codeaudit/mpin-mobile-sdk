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
