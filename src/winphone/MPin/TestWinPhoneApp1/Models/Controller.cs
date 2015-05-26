using MPinSDK;
using MPinSDK.Models;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.Runtime.CompilerServices;
using System.Threading.Tasks;
using Windows.ApplicationModel.Resources;
using Windows.UI.Core;
using Windows.UI.Popups;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;

namespace MPinDemo.Models
{
    public class Controller : INotifyPropertyChanged
    {
        #region Fields
        private const string DefautRpsPrefix = "rps";
        private const string ConfigBackend = "backend";
        private CoreDispatcher dispatcher;
        private MainPage rootPage = null;

        private static MPin sdk;

        public AppDataModel DataModel
        {
            get;
            set;
        }

        private bool isValidService;
        public bool IsValidService
        {
            get
            {
                return this.isValidService;
            }
            set
            {
                this.isValidService = value;
                OnPropertyChanged();
            }
        }

        #endregion // Fields

        #region C'tors
        static Controller()
        {
            Controller.sdk = new MPin();
        }

        public Controller()
        {
            rootPage = MainPage.Current;
            dispatcher = Window.Current.Dispatcher;

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
                    bool isOk = false;
                    if (!string.IsNullOrEmpty(this.DataModel.CurrentService.BackendUrl))
                    {
                        Status status = await ProcessServiceChanged();
                        isOk = status != null && status.StatusCode == Status.Code.OK;
                        rootPage.NotifyUser(!isOk
                            ? string.Format(ResourceLoader.GetForCurrentView().GetString("InitializationFailed"), (status == null ? "null" : status.StatusCode.ToString()))
                            : ResourceLoader.GetForCurrentView().GetString("ServiceSet"),
                            !isOk ? MainPage.NotifyType.ErrorMessage : MainPage.NotifyType.StatusMessage);
                    }

                    this.IsValidService = isOk;
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
        //    TODO:  maybe reconnect to the service.... on service editing
        //}

        #endregion // handlers

        #region Methods
        #region services

        private Status InitService()
        {
            if (!string.IsNullOrEmpty(this.DataModel.CurrentService.BackendUrl))
            {
                IDictionary<string, string> config = new Dictionary<string, string>();
                config.Add(ConfigBackend, this.DataModel.CurrentService.BackendUrl);
                if (!string.IsNullOrEmpty(this.DataModel.CurrentService.RpsPrefix))
                {
                    config.Add(DefautRpsPrefix, this.DataModel.CurrentService.RpsPrefix);
                }

                return sdk.Init(config, new Context());
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
                    sdk.SetUiDispatcher(Window.Current.Dispatcher);
                    set = true;
                }
            }
            else
            {
                if (!string.IsNullOrEmpty(this.DataModel.CurrentService.BackendUrl))
                    status = await Task.Factory.StartNew(() => sdk.SetBackend(this.DataModel.CurrentService.BackendUrl, this.DataModel.CurrentService.RpsPrefix));
            }

            return status;
        }

        public static Status RestartRegistration(User user)
        {
            if (user != null)
                return sdk.RestartRegistration(user);

            return new Status(-1, "No user!");
        }

        internal async Task TestBackend()
        {
            if (this.DataModel.CurrentService.BackendUrl != null)
            {
                Status status = null;
                await Task.Factory.StartNew(() =>
                {
                    status = sdk.TestBackend(this.DataModel.CurrentService.BackendUrl, this.DataModel.CurrentService.RpsPrefix);
                });

                await dispatcher.RunAsync(CoreDispatcherPriority.Normal, () =>
                {
                    rootPage.NotifyUser(ResourceLoader.GetForCurrentView().GetString("ServiceStatus") + status.StatusCode, status.StatusCode == Status.Code.OK ? MainPage.NotifyType.StatusMessage : MainPage.NotifyType.ErrorMessage);
                });
            }
        }

        internal async Task DeleteService(Backend backend)
        {
            this.DataModel.BackendsList.Remove(backend);
            await this.DataModel.SaveServices();
        }
        #endregion // services

