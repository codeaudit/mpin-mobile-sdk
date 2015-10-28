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

#include "JNIMPinSDKv2.h"
#include "JNICommon.h"
#include "HTTPConnector.h"
#include "Storage.h"
#include "ContextV2.h"


typedef sdkv2::Context Context;

static jlong nConstruct(JNIEnv* env, jobject jobj)
{
	return (jlong) new MPinSDKv2();
}

static void nDestruct(JNIEnv* env, jobject jobj, jlong jptr)
{
	MPinSDKv2* sdk = (MPinSDKv2*) jptr;
	delete sdk;
}

static jobject nInit(JNIEnv* env, jobject jobj, jlong jptr, jobject jconfig, jobject jcontext)
{
	MPinSDKv2::StringMap config;
	if(jconfig)
	{
		ReadJavaMap(env, jconfig, config);
	}

	MPinSDKv2* sdk = (MPinSDKv2*) jptr;
	return MakeJavaStatus(env, sdk->Init(config, Context::Instance(jcontext)));
}

static jobject nTestBackend(JNIEnv* env, jobject jobj, jlong jptr, jstring jserver)
{
	MPinSDKv2* sdk = (MPinSDKv2*) jptr;
	return MakeJavaStatus(env, sdk->TestBackend(JavaToStdString(env, jserver)));
}

static jobject nTestBackendRPS(JNIEnv* env, jobject jobj, jlong jptr, jstring jserver, jstring jrpsPrefix)
{
	MPinSDKv2* sdk = (MPinSDKv2*) jptr;
	return MakeJavaStatus(env, sdk->TestBackend(JavaToStdString(env, jserver), JavaToStdString(env, jrpsPrefix)));
}

static jobject nSetBackend(JNIEnv* env, jobject jobj, jlong jptr, jstring jserver)
{
	MPinSDKv2* sdk = (MPinSDKv2*) jptr;
	return MakeJavaStatus(env, sdk->SetBackend(JavaToStdString(env, jserver)));
}

static jobject nSetBackendRPS(JNIEnv* env, jobject jobj, jlong jptr, jstring jserver, jstring jrpsPrefix)
{
	MPinSDKv2* sdk = (MPinSDKv2*) jptr;
	return MakeJavaStatus(env, sdk->SetBackend(JavaToStdString(env, jserver), JavaToStdString(env, jrpsPrefix)));
}

static jobject nMakeNewUser(JNIEnv* env, jobject jobj, jlong jptr, jstring jid, jstring jdeviceName)
{
	MPinSDKv2* sdk = (MPinSDKv2*) jptr;
	MPinSDKv2::UserPtr user = sdk->MakeNewUser(JavaToStdString(env, jid), JavaToStdString(env, jdeviceName));
	jclass clsUser = env->FindClass("com/certivox/models/User");
	jmethodID ctorUser = env->GetMethodID(clsUser, "<init>", "(J)V");
	return env->NewObject(clsUser, ctorUser, (jlong) new MPinSDKv2::UserPtr(user));
}

static jobject nStartRegistration(JNIEnv* env, jobject jobj, jlong jptr, jobject juser, jstring juserData)
{
	MPinSDKv2* sdk = (MPinSDKv2*) jptr;
	return MakeJavaStatus(env, sdk->StartRegistration(JavaToMPinUser(env, juser), JavaToStdString(env, juserData)));
}

static jobject nRestartRegistration(JNIEnv* env, jobject jobj, jlong jptr, jobject juser, jstring juserData)
{
	MPinSDKv2* sdk = (MPinSDKv2*) jptr;
	return MakeJavaStatus(env, sdk->RestartRegistration(JavaToMPinUser(env, juser), JavaToStdString(env, juserData)));
}

static jobject nVerifyUser(JNIEnv* env, jobject jobj, jlong jptr, jobject juser, jstring jmpinId, jstring jactivationKey)
{
	MPinSDKv2* sdk = (MPinSDKv2*) jptr;
	return MakeJavaStatus(env, sdk->VerifyUser(JavaToMPinUser(env, juser), JavaToStdString(env, jmpinId), JavaToStdString(env, jactivationKey)));
}

static jobject nConfirmRegistration(JNIEnv* env, jobject jobj, jlong jptr, jobject juser, jstring jpushMessageIdentifier)
{
	MPinSDKv2* sdk = (MPinSDKv2*) jptr;
	return MakeJavaStatus(env, sdk->ConfirmRegistration(JavaToMPinUser(env, juser), JavaToStdString(env, jpushMessageIdentifier)));
}

