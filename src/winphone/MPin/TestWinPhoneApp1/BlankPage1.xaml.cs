using MPinDemo.Models;
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
using System.Linq;
using Windows.UI.Xaml.Data;

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
        private bool isInitialLoad = false;
        private MainPage rootPage = null;
        private CoreDispatcher _dispatcher;

        internal static ApplicationDataContainer RoamingSettings = null;
        private static Controller controller = null;
        private static bool showUsers = true;
        private static int selectedServiceIndex;
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
            RoamingSettings = ApplicationData.Current.RoamingSettings;
            controller.PropertyChanged += controller_PropertyChanged;            
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

            SetControlsIsEnabled();
            
            List<object> data = (Window.Current.Content as Frame).GetNavigationData() as List<object>;
            if (data != null && data.Count == 2)
            {
                string command = data[0].ToString();
                SetProperties(command);
                await controller.ProcessNavigation(command, data[1]);
            }
            else
            {
                string param = GetAllPossiblePassedParams(e.Parameter);
                isInitialLoad = !string.IsNullOrEmpty(param) && param.Equals("InitialLoad");
            }

            if (isInitialLoad)
            {
                var sampleDataGroup = await AppDataModel.GetBackendsAsync();
                controller.DataModel.BackendsList = sampleDataGroup;
            }

            LoadSettings();
        }

        #endregion

        #region methods

        private static void SetProperties(string command)
        {
            showUsers = !command.Contains("Service");
            if (command == "AddService")
                selectedServiceIndex = controller.NewAddedServiceIndex;
        }

        private void SetControlsIsEnabled()
        {
            this.IsEnabled = !controller.IsUserInProcessing;
            this.BottomAppBar.IsEnabled = !controller.IsUserInProcessing;
            Progress.Visibility = controller.IsUserInProcessing ? Windows.UI.Xaml.Visibility.Visible : Windows.UI.Xaml.Visibility.Collapsed;
        }

        private string GetAllPossiblePassedParams(object param)
        {
            string navigationData = (Window.Current.Content as Frame).GetNavigationData() as string;
            return string.IsNullOrEmpty(navigationData)
                ? param == null ? "" : param.ToString()
                : navigationData;
        }

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
            ResetPinButton.IsEnabled = UsersListBox.SelectedItem != null;

            if (controller.DataModel.CurrentService != null && controller.DataModel.CurrentService.BackendUrl != null && showUsers)
            {
                this.MainPivot.SelectedItem = this.UsersPivotItem;
            }
        }

        #region State

        internal static void SavePropertyState(string key, object value)
        {
            if (!RoamingSettings.Values.Keys.Contains(key))
            {
                RoamingSettings.Values.Add(key, value);
            }
            else
            {
                RoamingSettings.Values[key] = value;
            }
        }

        private User GetSelectedUserFromSettings()
        {
            int? selectedIndex = RoamingSettings.Values[SelectedUser] as int?;
            if (selectedIndex != null && selectedIndex >= 0 && selectedIndex < UsersListBox.Items.Count)
            {
                return this.UsersListBox.Items[selectedIndex.Value] as User;
            }

            return null;
        }

        private void SetSelectedServicesIndex()
        {
            int? selectedIndex = RoamingSettings.Values[SelectedService] as int?;

            if (isInitialLoad && (selectedIndex == null || selectedIndex < 0 || selectedIndex >= ServicesList.Items.Count))
            {
                // if the selected service in the list is different from the currentService -> reset it
                selectedIndex = 0;
            }

            if (selectedIndex != null && selectedIndex >= 0 && selectedIndex < ServicesList.Items.Count && showUsers)
            {
                controller.DataModel.CurrentService = (Backend)this.ServicesList.Items[selectedIndex.Value];
                this.ServicesList.SelectedIndex = selectedIndex.Value;
                this.ServicesList.ScrollIntoView(this.ServicesList.SelectedItem);
            }
        }
        #endregion

        #endregion // Methods

        #region handlers
        private void Select_Click(object sender, RoutedEventArgs e)
        {
            switch (this.MainPivot.SelectedIndex)
            {
                case 0:
                    controller.DataModel.CurrentService = (Backend)this.ServicesList.SelectedItem;
                    break;

                case 1:
                    controller.DataModel.CurrentUser = UsersListBox.SelectedItem as User;
                    break;
            }
        }

        private void UsersList_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            SelectAppBarButton.IsEnabled = !controller.IsUserInProcessing && UsersListBox.SelectedItem != null;
            ResetPinButton.IsEnabled = UsersListBox.SelectedItem != null;

            UsersListBox.ScrollIntoView(UsersListBox.SelectedItem);
            if (isInitialLoad)
            {
                controller.DataModel.CurrentUser = UsersListBox.SelectedItem as User;                
                isInitialLoad = false;
            }

            SavePropertyState(SelectedUser, UsersListBox.SelectedIndex);
        }

        private void Services_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            SelectAppBarButton.IsEnabled = ServicesList.SelectedItem != null;
            EditButton.IsEnabled = ServicesList.SelectedItem != null;
            ServicesList.ScrollIntoView(ServicesList.SelectedItem);
            SavePropertyState(SelectedService, ServicesList.SelectedIndex);
        }

        private void ServicesList_DataContextChanged(FrameworkElement sender, DataContextChangedEventArgs args)
        {
            if (isInitialLoad)
            {
                SetSelectedServicesIndex();
            }
        }

        private void controller_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            switch (e.PropertyName)
            {
                case "IsValidService":
                    if (controller.IsValidService)
                    {
                        this.MainPivot.SelectedItem = this.UsersPivotItem;
                    }
                    break;
            }
        }

        private void AddAppBarButton_Click(object sender, RoutedEventArgs e)
        {
            //var container = this.MainPivot.ContainerFromIndex(this.MainPivot.SelectedIndex) as ContentControl;
            //var listView = container.ContentTemplateRoot as ListView;

            switch (this.MainPivot.SelectedIndex)
            {
                case 0:
                    controller.AddService();                    
                    break;
                case 1:
                    controller.AddNewUser();
                    break;
            }
        }

        private void EditButton_Click(object sender, RoutedEventArgs e)
        {
            selectedServiceIndex = this.ServicesList.SelectedIndex;
            controller.EditService(this.ServicesList.SelectedIndex, this.ServicesList.SelectedIndex > 2);
        }
        
        private void MainPivot_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            SelectAppBarButton.IsEnabled = this.MainPivot.SelectedIndex == 0 ? ServicesList.SelectedItem != null : UsersListBox.SelectedItem != null;
            ResetPinButton.Visibility = this.MainPivot.SelectedIndex == 0 ? Visibility.Collapsed : Visibility.Visible;
            AddAppBarButton.Icon = new SymbolIcon(this.MainPivot.SelectedIndex == 0 ? Symbol.Add : Symbol.AddFriend);

            EditButton.Visibility = this.MainPivot.SelectedIndex == 0 ? Visibility.Visible : Visibility.Collapsed;
        }

        private async void Delete_Click(object sender, RoutedEventArgs e)
        {
            switch (this.MainPivot.SelectedIndex)
            {
                case 0:
                    Backend backend = ServicesList.SelectedItem as Backend;
                    if (backend != null && !string.IsNullOrEmpty(backend.BackendUrl))
                    {
                        await controller.DeleteService(backend, this.ServicesList.SelectedIndex > 2);
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

        private void UsersListBox_Loaded(object sender, RoutedEventArgs e)
        {
            // reset the pivot item header to properly display it on initial load 
            UsersPivotItem.Header = " " + UsersPivotItem.Header;

            if (UsersListBox != null && UsersListBox.ItemsSource != null)
            {
                UsersListBox.SelectedItem = GetSelectedUser(controller.DataModel.UsersList);
                isInitialLoad = false;                
            }
        }

        private void AboutButton_Click(object sender, RoutedEventArgs e)
        {
            if (!Frame.Navigate(typeof(About)))
            {
                throw new Exception(ResourceLoader.GetForCurrentView().GetString("NavigationFailedExceptionMessage"));
            }
        }
        
        private void ServicesList_Loaded(object sender, RoutedEventArgs e)
        {
            if (!showUsers && controller.DataModel.BackendsList.Count > selectedServiceIndex && selectedServiceIndex > -1)
            {
                // select a service after being edited/added
                this.ServicesList.SelectedItem = controller.DataModel.BackendsList[selectedServiceIndex];
                if (this.ServicesList.SelectedItem != null)
                    this.ServicesList.ScrollIntoView(this.ServicesList.SelectedItem);

                selectedServiceIndex = -1;
            }
        }
        #endregion // handlers
    }

    public class ConfigurationConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, string language)
        {
            Backend backend = value as Backend;
            if (backend == null)
                return string.Empty;

            if (backend.RequestAccessNumber)
                return ResourceLoader.GetForCurrentView().GetString("OnlineLogin");

            if (backend.RequestOtp)
                return ResourceLoader.GetForCurrentView().GetString("OTPLogin");

            return ResourceLoader.GetForCurrentView().GetString("MobileLogin");
        }

        public object ConvertBack(object value, Type targetType, object parameter, string language)
        {
            return value;
        }
    }
}
