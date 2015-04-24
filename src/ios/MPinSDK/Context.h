/*
 * Context.h
 *
 *  Created on: Oct 28, 2014
 *      Author: georgi
 */

#ifndef CONTEXT_H_
#define CONTEXT_H_

#include "def.h"

namespace sdk {

class Context: public IContext, public MPinSDK::IPinPad {
public:
	static Context* Instance();
	virtual IHttpRequest * CreateHttpRequest() const;
	virtual void ReleaseHttpRequest(IHttpRequest *request) const;
	virtual IStorage * GetStorage(IStorage::Type type) const;
	virtual MPinSDK::IPinPad* GetPinPad() const;
	virtual MPinSDK::CryptoType GetMPinCryptoType() const;
    virtual String Show();
	virtual ~Context();
    
    void setPin(const String & mpin);
    
private:
	Context();
	Context(Context const&){};
	Context& operator=(Context const&){ return *this;};
    
    static Context* m_pInstance;
	IStorage * m_pIstorageSecure;
	IStorage * m_pIstorageNonSecure;
};

} /* namespace store */
#endif /* CONTEXT_H_ */