static jobject nFinishRegistration(JNIEnv* env, jobject jobj, jlong jptr, jobject juser, jstring jpin)
{
	MPinSDKv2* sdk = (MPinSDKv2*) jptr;
	return MakeJavaStatus(env, sdk->FinishRegistration(JavaToMPinUser(env, juser), JavaToStdString(env, jpin)));
}

static jobject nStartAuthentication(JNIEnv* env, jobject jobj, jlong jptr, jobject juser)
{
	MPinSDKv2* sdk = (MPinSDKv2*) jptr;
	return MakeJavaStatus(env, sdk->StartAuthentication(JavaToMPinUser(env, juser)));
}

static jobject nCheckAccessNumber(JNIEnv* env, jobject jobj, jlong jptr, jstring jaccessNumber)
{
	MPinSDKv2* sdk = (MPinSDKv2*) jptr;
	return MakeJavaStatus(env, sdk->CheckAccessNumber(JavaToStdString(env, jaccessNumber)));
}

static jobject nFinishAuthentication(JNIEnv* env, jobject jobj, jlong jptr, jobject juser, jstring jpin)
{
	MPinSDKv2* sdk = (MPinSDKv2*) jptr;
	return MakeJavaStatus(env, sdk->FinishAuthentication(JavaToMPinUser(env, juser), JavaToStdString(env, jpin)));
}

static jobject nFinishAuthenticationResultData(JNIEnv* env, jobject jobj, jlong jptr, jobject juser, jstring jpin, jobject jresultData)
{
	MPinSDKv2* sdk = (MPinSDKv2*) jptr;

	MPinSDK::String authResultData;
	MPinSDK::Status status = sdk->FinishAuthentication(JavaToMPinUser(env, juser), JavaToStdString(env, jpin), authResultData);

	jclass clsStringBuilder = env->FindClass("java/lang/StringBuilder");
	jmethodID midSetLength = env->GetMethodID(clsStringBuilder, "setLength", "(I)V");
	env->CallVoidMethod(jresultData, midSetLength, authResultData.size());
	jmethodID midReplace = env->GetMethodID(clsStringBuilder, "replace", "(IILjava/lang/String;)Ljava/lang/StringBuilder;");
	env->CallObjectMethod(jresultData, midReplace, 0, authResultData.size(), env->NewStringUTF(authResultData.c_str()));

	return MakeJavaStatus(env, status);
}

