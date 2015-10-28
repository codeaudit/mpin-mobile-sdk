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

using MPinSDK.Common; // navigation extensions
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Net.NetworkInformation;
using System.Threading.Tasks;
using Windows.ApplicationModel.Activation;
using Windows.ApplicationModel.Resources;
using Windows.Graphics.Display;
using Windows.Networking.Connectivity;
using Windows.Storage;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=391641
namespace MPinDemo
{

    /// <summary>
    /// The main page used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainPage : Page
    {
        #region Fields
        public static MainPage Current;
        private DispatcherTimer timer;
        private string parameter = string.Empty;
        private ApplicationDataContainer localSettings = ApplicationData.Current.LocalSettings;
        private const string RunTimeString = "RunTime";
        #endregion // Fields

        #region C'tor
        public MainPage()
        {
            this.InitializeComponent();
            this.Loaded += MainPage_Loaded;
            Windows.Phone.UI.Input.HardwareButtons.BackPressed += HardwareButtons_BackPressed;
            NetworkInformation.NetworkStatusChanged += NetworkInformation_NetworkStatusChanged;
            DisplayInformation.AutoRotationPreferences = DisplayOrientations.Portrait;

            // This is a static public property that allows downstream pages to get a handle to the MainPage instance
            // in order to call methods that are in this class.
            Current = this;
        }
        #endregion // C'tor

        #region Members
        internal bool IsInternetConnected
        {
            get
            {
                bool isConnected = NetworkInterface.GetIsNetworkAvailable();
                if (isConnected)
                {
                    ConnectionProfile InternetConnectionProfile = NetworkInformation.GetInternetConnectionProfile();
                    NetworkConnectivityLevel connection = InternetConnectionProfile.GetNetworkConnectivityLevel();
                    if (connection == NetworkConnectivityLevel.None || connection == NetworkConnectivityLevel.ConstrainedInternetAccess)
                    {
                        isConnected = false;
                    }
                }

                return isConnected;
            }
        }
        #endregion // Members

        #region Methods
        /// <summary>
        /// Invoked when this page is about to be displayed in a Frame.
        /// </summary>
        /// <param name="e">Event data that describes how this page was reached.
        /// This parameter is typically used to configure the page.</param>
        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            if (MainFrame.Content == null)
            {
                this.parameter = (Window.Current.Content as Frame).GetNavigationData() as string; // get the passed parameter from the extension method
                this.parameter = string.IsNullOrEmpty(parameter) ? e.Parameter as string : parameter;    // get the passed parameter from the event    
                object passed = string.IsNullOrEmpty(parameter) ? "InitialLoad" : parameter;

                if (IsTheFirstAppLaunch())
                {
                    if (!MainFrame.Navigate(typeof(AppQuide), passed))
                    {
                        throw new Exception("Failed to create starup screen");
                    }
                    return;
                }   

                if (!this.IsInternetConnected)
                {
                    if (!MainFrame.Navigate(typeof(NoNetworkScreen)))
                    {
                        throw new Exception("Failed to create no internet screen");
                    }
                    return;
                }

                // When the navigation stack isn't restored navigate to the main screen; 
                // if no param passed - we consider to be the initial load and navigate to a screen depending on the last selected user state
                if (!MainFrame.Navigate(typeof(BlankPage1), passed))
                {
                    throw new Exception("Failed to create main screen");
                }
            }
        }
        
        internal async Task Clear()
        {
            if (MainFrame != null)
            {
                if (MainFrame.SourcePageType.Equals(typeof(OtpScreen)))
                {
                    OtpScreen page = MainFrame.Content as OtpScreen;
                    page.Otp.TtlSeconds = 0;
                }
                else if (MainFrame.SourcePageType.Equals(typeof(BlankPage1)))
                {
                    BlankPage1 page = MainFrame.Content as BlankPage1;
                    await page.Clear();
                }
            }            
        }

        private bool IsTheFirstAppLaunch()
        {
            if (!localSettings.Values.Keys.Contains(RunTimeString))
            {
                localSettings.Values.Add(RunTimeString, 1);
                return true;
            }
            else
            {                
                return false;
            }
        }

        void HardwareButtons_BackPressed(object sender, Windows.Phone.UI.Input.BackPressedEventArgs e)
        {
            if (MainFrame.CanGoBack)
            {
                MainFrame.GoBack("GoBack");

                //Indicate the back button press is handled so the app does not exit
                e.Handled = true;
            }
            else // back from PinPadPages
            {
                Frame currentFrame = Window.Current.Content as Frame;
                if (currentFrame != null && currentFrame.CanGoBack)
                {
                    currentFrame.GoBack("HardwareBack");
                    e.Handled = true;
                }
            }
        }

