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
    public sealed partial class AppQuide: Page
    {
        private MainPage rootPage = null;
        object passedParameters;

        public AppQuide()
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
            rootPage = MainPage.Current;
            this.passedParameters = e.Parameter;
        }

        private void AppBarButton_Click(object sender, RoutedEventArgs e)
        {   
            Frame mainFrame = rootPage.FindName("MainFrame") as Frame;            
            if (!mainFrame.Navigate(typeof(BlankPage1), passedParameters == null ? string.Empty : passedParameters))
            {
                throw new Exception("Failed to go to the initial screen.");
            }
        }

        private void NextButton_Click(object sender, RoutedEventArgs e)
        {
            IntroPivot.SelectedIndex++;
        }

        private void IntroPivot_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            bool isLastItem = IntroPivot.SelectedIndex == IntroPivot.Items.Count - 1;
            NextButton.Visibility = SkipButton.Visibility = !isLastItem ? Visibility.Visible : Visibility.Collapsed;
            DoneButton.Visibility = isLastItem ? Visibility.Visible : Visibility.Collapsed;
        }
    }
}
