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

using HockeyApp;
using MPinDemo.Models;
using MPinSDK.Common; // navigation extensions
using MPinSDK.Models;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;
using Windows.ApplicationModel.Core;
using Windows.ApplicationModel.Resources;
using Windows.Devices.Enumeration;
using Windows.Media.Capture;
using Windows.Media.MediaProperties;
using Windows.Phone.UI.Input;
using Windows.Storage;
using Windows.Storage.Streams;
using Windows.UI.Core;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Media.Imaging;
using Windows.UI.Xaml.Navigation;
using Windows.Web.Http;
using ZXing;
using ZXing.Common;

namespace MPinDemo
{
    /// <summary>
    /// The page containing the main app data.
    /// </summar>y  
    public sealed partial class BlankPage1 : Page
    {
        #region members

        private const string SelectedService = "ServiceSetIndex";
        private const string SelectedUser = "SelectedUser";
        private bool isInitialLoad = false;
        private bool isServiceAdding = false;
        private MainPage rootPage = null;
        private CoreDispatcher dispatcher;
        private Backend ExBackend;
        internal static ApplicationDataContainer RoamingSettings = ApplicationData.Current.RoamingSettings;
        private static Controller controller = null;
        private static bool showUsers = true;
        private static int selectedServiceIndex;
        private BarcodeReader _barcodeReader;
        private static readonly IEnumerable<string> SupportedImageFileTypes = new List<string> { ".jpeg", ".jpg", ".png" };
        private CoreApplicationView view;
        private MediaCapture captureManager;

        #endregion // members

        #region constructors
        static BlankPage1()
        {
            controller = new Controller();
        }

