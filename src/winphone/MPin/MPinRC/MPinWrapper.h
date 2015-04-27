#pragma once

#include <string>
#include <collection.h>

#include "mpin_sdk.h"
#include "HttpRequest.h"
#include "Storage.h"

using namespace Platform::Collections;
//using namespace System::Runtime::CompilerServices;
//[assembly:InternalsVisibleTo("")]

namespace MPinRC
{
	public enum class CryptoType
	{
		CRYPTO_TEE,
		CRYPTO_NON_TEE
	};

#pragma region IPinPd
	[Windows::Foundation::Metadata::WebHostHidden]
	public interface class IPinPad
	{
	public:
		//virtual IAsyncOperation<Platform::String^> ShowAsync() = 0;
		virtual Platform::String^ Show() = 0;
		virtual void SetUiDispatcher(Windows::UI::Core::CoreDispatcher^ dispatcher) = 0;
	};

	class PinPadProxy : public MPinSDK::IPinPad
	{
	private:
		MPinRC::IPinPad^ managedPinPad;

	public:
		PinPadProxy(){};
		PinPadProxy(MPinRC::IPinPad^ pinPad);

		void SetPinPad(MPinRC::IPinPad^ pinPad);

		virtual MPinSDK::String Show();
	};

#pragma endregion IPinPd

#pragma region IContext
	[Windows::Foundation::Metadata::WebHostHidden]
	public interface class IContext
	{
	public:
		virtual IHttpRequest^ CreateHttpRequest() = 0;
		virtual void ReleaseHttpRequest(IHttpRequest^ request) = 0;
		virtual IStorage^ GetStorage(MPinRC::StorageType type) = 0;
		virtual IPinPad^ GetPinPad() = 0;
		virtual CryptoType GetMPinCryptoType() = 0;
	};

	class ContextProxy : public MPinSDK::IContext
	{
	private:
		MPinRC::IContext^ managedContext;
		PinPadProxy pinPadProxy;

	public:
		ContextProxy(MPinRC::IContext^ context);

		typedef MPinSDK::IHttpRequest IHttpRequest;
		typedef MPinSDK::IPinPad IPinPad;
		typedef MPinSDK::CryptoType CryptoType;
		typedef MPinSDK::IStorage IStorage;

		virtual IHttpRequest * CreateHttpRequest() const;
		virtual void ReleaseHttpRequest(IN IHttpRequest *request) const;
		virtual IStorage * GetStorage(IStorage::Type type) const;
		virtual IPinPad * GetPinPad() const;
		virtual CryptoType GetMPinCryptoType() const;
	};

#pragma endregion IContext

#pragma region UserWrapper
	public ref class UserWrapper sealed
	{
	internal:
		MPinSDK::UserPtr user;
		UserWrapper(MPinSDK::UserPtr);

	public:
		Platform::String^ GetId();
		int GetState();
		void Destruct();
	};
#pragma endregion UserWrapper

#pragma region StatusWrapper
	public ref class StatusWrapper sealed
	{
	private:
		MPinSDK::Status status;

	internal:
		StatusWrapper(MPinSDK::Status::Code code) : status(code) {}
		StatusWrapper(MPinSDK::Status::Code code, MPinSDK::String error) : status(code, error) {}
		static MPinSDK::Status::Code ToCode(int codeInt);

	public:
		StatusWrapper() {}

		property int Code
		{
			int get() { return status.GetStatusCode(); }
			void set(int value)
			{
				status.SetStatusCode(StatusWrapper::ToCode(value));
			}
		}

		property Platform::String^ Error
		{
			Platform::String^ get();
			void set(Platform::String^ value);
		}

	};
#pragma endregion StatusWrapper

#pragma region OTPWrapper
	public ref class OTPWrapper sealed
	{
	internal:
		MPinSDK::OTP otp;

	public:
		property Platform::String^ Otp
		{
			Platform::String^ get();
			void set(Platform::String^ value);
		}

		property int64 ExpireTime
		{
			int64 get() { return otp.expireTime; }
			void set(int64 value) { otp.expireTime = value; }
		}

		property int TtlSeconds
		{
			int get() { return otp.ttlSeconds; }
			void set(int value) { otp.ttlSeconds = value; }
		}

		property int64 NowTime
		{
			int64 get() { return otp.nowTime; }
			void set(int64 value) { otp.nowTime = value; }
		}

		property MPinRC::StatusWrapper^ Status
		{
			MPinRC::StatusWrapper^ get();
			void set(MPinRC::StatusWrapper^ value);
		}
	};
#pragma endregion OTPWrapper

#pragma region MPinWrapper
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class MPinWrapper sealed
	{
	private:
		MPinSDK* sdk;
		MPinRC::ContextProxy* proxy;
		void Log(Object^);
	internal:
		static MPinSDK::StringMap ToNativeStringMap(Windows::Foundation::Collections::IMap<Platform::String^, Platform::String^>^ managedMap);
		static Platform::String^ ToStringHat(MPinSDK::String text);
		static MPinSDK::String ToNativeString(Platform::String^ text);
		static Windows::Foundation::Collections::IMap<Platform::String^, Platform::String^>^ ToManagedMap(const MPinSDK::StringMap& nMap);

	public:
		MPinWrapper();
		virtual ~MPinWrapper();
		void Destroy();
		void ClearUsers();

		MPinRC::StatusWrapper^ Construct(Windows::Foundation::Collections::IMap<Platform::String^, Platform::String^>^ config, MPinRC::IContext^ context);
		void ListUsers(Windows::Foundation::Collections::IVector<UserWrapper^>^ users);

		UserWrapper^ MakeNewUser(Platform::String^ id, Platform::String^ deviceName);
		void DeleteUser(UserWrapper^ user);
		MPinRC::StatusWrapper^ ResetPin(UserWrapper^ user);
		MPinRC::StatusWrapper^ StartRegistration(MPinRC::UserWrapper^ user, Platform::String^ userData);
		MPinRC::StatusWrapper^ RestartRegistration(MPinRC::UserWrapper^ user, Platform::String^ userData);
		MPinRC::StatusWrapper^ FinishRegistration(MPinRC::UserWrapper^ user);

		MPinRC::StatusWrapper^ Authenticate(MPinRC::UserWrapper^ user);
		MPinRC::StatusWrapper^ AuthenticateResultData(MPinRC::UserWrapper^ user, Platform::String^ authResultData);
		MPinRC::StatusWrapper^ AuthenticateOTP(MPinRC::UserWrapper^ user, MPinRC::OTPWrapper^ otp);
		MPinRC::StatusWrapper^ AuthenticateAN(MPinRC::UserWrapper^ user, Platform::String^ accessNumber);

		MPinRC::StatusWrapper^ TestBackend(Platform::String^ server, Platform::String^ rpsPrefix);
		MPinRC::StatusWrapper^ SetBackend(Platform::String^ server, Platform::String^ rpsPrefix);

		bool CanLogout(UserWrapper^ user);
		bool Logout(UserWrapper^ user);
	};
#pragma endregion MPinWrapper
}