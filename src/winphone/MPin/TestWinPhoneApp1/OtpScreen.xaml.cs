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

using MPinSDK.Common;
using MPinSDK.Models;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Runtime.CompilerServices;
using System.Threading.Tasks;
using Windows.UI.Core;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Navigation;


// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkID=390556

namespace MPinDemo
{
    /// <summary>
    /// An page used when login with OTP used.
    /// </summary>
    public sealed partial class OtpScreen : Page, INotifyPropertyChanged
    {
        #region Fields & Members
        private MainPage rootPage = null;
        private CoreDispatcher _dispatcher;

        private OTP _otp;
        public OTP Otp
        {
            get
            {
                return this._otp;
            }
            set
            {
                if (this._otp != value)
                {
                    this._otp = value;
                    this.OnPropertyChanged();
                }
            }
        }

        private User _user;
        public User CurrentUser
        {
            get
            {
                return this._user;
            }
            set
            {
                if (this._user != value)
                {
                    this._user = value;
                    this.OnPropertyChanged();
                }
            }
        }

        int progressPercent;
        public int ProgressPercent
        {
            get
            {
                return this.progressPercent;
            }
            set
            {
                if (this.progressPercent != value)
                    this.progressPercent = value;

                this.OnPropertyChanged();
            }
        }
        #endregion

        #region C'tor

        public OtpScreen()
        {
            this.InitializeComponent();
            _dispatcher = Window.Current.Dispatcher;
            this.DataContext = this;
        }
        #endregion

        #region Methods
        /// <summary>
        /// Invoked when this page is about to be displayed in a Frame.
        /// </summary>
        /// <param name="e">Event data that describes how this page was reached.
        /// This parameter is typically used to configure the page.</param>
        protected async override void OnNavigatedTo(NavigationEventArgs e)
        {
            rootPage = MainPage.Current;
            List<object> data = e.Parameter as List<object>;
            if (data != null && data.Count == 2)
            {
                this.CurrentUser = data[1] as User;
               
                this.Otp = data[0] as OTP;
                if (this.Otp != null && Otp.Status.StatusCode == Status.Code.OK)
                {
                    int start = DateTime.Now.Millisecond;
                    this.ProgressPercent = Otp.TtlSeconds;

                    IProgress<object> progress = new Progress<object>(_ => UpdateView());
                    await Task.Run(async () =>
                    {
                        while (Otp.TtlSeconds >= 0)
                        {
                            await Task.Delay(1000);
                            progress.Report(null);
                        }
                    });

                    Frame mainFrame = rootPage.FindName("MainFrame") as Frame;
                    if (mainFrame.CanGoBack)
                        mainFrame.GoBack(null);
                }
            }
        }

        private void UpdateView()
        {
            int remaining = Otp.TtlSeconds--;
            if (remaining < 0)
            {
                remaining = 0;
            }

            if (remaining > 0)
            {
                TimeLeftView.Visibility = Visibility.Visible;
                ExpiredTB.Visibility = Visibility.Collapsed;

                TimeLeft.Text = remaining.ToString();
                ProgressPercent = remaining;
            }
            else
            {
                ProgressPercent = 0;
                TimeLeftView.Visibility = Visibility.Collapsed;
                ExpiredTB.Visibility = Visibility.Visible;
            }
        }
        
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
        #endregion // Methods
    }
}