        public BlankPage1()
        {
            this.InitializeComponent();
            HardwareButtons.BackPressed += HardwareButtons_BackPressed;

            view = CoreApplication.GetCurrentView();
            dispatcher = Window.Current.Dispatcher;
            this.DataContext = controller.DataModel;
            controller.PropertyChanged += controller_PropertyChanged;

            _barcodeReader = new BarcodeReader
            {
                Options = new DecodingOptions()
                {
                    TryHarder = true,
                    PossibleFormats = new BarcodeFormat[] { BarcodeFormat.QR_CODE }
                },
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
            ClearBackStackIfNecessary();
            SetControlsIsEnabled(e.Parameter.ToString());

            List<object> data = (Window.Current.Content as Frame).GetNavigationData() as List<object>;
            if (data != null && data.Count == 2)
            {
                string command = data[0].ToString();
                showUsers = !command.Contains("Service");
                isServiceAdding = command == "AddService";
                ProcessControlsOperations(command);
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
            Clear();
            base.OnNavigatedFrom(e);
            if (controller != null)
            {
                await controller.Dispose();
            }

            SavePropertyState(SelectedService, controller.DataModel.BackendsList.IndexOf(controller.DataModel.SelectedBackend));
        }

        #endregion

        #region methods

        private void ClearBackStackIfNecessary()
        {
            Frame mainFrame = rootPage.FindName("MainFrame") as Frame;
            if (mainFrame.BackStack.Count == 1 && 
                ((mainFrame.BackStack[0] as PageStackEntry).SourcePageType.Equals(typeof(NoNetworkScreen)) ||
                 (mainFrame.BackStack[0] as PageStackEntry).SourcePageType.Equals(typeof(AppIntro))))
            {
                mainFrame.BackStack.RemoveAt(mainFrame.BackStack.Count - 1);
            }
        }

        private void Select()
        {
            switch (this.MainPivot.SelectedIndex)
            {
                case 0:
                    this.ExBackend = controller.DataModel.CurrentService;
                    controller.DataModel.CurrentService = controller.DataModel.SelectedBackend;
                    break;

                case 1:
                    controller.DataModel.CurrentUser = UsersListBox.SelectedItem as User;
                    break;
            }
        }

        private void ProcessControlsOperations(string command)
        {
            List<string> commandsToDisableScreen = new List<string>() { "AddUser", "SignIn"};
            if (commandsToDisableScreen.Contains(command))
                SetControlsIsEnabled(null, true);
        }

        private void SetControlsIsEnabled(string param, bool forceDisable = false, bool isInProgress = true)
        {
            // the process has been canceled
            if (!string.IsNullOrEmpty(param) && param.Equals("HardwareBack"))
                controller.IsUserInProcessing = false;

            bool deactivateAll = forceDisable ? isInProgress : controller.IsUserInProcessing;
            Progress.Visibility = deactivateAll ? Windows.UI.Xaml.Visibility.Visible : Windows.UI.Xaml.Visibility.Collapsed;
            BottomCommandBar.IsEnabled = !deactivateAll;
        }

        private void SetControlsVisibility(bool takePicture)
        {
            MainGrid.Visibility = takePicture ? Visibility.Collapsed : Visibility.Visible;
            ScanAppBarButton.Visibility = takePicture ? Visibility.Collapsed : Visibility.Visible;
            AddAppBarButton.Visibility = takePicture ? Visibility.Collapsed : Visibility.Visible;
            SelectAppBarButton.Visibility = takePicture ? Visibility.Collapsed : Visibility.Visible;
            DeleteButton.Visibility = takePicture ? Visibility.Collapsed : Visibility.Visible;
            EditButton.Visibility = takePicture ? Visibility.Collapsed : Visibility.Visible;
            ResetPinButton.Visibility = takePicture ? Visibility.Collapsed : Visibility.Visible;
            AboutButton.Visibility = takePicture ? Visibility.Collapsed : Visibility.Visible;
            UpdateCheckButton.Visibility = takePicture ? Visibility.Collapsed : Visibility.Visible;

            TakePictureButton.Visibility = takePicture ? Visibility.Visible : Visibility.Collapsed;
            PhotoContainer.Visibility = takePicture ? Visibility.Visible : Visibility.Collapsed;
        }

        private string GetAllPossiblePassedParams(object param)
        {
            string navigationData = (Window.Current.Content as Frame).GetNavigationData() as string;
            return string.IsNullOrEmpty(navigationData)
                ? param == null ? "" : param.ToString()
                : navigationData;
        }

        private User savedSelectedUser;
        private User SavedSelectedUser
        {
            get
            {
                if (savedSelectedUser == null)
                {
                    savedSelectedUser = GetSelectedUser(controller.DataModel.UsersList);
                }

                return savedSelectedUser;
            }
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

        internal static bool IsServiceNameExists(string name)
        {
            return controller.DataModel.BackendsList.Any(item => item.Name.Equals(name));
        }

        internal static bool IsActiveConfigurationURLChanged(List<int> existentsIndexes, List<Backend> newConfigurations)
        {
            int activeServiceIndex = controller.DataModel.BackendsList.IndexOf(controller.DataModel.CurrentService);
            if (existentsIndexes.Contains(activeServiceIndex))
            {
                return !newConfigurations[activeServiceIndex].BackendUrl.Equals(controller.DataModel.BackendsList[activeServiceIndex].BackendUrl);
            }

            return false;
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

            if (isInitialLoad && ServicesList != null && ServicesList.Items != null && (selectedIndex == null || selectedIndex < 0 || selectedIndex >= ServicesList.Items.Count))
            {
                // if the selected service in the list is different from the currentService -> reset it
                selectedIndex = 0;
            }

            if (selectedIndex != null && selectedIndex >= 0 && ServicesList != null && ServicesList.Items != null && selectedIndex < ServicesList.Items.Count && showUsers)
            {
                // do not change the current service if already set as it clears and initializes the Users which makes the currentUser invalid pointer
                if (controller.DataModel.CurrentService != (Backend)this.ServicesList.Items[selectedIndex.Value])
                    controller.DataModel.CurrentService = (Backend)this.ServicesList.Items[selectedIndex.Value];

                controller.DataModel.SelectedBackend = controller.DataModel.BackendsList[selectedIndex.Value];// selectedIndex.Value;
                ServicesList.ScrollIntoView(controller.DataModel.SelectedBackend);
            }
        }
        #endregion

        #region QRCode scanning

        internal async Task InitCamera()
        {
            var cameraID = await GetCameraID(Windows.Devices.Enumeration.Panel.Back);
            captureManager = null;
            captureManager = new MediaCapture();

            await captureManager.InitializeAsync(new MediaCaptureInitializationSettings
            {
                StreamingCaptureMode = StreamingCaptureMode.Video,
                PhotoCaptureSource = PhotoCaptureSource.VideoPreview,
                AudioDeviceId = string.Empty,
                VideoDeviceId = cameraID.Id
            });

            var maxResolution = captureManager.VideoDeviceController.GetAvailableMediaStreamProperties(MediaStreamType.Photo).Aggregate((i1, i2) => (i1 as VideoEncodingProperties).Width > (i2 as VideoEncodingProperties).Width ? i1 : i2);
            await captureManager.VideoDeviceController.SetMediaStreamPropertiesAsync(MediaStreamType.Photo, maxResolution);
        }

        private static async Task<DeviceInformation> GetCameraID(Windows.Devices.Enumeration.Panel desiredCamera)
        {
            // get available devices for capturing pictures
            DeviceInformation deviceID = (await DeviceInformation.FindAllAsync(DeviceClass.VideoCapture))
                .FirstOrDefault(x => x.EnclosureLocation != null && x.EnclosureLocation.Panel == desiredCamera);

            if (deviceID != null) return deviceID;
            else throw new Exception(string.Format("Camera of type {0} doesn't exist.", desiredCamera));
        }

        internal void Clear()
        {
            if (captureManager != null)
            {
                captureManager.Dispose();
                captureManager = null;
            }
        }

        private async Task<WriteableBitmap> GetImage()
        {
            StorageFile photoFile = await ApplicationData.Current.LocalFolder.CreateFileAsync("qrCode.jpg", CreationCollisionOption.ReplaceExisting);

            // take a photo with choosen Encoding
            await captureManager.CapturePhotoToStorageFileAsync(ImageEncodingProperties.CreateJpeg(), photoFile);

            await captureManager.StopPreviewAsync();

            var data = await FileIO.ReadBufferAsync(photoFile);
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

            return wb;
        }

        private async Task SendRequest(String serviceURL, Windows.Web.Http.HttpMethod http_method)
        {
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
                    await dispatcher.RunAsync(CoreDispatcherPriority.Normal, () =>
                    {
                        if (!Frame.Navigate(typeof(ReadConfiguration), new List<object> { backends, GetExistentsIndexes(backends) }))
                        {
                            throw new Exception(ResourceLoader.GetForCurrentView().GetString("NavigationFailedExceptionMessage"));
                        }
                    });
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

            Select();
        }

        private void ServicesList_DoubleTapped(object sender, Windows.UI.Xaml.Input.DoubleTappedRoutedEventArgs e)
        {
            Select();
        }

        private void UsersList_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            SelectAppBarButton.IsEnabled = !controller.IsUserInProcessing && UsersListBox.SelectedItem != null;
            ResetPinButton.IsEnabled = UsersListBox.SelectedItem != null;
            DeleteButton.IsEnabled = this.MainPivot.SelectedIndex == 0 ? ServicesList.Items.Count > 0 : UsersListBox.Items.Count > 0;

            UsersListBox.ScrollIntoView(UsersListBox.SelectedItem);
            if (isInitialLoad)
            {
                controller.DataModel.CurrentUser = UsersListBox.SelectedItem as User;
                isInitialLoad = false;
            }

            SavePropertyState(SelectedUser, UsersListBox.SelectedIndex);
        }

        private void ServicesList_Tapped(object sender, Windows.UI.Xaml.Input.TappedRoutedEventArgs e)
        {
            SelectAppBarButton.IsEnabled = EditButton.IsEnabled = controller.DataModel.SelectedBackend != null;
            ServicesList.ScrollIntoView(controller.DataModel.SelectedBackend);
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
                    else
                    {
                        // if the connection to the service is unsuccessful -> set the previous successful service.
                        controller.DataModel.SelectedBackend = this.ExBackend;
                    }
                    break;

                case "IsUserInProcessing":
                    // adding user to the server is async - reenable the page, if it is unsuccessful
                    SetControlsIsEnabled(null);
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
            selectedServiceIndex = controller.DataModel.BackendsList.IndexOf(controller.DataModel.SelectedBackend);// this.ServicesList.SelectedIndex;
            controller.EditService(selectedServiceIndex, selectedServiceIndex >= AppDataModel.PredefinedServicesCount);
        }

        private void MainPivot_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            SelectAppBarButton.IsEnabled = this.MainPivot.SelectedIndex == 0 ? controller.DataModel.SelectedBackend != null : UsersListBox.SelectedItem != null;
            DeleteButton.IsEnabled = this.MainPivot.SelectedIndex == 0 ? ServicesList.Items.Count > 0 : UsersListBox.Items.Count > 0;
            ResetPinButton.Visibility = this.MainPivot.SelectedIndex == 0 ? Visibility.Collapsed : Visibility.Visible;
            AddAppBarButton.Icon = new SymbolIcon(this.MainPivot.SelectedIndex == 0 ? Symbol.Add : Symbol.AddFriend);

            EditButton.Visibility = ScanAppBarButton.Visibility = this.MainPivot.SelectedIndex == 0 ? Visibility.Visible : Visibility.Collapsed;

            // if the user manually go to Users pivot item, but the connection to the selected backend is not successful -> set the selected to be the last successfully connected service
            if (this.MainPivot.SelectedIndex == 1 && controller.DataModel.SelectedBackend != controller.DataModel.CurrentService)
                controller.DataModel.SelectedBackend = controller.DataModel.CurrentService;

            ServicesList.ScrollIntoView(controller.DataModel.SelectedBackend);
        }

        private async void Delete_Click(object sender, RoutedEventArgs e)
        {
            switch (this.MainPivot.SelectedIndex)
            {
                case 0:
                    if (controller.DataModel.SelectedBackend != null && !string.IsNullOrEmpty(controller.DataModel.SelectedBackend.BackendUrl))
                    {
                        await controller.DeleteService(controller.DataModel.SelectedBackend, controller.DataModel.BackendsList.IndexOf(controller.DataModel.SelectedBackend) >= AppDataModel.PredefinedServicesCount);
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
                UsersListBox.SelectedItem = this.SavedSelectedUser;
                isInitialLoad = false;
            }
        }

        private void UsersListBox_LayoutUpdated(object sender, object e)
        {
            if (UsersListBox != null && UsersListBox.ItemsSource != null && this.SavedSelectedUser != null && UsersListBox.SelectedItem == null)
            {
                UsersListBox.SelectedItem = this.SavedSelectedUser;
                isInitialLoad = false;
            }
        }

        private void AboutButton_Click(object sender, RoutedEventArgs e)
        {
            Frame mainFrame = rootPage.FindName("MainFrame") as Frame;
            if (!mainFrame.Navigate(typeof(About), this.MainPivot.SelectedIndex == 0))
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
                controller.DataModel.SelectedBackend = controller.DataModel.BackendsList[selectedServiceIndex];
                ServicesList.ScrollIntoView(controller.DataModel.SelectedBackend);
            }

            selectedServiceIndex = -1;
        }

        private async void HardwareButtons_BackPressed(object sender, Windows.Phone.UI.Input.BackPressedEventArgs e)
        {
            if (PhotoContainer.Visibility == Windows.UI.Xaml.Visibility.Visible)
            {
                await captureManager.StopPreviewAsync();
                SetControlsVisibility(false);
                e.Handled = true;
            }
        }

        private async void ScanAppBarButton_Click(object sender, RoutedEventArgs e)
        {
            if (captureManager == null)
                await InitCamera();

            showUsers = false;

            SetControlsVisibility(true);

            // rotate to see preview vertically
            captureManager.SetPreviewRotation(VideoRotation.Clockwise90Degrees);
            capturePreview.Source = captureManager;
            await captureManager.StartPreviewAsync();
        }

        private async void AppBarButton_Click(object sender, RoutedEventArgs e)
        {
            SetControlsVisibility(false);
            SetControlsIsEnabled(null, true);

            var wb = await GetImage();
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
                    await SendRequest(result.Text, HttpMethod.Get);
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

        #endregion // handlers            
    }
}
