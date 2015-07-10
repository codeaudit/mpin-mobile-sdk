/*
 * MPinSDK::IContext and all related interfaces implementation for command line test client
 */

#include "auto_context.h"
#include "http_request.h"

#include <iostream>
#include <fstream>

typedef MPinSDK::String String;
typedef MPinSDK::IHttpRequest IHttpRequest;
typedef MPinSDK::IPinPad IPinPad;
typedef MPinSDK::CryptoType CryptoType;
typedef MPinSDK::UserPtr UserPtr;

/*
 * Storage class impl
 */

class MemoryStorage : public MPinSDK::IStorage
{
public:
    MemoryStorage() {}

    virtual bool SetData(const String& data)
    {
        m_data = data;
        return true;
    }

    virtual bool GetData(String &data)
    {
        data = m_data;
        return true;
    }

    virtual const String& GetErrorMessage() const
    {
        return m_errorMessage;
    }

private:
    String m_data;
    String m_errorMessage;
};

/*
 * Pinpad class impl
 */

class AutoPinpad : public MPinSDK::IPinPad
{
public:
    void SetPin(const String& pin)
    {
        m_pin = pin;
    }

    virtual String Show(UserPtr user, Mode mode)
    {
        return m_pin;
    }

private:
    String m_pin;
};

/*
 * Context class impl
 */

AutoContext::AutoContext()
{
    m_nonSecureStorage = new MemoryStorage();
    m_secureStorage = new MemoryStorage();
    m_pinpad = new AutoPinpad();
}

AutoContext::~AutoContext()
{
    delete m_nonSecureStorage;
    delete m_secureStorage;
    delete m_pinpad;
}

IHttpRequest * AutoContext::CreateHttpRequest() const
{
    return new HttpRequest();
}

void AutoContext::ReleaseHttpRequest(IN IHttpRequest *request) const
{
    delete request;
}

MPinSDK::IStorage * AutoContext::GetStorage(IStorage::Type type) const
{
    if(type == IStorage::SECURE)
    {
        return m_secureStorage;
    }

    return m_nonSecureStorage;
}

IPinPad * AutoContext::GetPinPad() const
{
    return m_pinpad;
}

CryptoType AutoContext::GetMPinCryptoType() const
{
    return MPinSDK::CRYPTO_NON_TEE;
}

void AutoContext::SetPin(const String& pin)
{
    ((AutoPinpad *) m_pinpad)->SetPin(pin);
}
