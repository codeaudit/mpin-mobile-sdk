using MPinSDK.Models;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MPinRC;
using Windows.UI.Core;

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
        public MPin()
        {
            mPtr = new MPinWrapper();
        }
        #endregion

        #region Methods
        public Status Init(IDictionary<string, string> config, IContext context)
        {
            StatusWrapper sw = mPtr.Construct(config, context);
            this.context = context;
            return new Status(sw.Code, sw.Error);
        }

        public User MakeNewUser(string id, string deviceName = "")
        {
            UserWrapper wrapper = mPtr.MakeNewUser(id, deviceName);
            return new User(wrapper);
        }

        public void DeleteUser(User user)
        {
            if (mPtr != null && user != null)
                mPtr.DeleteUser(user.Wrapper);
        }

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

        public Status ResetPin(User user)
        {
            StatusWrapper status = user != null ? mPtr.ResetPin(user.Wrapper) : new StatusWrapper() { Code = -1, Error="Null user!"};
            return new Status(status.Code, status.Error);
        }

        public Status StartRegistration(User user, string userData = "")
        {
            StatusWrapper sw = user != null ? mPtr.StartRegistration(user.Wrapper, userData) : new StatusWrapper() { Code = -1, Error = "Null user!" };
            return new Status(sw.Code, sw.Error);
        }

        public Status RestartRegistration(User user, string userData = "")
        {
            if (user == null)
                return new Status(-1, "Null user!");

            StatusWrapper sw = mPtr.RestartRegistration(user.Wrapper, userData);
            return new Status(sw.Code, sw.Error);
        }

        public Status FinishRegistration(User user)
        {
            if (user == null)
                return new Status(-1, "Null user!");

            StatusWrapper sw = mPtr.FinishRegistration(user.Wrapper);
            return new Status(sw.Code, sw.Error);
        }

        public Status Authenticate(User user)
        {
            if (user == null)
                return new Status(-1, "Null user!");

            StatusWrapper sw = mPtr.Authenticate(user.Wrapper);
            return new Status(sw.Code, sw.Error);
        }

        public Status Authenticate(User user, ref string authResultData)
        {
            if (user == null)
                return new Status(-1, "Null user!");

            StatusWrapper sw = mPtr.AuthenticateResultData(user.Wrapper, authResultData);
            return new Status(sw.Code, sw.Error);
        }

        public Status AuthenticateOTP(User user, ref OTP otp)
        {
            //TODO: check shouldn't it be ref OTP
            if (otp == null)
                return Authenticate(user);

            if (user == null)
                return new Status(-1, "Null user!");

            StatusWrapper sw = mPtr.AuthenticateOTP(user.Wrapper, otp.Wrapper);
            return new Status(sw.Code, sw.Error);
        }

        public Status AuthenticateAN(User user, string accessNumber)
        {
            if (user == null)
                return new Status(-1, "Null user!");

            //TODO: check shouldn't it be ref accessNumber
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
            IPinPad pinpad = context.GetPinPad();
            if (pinpad != null)
                pinpad.SetUiDispatcher(dispatcher);
        }
        #endregion
    }
}
