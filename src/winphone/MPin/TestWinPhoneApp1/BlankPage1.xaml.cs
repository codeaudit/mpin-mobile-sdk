using MPinSDK;
using MPinSDK.Models;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices.WindowsRuntime;
using System.Threading.Tasks;
using TestWinPhoneApp1.Models;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.System.Threading;
using Windows.UI.Core;
using Windows.UI.Popups;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;
using MPinSDK.Common; // navigation extensions
using Windows.UI;
using Windows.Storage;

namespace TestWinPhoneApp1
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class BlankPage1 : Page
    {
        #region members

        private const string DEFAULT_RPS_PREFIX = "rps";
        private const string CONFIG_BACKEND = "backend";
        private const string SelectedService = "ServiceSetIndex";
        private const string SelectedUser = "SelectedUser";

        private bool processSelection = true;
        private bool shouldSetService = false;
        private MainPage rootPage = null;
        private CoreDispatcher _dispatcher;
        private static MPin _sdk;
        private MPin Sdk
        {
            get
            {
                lock (this)
                {
                    return BlankPage1._sdk;
                }
            }
        }

        private static AppDataModel DataModel;
        private ApplicationDataContainer roamingSettings = null;
        #endregion // members

        #region constructors
        static BlankPage1()
        {
            BlankPage1._sdk = new MPin();
            BlankPage1.DataModel = new AppDataModel();

        }

        public BlankPage1()
        {
            this.InitializeComponent();
            _dispatcher = Window.Current.Dispatcher;

            this.DataContext = DataModel;
            roamingSettings = ApplicationData.Current.RoamingSettings;

            UpdateServicesList();
            UpdateUsersList();

            if (DataModel.CurrentService.BackendUrl != null)
            {
                this.MainPivot.SelectedItem = this.UsersPivotItem;
            }
        }

        #endregion // constructors

        #region Overrides
        /// <summary>
        /// Invoked when this page is about to be displayed in a Frame.
        /// </summary>
        /// <param name="e">Event data that describes how this page was reached.
        /// This parameter is typically used to configure the page.</param>
        protected async override void OnNavigatedTo(NavigationEventArgs e)
        {
            rootPage = MainPage.Current;

            List<string> data = (Window.Current.Content as Frame).GetNavigationData() as List<string>;
            if (data != null && data.Count == 2)
            {
                switch (data[0])
                {
                    case "AddUser":
                        await AddUser(data[1].ToString());
                        break;

                    case "EmailConfirmed":
                        if (string.IsNullOrEmpty(data[1]))
                        {
                            await NotConfirmedIdentity();
                        }
                        else
                        {
                            await ShowCreatingNewIdentity(DataModel.CurrentUser, null);
                        }
                        break;

                    case "AccessNumber":
                        await ShowAuthenticate(data[1]);
                        break;

                    case "Error":
                        await _dispatcher.RunAsync(CoreDispatcherPriority.Normal, () =>
                        {
                            rootPage.NotifyUser("Error occured: " + data[2], MainPage.NotifyType.ErrorMessage);
                        });
                        break;

                    default:
                        break;
                }
            }
        }

        #endregion

        #region users

        private void UpdateUsersList()
        {
            lock (UsersList)
            {
                List<User> users = new List<User>();
                this.Sdk.ListUsers(users);
                UpdateUsersSelection(users);                
                UsersList.ItemsSource = users;
                UsersList.SelectedItem = GetSelectedUser(users);
                AuthenticateButton.IsEnabled = UsersList.SelectedItem != null;
            }
        }

        private User GetSelectedUser(List<User> users)
        {
            if (users == null || users.Count == 0 || DataModel.CurrentUser == null)
                return GetSelectedUserFromSettings();

            foreach (var user in users)
                if (user.Equals(DataModel.CurrentUser))
                    return user;

            return null;
        }

        private void UpdateUsersSelection(List<User> users)
        {
            if (users == null || users.Count == 0 || DataModel.CurrentUser == null)
                return;

            foreach (var user in users)
            {
                user.IsSelected = user.Equals(DataModel.CurrentUser);
            }
        }

        private async Task NotConfirmedIdentity()
        {
            if (DataModel.CurrentUser != null)
            {
                User.State state = DataModel.CurrentUser.UserState;
                await _dispatcher.RunAsync(CoreDispatcherPriority.Normal, () =>
                {
                    rootPage.NotifyUser("User not confirmed! User status " + state.ToString(), state == User.State.INVALID ? MainPage.NotifyType.ErrorMessage : MainPage.NotifyType.StatusMessage);
                });

            }
            else
            {
                await _dispatcher.RunAsync(CoreDispatcherPriority.Normal, () =>
                {
                    rootPage.NotifyUser("A problem occurs during registration. Please, try again!", MainPage.NotifyType.ErrorMessage);
                });
            }

        }

        private async Task AddUser(string id)
        {
            User user = await AddAndRegisterUser(id);

            UpdateUsersList();
            if (user != null)
                DataModel.CurrentUser = user;

            if (user.UserState == User.State.STARTED_REGISTRATION)
            {
                Frame mainFrame = MainPage.Current.FindName("MainFrame") as Frame;
                mainFrame.Navigate(typeof(EmailConfirmed), DataModel.CurrentUser);
            }
            else if (user.UserState == User.State.ACTIVATED)
            {
                await Task.Factory.StartNew(() =>
                {
                    Sdk.FinishRegistration(user);
                });
            }
            else
            {
                await _dispatcher.RunAsync(CoreDispatcherPriority.Normal, () =>
                {
                    rootPage.NotifyUser("Problem with the registration of " + user.Id + "! State: " + user.UserState, MainPage.NotifyType.ErrorMessage);
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
                    user = this.Sdk.MakeNewUser(eMail);

                    string id = user.Id;
                    Debug.Assert(id.Equals(eMail));
                    MPinSDK.Models.User.State state = user.UserState;
                    Debug.Assert(state == User.State.INVALID);

                    status = this.Sdk.StartRegistration(user);
                }
            });

            return user;
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
            Debug.Assert(DataModel.CurrentUser.UserState == User.State.STARTED_REGISTRATION);

            Task.WaitAll();

            Status s = null;
            User user = DataModel.CurrentUser;
            await Task.Factory.StartNew(() =>
            {
                s = Sdk.FinishRegistration(user);
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
        #endregion // users

        #region authentication

        private async Task<Status> ShowAuthenticate(string accessNumber = "")
        {
            Status status = null;
            OTP otp = DataModel.CurrentService.RequestOtp ? new OTP() : null;
            User user = DataModel.CurrentUser;
            await Task.Factory.StartNew(() =>
            {
                string resultData = string.Empty;
                if (!string.IsNullOrEmpty(accessNumber))
                {
                    status = this.Sdk.AuthenticateAN(user, accessNumber);
                }
                else if (otp != null)
                {
                    status = this.Sdk.AuthenticateOTP(user, otp);
                }
                else
                {
                    status = this.Sdk.Authenticate(user, resultData);
                }
            });

            if (otp != null)
            {
                if (otp.Status != null && otp.Status.StatusCode == Status.Code.OK && otp.TtlSeconds > 0)
                {
                    Frame mainFrame = MainPage.Current.FindName("MainFrame") as Frame;
                    mainFrame.Navigate(typeof(OtpScreen), new List<object> { otp, DataModel.CurrentUser });
                }
                else
                {
                    return otp.Status;
                }
            }
            else
            {
                if (status == null)
                {
                    rootPage.NotifyUser("Error: " + status.ErrorMessage, MainPage.NotifyType.ErrorMessage);
                }
                else
                {
                    Frame mainFrame = MainPage.Current.FindName("MainFrame") as Frame;
                    mainFrame.Navigate(typeof(AuthenticationScreen), new List<object> { DataModel.CurrentUser, status });
                }
            }

            return status;
        }

        #endregion

        #region services
        private void UpdateServicesList()
        {
            this.ServicesList.ItemsSource = DataModel.BackendsList;
            SetSelectedServicesIndex();
        }

        private Status InitService()
        {
            if (!string.IsNullOrEmpty(DataModel.CurrentService.BackendUrl))
            {
                IDictionary<string, string> config = new Dictionary<string, string>();
                config.Add(CONFIG_BACKEND, DataModel.CurrentService.BackendUrl);
                if (!string.IsNullOrEmpty(DataModel.CurrentService.RpsPrefix))
                {
                    config.Add(DEFAULT_RPS_PREFIX, DataModel.CurrentService.RpsPrefix);
                }

                return this.Sdk.Init(config, new Context());
            }

            return null;
        }

        bool set;
        private async Task ProcessServiceChanged()
        {
            Status status = null;
            if (!set)
            {
                status = await Task.Factory.StartNew(() => InitService());
                if (status != null)
                {
                    Debug.WriteLine("InitStatus: " + status.StatusCode + " " + status.ErrorMessage);
                    this.Sdk.SetUiDispatcher(Window.Current.Dispatcher);
                    set = true;
                }              
            }
            else
            {
                if (!string.IsNullOrEmpty(DataModel.CurrentService.BackendUrl))
                    status = await Task.Factory.StartNew(() => this.Sdk.SetBackend(DataModel.CurrentService.BackendUrl, DataModel.CurrentService.RpsPrefix));
            }

            if (status == null || status.StatusCode != Status.Code.OK)
            {
                await _dispatcher.RunAsync(CoreDispatcherPriority.Normal, () =>
                                {
                                    rootPage.NotifyUser("Failed to initialize MPinSDK: status code = " + (status == null ? "null" : status.StatusCode.ToString()) + "Try again!", MainPage.NotifyType.ErrorMessage);
                                });
                bool current = this.processSelection;
                this.processSelection = false;
                this.ServicesList.SelectedItem = null;
                this.processSelection = current;
            }
            else
            {
                await _dispatcher.RunAsync(CoreDispatcherPriority.Normal, () =>
                                {
                                    rootPage.NotifyUser("Service set!", MainPage.NotifyType.StatusMessage);
                                });
            }

            UpdateUsersList();
        }

        internal static Status RestartRegistration(User user)
        {
            if (user != null)
                return _sdk.RestartRegistration(user);

            return new Status(-1, "No user!");
        }
        #endregion // services

        #region State

        private void SavePropertyState(string key, object value)
        {
            if (!roamingSettings.Values.Keys.Contains(key))
            {
                roamingSettings.Values.Add(key, value);
            }
            else
            {
                roamingSettings.Values[key] = value;
            }
        }

        private User GetSelectedUserFromSettings()
        {
            int? selectedIndex = roamingSettings.Values[SelectedUser] as int?;
            if (selectedIndex != null && selectedIndex >= 0 && selectedIndex < UsersList.Items.Count)
            {
                return this.UsersList.Items[selectedIndex.Value] as User;
            }

            return null;
        }

        private void SetSelectedServicesIndex()
        {
            int? selectedIndex = roamingSettings.Values[SelectedService] as int?;
            if (selectedIndex != null && selectedIndex >= 0 && selectedIndex < ServicesList.Items.Count)
            {
                // if the selected service in the list is different from the currentService -> reset it
                Backend? selectedService = ServicesList.Items[selectedIndex.Value] as Backend?;
                shouldSetService = false == selectedService.Equals(DataModel.CurrentService); 
                
                DataModel.CurrentService = (Backend)this.ServicesList.Items[selectedIndex.Value];
                this.ServicesList.SelectedIndex = selectedIndex.Value;
            }
            else
            {
                shouldSetService = true;
            }
        }
        #endregion

        #region handlers

        private void AddService_Click(object sender, RoutedEventArgs e)
        {
            // TODO
        }

        private void DeleteService_Click(object sender, RoutedEventArgs e)
        {
            // TODO
        }

        private async void Authenticate_Click(object sender, RoutedEventArgs e)
        {
            if (DataModel.CurrentUser == null)
            {
                rootPage.NotifyUser("No selected user!", MainPage.NotifyType.ErrorMessage);
                return;
            }

            Frame mainFrame = MainPage.Current.FindName("MainFrame") as Frame;

            switch (DataModel.CurrentUser.UserState)
            {
                case User.State.ACTIVATED:
                    Sdk.FinishRegistration(DataModel.CurrentUser); // to set the pin
                    break;

                case User.State.BLOCKED:
                    rootPage.NotifyUser("User is BLOCKED! Too many unsuccessful authentications!", MainPage.NotifyType.ErrorMessage);
                    break;

                case User.State.INVALID:
                    // user still not registered -> start the registration
                    rootPage.NotifyUser("User is in an INVALID state!", MainPage.NotifyType.ErrorMessage);
                    break;

                case User.State.STARTED_REGISTRATION:
                    mainFrame.Navigate(typeof(EmailConfirmed), DataModel.CurrentUser);
                    break;

                case User.State.REGISTERED:
                    if (DataModel.CurrentService.RequestAccessNumber)
                    {
                        mainFrame.Navigate(typeof(AccessNumberScreen), DataModel.CurrentUser);
                    }
                    else
                    {
                        await ShowAuthenticate();
                    }
                    break;
            }

            //await _dispatcher.RunAsync(CoreDispatcherPriority.Normal, () =>
            //{
            //    rootPage.NotifyUser("Authenitcate: " + (status == null ? "null" : status.StatusCode.ToString()), MainPage.NotifyType.ErrorMessage);
            //});
        }

        private void AddUser_Click(object sender, RoutedEventArgs e)
        {
            if (string.IsNullOrEmpty(DataModel.CurrentService.BackendUrl))
            {
                rootPage.NotifyUser("No backend set!", MainPage.NotifyType.ErrorMessage);
            }
            else
            {
                //(Window.Current.Content as Frame).Navigate(typeof(AddNewUser));
                Frame mainFrame = MainPage.Current.FindName("MainFrame") as Frame;
                mainFrame.Navigate(typeof(AddNewUser));
            }
        }

        private async void DeleteUser_Click(object sender, RoutedEventArgs e)
        {
            User user = UsersList.SelectedItem as User;
            if (user != null)
            {
                await Task.Factory.StartNew(() =>
                {
                    this.Sdk.DeleteUser(user);
                });

                UpdateUsersList();
            }
        }

        private void UsersList_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            AuthenticateButton.IsEnabled = UsersList.SelectedItem != null;
            DataModel.CurrentUser = UsersList.SelectedItem as User;
            SavePropertyState(SelectedUser, UsersList.SelectedIndex);
        }

        private async void Services_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            TestServiceButton.IsEnabled = ServicesList.SelectedItem != null;
            if (e.AddedItems.Count != 1 || (e.AddedItems.Count == 1 && e.RemovedItems.Count == 1 && e.AddedItems[0].Equals(e.RemovedItems[0])))
                return;

            if ((processSelection && shouldSetService) || ChangedByClick(e.AddedItems, e.RemovedItems))
                await ProcessServiceChanged();

            SavePropertyState(SelectedService, ServicesList.SelectedIndex);
        }

        private bool ChangedByClick(IList<object> addedItems, IList<object> removedItems)
        {
            return addedItems.Count == 1 && removedItems.Count == 1 && false == addedItems[0].Equals(removedItems[0]);
        }
       
        private async void TestBackend_click(object sender, RoutedEventArgs e)
        {
            if (DataModel.CurrentService.BackendUrl != null)
            {
                Status status = null;
                await Task.Factory.StartNew(() =>
                {
                    status = Sdk.TestBackend(DataModel.CurrentService.BackendUrl, DataModel.CurrentService.RpsPrefix);
                });

                await _dispatcher.RunAsync(CoreDispatcherPriority.Normal, () =>
                {
                    rootPage.NotifyUser("Backend status: " + status.StatusCode, status.StatusCode == Status.Code.OK ? MainPage.NotifyType.StatusMessage : MainPage.NotifyType.ErrorMessage);
                });
            }
        }

        private void StatusBorder_Tapped(object sender, TappedRoutedEventArgs e)
        {
            rootPage.NotifyUser(string.Empty);
        }

        #endregion // handlers
    }
}
