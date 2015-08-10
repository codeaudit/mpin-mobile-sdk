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
