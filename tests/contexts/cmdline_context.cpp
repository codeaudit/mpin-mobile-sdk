/*
 * MPinSDK::IContext and all related interfaces implementation for command line test client
 */

#include "cmdline_context.h"
#include "http_request.h"

#include <iostream>
#include <fstream>

typedef MPinSDK::String String;
typedef MPinSDK::IHttpRequest IHttpRequest;
typedef MPinSDK::IPinPad IPinPad;
typedef MPinSDK::CryptoType CryptoType;
using namespace std;

/*
 * Storage class impl
 */

class FileStorage : public MPinSDK::IStorage
{
public:
    FileStorage(const String& fileName) : m_fileName(fileName) {}

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
 * Pinpad class impl
 */

class CmdLinePinpad : public MPinSDK::IPinPad
{
public:
    virtual String Show(Context context)
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
