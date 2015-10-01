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

#include "http_request.h"
#include "CvHttpRequest.h"

typedef MPinSDK::String String;
typedef MPinSDK::StringMap StringMap;
typedef MPinSDK::IHttpRequest IHttpRequest;

static enHttpMethod_t MPinToCvMethod(IHttpRequest::Method method)
{
    switch(method)
    {
    case IHttpRequest::GET:
        return enHttpMethod_GET;
    case IHttpRequest::POST:
        return enHttpMethod_POST;
    case IHttpRequest::PUT:
        return enHttpMethod_PUT;
    case IHttpRequest::DELETE:
        return enHttpMethod_DEL;
    case IHttpRequest::OPTIONS:
    case IHttpRequest::PATCH:
    default:
        assert(false);
        return enHttpMethod_Unknown;
    }
}

void HttpRequest::SetHeaders(const StringMap& headers)
{
    m_requestHeaders = headers;
}

void HttpRequest::SetQueryParams(const StringMap& queryParams)
{
    //m_requestQueryParams = queryParams;
    // TODO: Implement this
    assert(false);
}

void HttpRequest::SetContent(const String& data)
{
    m_requestData = data;
}

void HttpRequest::SetTimeout(int seconds)
{
    m_timeout = seconds;
}

bool HttpRequest::Execute(Method method, const String& url)
{
    m_httpStatusCode = 0;
    m_responseHeaders.clear();
    m_responseData.clear();
    m_errorMessage.clear();

    CvHttpRequest *cvReq = new CvHttpRequest(MPinToCvMethod(method));

    CMapHttpHeaders cvHeaders;
    for(StringMap::iterator i = m_requestHeaders.begin(); i != m_requestHeaders.end(); ++i)
    {
        cvHeaders[i->first] = i->second;
    }
    cvReq->SetHeaders(cvHeaders);

    if(!m_requestData.empty())
    {
        cvReq->SetContent(m_requestData.c_str(), m_requestData.length());
    }

    cvReq->SetUrl(url);

    CvShared::Seconds timeout = CvHttpRequest::TIMEOUT_INFINITE;
    if(m_timeout > 0)
    {
        timeout = m_timeout;
    }

    CvHttpRequest::enStatus_t cvStatus = cvReq->Execute(timeout);
    if(cvStatus == CvHttpRequest::enStatus_NetworkError)
    {
        m_errorMessage = cvReq->GetResponse();
        delete cvReq;
        return false;
    }

    m_httpStatusCode = cvReq->GetResponseCode();
    assert(m_httpStatusCode != 0);

    const CMapHttpHeaders& cvResponseHeaders = cvReq->GetResponseHeaders();
    for(CMapHttpHeaders::const_iterator i = cvResponseHeaders.begin(); i != cvResponseHeaders.end(); ++i)
    {
        m_responseHeaders[i->first] = i->second;
    }

    m_responseData = cvReq->GetResponse();

    delete cvReq;

    m_requestHeaders.clear();
    m_requestQueryParams.clear();
    m_requestData.clear();
    m_timeout = 0;

    return true;
}

const String& HttpRequest::GetExecuteErrorMessage() const
{
    return m_errorMessage;
}

int HttpRequest::GetHttpStatusCode() const
{
    return m_httpStatusCode;
}

const StringMap& HttpRequest::GetResponseHeaders() const
{
    return m_responseHeaders;
}

const String& HttpRequest::GetResponseData() const
{
    return m_responseData;
}
