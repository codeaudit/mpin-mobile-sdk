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
using System.Linq;

namespace MPinDemo.Models
{
    public class Controller : INotifyPropertyChanged
    {
        #region Fields
        private const string DefautRpsPrefix = "rps";
        private const string ConfigBackend = "backend";
        private bool skipProcessing;
        private static CoreDispatcher dispatcher;
        private static MainPage rootPage = null;
        private int selectedServicesIndex = -1;

        private static MPin sdk;
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

        #region Members
        string DeviceName { get; set; }

        internal int NewAddedServiceIndex
        { get; set; }

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

        private bool isUserInProcessing;
        public bool IsUserInProcessing
        {
            get
            {
                return this.isUserInProcessing;
            }
            set
            {
                this.isUserInProcessing = value;
                OnPropertyChanged();
            }
        }

        #endregion

        #region handlers
        /// <summary>
        /// Handles the PropertyChanged event of the DataModel control.
        /// </summary>
        /// <param name="sender">The source of the event.</param>
        /// <param name="e">The <see cref="System.ComponentModel.PropertyChangedEventArgs"/> instance containing the event data.</param>
        async void DataModel_PropertyChanged(object sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            // TODO: check for memory leaks - http://stackoverflow.com/questions/12133551/c-sharp-events-memory-leak
            switch (e.PropertyName)
            {
                case "CurrentService":
                    bool isOk = false;
                    if (!string.IsNullOrEmpty(this.DataModel.CurrentService.BackendUrl))
                    {
                        isOk = await ConnectToService();
                        UpdateServices(isOk);
                    }

                    this.IsValidService = isOk;
                    UpdateUsersList();
                    break;

                case "CurrentUser":
                    if (!this.skipProcessing)
                    {
                        this.IsUserInProcessing = true;
                        await ProcessUser();
                        this.IsUserInProcessing = false;
                    }
                    break;

                case "UsersList":
                    break;

                case "BackendsList":
                    break;
            }
        }

        private async Task<bool> ConnectToService()
        {
            Status status = await ProcessServiceChanged();
            bool isOk = status != null && status.StatusCode == Status.Code.OK;
            rootPage.NotifyUser(!isOk
                ? string.Format(ResourceLoader.GetForCurrentView().GetString("InitializationFailed"), (status == null ? "null" : status.StatusCode.ToString()))
                : ResourceLoader.GetForCurrentView().GetString("ServiceSet"),
                !isOk ? MainPage.NotifyType.ErrorMessage : MainPage.NotifyType.StatusMessage);
            return isOk;
        }

        private void UpdateServices(bool isSet)
        {
            foreach (var service in DataModel.BackendsList)
            {
                service.IsSet = service.Equals(DataModel.CurrentService) && isSet ? true : false;
            }
        }

        #endregion // handlers

        #region Methods

        public async Task Dispose()
        {
            await ProcessSaveChanges();
        }

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

        private async Task ProcessSaveChanges()
        {
            await this.DataModel.SaveServices();
        }

        internal static async Task<Status> TestBackend(Backend backend)
        {
            if (backend != null && backend.BackendUrl != null)
            {
                Status status = null;
                await Task.Factory.StartNew(() =>
                {
                    status = sdk.TestBackend(backend.BackendUrl, backend.RpsPrefix);
                });

                return status;
            }

            return new Status(-1, ResourceLoader.GetForCurrentView().GetString("NotSpecifiedBackend"));
        }

        internal async Task DeleteService(Backend backend, bool canBeDeleted)
        {
            if (this.DataModel.BackendsList.Contains(backend))
            {
                if (!canBeDeleted)
                {
                    rootPage.NotifyUser(ResourceLoader.GetForCurrentView().GetString("PredifinedServices"), MainPage.NotifyType.ErrorMessage);
                    return;
                }

                var confirmation = new MessageDialog(string.Format(ResourceLoader.GetForCurrentView().GetString("DeleteServiceConfirmation"), backend.Name));
                confirmation.Commands.Add(new UICommand(ResourceLoader.GetForCurrentView().GetString("YesCommand")));
                confirmation.Commands.Add(new UICommand(ResourceLoader.GetForCurrentView().GetString("NoCommand")));
                confirmation.DefaultCommandIndex = 1;
                var result = await confirmation.ShowAsync();
                if (result.Equals(confirmation.Commands[0]))
                {
                    this.DataModel.BackendsList.Remove(backend);
                }
            }
        }

