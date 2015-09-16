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

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkID=390556

namespace MPinDemo
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class AccessNumberQuide : Page
    {
        //private MainPage rootPage = null;
        private BlankPage1 currentPage = null;

        public AccessNumberQuide()
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
            //rootPage = MainPage.Current;
            currentPage = e.Parameter as BlankPage1;
            if (currentPage == null)
                throw new ArgumentException("The navigated page should be passed for proper navigation further!");
        }

        private void AppBarButton_Click(object sender, RoutedEventArgs e)
        {
            currentPage.Select();
            //Frame mainFrame = rootPage.FindName("MainFrame") as Frame;
            //if (!mainFrame.Navigate(typeof(BlankPage1), string.Empty))
            //{
            //    throw new Exception("Failed to go to the initial screen.");
            //}
        }
    }
}
