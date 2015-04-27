using MPinSDK.Models;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace TestWinPhoneApp1.Models
{
    public class AppDataModel : INotifyPropertyChanged // DependencyObject,
    {
        #region Members
        public List<Backend> BackendsList;

        //public User CurrentUser
        //{
        //    get { return (User)GetValue(CurrentUserProperty); }
        //    set { SetValue(CurrentUserProperty, value); }
        //}

        //// Using a DependencyProperty as the backing store for CurrentUser.  This enables animation, styling, binding, etc...
        //public static readonly DependencyProperty CurrentUserProperty =
        //    DependencyProperty.Register("CurrentUser", typeof(User), typeof(BlankPage1), new PropertyMetadata(null, CurrentUserChanged));

        //private static void CurrentUserChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        //{

        //}

        #region CurrentUser
        static User _currentUser;
        public User CurrentUser
        {
            get
            {
                return _currentUser;
            }
            set
            {
                _currentUser = value;
                this.OnPropertyChanged();
            }
        }
        #endregion // CurrentUser

        #region CurrentService
        static Backend _currentService;
        public Backend CurrentService
        {
            get
            {
                return _currentService;
            }
            set
            {
                _currentService = value;
                this.OnPropertyChanged();
            }
        }

        #endregion // CurrentService

        #endregion

        #region C'tor
        public AppDataModel()
        {
            CreateBackends();
        }
        #endregion

        #region Methods
        private void CreateBackends()
        {
            //Backend backends[] = 
            //{
            //    {"https://m-pindemo.certivox.org"},
            //    {"http://ec2-54-77-232-113.eu-west-1.compute.amazonaws.com", "/rps/"},
            //    {"https://mpindemo-qa-v3.certivox.org", "rps"},
            //};
            //TODO:: leave only the last three services
            BackendsList = new List<Backend>();
            BackendsList.Add(new Backend()
            {
                BackendUrl = "https://m-pindemo.certivox.org",
                RequestAccessNumber = false,
                RequestOtp = false,
                Title = "Basic"
            });

            BackendsList.Add(new Backend()
            {
                BackendUrl = "http://ec2-54-77-232-113.eu-west-1.compute.amazonaws.com",
                RequestAccessNumber = false,
                RequestOtp = false,
                Title = "M-Pin Connect"
            });

            BackendsList.Add(new Backend()
            {
                BackendUrl = "https://mpindemo-qa-v3.certivox.org",
                RequestAccessNumber = true,
                RequestOtp = false,
                Title = "Bank service"
            });

            BackendsList.Add(new Backend()
            {
                BackendUrl = "http://otp.m-pin.id/rps",
                RequestAccessNumber = false,
                RequestOtp = true,
                Title = "Longest Journey Service"
            });

            BackendsList.Add(new Backend()
            {
                BackendUrl = "http://risso.certivox.org/",
                RequestAccessNumber = false,
                RequestOtp = true,
                Title = "OTP login"
            });

            BackendsList.Add(new Backend()
            {
                BackendUrl = "ntt-vpn.certivox.org",
                RequestAccessNumber = false,
                RequestOtp = true,
                Title = "OTP NTT login"
            });

            BackendsList.Add(new Backend()
            {
                BackendUrl = "http://tcb.certivox.org",
                RequestAccessNumber = false,
                RequestOtp = false,
                Title = "Mobile banking login"
            });

            BackendsList.Add(new Backend()
            {
                BackendUrl = "http://tcb.certivox.org",
                RequestAccessNumber = true,
                RequestOtp = false,
                Title = "Online banking login"
            });

            BackendsList.Add(new Backend()
            {
                BackendUrl = "http://otp.m-pin.id",
                RequestAccessNumber = false,
                RequestOtp = true,
                Title = "VPN login"
            });

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
