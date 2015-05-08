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
using Windows.ApplicationModel.Resources;

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkID=390556

namespace MPinDemo
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class AccessNumberScreen : Page
    {
        public int ANLength
        {
            get;
            set;
        }

        public AccessNumberScreen()
        {
            this.InitializeComponent();
            InputScope scope = new InputScope();
            InputScopeName name = new InputScopeName();

            name.NameValue = InputScopeNameValue.Number;
            scope.Names.Add(name);

            this.AccessNumberTB.InputScope = scope;
        }

        /// <summary>
        /// Invoked when this page is about to be displayed in a Frame.
        /// </summary>
        /// <param name="e">Event data that describes how this page was reached.
        /// This parameter is typically used to configure the page.</param>
        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            if (e.Parameter != null)
            {
                this.ANLength = int.Parse(e.Parameter.ToString());
                this.AccessNumberLength.Text = string.Format(ResourceLoader.GetForCurrentView().GetString("AccessNumberLength"), this.ANLength);
                this.AccessNumberTB.MaxLength = this.ANLength;
            }

            //This code opens up the keyboard when you navigate to the page.
            this.AccessNumberTB.UpdateLayout();
            this.AccessNumberTB.Focus(FocusState.Keyboard);
        }

        private void Done_Click(object sender, RoutedEventArgs e)
        {
            Frame mainFrame = MainPage.Current.FindName("MainFrame") as Frame;
            mainFrame.GoBack(new List<string>() { "AccessNumber", this.AccessNumberTB.Text});
        }

        void AccessNumberTB_TextChanged(object sender, TextChangedEventArgs e)
        {
            this.DoneButton.IsEnabled = this.AccessNumberTB.Text.Length == this.ANLength;
        }

    }
}
