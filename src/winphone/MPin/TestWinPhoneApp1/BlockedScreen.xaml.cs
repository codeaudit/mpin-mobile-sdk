using MPinSDK.Common;
using MPinSDK.Models;
using System.Collections.Generic;
using Windows.ApplicationModel.Resources;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Navigation;

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkID=390556

namespace MPinDemo
{
    /// <summary>
    /// An page to display that a user is in a Blocked state.
    /// </summary>
    public sealed partial class BlockedScreen : Page
    {
        #region C'tor
        /// <summary>
        /// Initializes a new instance of the <see cref="BlockedScreen"/> class.
        /// </summary>
        public BlockedScreen()
        {
            this.InitializeComponent();
        }
        #endregion // C'tor

        #region Members
        private User User
        {
            get;
            set;
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
            List<object> data = e.Parameter as List<object>;
            if (data != null)
            {
                this.User = data[0] as User;
                if (this.User != null)
                {
                    BlockedUesrTB.Text = string.Format(ResourceLoader.GetForCurrentView().GetString("BlockedUser"), this.User.Id);
                }
            }
        }
        #endregion // Overrides

        #region Methods
        private void GoIdentities_Click(object sender, RoutedEventArgs e)
        {
            Frame mainFrame = MainPage.Current.FindName("MainFrame") as Frame;
            mainFrame.GoBack(new List<object>() { "BlockedUser", string.Empty });
        }

        private void RemoveUserButton_Click(object sender, RoutedEventArgs e)
        {
            Frame mainFrame = MainPage.Current.FindName("MainFrame") as Frame;
            mainFrame.GoBack(new List<object>() { "BlockedUser", "Remove" });
        }

        private void ResetPinButton_Click(object sender, RoutedEventArgs e)
        {
            Frame mainFrame = MainPage.Current.FindName("MainFrame") as Frame;
            mainFrame.GoBack(new List<object>() { "BlockedUser", "ResetPIN" });
        }
        #endregion // Methods
    }
}
