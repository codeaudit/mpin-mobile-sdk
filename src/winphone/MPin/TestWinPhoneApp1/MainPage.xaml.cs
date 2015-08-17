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
using System.Diagnostics;
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
        #endregion // Fields

        #region C'tor
        public MainPage()
        {
            this.InitializeComponent();
            Windows.Phone.UI.Input.HardwareButtons.BackPressed += HardwareButtons_BackPressed;

            // This is a static public property that allows downstream pages to get a handle to the MainPage instance
            // in order to call methods that are in this class.
            Current = this;
        }
        #endregion // C'tor

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
                string parameter = (Window.Current.Content as Frame).GetNavigationData() as string; // get the passed parameter from the extension method
                parameter = string.IsNullOrEmpty(parameter) ? e.Parameter as string : parameter;    // get the passed parameter from the event

                // When the navigation stack isn't restored navigate to the main screen; 
                // if no param passed - we consider to be the initial load and navigate to a screen depending on the last selected user state
                if (!MainFrame.Navigate(typeof(BlankPage1), string.IsNullOrEmpty(parameter) ? "InitialLoad" : parameter))
                {
                    throw new Exception("Failed to create main screen"); 
                }
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

        #region notification
        /// <summary>
        /// Used to display messages to the user
        /// </summary>
        /// <param name="strMessage"></param>
        /// <param name="type"></param>
        public void NotifyUser(string strMessage, NotifyType type = NotifyType.StatusMessage)
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
                    StatusBorder.Opacity = 1;
                    StartTimer(type == NotifyType.StatusMessage ? 2 : 6);
                }
                else
                {
                    StatusBorder.Opacity = 0;
                }
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
