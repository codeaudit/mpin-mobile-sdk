/*
 * Context.cpp
 *
 *  Created on: Oct 28, 2014
 *      Author: georgi
 */
#include "Context.h"
#include "HTTPConnector.h"
#include "Storage.h"
#import "Constants.h"

namespace sdk {

typedef store::Storage Storage;
typedef net::HTTPConnector HttpRequest;

Context* Context::m_pInstance = NULL;
    
static NSCondition * cLock;
static String pin;

    
Context * Context::Instance() {
	if(m_pInstance == NULL) {
		m_pInstance = new Context();
	}
	return m_pInstance;
}

Context::Context() {
	cLock = [[NSCondition alloc] init];
    pin = kNegativeString;
    m_pIstorageSecure = new Storage(true);
	m_pIstorageNonSecure = new Storage(false);
}


MPinSDK::IHttpRequest * Context::CreateHttpRequest() const{
	return new HttpRequest();
}

void Context::ReleaseHttpRequest(IHttpRequest *request) const {
	RELEASE(request)
}

MPinSDK::IStorage * Context::GetStorage(IStorage::Type type) const {
	switch (type)
	{
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
    
void Context::setPin(const String & mpin) {
    [cLock lock];
    pin = mpin;
    [cLock signal];
    [cLock unlock];
}

String Context::Show() {
    dispatch_async(dispatch_get_main_queue(),^{
        [[NSNotificationCenter defaultCenter] postNotificationName:kShowPinPadNotification object:nil];
    });
    
    [cLock lock];
    while ( pin.compare(kNegativeString) == 0) {
        [cLock wait];
    }
    String _pin = pin;
    pin = kNegativeString;
    [cLock unlock];
    return _pin;
}

} /* namespace store */
