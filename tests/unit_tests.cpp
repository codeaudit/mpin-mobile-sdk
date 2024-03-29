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

#include "core/mpin_sdk.h"
#include "contexts/auto_context.h"
#include "common/http_request.h"
#include "common/access_number_thread.h"
#include "CvTime.h"
#include "CvLogger.h"

#define BOOST_TEST_MODULE Simple testcases
#include "boost/test/included/unit_test.hpp"

typedef MPinSDK::User User;
typedef MPinSDK::UserPtr UserPtr;
typedef MPinSDK::Status Status;
typedef MPinSDK::String String;

static AutoContext context;
static MPinSDK sdk;
static MPinSDK::StringMap config;
//static const char *backend = "http://ec2-54-77-232-113.eu-west-1.compute.amazonaws.com";
static const char *backend = "http://ec2-52-28-120-46.eu-central-1.compute.amazonaws.com";

using namespace boost::unit_test;

BOOST_AUTO_TEST_CASE(testNoInit)
{
    CvShared::InitLogger("cvlog.txt", CvShared::enLogLevel_None);

    int argc = framework::master_test_suite().argc;
    if(argc > 1)
    {
        char **argv = framework::master_test_suite().argv;
        backend = argv[1];
    }

    Status s = sdk.TestBackend("12354");
    BOOST_CHECK(s == Status::FLOW_ERROR);

    s = sdk.SetBackend("12354");
    BOOST_CHECK(s == Status::FLOW_ERROR);

    UserPtr user = sdk.MakeNewUser("testUser");

    s = sdk.StartRegistration(user);
    BOOST_CHECK(s == Status::FLOW_ERROR);
    BOOST_CHECK(user->GetState() == User::INVALID);

    s = sdk.RestartRegistration(user);
    BOOST_CHECK(s == Status::FLOW_ERROR);
    BOOST_CHECK(user->GetState() == User::INVALID);

    s = sdk.FinishRegistration(user);
    BOOST_CHECK(s == Status::FLOW_ERROR);
    BOOST_CHECK(user->GetState() == User::INVALID);

    s = sdk.Authenticate(user);
    BOOST_CHECK(s == Status::FLOW_ERROR);
    BOOST_CHECK(user->GetState() == User::INVALID);
}

BOOST_AUTO_TEST_CASE(testInit)
{
    config.Put(MPinSDK::CONFIG_BACKEND, backend);

    Status s = sdk.Init(config, &context);

    BOOST_CHECK(s == Status::OK);
}

BOOST_AUTO_TEST_CASE(testBackend)
{
    Status s = sdk.TestBackend("https://m-pindemo.certivox.org");
    BOOST_CHECK(s == Status::OK);

    s = sdk.TestBackend("https://blabla.certivox.org");
    BOOST_CHECK(s != Status::OK);
}

BOOST_AUTO_TEST_CASE(setBackend)
{
    Status s = sdk.SetBackend("https://blabla.certivox.org");
    BOOST_CHECK(s != Status::OK);

    s = sdk.SetBackend(backend);
    BOOST_CHECK(s == Status::OK);
}

BOOST_AUTO_TEST_CASE(testUsers1)
{
    UserPtr user = sdk.MakeNewUser("testUser");
    BOOST_CHECK(user->GetState() == User::INVALID);

    Status s = sdk.StartRegistration(user);

    BOOST_CHECK(s == Status::OK);
    BOOST_CHECK(user->GetState() == User::ACTIVATED);

    sdk.DeleteUser(user);
    std::vector<UserPtr> users;
    sdk.ListUsers(users);
    BOOST_CHECK(users.empty());
}

BOOST_AUTO_TEST_CASE(testUsers2)
{
    UserPtr user = sdk.MakeNewUser("testUser");
    Status s = sdk.StartRegistration(user);

    std::vector<UserPtr> users;
    sdk.ListUsers(users);
    BOOST_CHECK(users.size() == 1);

    sdk.DeleteUser(user);
}

BOOST_AUTO_TEST_CASE(testUsers3)
{
    UserPtr user = sdk.MakeNewUser("testUser");

    Status s = sdk.Authenticate(user);
    BOOST_CHECK(s == Status::FLOW_ERROR);
    BOOST_CHECK(user->GetState() == User::INVALID);

    s = sdk.FinishRegistration(user);
    BOOST_CHECK(s == Status::FLOW_ERROR);
    BOOST_CHECK(user->GetState() == User::INVALID);

    s = sdk.RestartRegistration(user);
    BOOST_CHECK(s == Status::FLOW_ERROR);
    BOOST_CHECK(user->GetState() == User::INVALID);
}

