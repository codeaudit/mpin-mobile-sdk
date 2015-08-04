using MPinDemo.Models;
using MPinSDK.Common;
using MPinSDK.Models;
using System.Collections.Generic;
using System.ComponentModel;
using System.Runtime.CompilerServices;
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