static jobject nFinishAuthenticationOTP(JNIEnv* env, jobject jobj, jlong jptr, jobject juser, jstring jpin, jobject jotp)
{
	MPinSDKv2* sdk = (MPinSDKv2*) jptr;

	MPinSDK::OTP otp;
	MPinSDK::Status status = sdk->FinishAuthenticationOTP(JavaToMPinUser(env, juser), JavaToStdString(env, jpin), otp);

	if(status == MPinSDK::Status::OK)
	{
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

static jobject nFinishAuthenticationAN(JNIEnv* env, jobject jobj, jlong jptr, jobject juser, jstring jpin, jstring jaccessNumber)
{
	MPinSDKv2* sdk = (MPinSDKv2*) jptr;
	return MakeJavaStatus(env, sdk->FinishAuthenticationAN(JavaToMPinUser(env, juser), JavaToStdString(env, jpin), JavaToStdString(env, jaccessNumber)));
}

static void nDeleteUser(JNIEnv* env, jobject jobj, jlong jptr, jobject juser)
{
	MPinSDKv2* sdk = (MPinSDKv2*) jptr;
	sdk->DeleteUser(JavaToMPinUser(env, juser));
}

static void nListUsers(JNIEnv* env, jobject jobj, jlong jptr, jobject jusersList)
{
	MPinSDKv2* sdk = (MPinSDKv2*) jptr;
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

static jstring nGetVersion(JNIEnv* env, jobject jobj, jlong jptr)
{
	MPinSDKv2* sdk = (MPinSDKv2*) jptr;
	const char *version = sdk->GetVersion();
	return env->NewStringUTF(version);
}

static jboolean nCanLogout(JNIEnv* env, jobject jobj, jlong jptr, jobject juser)
{
	MPinSDKv2* sdk = (MPinSDKv2*) jptr;
	return sdk->CanLogout(JavaToMPinUser(env, juser));
}

static jboolean nLogout(JNIEnv* env, jobject jobj, jlong jptr, jobject juser)
{
	MPinSDKv2* sdk = (MPinSDKv2*) jptr;
	return sdk->Logout(JavaToMPinUser(env, juser));
}

static jstring nGetClientParam(JNIEnv* env, jobject jobj, jlong jptr, jstring jkey)
{
	MPinSDKv2* sdk = (MPinSDKv2*) jptr;
	MPinSDK::String result = sdk->GetClientParam(JavaToStdString(env, jkey));
	return env->NewStringUTF(result.c_str());
}

static JNINativeMethod g_methodsMPinSDKv2[] =
{
	NATIVE_METHOD(nConstruct, "()J"),
	NATIVE_METHOD(nDestruct, "(J)V"),
	NATIVE_METHOD(nInit, "(JLjava/util/Map;Landroid/content/Context;)Lcom/certivox/models/Status;"),
	NATIVE_METHOD(nTestBackend, "(JLjava/lang/String;)Lcom/certivox/models/Status;"),
	NATIVE_METHOD(nTestBackendRPS, "(JLjava/lang/String;Ljava/lang/String;)Lcom/certivox/models/Status;"),
	NATIVE_METHOD(nSetBackend, "(JLjava/lang/String;)Lcom/certivox/models/Status;"),
	NATIVE_METHOD(nSetBackendRPS, "(JLjava/lang/String;Ljava/lang/String;)Lcom/certivox/models/Status;"),
	NATIVE_METHOD(nMakeNewUser, "(JLjava/lang/String;Ljava/lang/String;)Lcom/certivox/models/User;"),
	NATIVE_METHOD(nStartRegistration, "(JLcom/certivox/models/User;Ljava/lang/String;)Lcom/certivox/models/Status;"),
	NATIVE_METHOD(nRestartRegistration, "(JLcom/certivox/models/User;Ljava/lang/String;)Lcom/certivox/models/Status;"),
	NATIVE_METHOD(nVerifyUser, "(JLcom/certivox/models/User;Ljava/lang/String;Ljava/lang/String;)Lcom/certivox/models/Status;"),
	NATIVE_METHOD(nConfirmRegistration, "(JLcom/certivox/models/User;Ljava/lang/String;)Lcom/certivox/models/Status;"),
	NATIVE_METHOD(nFinishRegistration, "(JLcom/certivox/models/User;Ljava/lang/String;)Lcom/certivox/models/Status;"),
	NATIVE_METHOD(nStartAuthentication, "(JLcom/certivox/models/User;)Lcom/certivox/models/Status;"),
	NATIVE_METHOD(nCheckAccessNumber, "(JLjava/lang/String;)Lcom/certivox/models/Status;"),
	NATIVE_METHOD(nFinishAuthentication, "(JLcom/certivox/models/User;Ljava/lang/String;)Lcom/certivox/models/Status;"),
	NATIVE_METHOD(nFinishAuthenticationResultData, "(JLcom/certivox/models/User;Ljava/lang/String;Ljava/lang/StringBuilder;)Lcom/certivox/models/Status;"),
	NATIVE_METHOD(nFinishAuthenticationOTP, "(JLcom/certivox/models/User;Ljava/lang/String;Lcom/certivox/models/OTP;)Lcom/certivox/models/Status;"),
	NATIVE_METHOD(nFinishAuthenticationAN, "(JLcom/certivox/models/User;Ljava/lang/String;Ljava/lang/String;)Lcom/certivox/models/Status;"),
	NATIVE_METHOD(nDeleteUser, "(JLcom/certivox/models/User;)V"),
	NATIVE_METHOD(nListUsers, "(JLjava/util/List;)V"),
	NATIVE_METHOD(nGetVersion, "(J)Ljava/lang/String;"),
	NATIVE_METHOD(nCanLogout, "(JLcom/certivox/models/User;)Z"),
	NATIVE_METHOD(nLogout, "(JLcom/certivox/models/User;)Z"),
	NATIVE_METHOD(nGetClientParam, "(JLjava/lang/String;)Ljava/lang/String;")
};

void RegisterMPinSDKv2JNI(JNIEnv* env)
{
	RegisterNativeMethods(env, "com/certivox/mpinsdk/MPinSDKv2", g_methodsMPinSDKv2, ARR_LEN(g_methodsMPinSDKv2));
}
