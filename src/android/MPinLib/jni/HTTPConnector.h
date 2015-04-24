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
			HTTPConnector(JNIEnv*) throw(IllegalArgumentException);

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
			JNIEnv* m_pjenv;

			// JNI CLASES ::
			jclass m_pjhttpRequestCls;
			jclass m_pjhashtableCls;

			// JNI OBJECTS ::
			jobject m_pjhttpRequest;

			// C++ Member variables
			String m_errorMessage;
			StringMap  m_responseHeaders;
			String m_response;
			int m_statusCode;

			HTTPConnector();
			HTTPConnector(const HTTPConnector &);
			jobject createJavaMap(const StringMap& map);
			void convertJHashtable2StringMap(jobject jhashtable, IN OUT StringMap & a_map) throw(IllegalArgumentException);
			void convertJString2String(const jstring js, IN OUT String & str);
	};
}


#endif /* HTTPCONNECTOR_H_ */
