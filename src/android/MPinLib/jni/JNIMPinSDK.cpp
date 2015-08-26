/*******************************************************************************
 * Copyright (c) 2012-2015, Certivox All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 * following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 * disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 * following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote
 * products derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * For full details regarding our CertiVox terms of service please refer to the following links:
 *
 * * Our Terms and Conditions - http://www.certivox.com/about-certivox/terms-and-conditions/
 *
 * * Our Security and Privacy - http://www.certivox.com/about-certivox/security-privacy/
 *
 * * Our Statement of Position and Our Promise on Software Patents - http://www.certivox.com/about-certivox/patents/
 ******************************************************************************/

#include "JNIMPinSDK.h"
#include "JNICommon.h"
#include "HTTPConnector.h"
#include "Storage.h"
#include "Context.h"


typedef sdk::Context Context;

static jlong nConstruct(JNIEnv* env, jobject jobj, jobject jcontext, jobject jconfig)
{
	MPinSDK::StringMap config;
	if(jconfig)
	{
		ReadJavaMap(env, jconfig, config);
	}
	MPinSDK* sdk = new MPinSDK();
	MPinSDK::Status s = sdk->Init(config, Context::Instance(jcontext));
	LOGI("Init status %d: '%s'", s.GetStatusCode(), s.GetErrorMessage().c_str());
	return (jlong) sdk;
}

static void nDestruct(JNIEnv* env, jobject jobj, jlong jptr)
{
	MPinSDK* sdk = (MPinSDK*) jptr;
	delete sdk;
}

static jobject nMakeNewUser(JNIEnv* env, jobject jobj, jlong jptr, jstring jid, jstring jdeviceName)
{
	MPinSDK* sdk = (MPinSDK*) jptr;
	const char* cid = env->GetStringUTFChars(jid, NULL);
	MPinSDK::String id(cid);
	env->ReleaseStringUTFChars(jid, cid);
	const char* cdeviceName = env->GetStringUTFChars(jdeviceName, NULL);
	MPinSDK::String deviceName(cdeviceName);
	env->ReleaseStringUTFChars(jdeviceName, cdeviceName);
	MPinSDK::UserPtr user = sdk->MakeNewUser(id, deviceName);
	jclass clsUser = env->FindClass("com/certivox/models/User");
	jmethodID ctorUser = env->GetMethodID(clsUser, "<init>", "(J)V");
	return env->NewObject(clsUser, ctorUser, (jlong) new MPinSDK::UserPtr(user));
}

static jobject nStartRegistration(JNIEnv* env, jobject jobj, jlong jptr, jobject juser, jstring juserData)
{
	MPinSDK* sdk = (MPinSDK*) jptr;
	jclass clsUser = env->FindClass("com/certivox/models/User");
	jfieldID fidPtr = env->GetFieldID(clsUser, "mPtr", "J");
	MPinSDK::UserPtr user = *(MPinSDK::UserPtr*)env->GetLongField(juser, fidPtr);
	const char* cuserData = env->GetStringUTFChars(juserData, NULL);
	MPinSDK::String userData(cuserData);
	env->ReleaseStringUTFChars(juserData, cuserData);
	return MakeJavaStatus(env, sdk->StartRegistration(user, userData));
}

static jobject nRestartRegistration(JNIEnv* env, jobject jobj, jlong jptr, jobject juser, jstring juserData)
{
	MPinSDK* sdk = (MPinSDK*) jptr;
	jclass clsUser = env->FindClass("com/certivox/models/User");
	jfieldID fidPtr = env->GetFieldID(clsUser, "mPtr", "J");
	MPinSDK::UserPtr user = *(MPinSDK::UserPtr*)env->GetLongField(juser, fidPtr);
	const char* cuserData = env->GetStringUTFChars(juserData, NULL);
	MPinSDK::String userData(cuserData);
	env->ReleaseStringUTFChars(juserData, cuserData);
	return MakeJavaStatus(env, sdk->RestartRegistration(user, userData));
}

