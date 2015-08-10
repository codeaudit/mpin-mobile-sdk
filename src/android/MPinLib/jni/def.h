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
 * def.h
 *
 *  Created on: Oct 23, 2014
 *      Author: georgi
 */

#ifndef DEF_H_
#define DEF_H_

#include <jni.h>
#include "mpin_sdk.h"

	#if defined(__arm__)
	  #if defined(__ARM_ARCH_7A__)
		#if defined(__ARM_NEON__)
		  #if defined(__ARM_PCS_VFP)
			#define ABI "armeabi-v7a/NEON (hard-float)"
		  #else
			#define ABI "armeabi-v7a/NEON"
		  #endif
		#else
		  #if defined(__ARM_PCS_VFP)
			#define ABI "armeabi-v7a (hard-float)"
		  #else
			#define ABI "armeabi-v7a"
		  #endif
		#endif
	  #else
	   #define ABI "armeabi"
	  #endif
	#elif defined(__i386__)
	   #define ABI "x86"
	#elif defined(__x86_64__)
	   #define ABI "x86_64"
	#elif defined(__mips64)  /* mips64el-* toolchain defines __mips__ too */
	   #define ABI "mips64"
	#elif defined(__mips__)
	   #define ABI "mips"
	#elif defined(__aarch64__)
	   #define ABI "arm64-v8a"
	#else
	   #define ABI "unknown"
	#endif



#define RELEASE(pointer)  \
    if ((pointer) != NULL ) { \
        delete (pointer);    \
        (pointer) = NULL;    \
    } \


#define RELEASE_JNIREF(env , ref)  \
    if ((ref) != NULL ) { \
        (env)->DeleteGlobalRef((ref)); \
        (ref) = NULL;    \
    } \


extern "C" JNIEXPORT
JNIEnv* JNICALL JNI_getJENV();

/// input output parameter
#define IN
#define OUT

typedef MPinSDK::String String;
typedef MPinSDK::IContext IContext;
typedef MPinSDK::IHttpRequest IHttpRequest;
typedef MPinSDK::IStorage IStorage;
typedef MPinSDK::StringMap StringMap;

/*
 * Macro to get the elements count in an array. Don't use it on zero-sized arrays
 */
#define ARR_LEN(x) ((int)(sizeof(x) / sizeof((x)[0])))

/*
 * Helper macro to initialize arrays with JNI methods for registration. Naming convention is ClassName_MethodName.
 * Beware for overloaded methods (with same name and different signature) - make sure they have unique names in C++ land
 */
#define NATIVE_METHOD(className, methodName, signature) { #methodName, signature, (void*)(className ## _ ## methodName) }

/*
 * Helper function to register native methods
 */
void registerNativeMethods(JNIEnv* env, const char* className, const JNINativeMethod* methods, int numMethods);

/*
 * Register native methods for Java class com.certivox.mpinsdk.MainActivity
 */
void register_MainActivity(JNIEnv* env);

/*
 * Register native methods for Java class com.certivox.mpinsdk.Mpin
 */
void register_Mpin(JNIEnv* env);

/*
 * Register native methods for Java class com.certivox.data.User
 */
void register_User(JNIEnv* env);

void ReadJavaMap(JNIEnv* env, jobject jmap, MPinSDK::StringMap& map);

#endif /* DEF_H_ */
