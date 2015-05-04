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
using MPinDemo.Models;
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
using Windows.ApplicationModel.Resources;

namespace MPinDemo
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class BlankPage1 : Page
    {
        #region members

        private const string SelectedService = "ServiceSetIndex";
        private const string SelectedUser = "SelectedUser";
        private bool processSelection = true;
        private bool shouldSetService = false;
        private MainPage rootPage = null;
        private CoreDispatcher _dispatcher;

        private ApplicationDataContainer roamingSettings = null;
        private static Controller controller = null;
        #endregion // members

        #region constructors
        static BlankPage1()
        {
            controller = new Controller();
        }

        public BlankPage1()
        {
            this.InitializeComponent();

            _dispatcher = Window.Current.Dispatcher;
            this.DataContext = controller.DataModel;
            roamingSettings = ApplicationData.Current.RoamingSettings;

            UpdateServicesList();
            UpdateUsersList();

            if (controller.DataModel.CurrentService.BackendUrl != null)
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
                await controller.ProcessNavigation(data[0], data[1]);
            }
        }

        #endregion

        #region services
        private void UpdateServicesList()
        {
            SetSelectedServicesIndex();
        }

        #endregion // services

        #region users

        private void UpdateUsersList()
        {
            UsersList.SelectedItem = GetSelectedUser(controller.DataModel.UsersList);
            AuthenticateButton.IsEnabled = UsersList.SelectedItem != null;
        }

        private User GetSelectedUser(List<User> users)
        {
            if (users == null || users.Count == 0 || controller.DataModel.CurrentUser == null)
                return GetSelectedUserFromSettings();

            foreach (var user in users)
                if (user.Equals(controller.DataModel.CurrentUser))
                    return user;

            return null;
        }

        #endregion // users
        
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
                shouldSetService = false == selectedService.Equals(controller.DataModel.CurrentService);

                controller.DataModel.CurrentService = (Backend)this.ServicesList.Items[selectedIndex.Value];
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
            await controller.ProcessUser();
        }

        private void AddUser_Click(object sender, RoutedEventArgs e)
        {            
            if (string.IsNullOrEmpty(controller.DataModel.CurrentService.BackendUrl))
            {
                rootPage.NotifyUser(ResourceLoader.GetForCurrentView().GetString("NoServiceSet"), MainPage.NotifyType.ErrorMessage);
            }
            else
            {
                Frame mainFrame = MainPage.Current.FindName("MainFrame") as Frame;
                mainFrame.Navigate(typeof(AddNewUser));
            }
        }

        private async void DeleteUser_Click(object sender, RoutedEventArgs e)
        {
            User user = UsersList.SelectedItem as User;
            if (user != null)
            {
                await controller.DeleteUser(user);
            }
        }

        private void UsersList_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            AuthenticateButton.IsEnabled = UsersList.SelectedItem != null;
            controller.DataModel.CurrentUser = UsersList.SelectedItem as User;
            SavePropertyState(SelectedUser, UsersList.SelectedIndex);
        }

        private async void Services_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            TestServiceButton.IsEnabled = ServicesList.SelectedItem != null;
            if (e.AddedItems.Count != 1 || (e.AddedItems.Count == 1 && e.RemovedItems.Count == 1 && e.AddedItems[0].Equals(e.RemovedItems[0])))
                return;

            if ((processSelection && shouldSetService) || ChangedByClick(e.AddedItems, e.RemovedItems))
            {
                Status status = await controller.ProcessServiceChanged();
                if (status == null || status.StatusCode != Status.Code.OK)
                {
                    await _dispatcher.RunAsync(CoreDispatcherPriority.Normal, () =>
                    {
                        rootPage.NotifyUser(string.Format(ResourceLoader.GetForCurrentView().GetString("InitializationFailed"), (status == null ? "null" : status.StatusCode.ToString())), MainPage.NotifyType.ErrorMessage);
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
                        rootPage.NotifyUser(ResourceLoader.GetForCurrentView().GetString("ServiceSet"), MainPage.NotifyType.StatusMessage);
                    });
                }
            }

            SavePropertyState(SelectedService, ServicesList.SelectedIndex);
        }

        private bool ChangedByClick(IList<object> addedItems, IList<object> removedItems)
        {
            return addedItems.Count == 1 && removedItems.Count == 1 && false == addedItems[0].Equals(removedItems[0]);
        }

        private async void TestBackend_click(object sender, RoutedEventArgs e)
        {
            await controller.TestBackend();            
        }

        private void StatusBorder_Tapped(object sender, TappedRoutedEventArgs e)
        {
            rootPage.NotifyUser(string.Empty);
        }

        #endregion // handlers
    }
}