static jobject nFinishRegistration(JNIEnv* env, jobject jobj, jlong jptr, jobject juser)
{
	MPinSDK* sdk = (MPinSDK*) jptr;
	jclass clsUser = env->FindClass("com/certivox/models/User");
	jfieldID fidPtr = env->GetFieldID(clsUser, "mPtr", "J");
	MPinSDK::UserPtr user = *(MPinSDK::UserPtr*)env->GetLongField(juser, fidPtr);
	return MakeJavaStatus(env, sdk->FinishRegistration(user));
}

static jobject nAuthenticate(JNIEnv* env, jobject jobj, jlong jptr, jobject juser)
{
	MPinSDK* sdk = (MPinSDK*) jptr;
	jclass clsUser = env->FindClass("com/certivox/models/User");
	jfieldID fidPtr = env->GetFieldID(clsUser, "mPtr", "J");
	MPinSDK::UserPtr user = *(MPinSDK::UserPtr*)env->GetLongField(juser, fidPtr);
	return MakeJavaStatus(env, sdk->Authenticate(user));
}

static jobject nAuthenticateOtp(JNIEnv* env, jobject jobj, jlong jptr, jobject juser, jobject jotp)
{
	MPinSDK* sdk = (MPinSDK*) jptr;
	jclass clsUser = env->FindClass("com/certivox/models/User");
	jfieldID fidPtr = env->GetFieldID(clsUser, "mPtr", "J");
	MPinSDK::UserPtr user = *(MPinSDK::UserPtr*)env->GetLongField(juser, fidPtr);
	MPinSDK::OTP otp;
	MPinSDK::Status status = sdk->AuthenticateOTP(user, otp);
	if (status == MPinSDK::Status::OK) {
		jclass clsOTP = env->FindClass("com/certivox/models/OTP");
		jfieldID fidOtp = env->GetFieldID(clsOTP, "otp", "Ljava/lang/String;");
		jfieldID fidExpireTime = env->GetFieldID(clsOTP, "expireTime", "J");
		jfieldID fidTtlSeconds = env->GetFieldID(clsOTP, "ttlSeconds", "I");
		jfieldID fidNowTime = env->GetFieldID(clsOTP, "nowTime", "J");
		jfieldID fidStatus = env->GetFieldID(clsOTP, "status", "Lcom/certivox/models/Status;");
		env->SetObjectField(jotp, fidOtp, env->NewStringUTF(otp.otp.c_str()));
		env->SetLongField(jotp, fidExpireTime, otp.expireTime);
		env->SetIntField(jotp, fidTtlSeconds, otp.ttlSeconds);
		env->SetLongField(jotp, fidNowTime, otp.nowTime);
		env->SetObjectField(jotp, fidStatus, MakeJavaStatus(env, otp.status));
	}
	return MakeJavaStatus(env, status);
}

static jobject nAuthenticateResultData(JNIEnv* env, jobject jobj, jlong jptr, jobject juser, jobject jresultData)
{
	MPinSDK* sdk = (MPinSDK*) jptr;
	jclass clsUser = env->FindClass("com/certivox/models/User");
	jfieldID fidPtr = env->GetFieldID(clsUser, "mPtr", "J");
	MPinSDK::UserPtr user = *(MPinSDK::UserPtr*)env->GetLongField(juser, fidPtr);

	MPinSDK::String authResultData;
	MPinSDK::Status status = sdk->Authenticate(user, authResultData);

	jclass clsStringBuilder = env->FindClass("java/lang/StringBuilder");
	jmethodID midSetLength = env->GetMethodID(clsStringBuilder, "setLength", "(I)V");
	env->CallVoidMethod(jresultData, midSetLength, authResultData.size());
	jmethodID midReplace = env->GetMethodID(clsStringBuilder, "replace", "(IILjava/lang/String;)Ljava/lang/StringBuilder;");
	env->CallObjectMethod(jresultData, midReplace, 0, authResultData.size(), env->NewStringUTF(authResultData.c_str()));

	return MakeJavaStatus(env, status);
}

