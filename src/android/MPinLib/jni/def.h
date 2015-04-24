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