        private async Task AddService(Backend backend)
        {
            if (backend == null)
            {
                rootPage.NotifyUser("NoServiceSet", MainPage.NotifyType.ErrorMessage);
                return;
            }

            this.DataModel.BackendsList.Add(backend);
            this.NewAddedServiceIndex = this.DataModel.BackendsList.IndexOf(backend);
            Status status = await Controller.TestBackend(backend);
            if (status.StatusCode != Status.Code.OK)
                rootPage.NotifyUser(ResourceLoader.GetForCurrentView().GetString("ServiceStatus") + status.StatusCode, MainPage.NotifyType.ErrorMessage);
        }


        internal void AddService()
        {
            Frame mainFrame = MainPage.Current.FindName("MainFrame") as Frame;
            if (!mainFrame.Navigate(typeof(Configuration)))
            {
                throw new Exception(ResourceLoader.GetForCurrentView().GetString("NavigationFailedExceptionMessage"));
            }
        }

        internal void EditService(int index, bool canBeEdited)
        {
            if (index < 0 || index > this.DataModel.BackendsList.Count)
            {
                rootPage.NotifyUser(ResourceLoader.GetForCurrentView().GetString("NoSelectedService"), MainPage.NotifyType.ErrorMessage);
                return;
            }

            this.selectedServicesIndex = index;
            Backend service = this.DataModel.BackendsList[this.selectedServicesIndex];
            if (service == null)
            {
                rootPage.NotifyUser(ResourceLoader.GetForCurrentView().GetString("NoSelectedService"), MainPage.NotifyType.ErrorMessage);
                return;
            }

            if (!canBeEdited)
            {
                rootPage.NotifyUser(ResourceLoader.GetForCurrentView().GetString("PredifinedServices"), MainPage.NotifyType.ErrorMessage);
                return;
            }

            Frame mainFrame = MainPage.Current.FindName("MainFrame") as Frame;
            if (!mainFrame.Navigate(typeof(Configuration), service))
            {
                throw new Exception(ResourceLoader.GetForCurrentView().GetString("NavigationFailedExceptionMessage"));
            }
        }

        private async Task EditServiceInfo(Backend editBackend)
        {
            if (editBackend == null)
            {
                await dispatcher.RunAsync(CoreDispatcherPriority.Normal, () =>
                {
                    rootPage.NotifyUser(ResourceLoader.GetForCurrentView().GetString("NoServiceSet"), MainPage.NotifyType.ErrorMessage);
                });
                return;
            }

            if (editBackend != null && selectedServicesIndex > 0 && selectedServicesIndex < this.DataModel.BackendsList.Count)
            {
                this.DataModel.BackendsList[selectedServicesIndex].BackendUrl = editBackend.BackendUrl;
                this.DataModel.BackendsList[selectedServicesIndex].Name = editBackend.Name;
                this.DataModel.BackendsList[selectedServicesIndex].Type = editBackend.Type;
                this.DataModel.BackendsList[selectedServicesIndex].RpsPrefix = editBackend.RpsPrefix;
            }

            Status status = await Controller.TestBackend(editBackend);
            if (status.StatusCode != Status.Code.OK)
                rootPage.NotifyUser(ResourceLoader.GetForCurrentView().GetString("ServiceStatus") + status.StatusCode, MainPage.NotifyType.ErrorMessage);
        }
        #endregion // services

        #region users
        internal void UpdateUsersList()
        {
            List<User> users = new List<User>();
            sdk.ListUsers(users);
            DataModel.UsersList = new System.Collections.ObjectModel.ObservableCollection<User>(users);
        }

        private async Task ProcessUser()
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
                    await FinishUserRegistration(this.DataModel.CurrentUser);
                    break;

