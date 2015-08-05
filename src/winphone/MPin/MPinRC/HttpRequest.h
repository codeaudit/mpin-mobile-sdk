// Copyright (c) 2012-2015, Certivox
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// For full details regarding our CertiVox terms of service please refer to
// the following links:
//  * Our Terms and Conditions -
//    http://www.certivox.com/about-certivox/terms-and-conditions/
//  * Our Security and Privacy -
//    http://www.certivox.com/about-certivox/security-privacy/
//  * Our Statement of Position and Our Promise on Software Patents -
//    http://www.certivox.com/about-certivox/patents/

#pragma once

#include <string>
#include <collection.h>
#include "mpin_sdk.h"

using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Web::Http;

namespace MPinRC
{
	public interface class IHttpRequest
	{
		virtual void SetHeaders(Windows::Foundation::Collections::IMap<Platform::String^, Platform::String^>^ headers) = 0;
		virtual void SetQueryParams(Windows::Foundation::Collections::IMap<Platform::String^, Platform::String^>^ queryParams) = 0;
		virtual void SetContent(String^ data) = 0;
		virtual void SetTimeout(int seconds) = 0;		
	
		virtual bool Execute(Windows::Web::Http::HttpMethod^ method, String^ url) = 0;
		
		virtual String^ GetExecuteErrorMessage() = 0;
		virtual int GetHttpStatusCode() = 0;
		virtual Windows::Foundation::Collections::IMap<Platform::String^, Platform::String^>^ GetResponseHeaders() = 0;
		virtual String^ GetResponseData() = 0;
	};
	
	class HttpProxy : public MPinSDK::IHttpRequest
	{
	private:
		MPinRC::IHttpRequest^ managedRequest;
		MPinSDK::String responseData;
		MPinSDK::String errorMessage;
		MPinSDK::StringMap responseHeaders;
		int httpResponseCode;

		Windows::Web::Http::HttpMethod^ GetHttpMethod(MPinSDK::IHttpRequest::Method nativeMethod);
		
	public:
		HttpProxy(MPinRC::IHttpRequest^ request) { this->managedRequest = request; };
		~HttpProxy() {};

		virtual void SetHeaders(const MPinSDK::StringMap& headers);
		virtual void SetQueryParams(const MPinSDK::StringMap& queryParams);
		virtual void SetContent(const MPinSDK::String& data);
		virtual void SetTimeout(int seconds);
		virtual bool Execute(Method method, const MPinSDK::String& url);
		virtual const MPinSDK::String& GetExecuteErrorMessage() const;
		virtual int GetHttpStatusCode() const;
		virtual const MPinSDK::StringMap& GetResponseHeaders() const;
		virtual const MPinSDK::String& GetResponseData() const;
	};	
}