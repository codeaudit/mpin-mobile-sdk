/*
 Copyright (c) 2012-2015, Certivox
 All rights reserved.
 
 Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 
 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 
 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 
 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 
 For full details regarding our CertiVox terms of service please refer to
 the following links:
 * Our Terms and Conditions -
 http://www.certivox.com/about-certivox/terms-and-conditions/
 * Our Security and Privacy -
 http://www.certivox.com/about-certivox/security-privacy/
 * Our Statement of Position and Our Promise on Software Patents -
 http://www.certivox.com/about-certivox/patents/
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
    static BOOL isShown = false;
    
    
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
        NSLog(@"is SHOWN %d", isShown);
        if(isShown) {
            pin = mpin;
            [cLock signal];
        }
        [cLock unlock];
    }
    
String Context::Show(MPinSDK::UserPtr user, Mode mode) {
    dispatch_async(dispatch_get_main_queue(),^{
        [[NSNotificationCenter defaultCenter] postNotificationName:kShowPinPadNotification object:nil];
    });
    
        [cLock lock];
        isShown = true;
        while ( pin.compare(kNegativeString) == 0) {
            [cLock wait];
        }
        String _pin = pin;
        pin = kNegativeString;
        isShown = false;
        [cLock unlock];
        return _pin;
    }
    
} /* namespace store */
