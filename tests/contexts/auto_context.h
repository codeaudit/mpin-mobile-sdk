/*
 * MPinSDK::IContext and all related interfaces implementation for automatic tests
 */

#ifndef _AUTO_CONTEXT_H_
#define _AUTO_CONTEXT_H_

#include "core/mpin_sdk.h"

class AutoContext : public MPinSDK::IContext
{
public:
    typedef MPinSDK::String String;
    typedef MPinSDK::IHttpRequest IHttpRequest;
    typedef MPinSDK::IStorage IStorage;
    typedef MPinSDK::IPinPad IPinPad;
    typedef MPinSDK::CryptoType CryptoType;

    AutoContext();
    ~AutoContext();
    virtual IHttpRequest * CreateHttpRequest() const;
    virtual void ReleaseHttpRequest(IN IHttpRequest *request) const;
    virtual IStorage * GetStorage(IStorage::Type type) const;
    virtual IPinPad * GetPinPad() const;
    virtual CryptoType GetMPinCryptoType() const;
    void SetPin(const String& pin);

private:
    IStorage *m_nonSecureStorage;
    IStorage *m_secureStorage;
    IPinPad *m_pinpad;
};

#endif // _AUTO_CONTEXT_H_
