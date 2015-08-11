/*
 * MPinSDKv2::IContext and all related interfaces implementation for command line test client
 */

#ifndef _CMDLINE_CONTEXT_V2_H_
#define _CMDLINE_CONTEXT_V2_H_

#include "core/mpin_sdk_v2.h"

class CmdLineContextV2 : public MPinSDKv2::IContext
{
public:
    typedef MPinSDKv2::String String;
    typedef MPinSDKv2::IHttpRequest IHttpRequest;
    typedef MPinSDKv2::IStorage IStorage;
    typedef MPinSDKv2::CryptoType CryptoType;

    CmdLineContextV2(const String& usersFile, const String& tokensFile);
    ~CmdLineContextV2();
    virtual IHttpRequest * CreateHttpRequest() const;
    virtual void ReleaseHttpRequest(IN IHttpRequest *request) const;
    virtual IStorage * GetStorage(IStorage::Type type) const;
    virtual CryptoType GetMPinCryptoType() const;

private:
    IStorage *m_nonSecureStorage;
    IStorage *m_secureStorage;
};

#endif // _CMDLINE_CONTEXT_V2_H_
