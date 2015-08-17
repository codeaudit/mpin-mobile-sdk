// Copyright (c) 2012-2015, Certivox
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// For full details regarding our CertiVox terms of service please refer to
// the following links:
//  * Our Terms and Conditions -
//    http://www.certivox.com/about-certivox/terms-and-conditions/
//  * Our Security and Privacy -
//    http://www.certivox.com/about-certivox/security-privacy/
//  * Our Statement of Position and Our Promise on Software Patents -
//    http://www.certivox.com/about-certivox/patents/

using MPinDemo.Models;
using MPinSDK.Common; // navigation extensions
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Runtime.CompilerServices;
using Windows.ApplicationModel.Resources;
using Windows.UI.Popups;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Navigation;

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkID=390556

namespace MPinDemo
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class ReadConfiguration : Page, INotifyPropertyChanged
    {
        #region Fields
        private MainPage rootPage = null;
        private static List<int> ExistentsIndexes;
        #endregion // Fields

        #region C'tor
        public ReadConfiguration()
        {
            this.InitializeComponent();
            this.DataContext = this;
        }
        #endregion // C'tor

        #region Members
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
        #endregion // Members

        #region Methods
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
                else if (BlankPage1.IsActiveConfigurationURLChanged(ExistentsIndexes, this.Configurations))
                {
                    var activeServiceConfirmation = new MessageDialog(ResourceLoader.GetForCurrentView().GetString("OverideActiveService"));
                    activeServiceConfirmation.Commands.Add(new UICommand(ResourceLoader.GetForCurrentView().GetString("YesCommand")));
                    activeServiceConfirmation.Commands.Add(new UICommand(ResourceLoader.GetForCurrentView().GetString("NoCommand")));
                    activeServiceConfirmation.DefaultCommandIndex = 1;
                    var res = await activeServiceConfirmation.ShowAsync();
                    if (res.Equals(activeServiceConfirmation.Commands[1]))
                    {
                        // if no set, back to the configurations list to select
                        return;
                    }
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
        #endregion // Methods
    }    
}
