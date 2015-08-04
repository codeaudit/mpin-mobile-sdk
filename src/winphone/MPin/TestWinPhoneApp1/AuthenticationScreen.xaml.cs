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
