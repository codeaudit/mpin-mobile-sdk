using MPinDemo.Models;
using MPinSDK.Common; // navigation extensions
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.ApplicationModel.Resources;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI;
using Windows.UI.Popups;
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
    public sealed partial class ReadConfiguration : Page, INotifyPropertyChanged
    {
        MainPage rootPage = null;
        private List<int> existentsIndexes;

        public ReadConfiguration()
        {
            this.InitializeComponent();
            this.DataContext = this;
        }

        private List<Backend> configurationList;
        public List<Backend> ConfigurationList
        {
            get
            {
                return this.configurationList;
            }
            set
            {
                this.configurationList = value;
                OnPropertyChanged();
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
            List<object> data = e.Parameter as List<object>;            
            if (data == null || data.Count != 2 || !data[0].GetType().Equals(typeof(List<Backend>)) || !data[1].GetType().Equals(typeof(List<int>)))
            {
                rootPage.NotifyUser(ResourceLoader.GetForCurrentView().GetString("InvalidConfigurationList"), MainPage.NotifyType.ErrorMessage);
                return;
            }

            this.ConfigurationList = data[0] as List<Backend>;
            this.existentsIndexes = data[1] as List<int>;

            CheckAllConfigurations(true);
            MarkExistents();
        }

        private void MarkExistents()
        {
            //foreach(var i in existentsIndexes)
            //{
            //    this.ConfigurationList[i].
            //}
        }

        private void CheckAllConfigurations(bool isCheck)
        {
            foreach(var backend in this.ConfigurationList)
            {
                backend.IsSet = isCheck;
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

        private async void SaveAppBarButton_Click(object sender, RoutedEventArgs e)
        {
            bool areDuplicatesSelected = AreDuplicatesSelected();
            this.ConfigurationList.RemoveAll((item) => item.IsSet == false);
            CheckAllConfigurations(false);
            Frame mainFrame = MainPage.Current.FindName("MainFrame") as Frame;

            if (areDuplicatesSelected)
            {
                var confirmation = new MessageDialog(ResourceLoader.GetForCurrentView().GetString("OverideDiplicates"));
                confirmation.Commands.Add(new UICommand(ResourceLoader.GetForCurrentView().GetString("YesCommand")));
                confirmation.Commands.Add(new UICommand(ResourceLoader.GetForCurrentView().GetString("NoCommand")));
                confirmation.DefaultCommandIndex = 1;
                var result = await confirmation.ShowAsync();
                if (result.Equals(confirmation.Commands[1]))
                {
                    // if no set, back to the configurations list to select
                    //mainFrame.GoBack(new List<object>() { "NewConfigurations", this.ConfigurationList });   
                    return;
                }
            }

            mainFrame.GoBack(new List<object>() { "NewConfigurations", this.ConfigurationList });
        }

        private bool AreDuplicatesSelected()
        {
            return this.ConfigurationList.Any(item => item.IsSet == true && this.existentsIndexes.Contains(this.ConfigurationList.IndexOf(item)));
        }

        private void ListBox_Loaded(object sender, RoutedEventArgs e)
        {
            if (this.ConfigurationsListBox.Items == null || this.ConfigurationsListBox.Items.Count == 0)
                return;

            foreach(var i in existentsIndexes)
            {
                (this.ConfigurationsListBox.Items[i] as ListBoxItem).Foreground = new SolidColorBrush(Colors.Red);
            }
        }

    }
}