static jobject nAuthenticateAccessNumber(JNIEnv* env, jobject jobj, jlong jptr, jobject juser, jstring jaccessNumber)
{
	MPinSDK* sdk = (MPinSDK*) jptr;
	jclass clsUser = env->FindClass("com/certivox/models/User");
	jfieldID fidPtr = env->GetFieldID(clsUser, "mPtr", "J");
	MPinSDK::UserPtr user = *(MPinSDK::UserPtr*)env->GetLongField(juser, fidPtr);
	const char* caccessNumber = env->GetStringUTFChars(jaccessNumber, NULL);
	const MPinSDK::String accessNumber(caccessNumber);
	env->ReleaseStringUTFChars(jaccessNumber, caccessNumber);

	return MakeJavaStatus(env, sdk->AuthenticateAN(user, accessNumber));
}

static void nDeleteUser(JNIEnv* env, jobject jobj, jlong jptr, jobject juser)
{
	MPinSDK* sdk = (MPinSDK*) jptr;
	jclass clsUser = env->FindClass("com/certivox/models/User");
	jfieldID fidPtr = env->GetFieldID(clsUser, "mPtr", "J");
	MPinSDK::UserPtr user = *(MPinSDK::UserPtr*)env->GetLongField(juser, fidPtr);
	sdk->DeleteUser(user);
}

static void nListUsers(JNIEnv* env, jobject jobj, jlong jptr, jobject jusersList)
{
	MPinSDK* sdk = (MPinSDK*) jptr;
	std::vector<MPinSDK::UserPtr> users;
	sdk->ListUsers(users);

	jclass clsList = env->FindClass("java/util/List");
	jmethodID midAdd = env->GetMethodID(clsList, "add", "(Ljava/lang/Object;)Z");

	jclass clsUser = env->FindClass("com/certivox/models/User");
	jmethodID ctorUser = env->GetMethodID(clsUser, "<init>", "(J)V");

	for (std::vector<MPinSDK::UserPtr>::iterator i = users.begin(); i != users.end(); ++i) {
		MPinSDK::UserPtr user = *i;
		jobject juser = env->NewObject(clsUser, ctorUser, (jlong) new MPinSDK::UserPtr(user));
		env->CallBooleanMethod(jusersList, midAdd, juser);
	}
}

static jboolean nCanLogout(JNIEnv* env, jobject jobj, jlong jptr, jobject juser)
{
	MPinSDK* sdk = (MPinSDK*) jptr;
	jclass clsUser = env->FindClass("com/certivox/models/User");
	jfieldID fidPtr = env->GetFieldID(clsUser, "mPtr", "J");
	MPinSDK::UserPtr user = *(MPinSDK::UserPtr*)env->GetLongField(juser, fidPtr);
	return sdk->CanLogout(user);
}

static jboolean nLogout(JNIEnv* env, jobject jobj, jlong jptr, jobject juser)
{
	MPinSDK* sdk = (MPinSDK*) jptr;
	jclass clsUser = env->FindClass("com/certivox/models/User");
	jfieldID fidPtr = env->GetFieldID(clsUser, "mPtr", "J");
	MPinSDK::UserPtr user = *(MPinSDK::UserPtr*)env->GetLongField(juser, fidPtr);
	return sdk->Logout(user);
}

static jstring nGetClientParam(JNIEnv* env, jobject jobj, jlong jptr, jstring jkey)
{
	MPinSDK* sdk = (MPinSDK*) jptr;
	const char* ckey = env->GetStringUTFChars(jkey, NULL);
	MPinSDK::String key(ckey);
	env->ReleaseStringUTFChars(jkey, ckey);
	MPinSDK::String result = sdk->GetClientParam(key);
	return env->NewStringUTF(result.c_str());
}

static jobject nTestBackend(JNIEnv* env, jobject jobj, jlong jptr, jstring jbackend)
{
	MPinSDK* sdk = (MPinSDK*) jptr;
	const char* cbackend = env->GetStringUTFChars(jbackend, NULL);
	MPinSDK::String backend(cbackend);
	env->ReleaseStringUTFChars(jbackend, cbackend);
	return MakeJavaStatus(env, sdk->TestBackend(backend));
}


