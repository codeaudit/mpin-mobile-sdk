/*
 * MPinSDK::IContext and all related interfaces implementation for command line test client
 */

#ifndef _CMDLINE_CONTEXT_H_
#define _CMDLINE_CONTEXT_H_

#include "core/mpin_sdk.h"

class CmdLineContext : public MPinSDK::IContext
{
public:
    typedef MPinSDK::String String;
    typedef MPinSDK::IHttpRequest IHttpRequest;
    typedef MPinSDK::IStorage IStorage;
    typedef MPinSDK::IPinPad IPinPad;
    typedef MPinSDK::CryptoType CryptoType;

    CmdLineContext(const String& usersFile, const String& tokensFile);
    ~CmdLineContext();
    virtual IHttpRequest * CreateHttpRequest() const;
    virtual void ReleaseHttpRequest(IN IHttpRequest *request) const;
    virtual IStorage * GetStorage(IStorage::Type type) const;
    virtual IPinPad * GetPinPad() const;
    virtual CryptoType GetMPinCryptoType() const;

private:
    IStorage *m_nonSecureStorage;
    IStorage *m_secureStorage;
    IPinPad *m_pinpad;
};

#endif // _CMDLINE_CONTEXT_H_
