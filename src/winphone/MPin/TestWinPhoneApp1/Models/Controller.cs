using MPinSDK;
using MPinSDK.Models;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.ApplicationModel.Resources;
using Windows.UI.Core;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;

namespace MPinDemo.Models
{
    public class Controller
    {
        #region Fields
        private const string DEFAULT_RPS_PREFIX = "rps";
        private const string CONFIG_BACKEND = "backend";
        private CoreDispatcher _dispatcher;
        private MainPage rootPage = null;

        private static MPin _sdk;

        public AppDataModel DataModel
        {
            get;
            set;
        }

        #endregion // Fields

        #region C'tors
        static Controller()
        {
            Controller._sdk = new MPin();
        }

        public Controller()
        {
            rootPage = MainPage.Current;
            _dispatcher = Window.Current.Dispatcher;

            DataModel = new AppDataModel();
            DataModel.PropertyChanged += DataModel_PropertyChanged;
        }

        #endregion // C'tor

        #region handlers
        async void DataModel_PropertyChanged(object sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            // TODO: check for memory leaks - http://stackoverflow.com/questions/12133551/c-sharp-events-memory-leak
            switch (e.PropertyName)
            {
                case "CurrentService":
                    Status status = await ProcessServiceChanged();
                    bool isOk = status != null && status.StatusCode == Status.Code.OK;
                    await _dispatcher.RunAsync(CoreDispatcherPriority.Normal, () =>
                    {
                        rootPage.NotifyUser(!isOk
                            ? string.Format(ResourceLoader.GetForCurrentView().GetString("InitializationFailed"), (status == null ? "null" : status.StatusCode.ToString()))
                            : ResourceLoader.GetForCurrentView().GetString("ServiceSet"),
                            !isOk ? MainPage.NotifyType.ErrorMessage : MainPage.NotifyType.StatusMessage);
                    });

                    UpdateUsersList();
                    break;

                case "CurrentUser":
                    break;
                    
                case "UsersList":                    
                    break;
            }
        }

        //static void CurrentService_PropertyChanged(object sender, System.ComponentModel.PropertyChangedEventArgs e)
        //{
        //    maybe reconnect to the service....
        //}

        #endregion // handlers

        #region Methods
        #region services

        private Status InitService()
        {
            if (!string.IsNullOrEmpty(this.DataModel.CurrentService.BackendUrl))
            {
                IDictionary<string, string> config = new Dictionary<string, string>();
                config.Add(CONFIG_BACKEND, this.DataModel.CurrentService.BackendUrl);
                if (!string.IsNullOrEmpty(this.DataModel.CurrentService.RpsPrefix))
                {
                    config.Add(DEFAULT_RPS_PREFIX, this.DataModel.CurrentService.RpsPrefix);
                }

                return _sdk.Init(config, new Context());
            }

            return null;
        }

        bool set;
        private async Task<Status> ProcessServiceChanged()
        {
            Status status = null;
            if (!set)
            {
                status = await Task.Factory.StartNew(() => InitService());
                if (status != null)
                {
                    Debug.WriteLine("InitStatus: " + status.StatusCode + " " + status.ErrorMessage);
                    _sdk.SetUiDispatcher(Window.Current.Dispatcher);
                    set = true;
                }
            }
            else
            {
                if (!string.IsNullOrEmpty(this.DataModel.CurrentService.BackendUrl))
                    status = await Task.Factory.StartNew(() => _sdk.SetBackend(this.DataModel.CurrentService.BackendUrl, this.DataModel.CurrentService.RpsPrefix));
            }

            return status;
        }

        public static Status RestartRegistration(User user)
        {
            if (user != null)
                return _sdk.RestartRegistration(user);

            return new Status(-1, "No user!");
        }

        internal async Task TestBackend()
        {
            if (this.DataModel.CurrentService.BackendUrl != null)
            {
                Status status = null;
                await Task.Factory.StartNew(() =>
                {
                    status = _sdk.TestBackend(this.DataModel.CurrentService.BackendUrl, this.DataModel.CurrentService.RpsPrefix);
                });

                await _dispatcher.RunAsync(CoreDispatcherPriority.Normal, () =>
                {
                    rootPage.NotifyUser(ResourceLoader.GetForCurrentView().GetString("ServiceStatus") + status.StatusCode, status.StatusCode == Status.Code.OK ? MainPage.NotifyType.StatusMessage : MainPage.NotifyType.ErrorMessage);
                });
            }
        }
        #endregion // services

