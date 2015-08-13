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
 * File:   HttpRequest.cpp
 * Author: mony
 * 
 * Created on February 11, 2015, 10:24 AM
 */

#include "HttpRequest.h"

CHttpRequest::CHttpRequest(const Seconds& aTimeout) : m_timeout(aTimeout)
{
}

CHttpRequest::~CHttpRequest()
{
}

void CHttpRequest::SetHeaders(const StringMap& headers)
{
	CMapHttpHeaders tmpHeaders;
	
	StringMap::const_iterator itr = headers.begin();
	for ( ; itr != headers.end(); ++itr )
	{
		tmpHeaders[itr->first] = itr->second;
	}

	m_request.SetHeaders( tmpHeaders );
}

void CHttpRequest::SetQueryParams(const StringMap& queryParams)
{
	m_queryParams = queryParams;
}

void CHttpRequest::SetContent(const String& data)
{
	m_requestData = data;	// For debugging
	m_request.SetContent( data.data(), data.length() );
}

void CHttpRequest::SetTimeout(int seconds)
{
	m_timeout = seconds;
}

bool CHttpRequest::Execute( MPinSDK::IHttpRequest::Method method, const String& url)
{
	enHttpMethod_t cvHttpMethod = enHttpMethod_Unknown;
	String strMethod;
	
	switch ( method )
	{
		case MPinSDK::IHttpRequest::GET: cvHttpMethod = enHttpMethod_GET; strMethod = "GET"; break;
        case MPinSDK::IHttpRequest::POST: cvHttpMethod = enHttpMethod_POST; strMethod = "POST"; break;
        case MPinSDK::IHttpRequest::PUT: cvHttpMethod = enHttpMethod_PUT; strMethod = "PUT"; break;
        case MPinSDK::IHttpRequest::DELETE: cvHttpMethod = enHttpMethod_DEL; strMethod = "DEL"; break;
	}
	
	if ( cvHttpMethod == enHttpMethod_Unknown )
	{
		m_errorMsg = "Unsupported HTTP method";
		return false;
	}
	
	String fullUrl = url;
	
	if ( !m_queryParams.empty() )
	{
		fullUrl += '?';
		
		StringMap::const_iterator itr = m_queryParams.begin();
		for ( ;itr != m_queryParams.end(); ++itr )
		{
			fullUrl += String().Format( "%s=%s&", itr->first.c_str(), itr->second.c_str() );
		}

		fullUrl.TrimRight("&");
	}
	
//	printf( "--> %s %s [%s]\n", strMethod.c_str(), fullUrl.c_str(), m_requestData.c_str() );

	m_request.SetMethod( cvHttpMethod );
	m_request.SetUrl( fullUrl );
	
	if ( m_request.Execute( m_timeout ) == CvHttpRequest::enStatus_NetworkError )
	{
		m_errorMsg = "Failed to execute HTTP request";
		return false;
	}
	
	const CMapHttpHeaders& headers = m_request.GetResponseHeaders();
	
	CMapHttpHeaders::const_iterator itr = headers.begin();
	for ( ;itr != headers.end(); ++itr )
	{
		m_responseHeaders[itr->first] = itr->second;
	}

	m_responseData = m_request.GetResponse();
	
//	printf( "<-- %ld [%s]\n", m_request.GetResponseCode(), m_responseData.c_str() );

	return true;
}

const MPinSDK::String& CHttpRequest::GetExecuteErrorMessage() const
{
	return m_errorMsg;
}

int CHttpRequest::GetHttpStatusCode() const
{
	return (int)m_request.GetResponseCode();
}

const MPinSDK::StringMap& CHttpRequest::GetResponseHeaders() const
{
	return m_responseHeaders;
}

const MPinSDK::String& CHttpRequest::GetResponseData() const
{
	return m_responseData;
}


