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
using HockeyApp;
using ZXing;
using ZXing.Common;
using System.Threading.Tasks;
using System.Threading;
using Windows.Web.Http;
using System.IO;
using Windows.UI.Xaml.Media.Imaging;
using Windows.Storage.Streams;
using Windows.Storage.Pickers;

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
        private bool isServiceAdding = false;
        private MainPage rootPage = null;
        private CoreDispatcher _dispatcher;

        internal static ApplicationDataContainer RoamingSettings = null;
        private static Controller controller = null;
        private static bool showUsers = true;
        private static int selectedServiceIndex;
        private BarcodeReader _barcodeReader;
        private static readonly IEnumerable<string> SupportedImageFileTypes = new List<string> { ".jpeg", ".jpg", ".png" };

        #endregion // members

        #region constructors
        static BlankPage1()
        {
            controller = new Controller();
        }

        public BlankPage1()
        {
            this.InitializeComponent();
            NavigationCacheMode = NavigationCacheMode.Required;

            _dispatcher = Window.Current.Dispatcher;
            this.DataContext = controller.DataModel;
            RoamingSettings = ApplicationData.Current.RoamingSettings;
            controller.PropertyChanged += controller_PropertyChanged;

            // Attach event which will return the picked files
            var app = Application.Current as App;
            if (app != null)
            {
                app.FilesPicked += OnFilesPicked;
            }

            _barcodeReader = new BarcodeReader
            {
                Options = new DecodingOptions() { TryHarder = true },
                PossibleFormats = new BarcodeFormat[] { BarcodeFormat.QR_CODE },
                AutoRotate = true
            };
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

            SetControlsIsEnabled(e.Parameter.ToString());

            List<object> data = (Window.Current.Content as Frame).GetNavigationData() as List<object>;
            if (data != null && data.Count == 2)
            {
                string command = data[0].ToString();
                showUsers = !command.Contains("Service");
                isServiceAdding = command == "AddService";
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

        protected async override void OnNavigatedFrom(NavigationEventArgs e)
        {
            base.OnNavigatedFrom(e);
            if (controller != null)
                await controller.Dispose();
        }

        #endregion

        #region methods
        
        private void SetControlsIsEnabled(string param, bool force = false, bool isInProgress = true)
        {
            // the process has been canceled
            if (!string.IsNullOrEmpty(param) && param.Equals("HardwareBack"))
                controller.IsUserInProcessing = false;

            if (force)
            {
                Progress.Visibility = isInProgress ? Visibility.Visible : Visibility.Collapsed;
            }
            else
            {
                Progress.Visibility = controller.IsUserInProcessing ? Windows.UI.Xaml.Visibility.Visible : Windows.UI.Xaml.Visibility.Collapsed;
            }
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
                // do not change the current service if already set as it clears and initializes the Users which makes the currentUser invalid pointer
                if (controller.DataModel.CurrentService != (Backend)this.ServicesList.Items[selectedIndex.Value])
                    controller.DataModel.CurrentService = (Backend)this.ServicesList.Items[selectedIndex.Value];

                this.ServicesList.SelectedIndex = selectedIndex.Value;                
            }

            if (this.MainPivot.SelectedIndex == 0)
            {
                this.ServicesList.ScrollIntoView(this.ServicesList.SelectedItem);
            }
        }
        #endregion

        #region QRCode scanning
        private static void TriggerPicker(IEnumerable<string> fileTypeFilers, bool shouldPickMultiple = false)
        {
            var fop = new FileOpenPicker();
            foreach (var fileType in fileTypeFilers)
            {
                fop.FileTypeFilter.Add(fileType);
            }

            if (shouldPickMultiple)
            {
                fop.PickMultipleFilesAndContinue();
            }
            else
            {
                fop.PickSingleFileAndContinue();
            }
        }

        private async void OnFilesPicked(IReadOnlyList<StorageFile> files)
        {
            if (files == null || files.Count != 1)
            {
                rootPage.NotifyUser(files == null ? ResourceLoader.GetForCurrentView().GetString("NoImage") : ResourceLoader.GetForCurrentView().GetString("NoQRInImage"), MainPage.NotifyType.ErrorMessage);
                return;
            }

            SetControlsIsEnabled(null, true);
            
            var data = await FileIO.ReadBufferAsync(files[0]);
            // create a stream from the file
            var ms = new InMemoryRandomAccessStream();
            var dw = new Windows.Storage.Streams.DataWriter(ms);
            dw.WriteBuffer(data);
            await dw.StoreAsync();
            ms.Seek(0);

            // find out how big the image is
            var bm = new BitmapImage();
            await bm.SetSourceAsync(ms);

            // create a writable bitmap of the right size
            var wb = new WriteableBitmap(bm.PixelWidth, bm.PixelHeight);
            ms.Seek(0);

            // load the writable bitmap from the stream
            await wb.SetSourceAsync(ms);

            Result result = _barcodeReader.Decode(wb);
            if (result != null)
            {
                if (string.IsNullOrEmpty(result.Text))
                {
                    rootPage.NotifyUser(ResourceLoader.GetForCurrentView().GetString("EmptyURL"), MainPage.NotifyType.ErrorMessage);
                    return;
                }

                System.Uri uri;
                if (!System.Uri.TryCreate(System.Uri.EscapeUriString(result.Text), UriKind.Absolute, out uri))
                {
                    rootPage.NotifyUser(ResourceLoader.GetForCurrentView().GetString("InvalidURL"), MainPage.NotifyType.ErrorMessage);
                    return;
                }

                try
                {
                    await SendRequest(result.Text, HttpMethod.Get, string.Empty, null);
                }
                catch (FileNotFoundException)
                {
                    rootPage.NotifyUser(ResourceLoader.GetForCurrentView().GetString("NotFoundConfiguration"), MPinDemo.MainPage.NotifyType.ErrorMessage);
                }
                catch (ArgumentException ae)
                {
                    rootPage.NotifyUser(ae.Message, MPinDemo.MainPage.NotifyType.ErrorMessage);
                }
                catch (Exception exc)
                {
                    rootPage.NotifyUser(exc.Message.Contains("0x80072EFD")
                                           ? ResourceLoader.GetForCurrentView().GetString("NetworkProblem")
                                           : ResourceLoader.GetForCurrentView().GetString("RequestError"),
                                           MPinDemo.MainPage.NotifyType.ErrorMessage);
                }
            }
            else
            {
                rootPage.NotifyUser(ResourceLoader.GetForCurrentView().GetString("NoQRCode"), MPinDemo.MainPage.NotifyType.ErrorMessage);
            }

            SetControlsIsEnabled(null, true, false);
        }

        private async Task SendRequest(String serviceURL, Windows.Web.Http.HttpMethod http_method, String requestBody, IDictionary<String, String> requestProperties)
        {
            // TODO: check if the response is empty, if an exception is thrown
            // empty resonse returned on unsuccessful authentication
            HttpClient httpClient = new HttpClient();
            CancellationTokenSource cts = new CancellationTokenSource();
            try
            {
                System.Uri resourceAddress = new System.Uri(serviceURL);
                HttpRequestMessage request = new HttpRequestMessage(http_method, resourceAddress);

                HttpResponseMessage response = await httpClient.SendRequestAsync(
                    request,
                    HttpCompletionOption.ResponseHeadersRead).AsTask(cts.Token);

                string responseData = await response.Content.ReadAsStringAsync();
                if (!string.IsNullOrEmpty(responseData) && response.StatusCode.Equals(HttpStatusCode.Ok))
                {
                    List<Backend> backends = controller.DataModel.GetBackendsFromJson(responseData);
                    if (!Frame.Navigate(typeof(ReadConfiguration), new List<object> { backends, GetExistentsIndexes(backends) }))
                    {
                        throw new Exception(ResourceLoader.GetForCurrentView().GetString("NavigationFailedExceptionMessage"));
                    }
                }
            }
            catch (TaskCanceledException tce)
            {
                rootPage.NotifyUser(ResourceLoader.GetForCurrentView().GetString("CanceledRequest"), MPinDemo.MainPage.NotifyType.ErrorMessage);
                throw tce;
            }
        }

        private List<int> GetExistentsIndexes(List<Backend> newBackends)
        {
            List<int> duplicatesIndexes = new List<int>();
            foreach (var backend in newBackends)
            {
                if (controller.DataModel.BackendsList.Any((item) => item.Name.Equals(backend.Name)))
                {
                    duplicatesIndexes.Add(newBackends.IndexOf(backend));
                }
            }

            return duplicatesIndexes;
        }
        #endregion // QRCode scanning

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
            switch (this.MainPivot.SelectedIndex)
            {
                case 0:
                    showUsers = false;
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
            controller.EditService(this.ServicesList.SelectedIndex, this.ServicesList.SelectedIndex > AppDataModel.PredefinedServicesCount);
        }

        private void MainPivot_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            SelectAppBarButton.IsEnabled = this.MainPivot.SelectedIndex == 0 ? ServicesList.SelectedItem != null : UsersListBox.SelectedItem != null;
            ResetPinButton.Visibility = this.MainPivot.SelectedIndex == 0 ? Visibility.Collapsed : Visibility.Visible;
            AddAppBarButton.Icon = new SymbolIcon(this.MainPivot.SelectedIndex == 0 ? Symbol.Add : Symbol.AddFriend);

            EditButton.Visibility = this.MainPivot.SelectedIndex == 0 ? Visibility.Visible : Visibility.Collapsed;
            ScanAppBarButton.Visibility = this.MainPivot.SelectedIndex == 0 ? Visibility.Visible : Visibility.Collapsed;
        }

        private async void Delete_Click(object sender, RoutedEventArgs e)
        {
            switch (this.MainPivot.SelectedIndex)
            {
                case 0:
                    Backend backend = ServicesList.SelectedItem as Backend;
                    if (backend != null && !string.IsNullOrEmpty(backend.BackendUrl))
                    {
                        await controller.DeleteService(backend, this.ServicesList.SelectedIndex > AppDataModel.PredefinedServicesCount);
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
            UsersPivotItem.Header = " " + UsersPivotItem.Header.ToString().Trim();

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

        private async void CheckForUpdate_Click(object sender, RoutedEventArgs e)
        {
            await HockeyClient.Current.CheckForAppUpdateAsync(new UpdateCheckSettings()
            {
                UpdateMode = UpdateMode.InApp
            });
        }

        private void ServicesList_Loaded(object sender, RoutedEventArgs e)
        {
            if (isServiceAdding)
            {
                selectedServiceIndex = controller.NewAddedServiceIndex;
            }

            if (!showUsers && controller.DataModel.BackendsList.Count > selectedServiceIndex && selectedServiceIndex > -1)
            {
                // select a service after being edited/added
                this.ServicesList.SelectedItem = controller.DataModel.BackendsList[selectedServiceIndex];

                selectedServiceIndex = -1;
            }
        }

        private void ScanAppBarButton_Click(object sender, RoutedEventArgs e)
        {
            showUsers = false;
            TriggerPicker(SupportedImageFileTypes);
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

            if (backend.Type == ConfigurationType.Online)
                return ResourceLoader.GetForCurrentView().GetString("OnlineLogin");

            if (backend.Type == ConfigurationType.OTP)
                return ResourceLoader.GetForCurrentView().GetString("OTPLogin");

            return ResourceLoader.GetForCurrentView().GetString("MobileLogin");
        }

        public object ConvertBack(object value, Type targetType, object parameter, string language)
        {
            return value;
        }
    }
}