        #region users
        internal void UpdateUsersList()
        {
            lock (_sdk)
            {
                List<User> users = new List<User>();
                _sdk.ListUsers(users);
                DataModel.UsersList = new System.Collections.ObjectModel.ObservableCollection<User>(users);                
            }
        }

        public async Task ProcessUser()
        {
            if (this.DataModel.CurrentUser == null)
            {
                if (this.DataModel.UsersList.Count > 0)
                    rootPage.NotifyUser(ResourceLoader.GetForCurrentView().GetString("NoSelectedUser"), MainPage.NotifyType.ErrorMessage);

                return;
            }

            Frame mainFrame = MainPage.Current.FindName("MainFrame") as Frame;

            switch (this.DataModel.CurrentUser.UserState)
            {
                case User.State.ACTIVATED:
                    _sdk.FinishRegistration(this.DataModel.CurrentUser); // to set the pin
                    break;

                case User.State.BLOCKED:
                    await _dispatcher.RunAsync(CoreDispatcherPriority.Normal, () =>
                    {
                        rootPage.NotifyUser(ResourceLoader.GetForCurrentView().GetString("BlockedUser"), MainPage.NotifyType.ErrorMessage);
                    });
                    break;

                case User.State.INVALID:
                    // user still not registered -> start the registration
                    await _dispatcher.RunAsync(CoreDispatcherPriority.Normal, () =>
                    {
                        rootPage.NotifyUser(ResourceLoader.GetForCurrentView().GetString("InvalidUser"), MainPage.NotifyType.ErrorMessage);
                    });
                    break;

                case User.State.STARTED_REGISTRATION:
                    mainFrame.Navigate(typeof(EmailConfirmed), this.DataModel.CurrentUser);
                    break;

                case User.State.REGISTERED:
                    if (this.DataModel.CurrentService.RequestAccessNumber)
                    {
                        mainFrame.Navigate(typeof(AccessNumberScreen), this.DataModel.CurrentUser);
                    }
                    else
                    {
                        await ShowAuthenticate();
                    }
                    break;
            }
        }

        private async Task AddUser(string id)
        {
            User user = await AddAndRegisterUser(id);

            UpdateUsersList();
            if (user != null)
                this.DataModel.CurrentUser = user;

            if (user.UserState == User.State.STARTED_REGISTRATION)
            {
                Frame mainFrame = MainPage.Current.FindName("MainFrame") as Frame;
                mainFrame.Navigate(typeof(EmailConfirmed), this.DataModel.CurrentUser);
            }
            else if (user.UserState == User.State.ACTIVATED)
            {
                await Task.Factory.StartNew(() =>
                {
                    _sdk.FinishRegistration(user);
                });
            }
            else
            {
                await _dispatcher.RunAsync(CoreDispatcherPriority.Normal, () =>
                {
                    rootPage.NotifyUser(string.Format(ResourceLoader.GetForCurrentView().GetString("UserRegistrationProblem"), user.Id, user.UserState), MainPage.NotifyType.ErrorMessage);
                });
            }
        }

        private async Task<User> AddAndRegisterUser(string eMail)
        {
            User user = null;
            Status status = null;
            await Task.Factory.StartNew(() =>
            {
                if (!string.IsNullOrEmpty(eMail))
                {
                    user = _sdk.MakeNewUser(eMail);

                    string id = user.Id;
                    Debug.Assert(id.Equals(eMail));
                    MPinSDK.Models.User.State state = user.UserState;
                    Debug.Assert(state == User.State.INVALID);

                    status = _sdk.StartRegistration(user);
                }
            });

            return user;
        }

