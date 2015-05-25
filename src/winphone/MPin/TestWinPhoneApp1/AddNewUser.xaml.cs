using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using System.Threading.Tasks;
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
using System.Text.RegularExpressions;
using Windows.ApplicationModel.Resources;
using MPinDemo.Models;

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkID=390556

namespace MPinDemo
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class AddNewUser : Page
    {
        MainPage rootPage = null;

        public AddNewUser()
        {
            this.InitializeComponent();
            InputScope scope = new InputScope();
            InputScopeName name = new InputScopeName();

            name.NameValue = InputScopeNameValue.EmailSmtpAddress;
            scope.Names.Add(name);

            this.UserId.InputScope = scope;
        }

        /// <summary>
        /// Invoked when this page is about to be displayed in a Frame.
        /// </summary>
        /// <param name="e">Event data that describes how this page was reached.
        /// This parameter is typically used to configure the page.</param>
        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            rootPage = MainPage.Current;
        }

        public string eMail
        { 
            get;
            set;
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            if (Controller.IfUserExists(this.UserId.Text))
            {
                rootPage.NotifyUser(ResourceLoader.GetForCurrentView().GetString("ExistingUser"), MainPage.NotifyType.ErrorMessage);
            }
            else if (!IsMailValid(this.UserId.Text))
            {
                rootPage.NotifyUser(ResourceLoader.GetForCurrentView().GetString("NotValidMail"), MainPage.NotifyType.ErrorMessage);
            }
            else
            {
                Frame mainFrame = rootPage.FindName("MainFrame") as Frame;
                mainFrame.GoBack(new List<string>() { "AddUser", this.UserId.Text });
            }
        }


        private bool IsMailValid(string mailString)
        {
            if (String.IsNullOrEmpty(mailString))
                return false;

            try
            {
                return Regex.IsMatch(mailString,
                      @"^(?("")("".+?(?<!\\)""@)|(([0-9a-z]((\.(?!\.))|[-!#\$%&'\*\+/=\?\^`\{\}\|~\w])*)(?<=[0-9a-z])@))" +
                      @"(?(\[)(\[(\d{1,3}\.){3}\d{1,3}\])|(([0-9a-z][-\w]*[0-9a-z]*\.)+[a-z0-9][\-a-z0-9]{0,22}[a-z0-9]))$",
                      RegexOptions.IgnoreCase, TimeSpan.FromMilliseconds(250));
            }
            catch (RegexMatchTimeoutException)
            {
                return false;
            }
        }
    }
}
