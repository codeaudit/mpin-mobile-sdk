using MPinSDK.Models;
using MPinSDK.Common;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
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
using System.ComponentModel;
using System.Runtime.CompilerServices;
using MPinDemo.Models;

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkID=390556

namespace MPinDemo
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class EmailConfirmed : Page, INotifyPropertyChanged
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

        public EmailConfirmed()
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
            if (e.Parameter != null)
            {
                this.User = e.Parameter as User;
            }
        }

        protected override void OnNavigatedFrom(NavigationEventArgs e)
        {
            base.OnNavigatedFrom(e);
        }

        #region Handlers
        private void Button_Click(object sender, RoutedEventArgs e)
        {
            System.Diagnostics.Debug.WriteLine("EmailConfirmed Finish..");
            Frame mainFrame = MainPage.Current.FindName("MainFrame") as Frame;
            mainFrame.GoBack(new List<object>() { "EmailConfirmed", "Finish" });
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
            mainFrame.GoBack(new List<object >() { "EmailConfirmed", string.Empty});
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
