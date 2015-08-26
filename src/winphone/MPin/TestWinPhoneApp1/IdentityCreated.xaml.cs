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

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkID=390556

namespace MPinDemo
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class IdentityCreated : Page
    {
        User User { get; set; }

        public IdentityCreated()
        {
            this.InitializeComponent();
        }

        /// <summary>
        /// Invoked when this page is about to be displayed in a Frame.
        /// </summary>
        /// <param name="e">Event data that describes how this page was reached.
        /// This parameter is typically used to configure the page.</param>
        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            if (e.Parameter != null && e.Parameter.GetType().Equals(typeof(User)))
            {
                this.User = e.Parameter as User;
                IdentityMail.Text = this.User.Id;
            }
        }

        private void SignInButton_Click(object sender, RoutedEventArgs e)
        {
            Frame mainFrame = MainPage.Current.FindName("MainFrame") as Frame;
            mainFrame.GoBack(new List<object>() { "SignIn", this.User });            
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            Frame mainFrame = MainPage.Current.FindName("MainFrame") as Frame;
            mainFrame.GoBack(string.Empty);
        }
    }
}
