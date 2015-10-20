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
using MPinDemo.Models;

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkID=390556

namespace MPinDemo
{
    /// <summary>
    /// A page displayed after an idendity has been added.
    /// </summary>
    public sealed partial class IdentityCreated : Page
    {
        User User { get; set; }
        Controller Controller { get; set; }

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
            List<object> data = e.Parameter as List<object>;
            if (data != null && data.Count == 2 
                && data[0].GetType().Equals(typeof(User)) 
                && data[1].GetType().Equals(typeof(Controller)))
            {
                this.User = data[0] as User;
                this.Controller = data[1] as Controller;
                IdentityMail.Text = this.User.Id;
            }
        }

        private async void SignInButton_Click(object sender, RoutedEventArgs e)
        {
            await Controller.ProcessNavigation("SignIn", this.User);      
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            Frame mainFrame = MainPage.Current.FindName("MainFrame") as Frame;
            mainFrame.GoBack("Identities");
        }
    }
}
