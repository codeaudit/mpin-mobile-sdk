using MPinSDK.Models;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MPinRC;
using Windows.UI.Core;
using Windows.ApplicationModel.Resources;

namespace MPinSDK
{
    [Windows.Foundation.Metadata.WebHostHidden]
    public class MPin : IDisposable
    {
        #region Members
        static MPinWrapper mPtr;
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
        /// <returns></returns>
        public Status Init(IDictionary<string, string> config, IContext context)
        {
            StatusWrapper sw = mPtr.Construct(config, context);
            this.context = context;
            return new Status(sw.Code, sw.Error);
        }

        /// <summary>
        /// Creates a new <see cref="User"/> object.
        /// </summary>
        /// <param name="id">The unique identity of the user.</param>
        /// <param name="deviceName">Optional device name, which is passed to the RPA to store it and use it later to determine which M-Pin ID is associated with this device.</param>
        /// <returns></returns>
        public User MakeNewUser(string id, string deviceName = "")
        {
            UserWrapper wrapper = mPtr.MakeNewUser(id, deviceName);
            return new User(wrapper);
        }

        /// <summary>
        /// Deletes a <see cref="User"/> from the Users List maintained by the SDK and all the data related this User, such as the User’s M-Pin ID, State, and M-Pin Token.
        /// </summary>
        /// <param name="user">The user instance.</param>
        public void DeleteUser(User user)
        {
            if (mPtr != null && user != null)
                mPtr.DeleteUser(user.Wrapper);
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
        /// Resets the PIN of the specified <see cref="User"/> instance.
        /// </summary>
        /// <param name="user">The user.</param>
        /// <returns> A <see cref="Status"/> which indicates whether the operation was successful or not.</returns>
        public Status ResetPin(User user)
        {
            StatusWrapper status = user != null ? mPtr.ResetPin(user.Wrapper) : new StatusWrapper() { Code = -1, Error=ResourceLoader.GetForCurrentView().GetString("NullUser")};
            return new Status(status.Code, status.Error);
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
            StatusWrapper sw = user != null ? mPtr.StartRegistration(user.Wrapper, userData) : new StatusWrapper() { Code = -1, Error = ResourceLoader.GetForCurrentView().GetString("NullUser") };
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

            StatusWrapper sw = mPtr.RestartRegistration(user.Wrapper, userData);
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

            StatusWrapper sw = mPtr.FinishRegistration(user.Wrapper);
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
        /// <returns></returns>
        public Status AuthenticateOTP(User user, OTP otp)
        {
            if (otp == null)
                return Authenticate(user);

            if (user == null)
                return new Status(-1, ResourceLoader.GetForCurrentView().GetString("NullUser"));

            StatusWrapper sw = mPtr.AuthenticateOTP(user.Wrapper, otp.Wrapper);
            return new Status(sw.Code, sw.Error);
        }

        public Status AuthenticateAN(User user, string accessNumber)
        {
            if (user == null)
                return new Status(-1, ResourceLoader.GetForCurrentView().GetString("NullUser"));
            
            StatusWrapper sw = mPtr.AuthenticateAN(user.Wrapper, accessNumber);
            return new Status(sw.Code, sw.Error);
        }

        public Status TestBackend(string backend, string rpsPrefix = "")
        {
            StatusWrapper status = mPtr.TestBackend(backend, rpsPrefix);
            return new Status(status.Code, status.Error);
        }

        public Status SetBackend(string backend, string rpsPrefix = "")
        {
            StatusWrapper status = mPtr.SetBackend(backend, rpsPrefix);
            return new Status(status.Code, status.Error);
        }

        public bool CanLogout(User user)
        {
            if (user == null)
                return false;

            return mPtr.CanLogout(user.Wrapper);
        }

        public bool Logout(User user)
        {
            if (user == null)
                return false;
            
            return mPtr.Logout(user.Wrapper);
        }

        public string GetClientParam(string key)
        {
            if (string.IsNullOrEmpty(key))
                return string.Empty;

            return mPtr.GetClientParam(key);
        }

        #region IDisposable
        public void Dispose()
        {
            lock (this)
            {
                mPtr.Destroy();
                mPtr = null;
            }
        }
        #endregion // IDisposable

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
