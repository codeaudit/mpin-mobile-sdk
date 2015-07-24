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
        private static List<int> ExistentsIndexes;

        public ReadConfiguration()
        {
            this.InitializeComponent();
            this.DataContext = this;
        }

        private static List<Backend> ConfigurationList;
        public List<Backend> Configurations
        {
            get
            {
                return ConfigurationList;
            }
            set
            {
                ConfigurationList = value;
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
                if (ConfigurationList != null)                
                    Configurations.Clear();                
                if (ExistentsIndexes != null)
                    ExistentsIndexes.Clear();
                return;
            }

            this.Configurations = data[0] as List<Backend>;
            ExistentsIndexes = data[1] as List<int>;

            CheckAllConfigurations(true);            
        }
        

        private void CheckAllConfigurations(bool isCheck)
        {
            foreach (var backend in this.Configurations)
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
                    return;
                }
            }

            this.Configurations.RemoveAll((item) => item.IsSet == false);
            CheckAllConfigurations(false);
            Frame mainFrame = MainPage.Current.FindName("MainFrame") as Frame;
            mainFrame.GoBack(new List<object>() { "NewServices", this.Configurations });
        }

        private bool AreDuplicatesSelected()
        {
            return Configurations.Any(item => item.IsSet == true && ExistentsIndexes.Contains(this.Configurations.IndexOf(item)));
        }
               
        public static bool IsDuplicate(Backend item)
        {
            return ExistentsIndexes.Contains(ConfigurationList.IndexOf(item));
        }

    }


    public abstract class DataTemplateSelector : ContentControl
    {
        public virtual DataTemplate SelectTemplate(object item, DependencyObject container)
        {
            return null;
        }

        protected override void OnContentChanged(object oldContent, object newContent)
        {
            base.OnContentChanged(oldContent, newContent);

            ContentTemplate = SelectTemplate(newContent, this);
        }
    }

    public class ExistenceSelector : DataTemplateSelector
    {
        public DataTemplate UniqueTemplate
        {
            get;
            set;
        }
        public DataTemplate DuplicateTemplate
        {
            get;
            set;
        }

        public override DataTemplate SelectTemplate(object item, DependencyObject container)
        {
            Backend backendItem = item as Backend;
            if (backendItem != null)
            {
                if (ReadConfiguration.IsDuplicate(backendItem))
                {
                    return DuplicateTemplate;
                }
                else
                {
                    return UniqueTemplate;
                }
            }

            return base.SelectTemplate(item, container);
        }
    }
}
