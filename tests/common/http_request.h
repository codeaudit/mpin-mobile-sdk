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

/*
 * MPinSDK::IHttpRequest implementation used for tests
 */

#ifndef _TEST_HTTP_REQUEST_H_
#define _TEST_HTTP_REQUEST_H_

#include "core/mpin_sdk.h"

class HttpRequest : public MPinSDK::IHttpRequest
{
public:
    typedef MPinSDK::String String;
    typedef MPinSDK::StringMap StringMap;

    HttpRequest() : m_timeout(0), m_httpStatusCode(0) {}
    virtual void SetHeaders(const StringMap& headers);
    virtual void SetQueryParams(const StringMap& queryParams);
    virtual void SetContent(const String& data);
    virtual void SetTimeout(int seconds);
    virtual bool Execute(Method method, const String& url);
    virtual const String& GetExecuteErrorMessage() const;
    virtual int GetHttpStatusCode() const;
    virtual const StringMap& GetResponseHeaders() const;
    virtual const String& GetResponseData() const;

private:
    StringMap m_requestHeaders;
    String m_requestQueryParams;
    String m_requestData;
    int m_timeout;
    int m_httpStatusCode;
    StringMap m_responseHeaders;
    String m_responseData;
    String m_errorMessage;
};


#endif // _TEST_HTTP_REQUEST_H_
