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
