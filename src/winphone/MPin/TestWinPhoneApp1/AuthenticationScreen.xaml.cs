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

using MPinSDK.Models; // navigation extensions
using System.Collections.Generic;
using System.ComponentModel;
using System.Runtime.CompilerServices;
using Windows.ApplicationModel.Resources;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Navigation; 

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkID=390556

namespace MPinDemo
{
    /// <summary>
    /// An page used to display the user his/her authentication state.
    /// </summary>
    public sealed partial class AuthenticationScreen : Page, INotifyPropertyChanged
    {
        #region Members
        private User _user;
        /// <summary>
        /// Gets or sets the user that is being authenticated.
        /// </summary>
        /// <value>
        /// The user.
        /// </value>
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

        #region C'tor
        /// <summary>
        /// Initializes a new instance of the <see cref="AuthenticationScreen"/> class.
        /// </summary>
        public AuthenticationScreen()
        {
            this.InitializeComponent();
            this.DataContext = this;
        }
        #endregion // C'tor

        #region Overrides
        /// <summary>
        /// Invoked when this page is about to be displayed in a Frame.
        /// </summary>
        /// <param name="e">Event data that describes how this page was reached.
        /// This parameter is typically used to configure the page.</param>
        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            List<object> data = e.Parameter as List<object>;
            if (data != null && data.Count == 2)
            {
                this.User = data[0] as User;
                Status s = data[1] as Status;
                if (this.User != null && s != null)
                {
                    switch(s.StatusCode)
                    {
                        // todo.... -> NetworkError should not be returned
                        case Status.Code.OK:
                            AuthenticatedTB.Text = ResourceLoader.GetForCurrentView().GetString("SuccessfulAuth");
                            break;
                            
                        case Status.Code.IncorrectAccessNumber :
                            AuthenticatedTB.Text = ResourceLoader.GetForCurrentView().GetString("IncorrectAccessNumber");
                            break;

                        case Status.Code.IncorrectPIN:
                            AuthenticatedTB.Text = ResourceLoader.GetForCurrentView().GetString("IncorrectPin");
                            break;
                        default:
                            AuthenticatedTB.Text = ResourceLoader.GetForCurrentView().GetString("ErrorAuth") + s.ErrorMessage; 
                            break;
                    }

                }
            }
        }
        #endregion // Overrides

        #region INotifyPropertyChanged
        /// <summary>
        /// Occurs when a property value changes of the <see cref="AuthenticationScreen"/> page.
        /// </summary>
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
