/*
 * HTTPConnector.h
 *
 *  Created on: Oct 20, 2014
 *      Author: georgi.georgiev
 */

#ifndef HTTPCONNECTOR_H_
#define HTTPCONNECTOR_H_

#include "def.h"
#include "Exceptions.h"


using namespace std;

namespace net {

	class HTTPConnector : public IHttpRequest {
		public:
            HTTPConnector(){}
			virtual void SetHeaders(const StringMap& headers);
			virtual void SetQueryParams(const StringMap& queryParams);
			virtual void SetContent(const String& data);
			virtual void SetTimeout(int seconds);
			virtual bool Execute(Method method, const String& url);
			virtual const String& GetExecuteErrorMessage() const;
			virtual int GetHttpStatusCode() const;
			virtual const StringMap& GetResponseHeaders() const;
			virtual const String& GetResponseData() const;

			virtual  ~HTTPConnector();

		private:
        
            StringMap  m_queryParams;
            StringMap  m_requestHeaders;
            String m_bodyData;
            StringMap  m_responseHeaders;
            String m_errorMessage;
			String m_response;
			int m_statusCode;
            int timeout;

			HTTPConnector(const HTTPConnector &);
	};
}


#endif /* HTTPCONNECTOR_H_ */