        private async Task NotConfirmedIdentity()
        {
            if (this.DataModel.CurrentUser != null)
            {
                User.State state = this.DataModel.CurrentUser.UserState;
                await _dispatcher.RunAsync(CoreDispatcherPriority.Normal, () =>
                {
                    rootPage.NotifyUser(ResourceLoader.GetForCurrentView().GetString("NotConfirmedUser") + state.ToString(), state == User.State.INVALID ? MainPage.NotifyType.ErrorMessage : MainPage.NotifyType.StatusMessage);
                });

            }
            else
            {
                await _dispatcher.RunAsync(CoreDispatcherPriority.Normal, () =>
                {
                    rootPage.NotifyUser(ResourceLoader.GetForCurrentView().GetString("RegistrationProblem"), MainPage.NotifyType.ErrorMessage);
                });
            }
        }

        private async Task ShowCreatingNewIdentity(User user, Status reason)
        {
            Status s = null;
            if (user != null && user.UserState == User.State.STARTED_REGISTRATION)
            {
                s = await OnEmailConfirmed();
            }

            Debug.WriteLine("OnEmailConfirmed status: " + (s == null ? "null" : s.StatusCode.ToString()));
        }

        private async Task<Status> OnEmailConfirmed()
        {
            Debug.Assert(this.DataModel.CurrentUser.UserState == User.State.STARTED_REGISTRATION);

            Task.WaitAll();

            Status s = null;
            User user = this.DataModel.CurrentUser;
            await Task.Factory.StartNew(() =>
            {
                s = _sdk.FinishRegistration(user);
            });

            return s;
        }

        internal static bool IfUserExists(string id)
        {
            List<User> users = new List<User>();
            _sdk.ListUsers(users);
            if (users != null && users.Count > 0)
            {
                foreach (var user in users)
                    if (user.Id.Equals(id))
                        return true;
            }

            return false;
        }

        internal async Task DeleteUser(User user)
        {
            await Task.Factory.StartNew(() =>
            {
                _sdk.DeleteUser(user);
            });

            UpdateUsersList();
        }
        #endregion // users

        #region authentication

        public async Task ShowAuthenticate(string accessNumber = "")
        {
            Status status = null;
            OTP otp = this.DataModel.CurrentService.RequestOtp ? new OTP() : null;
            User user = this.DataModel.CurrentUser;
            await Task.Factory.StartNew(() =>
            {
                string resultData = string.Empty;
                if (!string.IsNullOrEmpty(accessNumber))
                {
                    status = _sdk.AuthenticateAN(user, accessNumber);
                }
                else if (otp != null)
                {
                    status = _sdk.AuthenticateOTP(user, otp);
                }
                else
                {
                    status = _sdk.Authenticate(user, resultData);
                }
            });

            if (otp != null && otp.Status != null && otp.Status.StatusCode == Status.Code.OK && otp.TtlSeconds > 0)
            {
                Frame mainFrame = MainPage.Current.FindName("MainFrame") as Frame;
                mainFrame.Navigate(typeof(OtpScreen), new List<object> { otp, this.DataModel.CurrentUser });
            }
            else
            {
                if (status == null)
                {
                    rootPage.NotifyUser(ResourceLoader.GetForCurrentView().GetString("Error") + status.ErrorMessage, MainPage.NotifyType.ErrorMessage);
                }
                else
                {
                    Frame mainFrame = MainPage.Current.FindName("MainFrame") as Frame;
                    mainFrame.Navigate(typeof(AuthenticationScreen), new List<object> { this.DataModel.CurrentUser, status });
                }
            }
        }

        #endregion // authentication
        #endregion // Methods

        internal async Task ProcessNavigation(string command, string parameter)
        {
            switch (command)
            {
                case "InitialLoad":
                    await ProcessUser();
                    break;

                case "AddUser":
                    await AddUser(parameter);
                    break;

                case "EmailConfirmed":
                    if (string.IsNullOrEmpty(parameter))
                    {
                        await NotConfirmedIdentity();
                    }
                    else
                    {
                        await ShowCreatingNewIdentity(this.DataModel.CurrentUser, null);
                    }
                    break;

                case "AccessNumber":
                    await ShowAuthenticate(parameter);
                    break;

                case "Error":
                    await _dispatcher.RunAsync(CoreDispatcherPriority.Normal, () =>
                    {
                        rootPage.NotifyUser(ResourceLoader.GetForCurrentView().GetString("Error") + parameter, MainPage.NotifyType.ErrorMessage);
                    });
                    break;

                default:
                    break;
            }
        }

    }
}
