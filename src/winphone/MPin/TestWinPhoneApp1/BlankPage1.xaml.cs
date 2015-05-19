﻿using MPinDemo.Models;
using MPinSDK.Common; // navigation extensions
using MPinSDK.Models;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using Windows.ApplicationModel.Resources;
using Windows.Storage;
using Windows.UI.Core;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Navigation;

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
        private bool shouldSetService = false;
        private bool isInitialLoad = false;
        private MainPage rootPage = null;
        private CoreDispatcher _dispatcher;

        private ApplicationDataContainer roamingSettings = null;
        private static Controller controller = null;
        private static bool IsSelectedBtnEnabled = true;
        private readonly ResourceLoader resourceLoader = ResourceLoader.GetForCurrentView("Resources");
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
            controller.PropertyChanged += controller_PropertyChanged;

            LoadSettings();
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
            else
            {
                string param = GetAllPossiblePassedParams(e.Parameter.ToString());
                isInitialLoad = !string.IsNullOrEmpty(param) && param.Equals("InitialLoad");
            }

            if (isInitialLoad)
            {
                //var sampleDataGroup = await AppDataModel.GetBackendsAsync();                
                //controller.DataModel.BackendsList = sampleDataGroup;
            }
        }

        private string GetAllPossiblePassedParams(string param)
        {
            string navigationData = (Window.Current.Content as Frame).GetNavigationData() as string;
            return string.IsNullOrEmpty(navigationData) ? param : navigationData;
        }

        #endregion

        #region methods

        private User GetSelectedUser(ICollection<User> users)
        {
            if (users == null || users.Count == 0 || controller.DataModel.CurrentUser == null)
                return GetSelectedUserFromSettings();

            foreach (var user in users)
                if (user.Equals(controller.DataModel.CurrentUser))
                    return user;

            return null;
        }

        private void LoadSettings()
        {
            SetSelectedServicesIndex();
            SelectAppBarButton.IsEnabled = UsersListBox.SelectedItem != null;
            ResetPinButton.IsEnabled = UsersListBox.SelectedItem != null;

            if (controller.DataModel.CurrentService.BackendUrl != null)
            {
                this.MainPivot.SelectedItem = this.UsersPivotItem;
            }
        }

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
            if (selectedIndex != null && selectedIndex >= 0 && selectedIndex < UsersListBox.Items.Count)
            {
                return this.UsersListBox.Items[selectedIndex.Value] as User;
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
                this.ServicesList.ScrollIntoView(this.ServicesList.SelectedItem);
            }
            else
            {
                shouldSetService = true;
            }
        }
        #endregion

        #endregion // Methods

        #region handlers
        private async void Authenticate_Click(object sender, RoutedEventArgs e)
        {
            IsSelectedBtnEnabled = false;
            await controller.ProcessUser();
            IsSelectedBtnEnabled = true;
        }

        private void UsersList_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            SelectAppBarButton.IsEnabled = IsSelectedBtnEnabled && UsersListBox.SelectedItem != null;
            ResetPinButton.IsEnabled = UsersListBox.SelectedItem != null;

            UsersListBox.ScrollIntoView(UsersListBox.SelectedItem);
            controller.DataModel.CurrentUser = UsersListBox.SelectedItem as User;
            SavePropertyState(SelectedUser, UsersListBox.SelectedIndex);
        }

        private void Services_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            TestBackendButton.IsEnabled = ServicesList.SelectedItem != null;
            ServicesList.ScrollIntoView(ServicesList.SelectedItem);
            SavePropertyState(SelectedService, ServicesList.SelectedIndex);
        }

        private void ServicesList_DataContextChanged(FrameworkElement sender, DataContextChangedEventArgs args)
        {
            if (isInitialLoad)
            {
                SetSelectedServicesIndex();

                if (controller.DataModel.CurrentService.BackendUrl != null)
                {
                    this.MainPivot.SelectedItem = this.UsersPivotItem;
                }
            }
        }

        private async void UsersList_DataContextChanged(FrameworkElement sender, DataContextChangedEventArgs args)
        {
            if (UsersListBox != null && UsersListBox.ItemsSource != null)
            {
                UsersListBox.SelectedItem = GetSelectedUser(controller.DataModel.UsersList);

                if (isInitialLoad)
                {
                    await controller.ProcessUser();
                    isInitialLoad = false;
                }
            }
        }

        private void controller_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            if (e.PropertyName == "IsValidService" && !controller.IsValidService)
                ServicesList.SelectedIndex = -1;
        }

        private void AddAppBarButton_Click(object sender, RoutedEventArgs e)
        {
            var container = this.MainPivot.ContainerFromIndex(this.MainPivot.SelectedIndex) as ContentControl;
            var listView = container.ContentTemplateRoot as ListView;

            switch (this.MainPivot.SelectedIndex)
            {
                case 0:
                    // Add a service

                    //listView.ScrollIntoView(newItem, ScrollIntoViewAlignment.Leading);
                    break;
                case 1:
                    // Add a user
                    if (string.IsNullOrEmpty(controller.DataModel.CurrentService.BackendUrl))
                    {
                        rootPage.NotifyUser(ResourceLoader.GetForCurrentView().GetString("NoServiceSet"), MainPage.NotifyType.ErrorMessage);
                    }
                    else
                    {
                        if (!Frame.Navigate(typeof(AddNewUser)))
                        {
                            throw new Exception(this.resourceLoader.GetString("NavigationFailedExceptionMessage"));
                        }

                    }
                    break;
            }
        }

        private async void TestBackendButton_Click(object sender, RoutedEventArgs e)
        {
            await controller.TestBackend();
        }

        private void MainPivot_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            SelectAppBarButton.Visibility = this.MainPivot.SelectedIndex == 0 ? Visibility.Collapsed : Visibility.Visible;
            ResetPinButton.Visibility = this.MainPivot.SelectedIndex == 0 ? Visibility.Collapsed : Visibility.Visible;

            TestBackendButton.Visibility = this.MainPivot.SelectedIndex == 0 ? Visibility.Visible : Visibility.Collapsed;
        }

        private async void Delete_Click(object sender, RoutedEventArgs e)
        {
            switch (this.MainPivot.SelectedIndex)
            {
                case 0:
                    Backend? backend = ServicesList.SelectedItem as Backend?;
                    if (backend != null && string.IsNullOrEmpty(backend.Value.BackendUrl))
                    {
                        await controller.DeleteService(backend.Value);
                    }
                    break;

                case 1:
                    User user = UsersListBox.SelectedItem as User;
                    if (user != null)
                    {
                        await controller.DeleteUser(user);
                    }
                    break;
            }
        }

        private async void ResetPinButton_Click(object sender, RoutedEventArgs e)
        {
            await controller.ResetPIN(controller.DataModel.CurrentUser);
        }
        #endregion // handlers

    }
}
