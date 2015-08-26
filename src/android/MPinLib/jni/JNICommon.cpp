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
/*
 * JNICommon.cpp
 *
 *  Created on: Nov 5, 2014
 *      Author: ogi
 */

#include "JNICommon.h"
#include "JNIUser.h"
#include "JNIMPinSDK.h"

static JavaVM * g_jvm;

JNIEnv* JNI_getJENV()
{
	 JNIEnv* env;
	 if(g_jvm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK)
	 {
		 return NULL;
	 }
	 return env;
}

jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
	g_jvm = vm;
	JNIEnv* env = JNI_getJENV();

	RegisterMPinSDKJNI(env);
	RegisterUserJNI(env);

	return JNI_VERSION_1_6;
}

void RegisterNativeMethods(JNIEnv* env, const char* className, const JNINativeMethod* methods, int numMethods)
{
	jclass cls = env->FindClass(className);

	if(!cls)
	{
		env->FatalError("RegisterNativeMethods failed");
		return;
	}

	if(env->RegisterNatives(cls, methods, numMethods) < 0)
	{
		env->FatalError("RegisterNativeMethods failed");
		return;
	}
}

void ReadJavaMap(JNIEnv* env, jobject jmap, MPinSDK::StringMap& map)
{
	jclass clsMap = env->FindClass("java/util/Map");
	jclass clsSet = env->FindClass("java/util/Set");
	jclass clsIterator = env->FindClass("java/util/Iterator");

	jmethodID midKeySet = env->GetMethodID(clsMap, "keySet", "()Ljava/util/Set;");
	jobject jkeySet = env->CallObjectMethod(jmap, midKeySet);

	jmethodID midIterator = env->GetMethodID(clsSet, "iterator", "()Ljava/util/Iterator;");
	jobject jkeySetIter = env->CallObjectMethod(jkeySet, midIterator);

	jmethodID midHasNext = env->GetMethodID(clsIterator, "hasNext", "()Z");
	jmethodID midNext = env->GetMethodID(clsIterator, "next", "()Ljava/lang/Object;");

	jmethodID midGet = env->GetMethodID(clsMap, "get", "(Ljava/lang/Object;)Ljava/lang/Object;");

	map.clear();

	while(env->CallBooleanMethod(jkeySetIter, midHasNext)) {
		jstring jkey = (jstring) env->CallObjectMethod(jkeySetIter, midNext);
		jstring jvalue = (jstring) env->CallObjectMethod(jmap, midGet, jkey);

		const char* cstr = env->GetStringUTFChars(jkey, NULL);
		MPinSDK::String key(cstr);
		env->ReleaseStringUTFChars(jkey, cstr);
		cstr = env->GetStringUTFChars(jvalue, NULL);
		MPinSDK::String value(cstr);
		env->ReleaseStringUTFChars(jvalue, cstr);

		map[key] = value;
	}
}

jobject MakeJavaStatus(JNIEnv* env, const MPinSDK::Status& status)
{
	jclass clsStatus = env->FindClass("com/certivox/models/Status");
	jmethodID ctorStatus = env->GetMethodID(clsStatus, "<init>", "(ILjava/lang/String;)V");
	return env->NewObject(clsStatus, ctorStatus, (jint) status.GetStatusCode(), env->NewStringUTF(status.GetErrorMessage().c_str()));
}

std::string JavaToStdString(JNIEnv* env, jstring jstr)
{
	const char* cstr = env->GetStringUTFChars(jstr, NULL);
	std::string str(cstr);
	env->ReleaseStringUTFChars(jstr, cstr);
	return str;
}

MPinSDK::UserPtr JavaToMPinUser(JNIEnv* env, jobject juser)
{
	jclass clsUser = env->FindClass("com/certivox/models/User");
	jfieldID fidPtr = env->GetFieldID(clsUser, "mPtr", "J");
	return *((MPinSDK::UserPtr*) env->GetLongField(juser, fidPtr));
}
