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
 * M-Pin SDK version 2 interface
 */

#ifndef _MPIN_SDK_V2_H_
#define _MPIN_SDK_V2_H_

#include "mpin_sdk.h"

class MPinSDKv2
{
public:
    typedef util::String String;
    typedef util::StringMap StringMap;
    typedef MPinSDK::User User;
    typedef MPinSDK::UserPtr UserPtr;
    typedef MPinSDK::CryptoType CryptoType;
    typedef MPinSDK::IHttpRequest IHttpRequest;
    typedef MPinSDK::IStorage IStorage;
    typedef MPinSDK::IPinPad IPinPad;
    typedef MPinSDK::Status Status;
    typedef MPinSDK::OTP OTP;

    class IContext
    {
    public:
        virtual ~IContext() {}
        virtual IHttpRequest * CreateHttpRequest() const = 0;
        virtual void ReleaseHttpRequest(IN IHttpRequest *request) const = 0;
        virtual IStorage * GetStorage(IStorage::Type type) const = 0;
        virtual CryptoType GetMPinCryptoType() const = 0;
    };

    MPinSDKv2();
    ~MPinSDKv2();

    Status Init(const StringMap& config, IN IContext* ctx);
    void Destroy();
    void ClearUsers();

    Status TestBackend(const String& server, const String& rpsPrefix = MPinSDK::DEFAULT_RPS_PREFIX) const;
    Status SetBackend(const String& server, const String& rpsPrefix = MPinSDK::DEFAULT_RPS_PREFIX);
    UserPtr MakeNewUser(const String& id, const String& deviceName = "") const;

    Status StartRegistration(INOUT UserPtr user, const String& userData = "");
    Status RestartRegistration(INOUT UserPtr user, const String& userData = "");
    Status ConfirmRegistration(INOUT UserPtr user);
    Status FinishRegistration(INOUT UserPtr user, const String& pin);

    Status StartAuthentication(INOUT UserPtr user);
    Status CheckAccessNumber(const String& accessNumber);
    Status FinishAuthentication(INOUT UserPtr user, const String& pin);
    Status FinishAuthentication(INOUT UserPtr user, const String& pin, OUT String& authResultData);
    Status FinishAuthenticationOTP(INOUT UserPtr user, const String& pin, OUT OTP& otp);
    Status FinishAuthenticationAN(INOUT UserPtr user, const String& pin, const String& accessNumber);

    void DeleteUser(INOUT UserPtr user);
    void ListUsers(OUT std::vector<UserPtr>& users);
    const char * GetVersion();
    bool CanLogout(IN UserPtr user);
    bool Logout(IN UserPtr user);
	String GetClientParam(const String& key);

private:
    Status FinishAuthenticationImpl(INOUT UserPtr user, const String& pin, const String& accessNumber, OUT String *otp, OUT util::JsonObject& authResultData);

    typedef MPinSDK::TimePermitCache TimePermitCache;
    typedef MPinSDK::HttpResponse HttpResponse;
    typedef MPinSDK::State State;
    typedef MPinSDK::LogoutData LogoutData;

    class Context : public MPinSDK::IContext
    {
    public:
        Context();
        ~Context();
        void Init(MPinSDKv2::IContext *appContext);
        void SetPin(const String& pin);
        virtual IHttpRequest * CreateHttpRequest() const;
        virtual void ReleaseHttpRequest(IN IHttpRequest *request) const;
        virtual IStorage * GetStorage(IStorage::Type type) const;
        virtual IPinPad * GetPinPad() const;
        virtual CryptoType GetMPinCryptoType() const;

    private:
        class Pinpad : public IPinPad
        {
        public:
            void SetPin(const String& pin) { m_pin = pin; }
            virtual String Show(UserPtr user, Mode mode) { return m_pin; }
        private:
            String m_pin;
        };

        MPinSDKv2::IContext *m_appContext;
        Pinpad *m_pinpad;
    };

    MPinSDK m_v1Sdk;
    Context m_context;
};

#endif // _MPIN_SDK_V2_H_