BOOST_AUTO_TEST_CASE(testUsers4)
{
    int count = 10;
    for(int i = 0; i < count; ++i)
    {
        String id;
        id.Format("testUser%03d", i);
        UserPtr user = sdk.MakeNewUser(id);

        Status s = sdk.StartRegistration(user);
        BOOST_CHECK(s == Status::OK);
        BOOST_CHECK(user->GetState() == User::ACTIVATED);
    }

    std::vector<UserPtr> users;
    sdk.ListUsers(users);
    BOOST_CHECK(users.size() == count);

    for(std::vector<UserPtr>::iterator i = users.begin(); i != users.end(); ++i)
    {
        UserPtr user = *i;
        sdk.DeleteUser(user);
    }

    sdk.ListUsers(users);
    BOOST_CHECK(users.empty());
}

BOOST_AUTO_TEST_CASE(testUsers5)
{
    UserPtr user = sdk.MakeNewUser("testUser");
    sdk.StartRegistration(user);

    BOOST_CHECK(!sdk.CanLogout(user));
    BOOST_CHECK(!sdk.Logout(user));
    sdk.DeleteUser(user);
}

BOOST_AUTO_TEST_CASE(testRegister1)
{
    UserPtr user = sdk.MakeNewUser("testUser");
    Status s = sdk.StartRegistration(user);

    BOOST_CHECK(s == Status::OK);
    BOOST_CHECK(user->GetState() == User::ACTIVATED);

    context.SetPin("1234");

    s = sdk.FinishRegistration(user);
    BOOST_CHECK(s == Status::OK);

    sdk.DeleteUser(user);
}

BOOST_AUTO_TEST_CASE(testRegister2)
{
    UserPtr user = sdk.MakeNewUser("testUser");
    Status s = sdk.StartRegistration(user);

    BOOST_CHECK(s == Status::OK);
    BOOST_CHECK(user->GetState() == User::ACTIVATED);

    s = sdk.RestartRegistration(user);
    BOOST_CHECK(s == Status::FLOW_ERROR);
    BOOST_CHECK(user->GetState() == User::ACTIVATED);

    sdk.DeleteUser(user);
}

BOOST_AUTO_TEST_CASE(testRegister3)
{
    UserPtr user = sdk.MakeNewUser("");
    Status s = sdk.StartRegistration(user);

    BOOST_CHECK(s == Status::HTTP_REQUEST_ERROR);
    BOOST_CHECK(user->GetState() == User::INVALID);

    s = sdk.FinishRegistration(user);
    BOOST_CHECK(s == Status::FLOW_ERROR);

    sdk.DeleteUser(user);
}

BOOST_AUTO_TEST_CASE(testRegister4)
{
    UserPtr user = sdk.MakeNewUser("testUser");
    Status s = sdk.StartRegistration(user);

    BOOST_CHECK(s == Status::OK);
    BOOST_CHECK(user->GetState() == User::ACTIVATED);

    context.SetPin("");

    s = sdk.FinishRegistration(user);
    BOOST_CHECK(s == Status::PIN_INPUT_CANCELED);
    BOOST_CHECK(user->GetState() == User::ACTIVATED);

    sdk.DeleteUser(user);
}

BOOST_AUTO_TEST_CASE(testAuthenticate1)
{
    UserPtr user = sdk.MakeNewUser("testUser");
    Status s = sdk.StartRegistration(user);

    BOOST_CHECK(s == Status::OK);
    BOOST_CHECK(user->GetState() == User::ACTIVATED);

    context.SetPin("1234");

    s = sdk.FinishRegistration(user);
    BOOST_CHECK(s == Status::OK);

    s = sdk.Authenticate(user);
    BOOST_CHECK(s == Status::OK);

    String authData;
    s = sdk.Authenticate(user, authData);
    BOOST_CHECK(s == Status::OK);

    context.SetPin("1235");
    s = sdk.Authenticate(user);
    BOOST_CHECK(s == Status::INCORRECT_PIN);

    context.SetPin("1234");
    s = sdk.Authenticate(user);
    BOOST_CHECK(s == Status::OK);

    sdk.DeleteUser(user);
}

BOOST_AUTO_TEST_CASE(testAuthenticate2)
{
    UserPtr user = sdk.MakeNewUser("testUser");
    Status s = sdk.StartRegistration(user);

    BOOST_CHECK(s == Status::OK);
    BOOST_CHECK(user->GetState() == User::ACTIVATED);

    context.SetPin("1234");

    s = sdk.FinishRegistration(user);
    BOOST_CHECK(s == Status::OK);

    context.SetPin("1111");
    s = sdk.Authenticate(user);
    BOOST_CHECK(s == Status::INCORRECT_PIN);
    BOOST_CHECK(user->GetState() == User::REGISTERED);
    s = sdk.Authenticate(user);
    BOOST_CHECK(s == Status::INCORRECT_PIN);
    BOOST_CHECK(user->GetState() == User::REGISTERED);
    s = sdk.Authenticate(user);
    BOOST_CHECK(s == Status::INCORRECT_PIN);
    BOOST_CHECK(user->GetState() == User::BLOCKED);

    sdk.DeleteUser(user);
}

