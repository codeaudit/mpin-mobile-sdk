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
