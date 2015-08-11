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