static jobject nTestBackendRPS(JNIEnv* env, jobject jobj, jlong jptr, jstring jbackend, jstring jrpsPrefix)
{
	MPinSDK* sdk = (MPinSDK*) jptr;
	const char* cbackend = env->GetStringUTFChars(jbackend, NULL);
	MPinSDK::String backend(cbackend);
	env->ReleaseStringUTFChars(jbackend, cbackend);
	const char* crpsPrefix = env->GetStringUTFChars(jrpsPrefix, NULL);
	MPinSDK::String rpsPrefix(crpsPrefix);
	env->ReleaseStringUTFChars(jrpsPrefix, crpsPrefix);
	return MakeJavaStatus(env, sdk->TestBackend(backend, rpsPrefix));
}

static jobject nSetBackend(JNIEnv* env, jobject jobj, jlong jptr, jstring jbackend, jstring jrpsPrefix)
{
	MPinSDK* sdk = (MPinSDK*) jptr;
	const char* cbackend = env->GetStringUTFChars(jbackend, NULL);
	MPinSDK::String backend(cbackend);
	env->ReleaseStringUTFChars(jbackend, cbackend);
	const char* crpsPrefix = env->GetStringUTFChars(jrpsPrefix, NULL);
	MPinSDK::String rpsPrefix(crpsPrefix);
	env->ReleaseStringUTFChars(jbackend, crpsPrefix);
	return MakeJavaStatus(env, sdk->SetBackend(backend, rpsPrefix));
}

static JNINativeMethod g_methodsMpin[] =
{
	NATIVE_METHOD(nConstruct, "(Landroid/content/Context;Ljava/util/Map;)J"),
	NATIVE_METHOD(nDestruct, "(J)V"),
	NATIVE_METHOD(nMakeNewUser, "(JLjava/lang/String;Ljava/lang/String;)Lcom/certivox/models/User;"),
	NATIVE_METHOD(nStartRegistration, "(JLcom/certivox/models/User;Ljava/lang/String;)Lcom/certivox/models/Status;"),
	NATIVE_METHOD(nRestartRegistration, "(JLcom/certivox/models/User;Ljava/lang/String;)Lcom/certivox/models/Status;"),
	NATIVE_METHOD(nFinishRegistration, "(JLcom/certivox/models/User;)Lcom/certivox/models/Status;"),
	NATIVE_METHOD(nAuthenticate, "(JLcom/certivox/models/User;)Lcom/certivox/models/Status;"),
	NATIVE_METHOD(nAuthenticateOtp, "(JLcom/certivox/models/User;Lcom/certivox/models/OTP;)Lcom/certivox/models/Status;"),
	NATIVE_METHOD(nAuthenticateResultData, "(JLcom/certivox/models/User;Ljava/lang/StringBuilder;)Lcom/certivox/models/Status;"),
	NATIVE_METHOD(nAuthenticateAccessNumber, "(JLcom/certivox/models/User;Ljava/lang/String;)Lcom/certivox/models/Status;"),
	NATIVE_METHOD(nDeleteUser, "(JLcom/certivox/models/User;)V"),
	NATIVE_METHOD(nListUsers, "(JLjava/util/List;)V"),
	NATIVE_METHOD(nCanLogout, "(JLcom/certivox/models/User;)Z"),
	NATIVE_METHOD(nLogout, "(JLcom/certivox/models/User;)Z"),
	NATIVE_METHOD(nTestBackend, "(JLjava/lang/String;)Lcom/certivox/models/Status;"),
	NATIVE_METHOD(nTestBackendRPS, "(JLjava/lang/String;Ljava/lang/String;)Lcom/certivox/models/Status;"),
	NATIVE_METHOD(nSetBackend, "(JLjava/lang/String;Ljava/lang/String;)Lcom/certivox/models/Status;"),
	NATIVE_METHOD(nGetClientParam, "(JLjava/lang/String;)Ljava/lang/String;")
};

void RegisterMPinSDKJNI(JNIEnv* env)
{
	RegisterNativeMethods(env, "com/certivox/mpinsdk/Mpin", g_methodsMpin, ARR_LEN(g_methodsMpin));
}
