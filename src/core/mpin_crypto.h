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

    virtual ~IMPinCrypto() {}

    virtual Status OpenSession() = 0;
    virtual void CloseSession() = 0;
    virtual Status Register(const String& mpinId, IN std::vector<String>& clientSecretShares) = 0;
    virtual Status AuthenticatePass1(const String& mpinId, IN std::vector<String>& timePermitShares, OUT String& commitmentU, OUT String& commitmentUT) = 0;
    virtual Status AuthenticatePass2(const String& mpinId, const String& challenge, OUT String& validator) = 0;
    virtual void DeleteToken(const String& mpinId) = 0;
    
    virtual Status SaveRegOTT(const String& mpinId, const String& regOTT) = 0;
    virtual Status LoadRegOTT(const String& mpinId, OUT String& regOTT) = 0;
    virtual Status DeleteRegOTT(const String& mpinId) = 0;
};


#endif // _MPIN_CRYPTO_H_