BOOST_AUTO_TEST_CASE(testAuthenticateOTP)
{
    UserPtr user = sdk.MakeNewUser("testUser");
    Status s = sdk.StartRegistration(user);

    BOOST_CHECK(s == Status::OK);
    BOOST_CHECK(user->GetState() == User::ACTIVATED);

    context.SetPin("1234");

    s = sdk.FinishRegistration(user);
    BOOST_CHECK(s == Status::OK);

    MPinSDK::OTP otp;
    s = sdk.AuthenticateOTP(user, otp);
    BOOST_CHECK(s == Status::OK);
    BOOST_CHECK(user->GetState() == User::REGISTERED);
    BOOST_CHECK(otp.status == Status::RESPONSE_PARSE_ERROR);

    sdk.DeleteUser(user);
}

static AccessNumberThread g_accessNumberThread;

BOOST_AUTO_TEST_CASE(testAuthenticateAN1)
{
    // Register user
    UserPtr user = sdk.MakeNewUser("testUser");
    Status s = sdk.StartRegistration(user);

    BOOST_CHECK(s == Status::OK);
    BOOST_CHECK(user->GetState() == User::ACTIVATED);

    context.SetPin("1234");

    s = sdk.FinishRegistration(user);
    BOOST_CHECK(s == Status::OK);

    // Request access number
    HttpRequest req;
    HttpRequest::StringMap headers;
    headers.Put("Content-Type", "application/json");
    headers.Put("Accept", "*/*");
    req.SetHeaders(headers);

    String url = String().Format("%s/rps/getAccessNumber", backend);
    bool res = req.Execute(MPinSDK::IHttpRequest::POST, url);

    BOOST_CHECK(res == true);
    BOOST_CHECK(req.GetHttpStatusCode() == 200);

    String data = req.GetResponseData();
    util::JsonObject json;
    res = json.Parse(data.c_str());
    BOOST_CHECK(res == true);

    String accessNumber = json.GetStringParam("accessNumber");
    BOOST_CHECK(accessNumber.length() > 0);

    // Start access number thread
    g_accessNumberThread.Start(backend, json.GetStringParam("webOTT"), sdk.GetClientParam("authenticateURL"));

    // Authenticate with access number
    s = sdk.AuthenticateAN(user, accessNumber);
    BOOST_CHECK(s == Status::OK);

    // The backend *must* support logout data
    BOOST_CHECK(sdk.CanLogout(user));
    BOOST_CHECK(sdk.Logout(user));

    sdk.DeleteUser(user);
}

BOOST_AUTO_TEST_CASE(testAuthenticateAN2)
{
    // Register user
    UserPtr user = sdk.MakeNewUser("testUser");
    Status s = sdk.StartRegistration(user);

    BOOST_CHECK(s == Status::OK);
    BOOST_CHECK(user->GetState() == User::ACTIVATED);

    context.SetPin("1234");

    s = sdk.FinishRegistration(user);
    BOOST_CHECK(s == Status::OK);

    // Request access number
    HttpRequest req;
    HttpRequest::StringMap headers;
    headers.Put("Content-Type", "application/json");
    headers.Put("Accept", "*/*");
    req.SetHeaders(headers);

    String url = String().Format("%s/rps/getAccessNumber", backend);
    bool res = req.Execute(MPinSDK::IHttpRequest::POST, url);

    BOOST_CHECK(res == true);
    BOOST_CHECK(req.GetHttpStatusCode() == 200);

    String data = req.GetResponseData();
    util::JsonObject json;
    res = json.Parse(data.c_str());
    BOOST_CHECK(res == true);

    String accessNumber = json.GetStringParam("accessNumber");
    BOOST_CHECK(accessNumber.length() > 0);

    // Simulate wrong access number
    String originalAccessNumber = accessNumber;
    accessNumber[3] += (char) 1;
    if(accessNumber[3] > '9')
    {
        accessNumber[3] = '0';
    }

    // Start access number thread
    g_accessNumberThread.Start(backend, json.GetStringParam("webOTT"), sdk.GetClientParam("authenticateURL"));

    // Authenticate with access number
    s = sdk.AuthenticateAN(user, accessNumber);
    BOOST_CHECK(s == Status::INCORRECT_ACCESS_NUMBER);

    // The backend *must* support logout data
    BOOST_CHECK(!sdk.CanLogout(user));

    // Simulate wrong pin - must fail on access number validation too
    context.SetPin("1233");
    s = sdk.AuthenticateAN(user, accessNumber);
    BOOST_CHECK(s == Status::INCORRECT_ACCESS_NUMBER);

    // Fix access number - must fail with incorrect pin already
    accessNumber = originalAccessNumber;
    s = sdk.AuthenticateAN(user, accessNumber);
    BOOST_CHECK(s == Status::INCORRECT_PIN);

    sdk.DeleteUser(user);

    // Wait the AccessNumberThread to complete
    CvShared::SleepFor(CvShared::Millisecs(AccessNumberThread::RETRY_INTERVAL_MILLISEC * AccessNumberThread::MAX_TRIES).Value());
}
