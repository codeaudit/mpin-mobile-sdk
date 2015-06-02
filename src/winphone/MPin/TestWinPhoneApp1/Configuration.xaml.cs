using MPinSDK.Common; // navigation extensions
using MPinDemo.Models;
using System;
using System.Collections.Generic;
using System.ComponentModel;
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
using System.Runtime.CompilerServices;
using MPinSDK.Models;
using Windows.ApplicationModel.Resources;
using Windows.UI.Popups;

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkID=390556

namespace MPinDemo
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class Configuration : Page, INotifyPropertyChanged
    {
        bool isAdding, isUrlChanged;
        MainPage rootPage = null;

        public Configuration()
        {
            this.InitializeComponent();
        }

        private Backend backend;
        public Backend Backend
        {
            get
            {
                return this.backend;
            }
            set
            {
                if (this.backend != value)
                {
                    this.backend = value;
                    OnPropertyChanged();
                }
            }
        }

        /// <summary>
        /// Invoked when this page is about to be displayed in a Frame.
        /// </summary>
        /// <param name="e">Event data that describes how this page was reached.
        /// This parameter is typically used to configure the page.</param>
        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            rootPage = MainPage.Current;
            this.isAdding = e.Parameter == null;
            this.Backend = isAdding ? new Backend() : e.Parameter as Backend;
            this.backend.PropertyChanged += backend_PropertyChanged;

            if (this.Backend == null || (!this.Backend.RequestAccessNumber && !this.Backend.RequestOtp))
            {
                MobileLoginRadioButton.IsChecked = true;
            }

            RegisterService.Content = ResourceLoader.GetForCurrentView().GetString(isAdding ? "AddService" : "EditService");
        }

        void backend_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            if (e.PropertyName == "BackendUrl")
            {
                this.isUrlChanged = true;
            }
        }

        private async void Button_Click(object sender, RoutedEventArgs e)
        {
            if (!string.IsNullOrEmpty(this.Backend.Title) && Uri.IsWellFormedUriString(this.Backend.BackendUrl, UriKind.Absolute))
            {
                if (!isAdding && isUrlChanged)
                {
                    var confirmation = new MessageDialog(ResourceLoader.GetForCurrentView().GetString("LostUsers"));
                    confirmation.Commands.Add(new UICommand(ResourceLoader.GetForCurrentView().GetString("YesCommand")));
                    confirmation.Commands.Add(new UICommand(ResourceLoader.GetForCurrentView().GetString("NoCommand")));
                    confirmation.DefaultCommandIndex = 1;
                    var result = await confirmation.ShowAsync();
                    if (result.Equals(confirmation.Commands[0]))
                    {
                        Frame.GoBack(new List<object>() { "EditService", this.Backend });
                    }
                }
                else
                {
                    Frame.GoBack(new List<object>() { isAdding ? "AddService" : "EditService", this.Backend });
                }
            }
            else
            {
                rootPage.NotifyUser(string.IsNullOrEmpty(this.Backend.Title) ? ResourceLoader.GetForCurrentView().GetString("EmptyTitle") : ResourceLoader.GetForCurrentView().GetString("WrongURL"), MainPage.NotifyType.ErrorMessage);
            }
        }

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

        private async void Test_Click(object sender, RoutedEventArgs e)
        {
            TestBackendButton.IsEnabled = false;
            Status status = await Controller.TestBackend(this.Backend);
            rootPage.NotifyUser(ResourceLoader.GetForCurrentView().GetString("ServiceStatus") + status.StatusCode, status.StatusCode != 0 ? MainPage.NotifyType.ErrorMessage : MainPage.NotifyType.StatusMessage);
            TestBackendButton.IsEnabled = true;
        }

        private void TextBox_KeyUp(object sender, KeyRoutedEventArgs e)
        {
            TextBox tb = sender as TextBox;
            if (tb != null && (e.Key == Windows.System.VirtualKey.Enter || e.Key == Windows.System.VirtualKey.Tab))
            {
                if (tb.Equals(NameTB))
                {
                    UrlTB.Focus(FocusState.Keyboard);
                }
                else if (tb.Equals(UrlTB))
                {
                    RpsTB.Focus(FocusState.Keyboard);
                }
                else
                {
                    MobileLoginRadioButton.Focus(FocusState.Keyboard);
                }
            }
        }
    }
}
