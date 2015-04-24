/*
 * Context.cpp
 *
 *  Created on: Oct 28, 2014
 *      Author: georgi
 */
#include "Context.h"
#include "HTTPConnector.h"
#include "Storage.h"

namespace sdk {

typedef store::Storage Storage;
typedef net::HTTPConnector HttpRequest;

Context* Context::m_pInstance = NULL;

Context * Context::Instance(jobject jcontext) {
	if (m_pInstance == NULL) {
		m_pInstance = new Context(jcontext);
	}
	return m_pInstance;
}

Context::Context(jobject jcontext) {
	m_pIstorageSecure = new Storage(jcontext, true);
	m_pIstorageNonSecure = new Storage(jcontext, false);
}

MPinSDK::IHttpRequest * Context::CreateHttpRequest() const {
	return new HttpRequest(JNI_getJENV());
}

void Context::ReleaseHttpRequest(IHttpRequest *request) const {
	RELEASE(request)
}

MPinSDK::IStorage * Context::GetStorage(IStorage::Type type) const {
	switch (type) {
	case MPinSDK::IStorage::SECURE:
		return m_pIstorageSecure;
	case MPinSDK::IStorage::NONSECURE:
		return m_pIstorageNonSecure;
	default:
		return NULL;
	}
}

MPinSDK::IPinPad* Context::GetPinPad() const {
	return const_cast<Context*>(this); //TODO
}

MPinSDK::CryptoType Context::GetMPinCryptoType() const {
	return MPinSDK::CRYPTO_NON_TEE;
}

Context::~Context() {
	RELEASE(m_pIstorageSecure)
	RELEASE(m_pIstorageNonSecure)
	RELEASE(m_pInstance)
}

MPinSDK::String Context::Show() {
	JNIEnv* env = JNI_getJENV();
	jclass clsPinPad = env->FindClass("com/certivox/activities/MPinActivity");
	jmethodID midShow = env->GetStaticMethodID(clsPinPad, "show",
			"()Ljava/lang/String;");
	jstring jstr = (jstring) env->CallStaticObjectMethod(clsPinPad, midShow);
	const char* cstr = env->GetStringUTFChars(jstr, NULL);
	MPinSDK::String pin(cstr);
	env->ReleaseStringUTFChars(jstr, cstr);
	return pin;
}

} /* namespace store */
