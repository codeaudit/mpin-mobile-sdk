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
using MPinSDK.Models;
using System.Collections.Generic;
using System.ComponentModel;
using System.Runtime.CompilerServices;
using Windows.ApplicationModel.Resources;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Navigation;

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkID=390556

namespace MPinDemo
{
    /// <summary>
    /// An page displayed after adding a user to ask for confirmation.
    /// </summary>
    public sealed partial class EmailConfirmed : Page, INotifyPropertyChanged
    {
        #region fields
        private MainPage rootPage = null;
        #endregion // fields

        #region C'tor
        public EmailConfirmed()
        {
            this.InitializeComponent();
            this.DataContext = this;
        }
        #endregion // C'tor

        #region Members
        private User _user;
        public User User
        {
            get
            {
                return _user;
            }
            set
            {
                if (_user != value)
                {
                    _user = value;
                    this.OnPropertyChanged();
                }
            }
        }
        #endregion

        /// <summary>
        /// Invoked when this page is about to be displayed in a Frame.
        /// </summary>
        /// <param name="e">Event data that describes how this page was reached.
        /// This parameter is typically used to configure the page.</param>
        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            rootPage = MainPage.Current;
            if (e.Parameter != null)
            {
                this.User = e.Parameter as User;
            }
        }

        #region Handlers
        private async void Button_Click(object sender, RoutedEventArgs e)
        {
            Status s = await Controller.OnEmailConfirmed(this.User);

            string errorMsg = s == null
                ? string.Format(ResourceLoader.GetForCurrentView().GetString("UserRegistrationProblem"), User.Id, User.UserState)
                : s.StatusCode != Status.Code.OK 
                    ?  s.StatusCode  == Status.Code.IdentityNotVerified 
                        ? ResourceLoader.GetForCurrentView().GetString("UserNotConfirmed")
                            : string.Format(ResourceLoader.GetForCurrentView().GetString("UserRegistrationProblemReason"), User.Id, s.ErrorMessage) 
                            : string.Empty;

            if (!string.IsNullOrEmpty(errorMsg))
            {
                rootPage.NotifyUser(errorMsg, MainPage.NotifyType.ErrorMessage);
            }            
        }

        private void Resend_Click(object sender, RoutedEventArgs e)
        {
            if (this.User != null)
                lock (Window.Current.Content)
                {
                    Status st = Controller.RestartRegistration(this.User);
                    if (st.StatusCode != Status.Code.OK)
                    {
                        Frame mainFrame = MainPage.Current.FindName("MainFrame") as Frame;
                        mainFrame.GoBack(new List<object>() { "Error", st.ErrorMessage });
                    }
                }
        }

        private void GoIdentities_Click(object sender, RoutedEventArgs e)
        {
            System.Diagnostics.Debug.WriteLine("EmailConfirmed -> Identities");
            Frame mainFrame = MainPage.Current.FindName("MainFrame") as Frame;
            mainFrame.GoBack(new List<object>() { "EmailConfirmed", string.Empty });
        }
        #endregion

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
    }
}
