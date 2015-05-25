/* 
 * File:   HttpRequest.h
 * Author: mony
 *
 * Created on February 11, 2015, 10:24 AM
 */

#ifndef HTTPREQUEST_H
#define	HTTPREQUEST_H

#include "mpin_sdk.h"

#include "CvHttpRequest.h"

using CvShared::Seconds;

class CHttpRequest : public MPinSDK::IHttpRequest
{
public:
	typedef MPinSDK::String String;
	typedef MPinSDK::StringMap StringMap;
	
	CHttpRequest(const Seconds& aTimeout = CvHttpRequest::TIMEOUT_INFINITE);
	virtual ~CHttpRequest();
	
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
	CHttpRequest(const CHttpRequest& orig);
	
	String			m_id;
	
	CvHttpRequest	m_request;
	String			m_requestData;	//Used for debugging purposes only	
	StringMap		m_queryParams;
	Seconds			m_timeout;
	
	StringMap		m_responseHeaders;
	String			m_responseData;
	String			m_errorMsg;
};

#endif	/* HTTPREQUEST_H */

