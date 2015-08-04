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
 * Storage.cpp
 *
 *  Created on: Oct 28, 2014
 *      Author: georgi
 */

#include "Storage.h"
#include "def.h"

namespace store {

Storage::Storage(jobject context, bool isMpinType) {
	JNIEnv* p_jenv = JNI_getJENV();
	m_pjstorageCls = (jclass) p_jenv->NewGlobalRef(p_jenv->FindClass("com/certivox/storage/Storage"));
	const jmethodID midInit = p_jenv->GetMethodID(m_pjstorageCls, "<init>", "(Landroid/content/Context;Z)V");
	m_pjstorage = p_jenv->NewGlobalRef(p_jenv->NewObject(m_pjstorageCls, midInit, context, isMpinType));

}

void Storage::setErrorMessage() {
	JNIEnv* p_jenv = JNI_getJENV();
	jclass cls = p_jenv->FindClass("com/certivox/storage/Storage");
	const jmethodID midGetErrorMessage = p_jenv->GetMethodID(cls, "GetErrorMessage", "()Ljava/lang/String;");
	jstring jerror = static_cast<jstring>(p_jenv->CallObjectMethod(m_pjstorage, midGetErrorMessage));
	const char * c_error = "";
	if (jerror) c_error = (char *)p_jenv->GetStringUTFChars(jerror, NULL);
	m_errorMessage = c_error;
}

bool Storage::SetData(const String& data) {
	JNIEnv* p_jenv = JNI_getJENV();
	const jmethodID midSetData = p_jenv->GetMethodID(m_pjstorageCls, "SetData", "(Ljava/lang/String;)Z");
	jstring jdata = p_jenv->NewStringUTF(data.c_str());
	bool bresult = p_jenv->CallBooleanMethod(m_pjstorage, midSetData, jdata);
	if(bresult == false)  {
		setErrorMessage();
	}
	return bresult;
}

bool Storage::GetData(String &data) {
	JNIEnv* p_jenv = JNI_getJENV();
	const jmethodID midGetData = p_jenv->GetMethodID(m_pjstorageCls, "GetData", "()Ljava/lang/String;");
	jstring jreadData = (jstring)p_jenv->CallObjectMethod(m_pjstorage, midGetData);
	if(jreadData == NULL) {
		setErrorMessage();
		return false;
	}
	const char * c_data = (char *)p_jenv->GetStringUTFChars(jreadData, NULL);
	data.append(c_data);
	p_jenv->ReleaseStringUTFChars(jreadData, c_data);
	return true;
}

const String& Storage::GetErrorMessage() const { return m_errorMessage; }

Storage::~Storage() {
	JNIEnv* p_jenv = JNI_getJENV();
	if(p_jenv == NULL) {
		return;
	}
	RELEASE_JNIREF(p_jenv, m_pjstorageCls)
	RELEASE_JNIREF(p_jenv, m_pjstorage)
}

}
