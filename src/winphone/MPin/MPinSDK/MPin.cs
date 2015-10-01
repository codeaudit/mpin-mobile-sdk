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

using MPinSDK.Models;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MPinRC;
using Windows.UI.Core;
using Windows.ApplicationModel.Resources;
using System.Diagnostics;
using Windows.Data.Json;

namespace MPinSDK
{
    /// <summary>
    /// The MPin SDK class.
    /// </summary>
    [Windows.Foundation.Metadata.WebHostHidden]
    public class MPin : IDisposable
    {
        #region Members
        static MPinWrapper mPtr;
        private static readonly object lockObject = new object();
        private IContext context { get; set; }
        #endregion

        #region C'tor
        /// <summary>
        /// Initializes a new instance of the <see cref="MPin"/> SDK class.
        /// </summary>
        public MPin()
        {
            mPtr = new MPinWrapper();            
        }
        #endregion
        
        #region Methods
        /// <summary>
        /// Initializes the <see cref="MPin"/> SDK instance.
        /// </summary>
        /// <param name="config">A key-value map of configuration parameters. Unsupported parameters will be ignored. Currently, the Core recognized the following parameters: backend - the URL of the M-Pin back-end service (Mandatory) and rpsPrefix - the prefix that should be added for requests to the RPS (Optional). The default value is "rps". </param>
        /// <param name="context">An <see cref="IContext"/> instance.</param>
        /// <returns> A <see cref="Status"/> which indicates whether the operation was successful or not.</returns>
        public Status Init(IDictionary<string, string> config, IContext context)
        {
            StatusWrapper sw;
            lock (lockObject)
            {
                sw = mPtr.Construct(config, context);
                this.context = context;                
            }
            
            return new Status(sw.Code, sw.Error);
        }

        /// <summary>
        /// Creates a new <see cref="User"/> object.
        /// </summary>
        /// <param name="id">The unique identity of the user.</param>
        /// <param name="deviceName">Optional device name, which is passed to the RPA to store it and use it later to determine which M-Pin ID is associated with this device.</param>
        /// <returns> A <see cref="Status"/> which indicates whether the operation was successful or not.</returns>
        public User MakeNewUser(string id, string deviceName = "")
        {
            UserWrapper wrapper;
            lock (lockObject)
            {
                wrapper = mPtr.MakeNewUser(id, deviceName);
            }

            return new User(wrapper);
        }

        /// <summary>
        /// Deletes a <see cref="User"/> from the Users List maintained by the SDK and all the data related this User, such as the User’s M-Pin ID, State, and M-Pin Token.
        /// </summary>
        /// <param name="user">The user instance.</param>
        public void DeleteUser(User user)
        {            
            lock (lockObject)
            {
                if (mPtr != null && user != null)
                    mPtr.DeleteUser(user.Wrapper);
            }         
        }

        /// <summary>
        /// Populates a list with all currently existing Users, irrespective of their state. (Different Users might be in different states, reflecting their registration status.) These are the users that are currently available in the SDK’s Users List.
        /// </summary>
        /// <param name="users">Returns a list of users in List format.</param>
        public void ListUsers(List<User> users)
        {
            if (users != null)
            {
                IList<UserWrapper> usersList = new List<UserWrapper>();
                mPtr.ListUsers(usersList);
                foreach (var user in usersList)
                {
                    users.Add(new User(user));
                }
            }
        }

        /// <summary>
        /// Initializes the registration process for a <see cref="User"/> which has been alredy created with the MakeNewUser method. This causes the RPA to begin an identity verification procedure for the User (like sending a verification email, for instance). At that, the User’s status changes to StartedRegistration and remains like this until the FinishRegistration method has been executed successfully.
        /// </summary>
        /// <param name="user">The <see cref="User"/> object instance.</param>
        /// <param name="userData"> Optionally, the application might pass additional userData which might help the RPA to verify the user identity. The RPA might decide to verify the identity without starting a verification process. In this case the Status of the call will still be Status::OK, but the User State will be Activated. </param>
        /// <returns> A <see cref="Status"/> which indicates whether the operation was successful or not.</returns>
        /// <remarks> Under certain scenarios, like a demo application, the RPA might be configured to verify identities without starting a verification process. In this case, the status of the call will still be OK, but the User state will be set to Activated. </remarks>
        public Status StartRegistration(User user, string userData = "")
        {
            StatusWrapper sw;
            lock (lockObject)
            {
                sw = user != null ? mPtr.StartRegistration(user.Wrapper, userData) : new StatusWrapper() { Code = -1, Error = ResourceLoader.GetForCurrentView().GetString("NullUser") };
            }

            return new Status(sw.Code, sw.Error);
        }

