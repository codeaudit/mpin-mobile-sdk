#include "MPinWrapper.h"
#include <string>
#include <ctime>
// for debugging only
#include <windows.h>
#include <codecvt>

using namespace MPinRC;
using namespace Platform;
using namespace Platform::Collections;
using namespace std;

#pragma region MPinWrapper
MPinWrapper::MPinWrapper() : sdk(new MPinSDK) {}

MPinWrapper::~MPinWrapper()
{
	if (sdk != nullptr)
		delete sdk;

	if (proxy != nullptr)
		delete sdk;
}

MPinRC::StatusWrapper^ MPinWrapper::Construct(Windows::Foundation::Collections::IMap<Platform::String^, Platform::String^>^ config, MPinRC::IContext^ context)
{
	MPinSDK::StringMap map = ToNativeStringMap(config);
	this->proxy = new ContextProxy(context);
	MPinSDK::Status s = sdk->Init(map, proxy);

	return ref new StatusWrapper(s.GetStatusCode(), s.GetErrorMessage());
}

void MPinWrapper::ListUsers(Windows::Foundation::Collections::IVector<UserWrapper^>^ users)
{
	std::vector<MPinSDK::UserPtr> _users;
	sdk->ListUsers(_users);
	for each (MPinSDK::UserPtr user in _users)
	{
		MPinRC::UserWrapper^ uw = ref new MPinRC::UserWrapper(user);
		users->Append(uw);
	}
}

void MPinWrapper::Destroy()
{
	this->sdk->Destroy();
}

void MPinWrapper::ClearUsers()
{
	this->sdk->ClearUsers();
}

UserWrapper^ MPinWrapper::MakeNewUser(Platform::String^ id, Platform::String^ deviceName)
{
	MPinSDK::String nativeID = ToNativeString(id);

	/*String^ sss = "start init to ";
	auto dbg = String::Concat(sss, id);
	Log(dbg);*/

	MPinSDK::String nativeDeviceName = ToNativeString(deviceName);

	MPinSDK::UserPtr userPtr = this->sdk->MakeNewUser(nativeID, nativeDeviceName);
	MPinRC::UserWrapper^ uw = ref new MPinRC::UserWrapper(userPtr);

	/*Platform::String^ idddd = uw->GetId();
	String^ ss = "end inited to ";
	dbg = String::Concat(ss, idddd);
	Log(dbg);*/

	return uw;
}

MPinSDK::StringMap MPinWrapper::ToNativeStringMap(Windows::Foundation::Collections::IMap<Platform::String^, Platform::String^>^ managedMap)
{
	MPinSDK::StringMap map = MPinSDK::StringMap();
	for (auto pair : managedMap)
	{
		MPinSDK::String key = MPinWrapper::ToNativeString(pair->Key);
		MPinSDK::String value = MPinWrapper::ToNativeString(pair->Value);
		std::pair<MPinSDK::String, MPinSDK::String> nPair(key, value);
		map.insert(nPair);
	}

	return map;
}

Windows::Foundation::Collections::IMap<Platform::String^, Platform::String^>^ MPinWrapper::ToManagedMap(const MPinSDK::StringMap& nMap)
{
	Windows::Foundation::Collections::IMap<Platform::String^, Platform::String^>^ map = ref new Platform::Collections::Map<Platform::String^, Platform::String^>();

	for each(auto pair in nMap)
	{
		Platform::String^ key = MPinWrapper::ToStringHat(pair.first);
		Platform::String^ value = MPinWrapper::ToStringHat(pair.second);

		map->Insert(key, value);
	}

	return map;
}

MPinSDK::String MPinWrapper::ToNativeString(Platform::String^ text)
{
	std::wstring textWString(text->Begin());
	std::string textStr(textWString.begin(), textWString.end());
	return textStr;
}

Platform::String^ MPinWrapper::ToStringHat(MPinSDK::String text)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> convert;
	std::wstring textStr = convert.from_bytes(text.data());
	return ref new Platform::String(textStr.c_str());
}

void MPinWrapper::Log(Object^ parameter)
{
	auto paraString = parameter->ToString();
	auto formattedTest = std::wstring(paraString->Data()).append(L"\r\n");
	OutputDebugString(formattedTest.c_str());
}

void MPinWrapper::DeleteUser(MPinRC::UserWrapper^ user)
{
	MPinSDK::UserPtr up = (MPinSDK::UserPtr)user->user;
	sdk->DeleteUser(up);
}

MPinRC::StatusWrapper^ MPinWrapper::StartRegistration(MPinRC::UserWrapper^ user, Platform::String^ userData)
{
	MPinSDK::String userStringData = ToNativeString(userData);
	MPinSDK::Status st = sdk->StartRegistration(user->user, userStringData);
	return ref new MPinRC::StatusWrapper(st.GetStatusCode(), st.GetErrorMessage());
}

MPinRC::StatusWrapper^ MPinWrapper::RestartRegistration(MPinRC::UserWrapper^ user, Platform::String^ userData)
{
	MPinSDK::String userStringData = ToNativeString(userData);
	MPinSDK::Status st = sdk->RestartRegistration(user->user, userStringData);
	return ref new MPinRC::StatusWrapper(st.GetStatusCode(), st.GetErrorMessage());
}

