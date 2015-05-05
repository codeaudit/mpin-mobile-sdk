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
using System.Collections.ObjectModel;

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
        private bool isInitialLoad = false;
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

            LoadSettings();

            //controller.DataModel.PropertyChanged += DataModel_PropertyChanged;
            UsersList.Items.VectorChanged += Items_VectorChanged;
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
                string param = e.Parameter.ToString();
                isInitialLoad = !string.IsNullOrEmpty(param) && param.Equals("InitialLoad");
            }
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
            //UsersList.SelectedItem = GetSelectedUser(controller.DataModel.UsersList);
            AuthenticateButton.IsEnabled = UsersList.SelectedItem != null;

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
            if (selectedIndex != null && selectedIndex >= 0 && selectedIndex < UsersList.Items.Count)
            {
                return this.UsersList.Items[selectedIndex.Value] as User;
            }

            return null;
        }

        private User GetSavedSelectedUser()
        {
            int? selectedIndex = roamingSettings.Values[SelectedUser] as int?;
            if (selectedIndex != null && selectedIndex >= 0 && controller.DataModel.UsersList != null && selectedIndex < controller.DataModel.UsersList.Count)
            {
                return controller.DataModel.UsersList[selectedIndex.Value] as User;
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

        #endregion // Methods

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
            //controller.DataModel.CurrentUser = UsersList.SelectedItem as User;
            SavePropertyState(SelectedUser, UsersList.SelectedIndex);
        }

        private void Services_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            TestServiceButton.IsEnabled = ServicesList.SelectedItem != null;
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

        private void ServicesList_DataContextChanged(FrameworkElement sender, DataContextChangedEventArgs args)
        {
            if (isInitialLoad)
            {
                //LoadSettings();
                SetSelectedServicesIndex();
                //await controller.ProcessUser();

                if (controller.DataModel.CurrentService.BackendUrl != null)
                {
                    this.MainPivot.SelectedItem = this.UsersPivotItem;
                }
            }
        }

        //private void UsersList_DataContextChanged(FrameworkElement sender, DataContextChangedEventArgs args)
        //{

        //}

        //private async void UsersList_Loaded(object sender, RoutedEventArgs e)
        //{
        //    ListBox lb = sender as ListBox;
        //    lb.Items.VectorChanged += Items_VectorChanged;
        //    //if (lb != null)
        //    //{
        //    //    Binding b = new Binding() { Path = new PropertyPath("CurrentUser"), Source = controller.DataModel};                
        //    //    UsersList.SetBinding(Selector.SelectedItemProperty, b);
        //    //}

        //    ObservableCollection<User> source = (sender as ListBox).ItemsSource as ObservableCollection<User>;
        //    if (source != null)
        //    {
        //        source.CollectionChanged += source_CollectionChanged;                
        //    }
        //}

        void Items_VectorChanged(IObservableVector<object> sender, IVectorChangedEventArgs @event)
        {
            //UsersList.SelectedItem = GetSelectedUserFromSettings();
            controller.DataModel.CurrentUser = GetSelectedUserFromSettings();
        }

        //async void source_CollectionChanged(object sender, System.Collections.Specialized.NotifyCollectionChangedEventArgs e)
        //{

        //}

        //async void DataModel_PropertyChanged(object sender, PropertyChangedEventArgs e)
        //{
        //    //if (e.PropertyName == "UsersList")
        //    //{
        //    //    //UsersList.SelectedItem = GetSavedSelectedUser();// GetSelectedUser(controller.DataModel.UsersList);
        //    //    //UsersList.SelectedIndex = GetSelectedUserIndex();
        //    //    controller.DataModel.CurrentUser = GetSavedSelectedUser();
        //    //    AuthenticateButton.IsEnabled = UsersList.SelectedItem != null;

        //    //    if (isInitialLoad)
        //    //    {
        //    //        //LoadSettings();
        //    //        if (UsersList.SelectedItem != null)
        //    //            await controller.ProcessUser();
        //    //    }
        //    //}
        //}

        //private int GetSelectedUserIndex()
        //{
        //    int? index = roamingSettings.Values[SelectedUser] as int?;
        //    return index.HasValue ? index.Value : -1;
        //}
        #endregion // handlers

    }
}