        void MainPage_Loaded(object sender, RoutedEventArgs e)
        {
            NotifyConnectionExisting();
        }

        void NetworkInformation_NetworkStatusChanged(object sender)
        {
            Dispatcher.RunAsync(Windows.UI.Core.CoreDispatcherPriority.Normal, () =>
                {
                    if (MainFrame.Content.GetType() == typeof(NoNetworkScreen))
                    {
                        if (this.IsInternetConnected)
                        {
                            if (!MainFrame.Navigate(typeof(BlankPage1), string.IsNullOrEmpty(this.parameter) ? "InitialLoad" : this.parameter))
                            {
                                throw new Exception("Failed to create main screen");
                            }
                        }
                    }
                    else
                    {
                        NotifyConnectionExisting();
                    }
                });
        }

        private void NotifyConnectionExisting()
        {
            List<Type> excludedTextTypes = new List<Type>() { typeof(NoNetworkScreen), typeof(AuthenticationScreen) };
            if (!excludedTextTypes.Contains(MainFrame.Content.GetType()))
            {
                NotifyUser(this.IsInternetConnected ? string.Empty : ResourceLoader.GetForCurrentView().GetString("NoConnection"), NotifyType.ErrorMessage, false);
            }
        }

        #region uri associations
        private ProtocolActivatedEventArgs _protocolEventArgs = null;
        public ProtocolActivatedEventArgs ProtocolEvent
        {
            get { return _protocolEventArgs; }
            set { _protocolEventArgs = value; }
        }

        public void NavigateToProtocolPage()
        {
            //ScenarioFrame.Navigate(pageTypeToNavigete, this.ProtocolEvent.Uri); 
            parameter = this.ProtocolEvent.Uri.ToString(); // -> should be mpin://?mpinId=value1&activateKey=value2
            // TODO: SMS flow: call blankPage1(parameter) which should call controllera.VerifyUser(value1, value2); instead of FinishRegistration(..)
        }
        #endregion 
        
        #region notification
        /// <summary>
        /// Used to display messages to the user
        /// </summary>
        /// <param name="strMessage"></param>
        /// <param name="type"></param>
        public void NotifyUser(string strMessage, NotifyType type = NotifyType.StatusMessage, bool shouldDisappear = true)
        {
            Debug.WriteLine("NotifyUser: " + strMessage + type.ToString());

            if (StatusBlock != null)
            {
                switch (type)
                {
                    case NotifyType.StatusMessage:
                        StatusBorder.Background = new SolidColorBrush(Windows.UI.Colors.Green);
                        break;
                    case NotifyType.ErrorMessage:
                        StatusBorder.Background = new SolidColorBrush(Windows.UI.Colors.Red);
                        break;
                }
                StatusBlock.Text = strMessage;

                // Collapse the StatusBlock if it has no text to conserve real estate.
                if (StatusBlock.Text != String.Empty)
                {
                    StatusBorder.Visibility = Windows.UI.Xaml.Visibility.Visible;
                    if (shouldDisappear)
                        StartTimer(type == NotifyType.StatusMessage ? 2 : 8);
                }
                else
                {
                    StatusBorder.Visibility = Windows.UI.Xaml.Visibility.Collapsed;
                }

                SetMessagePosition(string.IsNullOrEmpty(StatusBlock.Text));
            }
        }

        private void SetMessagePosition(bool restorePosition)
        {
            Page framePage = MainFrame.Content as Page;
            if (framePage == null)
                return;

            AppBar bottomAppBar = framePage.BottomAppBar;
            if (bottomAppBar != null || restorePosition)
            {
                double bottom = restorePosition ? 0 : bottomAppBar.Height;                
                StatusBorder.Margin = new Thickness(StatusBorder.Margin.Left, StatusBorder.Margin.Top, StatusBorder.Margin.Right, bottom); 
            }
        }

        private void StartTimer(int seconds)
        {
            if (timer == null)
            {
                timer = new DispatcherTimer();
                timer.Tick += timer_Tick;
                timer.Interval = new TimeSpan(0, 0, seconds);
            }

            timer.Start();
        }

        void timer_Tick(object sender, object e)
        {
            NotifyUser(string.Empty);
            timer.Stop();
        }

        public enum NotifyType
        {
            StatusMessage,
            ErrorMessage
        };

        #endregion // notification
        #endregion // Methods
    }
}
