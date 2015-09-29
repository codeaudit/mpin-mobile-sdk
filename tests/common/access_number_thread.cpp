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

#include "access_number_thread.h"
#include "../common/http_request.h"
#include "CvTime.h"

void AccessNumberThread::Start(const String& backend, const String& webOTT, const String& authenticateURL)
{
    m_backend = backend;
    m_webOTT = webOTT;
    m_authenticateURL = authenticateURL;
    Create(NULL);
}

long AccessNumberThread::Body(void*)
{
    HttpRequest req;
    HttpRequest::StringMap headers;
    headers.Put("Content-Type", "application/json");
    headers.Put("Accept", "*/*");

    util::JsonObject json;
    json["webOTT"] = json::String(m_webOTT);
    String payload = json.ToString();

    String url = String().Format("%s/rps/accessnumber", m_backend.c_str());

    int retryCount = 0;
    while(req.GetHttpStatusCode() != 200 && retryCount++ < MAX_TRIES)
    {
        CvShared::SleepFor(CvShared::Millisecs(RETRY_INTERVAL_MILLISEC).Value());

        req.SetHeaders(headers);
        req.SetContent(payload);
        req.Execute(MPinSDK::IHttpRequest::POST, url);
    }

    json.Clear();
    util::JsonObject mpinResponse;
    mpinResponse.Parse(req.GetResponseData().c_str());
    json["mpinResponse"] = mpinResponse;
    payload = json.ToString();

    req.SetHeaders(headers);
    req.SetContent(payload);
    req.Execute(MPinSDK::IHttpRequest::POST, m_authenticateURL);

    return 0;
}