        /// <summary>
        /// This method re-initializes the registration process for a <see cref="User"/> that already started it. 
        /// </summary>
        /// <param name="user">The <see cref="User"/> object instance.</param>
        /// <param name="userData"> Optionally, the application might pass additional userData which might help the RPA to verify the user identity. The RPA might decide to verify the identity without starting a verification process. In this case the Status of the call will still be Status::OK, but the User State will be Activated. </param>
        /// <returns> A <see cref="Status"/> which indicates whether the operation was successful or not.</returns>
        /// <remarks>The difference between this method and the StartRegistration() is that during this one, no new M-Pin ID will be generated for the user, but the already generated one will be used. So StartRegistration can be called only for Users in the StartedRegistration state and RestartRegistration is designed to be used for Users in the Invalid state.</remarks>        
        public Status RestartRegistration(User user, string userData = "")
        {
            if (user == null)
                return new Status(-1, ResourceLoader.GetForCurrentView().GetString("NullUser"));

            StatusWrapper sw;            
            lock (lockObject)
            {
                sw = mPtr.RestartRegistration(user.Wrapper, userData);
            }

            return new Status(sw.Code, sw.Error);
        }

        /// <summary>
        /// A method used to registers a user using the SMS flow.
        /// </summary>
        /// <param name="mpinId">The mpin identifier.</param>
        /// <param name="activationKey">The activation key.</param>
        /// <returns></returns>
        public Status VerifyUser(string mpinId, string activationKey)
        {
            if (string.IsNullOrEmpty(mpinId) || string.IsNullOrEmpty(activationKey))
                return new Status(-1, ResourceLoader.GetForCurrentView().GetString("NullUser"));
            // TODOOO: check what is passed from the server and how to parse it

            JsonObject mpinIdJSON = JsonObject.Parse(mpinId);

            StatusWrapper sw;
            string userId = mpinIdJSON.GetNamedString("userID"); // extract from json..
            User user = MakeNewUser(userId);
            lock(lockObject)
            {
                sw = mPtr.VerifyUser(user.Wrapper, mpinId, activationKey);
            }

            return new Status(sw.Code, sw.Error);
        }

        /// <summary>
        /// Finalizes the <see cref="User"/> registration process. The method attempts to retrieve the M-Pin Client Key; if the User's identity has been verified, the Client Key is obtained and then the M-Pin PIN-Pad displayed to the user for setting their PIN code. 
        /// </summary>
        /// <param name="user">The <see cref="User"/> object instance.</param>
        /// <returns> A <see cref="Status"/> which indicates whether the operation was successful or not. On successful completion, the <see cref="User"/> state is set to Registered and the method returns OK.</returns>
        public Status FinishRegistration(User user)
        {
            if (user == null)
                return new Status(-1, ResourceLoader.GetForCurrentView().GetString("NullUser"));

            StatusWrapper sw;
            lock (lockObject)
            {
                sw = mPtr.FinishRegistration(user.Wrapper);
            }

            return new Status(sw.Code, sw.Error);
        }

