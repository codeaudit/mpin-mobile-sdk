﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;
using MPinSDK.Common;
using MPinSDK.Models; // navigation extensions
using Windows.ApplicationModel.Resources; 

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkID=390556

namespace MPinDemo
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class AuthenticationScreen : Page, INotifyPropertyChanged
    {
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

        public AuthenticationScreen()
        {
            this.InitializeComponent();
            this.DataContext = this;
        }

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