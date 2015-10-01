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

using MPinSDK.Common;
using System.Collections.Generic;
using Windows.ApplicationModel.Resources;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Navigation;
using System.Linq;
using System;

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
            
            this.AccessNumber.InputScope = scope;
        }

        /// <summary>
        /// Invoked when this page is about to be displayed in a Frame.
        /// </summary>
        /// <param name="e">Event data that describes how this page was reached.
        /// This parameter is typically used to configure the page.</param>
        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            List<string> data = e.Parameter as List<string>;
            if (data != null && data.Count == 4)
            {
                ANUser.Text = data[0].ToString();
                ANUrl.Text = RemoveHTTP(data[2].ToString());
                ANName.Text = data[3].ToString();

                this.ANLength = int.Parse(data[1]);
                this.AccessNumber.MaxLength = this.ANLength;
                this.AccessNumberTB.Text = string.Format(ResourceLoader.GetForCurrentView().GetString("ANTB"), this.ANLength);
            }

            //This code opens up the keyboard when you navigate to the page.
            this.ANReadOnly.UpdateLayout();
            this.ANReadOnly.Focus(FocusState.Keyboard);

            ClearBackStack();
        }

        private string RemoveHTTP(string link)
        {
            return link.Remove(0, link.IndexOf("://") + 3);
        }

        private void ClearBackStack()
        {
            ClearFrame(typeof(AccessNumberQuide));
            ClearFrame(typeof(IdentityCreated));
        }

        private void ClearFrame(Type typeToRemove)
        {
            Frame mainFrame = MainPage.Current.FindName("MainFrame") as Frame;
            if (mainFrame != null && mainFrame.BackStack.Any(item => item.SourcePageType.Equals(typeToRemove)))
            {
                mainFrame.BackStack.Remove(mainFrame.BackStack.First(item => item.SourcePageType.Equals(typeToRemove)));
            }
        }

        private void Done_Click(object sender, RoutedEventArgs e)
        {
            ProcessAN();
        }

        private void ProcessAN()
        {
            Frame mainFrame = MainPage.Current.FindName("MainFrame") as Frame;
            mainFrame.GoBack(new List<object>() { "AccessNumber", this.AccessNumber.Text });
        }

        void AccessNumberTB_TextChanged(object sender, TextChangedEventArgs e)
        {
            this.DoneButton.IsEnabled = this.AccessNumber.Text.Length == this.ANLength;
            this.ANReadOnly.Text = this.AccessNumber.Text;
        }

        private void AccessNumberTB_KeyUp(object sender, KeyRoutedEventArgs e)
        {
            if (this.DoneButton.IsEnabled && e.Key == Windows.System.VirtualKey.Enter)
            {
                ProcessAN();
            }
        }

        private void ANReadOnly_GotFocus(object sender, RoutedEventArgs e)
        {
            this.AccessNumber.UpdateLayout();
            this.AccessNumber.Focus(FocusState.Keyboard);
        }

        private void AccessNumber_LostFocus(object sender, RoutedEventArgs e)
        {
            VisualStateManager.GoToState(this.ANReadOnly, "Normal", true);
        }

        private void AccessNumber_GotFocus(object sender, RoutedEventArgs e)
        {
            VisualStateManager.GoToState(this.ANReadOnly, "Focused", true);
        }

    }
}
