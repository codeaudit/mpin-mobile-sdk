/*
 * M-Pin SDK version 2 interface
 */

#ifndef _MPIN_SDK_V2_H_
#define _MPIN_SDK_V2_H_

#include "mpin_sdk.h"

class IMPinCryptoV2;

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
    typedef MPinSDK::IContext IContext;
    typedef MPinSDK::Status Status;
    typedef MPinSDK::OTP OTP;

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

    class Context : public IContext
    {
    public:
        Context();
        ~Context();
        void Init(IContext *appContext);
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

        IContext *m_appContext;
        Pinpad *m_pinpad;
    };

    MPinSDK m_v1Sdk;
    Context m_context;
};

#endif // _MPIN_SDK_V2_H_
