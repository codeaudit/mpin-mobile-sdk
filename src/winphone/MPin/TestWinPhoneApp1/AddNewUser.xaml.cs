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

using MPinDemo.Models;
using MPinSDK.Common;
using System;
using System.Collections.Generic;
using System.Text.RegularExpressions;
using Windows.ApplicationModel.Resources;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Navigation;

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkID=390556

namespace MPinDemo
{
    /// <summary>
    /// An page used for adding a new user.
    /// </summary>
    public sealed partial class AddNewUser : Page
    {
        #region Fields
        internal const string DefaultDeviceName = "Sample App (WinPhone)";
        private const string DeviceNameString = "DeviceName";
        private MainPage rootPage = null;
        private bool displayDeviceName = false;
        #endregion // Fields

        #region Constructor
        /// <summary>
        /// Initializes a new instance of the <see cref="AddNewUser"/> class.
        /// </summary>
        public AddNewUser()
        {
            this.InitializeComponent();
            InputScope scope = new InputScope();
            InputScopeName name = new InputScopeName();

            name.NameValue = InputScopeNameValue.EmailSmtpAddress;
            scope.Names.Add(name);

            this.UserId.InputScope = scope;
        }
        #endregion // Constructor

        #region Members
        private string CachedDeviceName
        {
            get
            {
                return BlankPage1.RoamingSettings.Values[DeviceNameString] == null ? string.Empty : BlankPage1.RoamingSettings.Values[DeviceNameString].ToString();
            }
        }
        #endregion // Members

        #region Methods
        #region Overrides
        /// <summary>
        /// Invoked when this page is about to be displayed in a Frame.
        /// </summary>
        /// <param name="e">Event data that describes how this page was reached.
        /// This parameter is typically used to configure the page.</param>
        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            rootPage = MainPage.Current;
            if (bool.TryParse(e.Parameter.ToString(), out displayDeviceName))
            {
                DeviceNameContainer.Visibility = displayDeviceName ? Windows.UI.Xaml.Visibility.Visible : Windows.UI.Xaml.Visibility.Collapsed;
                if (displayDeviceName)
                {
                    DeviceName.Text = !string.IsNullOrEmpty(this.CachedDeviceName) ? this.CachedDeviceName : DefaultDeviceName;
                }
            }

            //This code opens up the keyboard when you navigate to the page.
            this.UserId.UpdateLayout();
            this.UserId.Focus(FocusState.Keyboard);
        }
        #endregion // Overrides
        
        private void Button_Click(object sender, RoutedEventArgs e)
        {
            ProcessNewUser();
        }

        private void ProcessNewUser()
        {
            if (Controller.IfUserExists(this.UserId.Text))
            {
                rootPage.NotifyUser(ResourceLoader.GetForCurrentView().GetString("ExistingUser"), MainPage.NotifyType.ErrorMessage);
            }
            else if (!IsMailValid(this.UserId.Text))
            {
                rootPage.NotifyUser(ResourceLoader.GetForCurrentView().GetString("NotValidMail"), MainPage.NotifyType.ErrorMessage);
            }
            else if (rootPage.IsInternetConnected)
            {
                CacheDeviceName();
                Frame mainFrame = rootPage.FindName("MainFrame") as Frame;
                mainFrame.GoBack(new List<object>() { "AddUser", new List<string> { this.UserId.Text, DeviceName.Text } });
            }
            else
            {
                Controller.DisplayNoNetworkMessage();
            }
        }

        private void CacheDeviceName()
        {
            if (string.IsNullOrEmpty(this.CachedDeviceName) && !this.CachedDeviceName.Equals(DeviceName.Text))
            {
                BlankPage1.SavePropertyState(DeviceNameString, DeviceName.Text);
            }
        }

        private bool IsMailValid(string mailString)
        {
            if (String.IsNullOrEmpty(mailString))
                return false;

            try
            {
                return Regex.IsMatch(mailString,
                      @"^(?("")("".+?(?<!\\)""@)|(([0-9a-z]((\.(?!\.))|[-!#\$%&'\*\+/=\?\^`\{\}\|~\w])*)(?<=[0-9a-z])@))" +
                      @"(?(\[)(\[(\d{1,3}\.){3}\d{1,3}\])|(([0-9a-z][-\w]*[0-9a-z]*\.)+[a-z0-9][\-a-z0-9]{0,22}[a-z0-9]))$",
                      RegexOptions.IgnoreCase, TimeSpan.FromMilliseconds(250));
            }
            catch (RegexMatchTimeoutException)
            {
                return false;
            }
        }

        private void UserId_KeyUp(object sender, KeyRoutedEventArgs e)
        {
            if (e.Key == Windows.System.VirtualKey.Enter)
            {
                if (this.displayDeviceName)
                {
                    DeviceName.Focus(FocusState.Keyboard);
                }
                else
                {
                    ProcessNewUser();
                }
            }
        }

        private void DeviceName_KeyUp(object sender, KeyRoutedEventArgs e)
        {
            if (e.Key == Windows.System.VirtualKey.Enter)
            {
                ProcessNewUser();
            }
        }

        private void DeviceName_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (DeviceName.Text != DefaultDeviceName || (!string.IsNullOrEmpty(this.CachedDeviceName) && !this.CachedDeviceName.Equals(DeviceName.Text)))
            {
                BlankPage1.SavePropertyState("DeviceName", DeviceName.Text);
            }
        }
        #endregion // Methods
    }
}