MPinRC::StatusWrapper^ MPinWrapper::FinishRegistration(MPinRC::UserWrapper^ user)
{
	MPinSDK::Status st = sdk->FinishRegistration(user->user);
	return ref new MPinRC::StatusWrapper(st.GetStatusCode(), st.GetErrorMessage());
}

MPinRC::StatusWrapper^ MPinWrapper::Authenticate(MPinRC::UserWrapper^ user)
{
	MPinSDK::Status st = sdk->Authenticate(user->user);
	return ref new StatusWrapper(st.GetStatusCode(), st.GetErrorMessage());
}

MPinRC::StatusWrapper^ MPinWrapper::AuthenticateResultData(MPinRC::UserWrapper^ user, Platform::String^ authResultData)
{
	MPinSDK::String aRD = ToNativeString(authResultData);
	MPinSDK::Status st = sdk->Authenticate(user->user, aRD);
	return ref new StatusWrapper(st.GetStatusCode(), st.GetErrorMessage());
}

MPinRC::StatusWrapper^ MPinWrapper::AuthenticateOTP(MPinRC::UserWrapper^ user, MPinRC::OTPWrapper^ otp)
{
	if (otp == nullptr)
	{
		//return Authenticate(user);

		throw ref new InvalidArgumentException("OTP should not be null!");
	}

	//MPinSDK::OTP& otpPtr = otp.otp;

	//MPinSDK::OTP otpPtr;
	MPinSDK::Status st = sdk->AuthenticateOTP(user->user, otp->otp);
	/*otp = ref new OTPWrapper();
	if (st == MPinSDK::Status::OK)
	{
		otp->ExpireTime = otpPtr.expireTime;
		otp->NowTime = otpPtr.nowTime;
		otp->Otp = MPinWrapper::ToStringHat(otpPtr.otp);
		otp->Status = ref new StatusWrapper(otpPtr.status.GetStatusCode(), otpPtr.status.GetErrorMessage());
		otp->TtlSeconds = otpPtr.ttlSeconds;
	}*/

	return ref new StatusWrapper(st.GetStatusCode(), st.GetErrorMessage());
}

MPinRC::StatusWrapper^ MPinWrapper::AuthenticateAN(MPinRC::UserWrapper^ user, Platform::String^ accessNumber)
{
	const MPinSDK::String accessNumberString = MPinWrapper::ToNativeString(accessNumber);	
	MPinSDK::Status st = sdk->AuthenticateAN(user->user, accessNumberString);
	return ref new StatusWrapper(st.GetStatusCode(), st.GetErrorMessage());
}

MPinRC::StatusWrapper^ MPinWrapper::TestBackend(Platform::String^ server, Platform::String^ rpsPrefix)
{
	MPinSDK::String ntvServer = ToNativeString(server);
	MPinSDK::String ntvRpsPrefix = ToNativeString(rpsPrefix);
	MPinSDK::Status st = this->sdk->TestBackend(ntvServer, ntvRpsPrefix);
	return ref new MPinRC::StatusWrapper(st.GetStatusCode(), st.GetErrorMessage());
}

MPinRC::StatusWrapper^ MPinWrapper::SetBackend(Platform::String^ server, Platform::String^ rpsPrefix)
{
	MPinSDK::String ntvServer = ToNativeString(server);
	MPinSDK::String ntvRpsPrefix = ToNativeString(rpsPrefix);
	MPinSDK::Status st = this->sdk->SetBackend(ntvServer, ntvRpsPrefix);
	return ref new MPinRC::StatusWrapper(st.GetStatusCode(), (st.GetErrorMessage()));
}

bool MPinWrapper::CanLogout(UserWrapper^ user)
{
	return this->sdk->CanLogout(user->user);
}

bool MPinWrapper::Logout(UserWrapper^ user)
{
	return this->sdk->Logout(user->user);
}

Platform::String^ MPinWrapper::GetClientParam(Platform::String^ key)
{
	MPinSDK::String nKey = MPinWrapper::ToNativeString(key);
	return MPinWrapper::ToStringHat(this->sdk->GetClientParam(nKey));
}
#pragma endregion MPinWrapper

#pragma region UserWrapper

UserWrapper::UserWrapper(MPinSDK::UserPtr ptr)
{
	this->user = ptr;
}

Platform::String^ UserWrapper::GetId()
{
	return MPinWrapper::ToStringHat(user->GetId());
}

int UserWrapper::GetState()
{
	return this->user->GetState();
}

void UserWrapper::Destruct()
{
}
#pragma endregion UserWrapper