        /// <summary>
        /// Authenticates a <see cref="User" /> for the needs of the overlaying application. This method will attempt to retrieve the Time Permits for the user, and if successful, it will show the PIN Pad UI to get the user's PIN code. Having the user PIN Code and the stored M-Pin Token, the method will do the authentication against the M-Pin Authentication Server and then will login into the RPA.
        /// </summary>
        /// <param name="user">The <see cref="User" /> to be authenticated.</param>
        /// <param name="authResultData"> A <see cref="User"/> data passed back by the RPA (if configured to) together with the authentication response.</param>
        /// <returns> A <see cref="Status"/> which indicates whether the operation was successful or not.</returns>        
        public Status Authenticate(User user, string authResultData = null)
        {
            if (user == null)
                return new Status(-1, ResourceLoader.GetForCurrentView().GetString("NullUser"));

            StatusWrapper sw = authResultData == null
                    ? mPtr.Authenticate(user.Wrapper)
                    : mPtr.AuthenticateResultData(user.Wrapper, authResultData);
            return new Status(sw.Code, sw.Error);
        }

        /// <summary>
        /// Authenticates the <see cref="User"/> and, if authentication has been successful, the RPA issues One-Time Password (OTP) for authenticating with a RADIUS server. (The authentication itself doesn’t log the User in: instead, the result of the authentication is the issuing of the OTP.)
        /// </summary>
        /// <param name="user">The <see cref="User"/> to be authenticated.</param>
        /// <param name="otp">When the authentication is successful, in addition to the OK status, the method returns also an <see cref="OTP"/> structure generated by the RPA.</param>
        /// <returns> A <see cref="Status"/> which indicates whether the operation was successful or not.</returns>
        public Status AuthenticateOTP(User user, OTP otp)
        {
            if (otp == null)
                return Authenticate(user);

            if (user == null)
                return new Status(-1, ResourceLoader.GetForCurrentView().GetString("NullUser"));

            StatusWrapper sw = mPtr.AuthenticateOTP(user.Wrapper, otp.Wrapper);
            return new Status(sw.Code, sw.Error);
        }

        /// <summary>
        /// Authenticates a <see cref="User"/> against an Access Number provided by a PC/browser session. After this authentication, the user will be able to log-in on to the PC/browser with the provided the Access Number while the authentication itself is performed on the user's mobile device.
        /// </summary>
        /// <param name="user">The <see cref="User"/> to be authenticated.</param>
        /// <param name="accessNumber">The Access Number provided by the PC/browser session. Required if Access Number authentication is being performed.</param>
        /// <returns> A <see cref="Status"/> which indicates whether the operation was successful or not.</returns>
        public Status AuthenticateAN(User user, string accessNumber)
        {
            if (user == null)
                return new Status(-1, ResourceLoader.GetForCurrentView().GetString("NullUser"));

            StatusWrapper sw = mPtr.AuthenticateAN(user.Wrapper, accessNumber);
            return new Status(sw.Code, sw.Error);
        }

        /// <summary>
        /// Tests whether the M-Pin back-end service is operational by sending a request for retrieving the Client settings to back-end’s URL.
        /// </summary>
        /// <param name="backend">The URL of the M-Pin back-end service to test.</param>
        /// <param name="rpsPrefix">An optional string representing the prefix for the requests to the RPS. Required only if the default prefix has been changed. If not provided, the value defaults to rps.</param>
        /// <returns> A <see cref="Status"/> which indicates whether the operation was successful or not.</returns>
        public Status TestBackend(string backend, string rpsPrefix = "")
        {
            StatusWrapper status;
            lock (lockObject)
            {
                status = mPtr.TestBackend(backend, rpsPrefix);
            }

            return new Status(status.Code, status.Error);
        }

        /// <summary>
        /// Modifies the currently configured M-Pin back-end service. The back-end is initially set at SDK initialization (i.e. through the <see cref="M:MPinSDK.MPin.Init"/> method), but it can be changed at any time using SetBackend.
        /// </summary>
        /// <param name="backend">The URL of the new M-Pin back-end service.</param>
        /// <param name="rpsPrefix">An optional string representing the prefix for the requests to the RPS. Required only if the default prefix has been changed. If not provided, the value defaults to rps.</param>
        /// <returns> A <see cref="Status"/> which indicates whether the operation was successful or not.</returns>
        public Status SetBackend(string backend, string rpsPrefix = "")
        {
            StatusWrapper status;
            lock (lockObject)
            {
                status = mPtr.SetBackend(backend, rpsPrefix);
            }
            
            return new Status(status.Code, status.Error);
        }

