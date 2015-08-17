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
 * jni_common.cpp
 *
 *  Created on: Nov 5, 2014
 *      Author: ogi
 */

#include "def.h"

void registerNativeMethods(JNIEnv* env, const char* className, const JNINativeMethod* methods, int numMethods)
{
	jclass cls = env->FindClass(className);

	if (!cls)
	{
		env->FatalError("registerNativeMethods failed");
		return;
	}

	if (env->RegisterNatives(cls, methods, numMethods) < 0)
	{
		env->FatalError("registerNativeMethods failed");
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

	while (env->CallBooleanMethod(jkeySetIter, midHasNext)) {
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