                case User.State.Blocked:
                    mainFrame.Navigate(typeof(BlockedScreen), new List<object> { this.DataModel.CurrentUser });
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
                    if (this.DataModel.CurrentService.Type == ConfigurationType.Online)
                    {
                        mainFrame.Navigate(typeof(AccessNumberScreen), new List<string> { this.DataModel.CurrentUser.Id, sdk.GetClientParam("accessNumberDigits") });
                    }
                    else
                    {
                        await ShowAuthenticate();
                    }
                    break;
            }
        }

        internal void AddNewUser()
        {
            // Add a user
            if (string.IsNullOrEmpty(this.DataModel.CurrentService.BackendUrl))
            {
                rootPage.NotifyUser(ResourceLoader.GetForCurrentView().GetString("NoServiceSet"), MainPage.NotifyType.ErrorMessage);
            }
            else
            {
                Frame mainFrame = MainPage.Current.FindName("MainFrame") as Frame;
                if (!mainFrame.Navigate(typeof(AddNewUser), sdk.GetClientParam("setDeviceName")))
                {
                    throw new Exception(ResourceLoader.GetForCurrentView().GetString("NavigationFailedExceptionMessage"));
                }
            }
        }

        private async Task AddUser(List<string> data)
        {
            this.IsUserInProcessing = true;
            User user = await AddAndRegisterUser(data);

            UpdateUsersList();
            if (user != null)
            {
                bool currentValue = this.skipProcessing;
                this.skipProcessing = true;
                this.DataModel.CurrentUser = this.DataModel.UsersList.SingleOrDefault(u => u.Equals(user));

                this.skipProcessing = currentValue;
            }
            this.IsUserInProcessing = false;

            await FinishUserRegistration(user);
        }

        public static Status RestartRegistration(User user)
        {
            if (user != null)
                return sdk.RestartRegistration(user);

            return new Status(-1, "No user!");
        }

        private async Task FinishUserRegistration(User user)
        {
            if (user.UserState == User.State.StartedRegistration)
            {
                Frame mainFrame = MainPage.Current.FindName("MainFrame") as Frame;
                mainFrame.Navigate(typeof(EmailConfirmed), this.DataModel.CurrentUser);
            }
            else if (user.UserState == User.State.Activated)
            {
                await Finish(user);
            }
        }

        private static async Task<Status> Finish(User user)
        {
            Status st = null;
            await Task.Factory.StartNew(() =>
            {
                st = sdk.FinishRegistration(user);
            });

            if (st != null)
            {
                await dispatcher.RunAsync(CoreDispatcherPriority.Normal, () =>
                {
                    if (st.StatusCode == Status.Code.OK)
                    {
                        // successful registration
                        Frame mainFrame = MainPage.Current.FindName("MainFrame") as Frame;
                        mainFrame.Navigate(typeof(IdentityCreated), user);
                    }
                    else
                    {
                        rootPage.NotifyUser(
                            string.Format(ResourceLoader.GetForCurrentView().GetString("UserRegistrationProblemReason"), user.Id, st.ErrorMessage),
                            MainPage.NotifyType.ErrorMessage);
                    }
                });
            }

            return st;
        }

        private async Task<User> AddAndRegisterUser(List<string> data)
        {
            string eMail = data[0];
            this.DeviceName = data[1] ?? string.Empty;
            User user = null;
            Status status = null;
            await Task.Factory.StartNew(() =>
            {
                if (!string.IsNullOrEmpty(eMail))
                {
                    user = sdk.MakeNewUser(eMail, this.DeviceName);

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

      
        internal static async Task<Status> OnEmailConfirmed(User user)
        {
            Debug.Assert(user.UserState == User.State.StartedRegistration);
            Task.WaitAll();

            return await Finish(user);
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

            await AddUser(new List<string> { user.Id, this.DeviceName });
        }
        #endregion // users

        #region authentication

        public async Task ShowAuthenticate(string accessNumber = "")
        {
            Status status = null;
            OTP otp = this.DataModel.CurrentService.Type == ConfigurationType.OTP ? new OTP() : null;
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
                    mainFrame.Navigate(
                        user.UserState == User.State.Blocked ? typeof(BlockedScreen) : typeof(AuthenticationScreen),
                        new List<object> { this.DataModel.CurrentUser, status });
                }
            }
        }

        #endregion // authentication

        internal async Task ProcessNavigation(string command, object parameter)
        {
            switch (command)
            {
                case "AddService":
                    await AddService(parameter as Backend);
                    break;

                case "EditService":
                    await EditServiceInfo(parameter as Backend);
                    break;

                case "SignIn":
                    this.DataModel.CurrentUser = parameter as User;
                    break;

                case "InitialLoad":                
                    await ProcessUser();
                    break;

                case "AddUser":
                    List<string> data = parameter as List<string>;
                    if (data != null && data.Count == 2)
                        await AddUser(data);

                    break;

                case "EmailConfirmed":
                    if (parameter == null || string.IsNullOrEmpty(parameter.ToString()))
                    {
                        await NotConfirmedIdentity();
                    }
                    break;

                case "AccessNumber":
                    await ShowAuthenticate(parameter.ToString());
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

                case "NewServices":
                    List<Backend> newBackends = parameter as List<Backend>;
                    if (newBackends == null)
                    {
                        await dispatcher.RunAsync(CoreDispatcherPriority.Normal, () =>
                        {
                            rootPage.NotifyUser(ResourceLoader.GetForCurrentView().GetString("InvalidConfigurationList"), MainPage.NotifyType.ErrorMessage);
                        });
                        return;
                    }

                    bool shouldReconnect = newBackends.Any(item => item.Name.Equals(this.DataModel.CurrentService.Name));
                    await this.DataModel.MergeConfigurations(newBackends);
                    if (shouldReconnect)
                    {
                        await ConnectToService();                        
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