        /// <summary>
        /// Examines whether RPA supports logging out the <see cref="User" /> from the mobile device that have been used to provide the Access Number for authenticating the user to another device/browser session. Therefore, the method should be used after Access Number authentication, i.e. following the <see cref="M:MPinSDK.MPin.AuthenticateAN">AuthenticateAN(user, accessNumber)</see> method.
        /// </summary>
        /// <param name="user">The user.</param>
        /// <returns>True if the user can be logged out from the remote server, False - if (s)he cannot.</returns>
        public bool CanLogout(User user)
        {
            if (user == null)
                return false;

            bool canLogout;            
            lock (lockObject)
            {
                canLogout = mPtr.CanLogout(user.Wrapper);
            }

            return canLogout;
        }

        /// <summary>
        /// Attempts to log out the end-user from a remote (browser) session after successful authentication through the <see cref="M:MPinSDK.MPin.AuthenticateAN">AuthenticateAN(user, accessNumber)</see> method. 
        /// <remarks>Before calling this method, make sure that the logout data has been provided by the RPA and that the logout operation is feasible.</remarks>
        /// </summary>
        /// <param name="user">The user.</param>
        /// <returns>True if the log-out request to the RPA has been successful, false - if failed.</returns>
        public bool Logout(User user)
        {
            if (user == null)
                return false;

            bool logout;
            lock (lockObject)
            {
                logout = mPtr.Logout(user.Wrapper);
            }
            
            return logout;
        }

        /// <summary>
        /// Returns the value for a Client Setting with the given key. Client settings that might interest the applications are: 
        /// accessNumberDigits - The number of access number digits that should be entered by the user, prior to calling <see cref="M:MPinSDK.MPin.AuthenticateAN">AuthenticateAN(user, accessNumber)</see> method. 
        /// setDeviceName - Indicator (true/false) whether the application should ask the user to insert a Device Name and pass it to the MakeNewUser() method.
        /// appID - The App ID used by the backend. The App ID is a unique ID assigned to each customer or application. It is a hex-encoded long numeric value. The App ID can be used only for information purposes, it doesn't affect the application's behavior in any way.
        /// </summary>
        /// <remarks> The value is returned as a <see cref="T:System.String"/> always, i.e. when a numeric or a boolean value is expected, the conversion should be handled by by the application.</remarks>
        /// <param name="key">The key.</param>
        /// <returns>А <see cref="T:System.String"/> value for a Client Setting with the given key.</returns>
        public string GetClientParam(string key)
        {
            if (string.IsNullOrEmpty(key))
                return string.Empty;

            string param = string.Empty;
            lock (lockObject)
            {
                param = mPtr.GetClientParam(key);
            }
            
            return param;
        }

        #region IDisposable
        /// <summary>
        /// Performs application-defined tasks associated with freeing, releasing, or resetting unmanaged resources.
        /// </summary>
        public void Dispose()
        {
            lock (lockObject)
            {
                mPtr.Destroy();
                mPtr = null;
            }            
        }
        #endregion // IDisposable

        /// <summary>
        /// Pass the application UI dispatcher to the MPin SDK so it could display the Pin Pad for setting up and entering a PIN when necessary.
        /// <remarks>It is important the method to be called after execution of the <see cref="M:MPinSDK.MPin.Init">Init</see> method. If not called - the application flow cannot be properly executed./></remarks>
        /// </summary>
        /// <param name="dispatcher">The application UI dispatcher.</param>
        public void SetUiDispatcher(Windows.UI.Core.CoreDispatcher dispatcher)
        {
            IPinPad pinpad = null;
            if (context != null)
            {
                pinpad = context.GetPinPad();
                if (pinpad != null)
                    pinpad.SetUiDispatcher(dispatcher);
            }

            if (context == null || pinpad == null)
            {
            }
        }
        #endregion
    }
}
