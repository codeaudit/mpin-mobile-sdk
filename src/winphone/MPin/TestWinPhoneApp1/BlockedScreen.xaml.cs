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
using MPinSDK.Common;
using MPinSDK.Models;
using Windows.ApplicationModel.Resources;

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkID=390556

namespace MPinDemo
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class BlockedScreen : Page
    {
        public BlockedScreen()
        {
            this.InitializeComponent();
        }

        User User
        {
            get;
            set;
        }

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

        private void GoIdentities_Click(object sender, RoutedEventArgs e)
        {
            System.Diagnostics.Debug.WriteLine(" BlockedUser -> Identities");
            Frame mainFrame = MainPage.Current.FindName("MainFrame") as Frame;
            mainFrame.GoBack(new List<object>() { "BlockedUser", string.Empty });
        }

        private void RemoveUserButton_Click(object sender, RoutedEventArgs e)
        {
            System.Diagnostics.Debug.WriteLine(" BlockedUser -> Remove User");
            Frame mainFrame = MainPage.Current.FindName("MainFrame") as Frame;
            mainFrame.GoBack(new List<object>() { "BlockedUser", "Remove" });
        }

        private void ResetPinButton_Click(object sender, RoutedEventArgs e)
        {
            System.Diagnostics.Debug.WriteLine(" BlockedUser -> Reset PIN");
            Frame mainFrame = MainPage.Current.FindName("MainFrame") as Frame;
            mainFrame.GoBack(new List<object>() { "BlockedUser", "ResetPIN" });                 
        }
    }
}
