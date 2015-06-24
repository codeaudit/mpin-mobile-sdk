#pragma once

#include <string>
#include <collection.h>

#include "mpin_sdk.h"
#include "HttpRequest.h"
#include "Storage.h"

using namespace Platform::Collections;

/// <summary>
/// The MPinRC assembly ports all unmanaged files to managed code so the MPinSDK could be used by Windows Phone c# compiler.
/// </summary>
namespace MPinRC
{
	/// <summary>
	/// The CryptoType enumeration used for generating the supported Crypto Type on the specific platform.
	/// <remarks>Currently, only on the Android platform this method might return something different than Non-TEE Crypto. Other platforms will always return Non-TEE Crypto</remarks>
	/// </summary>
	public enum class CryptoType
	{
		CRYPTO_TEE,
		CRYPTO_NON_TEE
	};

	public enum class Mode
	{
		REGISTER,
		AUTHENTICATE
	};

#pragma region UserWrapper
	/// <summary>
	/// A wrapper class used to pass User data from managed to unmanaged User objects and vice versa.
	/// </summary>
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

#pragma region IPinPd
	/// <summary>
	/// Provides an interface to trigger the display of the PIN Pad.
	/// </summary>
	[Windows::Foundation::Metadata::WebHostHidden]
	public interface class IPinPad
	{
	public:
		virtual Platform::String^ Show(MPinRC::UserWrapper^ user, MPinRC::Mode mode) = 0;
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

		virtual MPinSDK::String Show(MPinSDK::UserPtr user, MPinSDK::IPinPad::Mode mode);
	};

#pragma endregion IPinPd

#pragma region IContext
	/// <summary>
	/// The Context Interface is the one that "bundles" all the rest of the interfaces. Only this interface is provided to the Core and the others are used/accessed through it.
	/// </summary>
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

#pragma region StatusWrapper
	/// <summary>
	/// A wrapper class used to pass Status data from managed to unmanaged Status objects and vice versa.
	/// </summary>
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
	/// <summary>
	/// A wrapper class used to pass OTP data from managed to unmanaged OTP objects and vice versa.
	/// </summary>
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
	/// <summary>
	/// A wrapper class used to pass the MPin SDK fields and methods from managed to unmanaged objects and vice versa.
	/// </summary>
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
		Platform::String^ GetClientParam(Platform::String^ key);
	};
#pragma endregion MPinWrapper
}