using MPinSDK.Common;
using System.Collections.Generic;
using Windows.ApplicationModel.Resources;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Navigation;

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkID=390556

namespace MPinDemo
{
    /// <summary>
    /// An page used for entering the Access Number of the identifier.
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
            List<string> data = e.Parameter as List<string>;
            if (data != null && data.Count == 2)
            {
                ANUser.Text = string.Format(ResourceLoader.GetForCurrentView().GetString("ANUser"), data[0]);

                this.ANLength = int.Parse(data[1]);
                this.AccessNumberLength.Text = string.Format(ResourceLoader.GetForCurrentView().GetString("AccessNumberLength"), this.ANLength);
                this.AccessNumberTB.MaxLength = this.ANLength;
            }
            
            //This code opens up the keyboard when you navigate to the page.
            this.AccessNumberTB.UpdateLayout();
            this.AccessNumberTB.Focus(FocusState.Keyboard);
        }

        private void Done_Click(object sender, RoutedEventArgs e)
        {
            ProcessAN();
        }

        private void ProcessAN()
        {
            Frame mainFrame = MainPage.Current.FindName("MainFrame") as Frame;
            mainFrame.GoBack(new List<object>() { "AccessNumber", this.AccessNumberTB.Text });
        }

        void AccessNumberTB_TextChanged(object sender, TextChangedEventArgs e)
        {
            this.DoneButton.IsEnabled = this.AccessNumberTB.Text.Length == this.ANLength;
        }

        private void AccessNumberTB_KeyUp(object sender, KeyRoutedEventArgs e)
        {
            if (this.DoneButton.IsEnabled && e.Key == Windows.System.VirtualKey.Enter)
            {
                ProcessAN();
            }
        }

    }
}
