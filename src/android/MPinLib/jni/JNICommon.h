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

#ifndef _JNI_COMMON_H_
#define _JNI_COMMON_H_

#include <jni.h>
#include <android/log.h>
#include "mpin_sdk.h"

/*
 * Helper macros
 */
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

/*
 * Macro to get the elements count in an array. Don't use it on zero-sized arrays
 */
#define ARR_LEN(x) ((int)(sizeof(x) / sizeof((x)[0])))

/*
 * Helper macro to initialize arrays with JNI methods for registration. Naming convention is ClassName_MethodName.
 * Beware for overloaded methods (with same name and different signature) - make sure they have unique names in C++ land
 */
#define NATIVE_METHOD(methodName, signature) { #methodName, signature, (void*) methodName }

#define  LOG_TAG    "CV"
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__))


/*
 * Helper functions
 */

JNIEnv* JNI_getJENV();

/*
 * Helper function to register native methods
 */
void RegisterNativeMethods(JNIEnv* env, const char* className, const JNINativeMethod* methods, int numMethods);

void ReadJavaMap(JNIEnv* env, jobject jmap, MPinSDK::StringMap& map);

jobject MakeJavaStatus(JNIEnv* env, const MPinSDK::Status& status);

std::string JavaToStdString(JNIEnv* env, jstring jstr);

MPinSDK::UserPtr JavaToMPinUser(JNIEnv* env, jobject juser);

#endif // _JNI_COMMON_H_