#pragma region StatusWrapper
MPinSDK::Status::Code StatusWrapper::ToCode(int codeInt)
{
	switch (codeInt)
	{
	case 1:
		return MPinSDK::Status::Code::PIN_INPUT_CANCELED;
	case 2:
		return MPinSDK::Status::Code::CRYPTO_ERROR;
	case 3:
		return MPinSDK::Status::Code::STORAGE_ERROR;
	case 4:
		return MPinSDK::Status::Code::NETWORK_ERROR;
	case 5:
		return MPinSDK::Status::Code::RESPONSE_PARSE_ERROR;
	case 6:
		return MPinSDK::Status::Code::FLOW_ERROR;
	case 7:
		return MPinSDK::Status::Code::IDENTITY_NOT_AUTHORIZED;
	case 8:
		return MPinSDK::Status::Code::IDENTITY_NOT_VERIFIED;
	case 9:
		return MPinSDK::Status::Code::REQUEST_EXPIRED;
	case 10:
		return MPinSDK::Status::Code::REVOKED;
	case 11:
		return MPinSDK::Status::Code::INCORRECT_PIN;
	case 12:
		return MPinSDK::Status::Code::INCORRECT_ACCESS_NUMBER;
	case 13:
		return MPinSDK::Status::Code::HTTP_SERVER_ERROR;
	case 14:
		return MPinSDK::Status::Code::HTTP_REQUEST_ERROR;
	default:
		return MPinSDK::Status::Code::OK;
	}
}

Platform::String^ StatusWrapper::Error::get()
{
	return MPinWrapper::ToStringHat(status.GetErrorMessage());
}

void StatusWrapper::Error::set(Platform::String^ value)
{
	status.SetErrorMessage(MPinWrapper::ToNativeString(value));
}

#pragma endregion StatusWrapper

#pragma region OtpWrapper

Platform::String^ OTPWrapper::Otp::get()
{
	return MPinWrapper::ToStringHat(otp.otp);
}
void OTPWrapper::Otp::set(Platform::String^ value)
{
	otp.otp = MPinWrapper::ToNativeString(value);
}

MPinRC::StatusWrapper^ OTPWrapper::Status::get()
{
	return ref new MPinRC::StatusWrapper(otp.status.GetStatusCode(), otp.status.GetErrorMessage());
}
void OTPWrapper::Status::set(MPinRC::StatusWrapper^ value)
{
	MPinSDK::Status::Code code = StatusWrapper::ToCode(value->Code);
	MPinSDK::String error = MPinWrapper::ToNativeString(value->Error);
	MPinSDK::Status newStatus = MPinSDK::Status(code, error);
	otp.status = newStatus;
}

#pragma endregion OtpWrapper

#pragma region PinPadProxy

PinPadProxy::PinPadProxy(MPinRC::IPinPad^ pinPad)
{
	if (this->managedPinPad != pinPad)
		this->managedPinPad = pinPad;
}

void PinPadProxy::SetPinPad(MPinRC::IPinPad^ pinPad)
{
	if (this->managedPinPad != pinPad)
		this->managedPinPad = pinPad;
}

MPinSDK::String PinPadProxy::Show(MPinSDK::IPinPad::Mode mode)
{
	MPinRC::Mode managedMode = mode == MPinSDK::IPinPad::Mode::AUTHENTICATE
		? MPinRC::Mode::AUTHENTICATE
		: MPinRC::Mode::REGISTER;

	Platform::String^ pin = this->managedPinPad->Show(managedMode);
	return MPinWrapper::ToNativeString(pin);
}

#pragma endregion PinPadProxy

#pragma region ContextProxy
ContextProxy::ContextProxy(MPinRC::IContext^ context)
{
	this->managedContext = context;

	// TODO: check if the pin pad is created
	MPinRC::IPinPad^ managedPad = this->managedContext->GetPinPad();
	pinPadProxy.SetPinPad(managedPad);
}

MPinSDK::IHttpRequest* ContextProxy::CreateHttpRequest() const
{
	MPinRC::IHttpRequest^ httpRequest = this->managedContext->CreateHttpRequest();
	return new MPinRC::HttpProxy(httpRequest);
}

void ContextProxy::ReleaseHttpRequest(IN IHttpRequest *request) const
{
	delete request;
}

MPinSDK::IStorage* ContextProxy::GetStorage(MPinSDK::IStorage::Type type) const
{
	MPinRC::StorageType managedType = type == MPinSDK::IStorage::Type::SECURE
		? MPinRC::StorageType::SECURE
		: MPinRC::StorageType::NONSECURE;

	MPinRC::IStorage^ storage = this->managedContext->GetStorage(managedType);
	MPinSDK::IStorage* nStorage = new MPinRC::StorageProxy(storage);
	return nStorage;
}

MPinSDK::IPinPad* ContextProxy::GetPinPad() const
{
	return (MPinSDK::IPinPad*)&pinPadProxy;
}

MPinSDK::CryptoType ContextProxy::GetMPinCryptoType() const
{
	MPinRC::CryptoType type = this->managedContext->GetMPinCryptoType();
	switch (type)
	{
	case MPinRC::CryptoType::CRYPTO_TEE:
		return MPinSDK::CryptoType::CRYPTO_TEE;
	case MPinRC::CryptoType::CRYPTO_NON_TEE:
	default:
		return MPinSDK::CryptoType::CRYPTO_NON_TEE;
	}
}

#pragma endregion ContextProxy
