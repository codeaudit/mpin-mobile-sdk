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
