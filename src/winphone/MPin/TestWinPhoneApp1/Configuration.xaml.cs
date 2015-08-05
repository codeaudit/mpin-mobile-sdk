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
using MPinSDK.Common; // navigation extensions
using MPinSDK.Models;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Runtime.CompilerServices;
using Windows.ApplicationModel.Resources;
using Windows.UI.Popups;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Navigation;

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkID=390556

namespace MPinDemo
{
    /// <summary>
    /// The page used to add or edit a configuration to the app.
    /// </summary>
    public sealed partial class Configuration : Page, INotifyPropertyChanged
    {
        #region Fields
        bool isAdding, isUrlChanged;
        MainPage rootPage = null;
        #endregion // Fields

        #region C'tor
        public Configuration()
        {
            this.InitializeComponent();
            InputScope scope = new InputScope();
            InputScopeName name = new InputScopeName();

            name.NameValue = InputScopeNameValue.Url;
            scope.Names.Add(name);

            this.UrlTB.InputScope = scope;
        }
        #endregion // C'tor

        #region Members
        private Backend backend;
        /// <summary>
        /// Gets or sets the backend being edited/added.
        /// </summary>
        /// <value>
        /// The backend.
        /// </value>
        public Backend Backend
        {
            get
            {
                return this.backend;
            }
            set
            {
                if (this.backend != value)
                {
                    this.backend = value;
                    OnPropertyChanged();
                }
            }
        }
        #endregion // Members

        #region Overrides
        /// <summary>
        /// Invoked when this page is about to be displayed in a Frame.
        /// </summary>
        /// <param name="e">Event data that describes how this page was reached.
        /// This parameter is typically used to configure the page.</param>
        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            rootPage = MainPage.Current;
            this.isAdding = e.Parameter == null;
            this.Backend = isAdding ? new Backend() : e.Parameter as Backend;
            this.backend.PropertyChanged += backend_PropertyChanged;

            if (this.Backend == null || this.Backend.Type == ConfigurationType.Mobile)
            {
                MobileLoginRadioButton.IsChecked = true;
            }

            RegisterService.Content = ResourceLoader.GetForCurrentView().GetString(isAdding ? "AddService" : "EditService");
        }
        #endregion // Overrides

        #region Methods
        void backend_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            if (e.PropertyName == "BackendUrl")
            {
                this.isUrlChanged = true;
            }
        }

        #region handlers
        private async void Button_Click(object sender, RoutedEventArgs e)
        {
            bool correctName = IsNameCorrect();
            if (correctName && Uri.IsWellFormedUriString(this.Backend.BackendUrl, UriKind.Absolute))
            {
                if (!isAdding && isUrlChanged)
                {
                    var confirmation = new MessageDialog(ResourceLoader.GetForCurrentView().GetString("LostUsers"));
                    confirmation.Commands.Add(new UICommand(ResourceLoader.GetForCurrentView().GetString("YesCommand")));
                    confirmation.Commands.Add(new UICommand(ResourceLoader.GetForCurrentView().GetString("NoCommand")));
                    confirmation.DefaultCommandIndex = 1;
                    var result = await confirmation.ShowAsync();
                    if (result.Equals(confirmation.Commands[0]))
                    {
                        Frame.GoBack(new List<object>() { "EditService", this.Backend });
                    }
                }
                else
                {
                    Frame.GoBack(new List<object>() { isAdding ? "AddService" : "EditService", this.Backend });
                }
            }
            else
            {
                rootPage.NotifyUser(
                    !correctName 
                    ? string.IsNullOrEmpty(this.Backend.Name) 
                        ? ResourceLoader.GetForCurrentView().GetString("EmptyTitle") 
                        : ResourceLoader.GetForCurrentView().GetString("DuplicateName")
                    : ResourceLoader.GetForCurrentView().GetString("WrongURL"), MainPage.NotifyType.ErrorMessage);
            }
        }

        private bool IsNameCorrect()
        {
            if (string.IsNullOrEmpty(this.Backend.Name))
                return false;

            if (isAdding)
            {
                return !BlankPage1.IsServiceNameExists(this.Backend.Name);
            }

            return true;
        }

        private async void Test_Click(object sender, RoutedEventArgs e)
        {
            TestBackendButton.IsEnabled = false;
            Status status = await Controller.TestBackend(this.Backend);
            rootPage.NotifyUser(ResourceLoader.GetForCurrentView().GetString("ServiceStatus") + status.StatusCode, status.StatusCode != 0 ? MainPage.NotifyType.ErrorMessage : MainPage.NotifyType.StatusMessage);
            TestBackendButton.IsEnabled = true;
        }

        private void TextBox_KeyUp(object sender, KeyRoutedEventArgs e)
        {
            TextBox tb = sender as TextBox;
            if (tb != null && (e.Key == Windows.System.VirtualKey.Enter || e.Key == Windows.System.VirtualKey.Tab))
            {
                if (tb.Equals(NameTB))
                {
                    UrlTB.Focus(FocusState.Keyboard);
                }
                else if (tb.Equals(UrlTB))
                {
                    RpsTB.Focus(FocusState.Keyboard);
                }
                else
                {
                    MobileLoginRadioButton.Focus(FocusState.Keyboard);
                }
            }
        }

        #endregion // handlers

        #region INotifyPropertyChanged
        public event PropertyChangedEventHandler PropertyChanged;
        void OnPropertyChanged([CallerMemberName]string name = "")
        {
            PropertyChangedEventHandler handler = PropertyChanged;
            if (handler != null)
            {
                handler(this, new PropertyChangedEventArgs(name));
            }
        }
        #endregion // INotifyPropertyChanged

        #endregion Methods
    }
}