        #region users
        internal void UpdateUsersList()
        {
            List<User> users = new List<User>();
            sdk.ListUsers(users);
            DataModel.UsersList = new System.Collections.ObjectModel.ObservableCollection<User>(users);
        }

        public async Task ProcessUser()
        {
            if (this.DataModel.CurrentUser == null)
            {
                //if (this.DataModel.UsersList.Count > 0)
                //    rootPage.NotifyUser(ResourceLoader.GetForCurrentView().GetString("NoSelectedUser"), MainPage.NotifyType.ErrorMessage);

                return;
            }

            Frame mainFrame = MainPage.Current.FindName("MainFrame") as Frame;

            switch (this.DataModel.CurrentUser.UserState)
            {
                case User.State.Activated:
                    sdk.FinishRegistration(this.DataModel.CurrentUser); // to set the pin
                    break;

                case User.State.Blocked:
                    mainFrame.Navigate(typeof(BlockedScreen), this.DataModel.CurrentUser);
                    break;

                case User.State.Invalid:
                    // user still not registered -> start the registration
                    await dispatcher.RunAsync(CoreDispatcherPriority.Normal, () =>
                    {
                        rootPage.NotifyUser(ResourceLoader.GetForCurrentView().GetString("InvalidUser"), MainPage.NotifyType.ErrorMessage);
                    });
                    break;

                case User.State.StartedRegistration:
                    mainFrame.Navigate(typeof(EmailConfirmed), this.DataModel.CurrentUser);
                    break;

                case User.State.Registered:
                    if (this.DataModel.CurrentService.RequestAccessNumber)
                    {
                        mainFrame.Navigate(typeof(AccessNumberScreen), sdk.GetClientParam("accessNumberDigits"));
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

            await FinishRegistration(user);
        }

        private async Task FinishRegistration(User user)
        {
            Status st = null;
            if (user.UserState == User.State.StartedRegistration)
            {
                Frame mainFrame = MainPage.Current.FindName("MainFrame") as Frame;
                mainFrame.Navigate(typeof(EmailConfirmed), this.DataModel.CurrentUser);
            }
            else if (user.UserState == User.State.Activated)
            {
                await Task.Factory.StartNew(() =>
                {
                    st = sdk.FinishRegistration(user);
                });

                if (st != null && st.StatusCode != Status.Code.OK)
                {
                    await dispatcher.RunAsync(CoreDispatcherPriority.Normal, () =>
                    {
                        rootPage.NotifyUser(string.Format(ResourceLoader.GetForCurrentView().GetString("UserRegistrationProblemReason"), user.Id, st.ErrorMessage), MainPage.NotifyType.ErrorMessage);
                    });
                }
            }
            else
            {
                await dispatcher.RunAsync(CoreDispatcherPriority.Normal, () =>
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
                    user = sdk.MakeNewUser(eMail);

                    string id = user.Id;
                    Debug.Assert(id.Equals(eMail));
                    MPinSDK.Models.User.State state = user.UserState;
                    Debug.Assert(state == User.State.Invalid);

                    status = sdk.StartRegistration(user);
                }
            });

            if (status != null && status.StatusCode != Status.Code.OK)
            {
                await dispatcher.RunAsync(CoreDispatcherPriority.Normal, () =>
                {
                    rootPage.NotifyUser(string.Format(ResourceLoader.GetForCurrentView().GetString("UserRegistrationProblemReason"), user.Id, status.ErrorMessage), MainPage.NotifyType.ErrorMessage);
                });
            }

            return user;
        }

        private async Task NotConfirmedIdentity()
        {
            if (this.DataModel.CurrentUser != null)
            {
                User.State state = this.DataModel.CurrentUser.UserState;
                await dispatcher.RunAsync(CoreDispatcherPriority.Normal, () =>
                {
                    rootPage.NotifyUser(ResourceLoader.GetForCurrentView().GetString("NotConfirmedUser") + state.ToString(), state == User.State.Invalid ? MainPage.NotifyType.ErrorMessage : MainPage.NotifyType.StatusMessage);
                });

            }
            else
            {
                await dispatcher.RunAsync(CoreDispatcherPriority.Normal, () =>
                {
                    rootPage.NotifyUser(ResourceLoader.GetForCurrentView().GetString("RegistrationProblem"), MainPage.NotifyType.ErrorMessage);
                });
            }
        }

        private async Task ShowCreatingNewIdentity(User user, Status reason)
        {
            Status s = null;
            if (user != null && user.UserState == User.State.StartedRegistration)
            {
                s = await OnEmailConfirmed();
            }

            string errorMsg = s == null
                ? string.Format(ResourceLoader.GetForCurrentView().GetString("UserRegistrationProblem"), user.Id, user.UserState)
                : s.StatusCode != Status.Code.OK ? string.Format(ResourceLoader.GetForCurrentView().GetString("UserRegistrationProblemReason"), user.Id, s.ErrorMessage) : string.Empty;

            if (!string.IsNullOrEmpty(errorMsg))
            {
                await dispatcher.RunAsync(CoreDispatcherPriority.Normal, () =>
                {
                    rootPage.NotifyUser(errorMsg, MainPage.NotifyType.ErrorMessage);
                });
            }
        }

        private async Task<Status> OnEmailConfirmed()
        {
            Debug.Assert(this.DataModel.CurrentUser.UserState == User.State.StartedRegistration);

            Task.WaitAll();

            Status s = null;
            User user = this.DataModel.CurrentUser;
            await Task.Factory.StartNew(() =>
            {
                s = sdk.FinishRegistration(user);
            });

            return s;
        }

        internal static bool IfUserExists(string id)
        {
            List<User> users = new List<User>();
            sdk.ListUsers(users);
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
            var confirmation = new MessageDialog(string.Format(ResourceLoader.GetForCurrentView().GetString("DeleteUserConfirmation"), this.DataModel.CurrentUser.Id));
            confirmation.Commands.Add(new UICommand(ResourceLoader.GetForCurrentView().GetString("YesCommand")));
            confirmation.Commands.Add(new UICommand(ResourceLoader.GetForCurrentView().GetString("NoCommand")));
            confirmation.DefaultCommandIndex = 1;
            var result = await confirmation.ShowAsync();
            if (result.Equals(confirmation.Commands[0]))
            {
                await Task.Factory.StartNew(() =>
                {
                    sdk.DeleteUser(user);
                });

                UpdateUsersList();
            }
        }

        internal async Task ResetPIN(User user)
        {
            await Task.Factory.StartNew(() =>
            {
                sdk.DeleteUser(user);
            });

            await AddUser(user.Id);
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
                    status = sdk.AuthenticateAN(user, accessNumber);
                }
                else if (otp != null)
                {
                    status = sdk.AuthenticateOTP(user, otp);
                }
                else
                {
                    status = sdk.Authenticate(user, resultData);
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
                    await dispatcher.RunAsync(CoreDispatcherPriority.Normal, () =>
                    {
                        rootPage.NotifyUser(ResourceLoader.GetForCurrentView().GetString("Error") + parameter, MainPage.NotifyType.ErrorMessage);
                    });
                    break;

                case "BlockedUser":
                    if (this.DataModel.CurrentUser == null)
                    {
                        await dispatcher.RunAsync(CoreDispatcherPriority.Normal, () =>
                        {
                            rootPage.NotifyUser(ResourceLoader.GetForCurrentView().GetString("NoSelectedUser"), MainPage.NotifyType.ErrorMessage);
                        });
                        return;
                    }

                    if (parameter.Equals("Remove"))
                    {
                        await DeleteUser(this.DataModel.CurrentUser);
                    }
                    else if (parameter.Equals("ResetPIN"))
                    {
                        await ResetPIN(this.DataModel.CurrentUser);
                    }

                    break;

                default:
                    break;
            }
        }

        #endregion // Methods

        #region INotifyPropertyChanged
        public event PropertyChangedEventHandler PropertyChanged;
        void OnPropertyChanged([CallerMemberName]string name = "")
        {
            PropertyChangedEventHandler handler = PropertyChanged;
            if (handler != null)
            {
                handler(this, new PropertyChangedEventArgs(name));
            }
        }
        #endregion // INotifyPropertyChanged

    }
}
