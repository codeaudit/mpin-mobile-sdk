/*
 * def.h
 *
 *  Created on: Oct 23, 2014
 *      Author: georgi georgiev
 */

#ifndef DEF_H_
#define DEF_H_

#include "mpin_sdk.h"

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

/// input output parameter
#define IN
#define OUT

typedef MPinSDK::String String;
typedef MPinSDK::IContext IContext;
typedef MPinSDK::IHttpRequest IHttpRequest;
typedef MPinSDK::IStorage IStorage;
typedef MPinSDK::StringMap StringMap;
typedef IHttpRequest::Method Method;


static const String kEmptyString = "";
static const String kNegativeString = "-1";

/*
 * Macro to get the elements count in an array. Don't use it on zero-sized arrays
 */
#define ARR_LEN(x) ((int)(sizeof(x) / sizeof((x)[0])))


#endif /* DEF_H_ */
