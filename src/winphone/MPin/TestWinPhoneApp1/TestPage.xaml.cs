using MPinDemo.Models;
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
using MPinSDK.Common; // navigation extensions

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkID=390556

namespace MPinDemo
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class TestPage : Page
    {
        public TestPage()
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
            
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            CreateBackends();

            Frame mainFrame = MainPage.Current.FindName("MainFrame") as Frame;
            //mainFrame.GoBack();
            mainFrame.GoBack(new List<object>() { "Service", "" });
        }

        /// <summary>
        /// Creates the backends.
        /// </summary>
        private void CreateBackends()
        {
            string i = "";

            MPinDemo.Models.Controller controller = BlankPage1.controller;

            controller.DataModel.BackendsList.Add(new Backend()
            {
                BackendUrl = "https://m-pindemo.certivox.org",
                Type = ConfigurationType.Mobile,
                Name = "Basic" + i
            });

            controller.DataModel.BackendsList.Add(new Backend()
            {
                BackendUrl = "http://ec2-52-28-120-46.eu-central-1.compute.amazonaws.com",
                Type = ConfigurationType.Mobile,
                Name = "Force Activation" + i
            });

            controller.DataModel.BackendsList.Add(new Backend()
            {
                BackendUrl = "https://mpindemo-qa-v3.certivox.org",
                Type = ConfigurationType.Online,
                Name = "Bank service" + i
            });

            controller.DataModel.BackendsList.Add(new Backend()
            {
                BackendUrl = "http://otp.m-pin.id/rps",
                Type = ConfigurationType.OTP,
                Name = "Longest Journey Service" + i
            });

            controller.DataModel.BackendsList.Add(new Backend()
            {
                BackendUrl = "http://risso.certivox.org/",
                Type = ConfigurationType.OTP,
                Name = "OTP login" + i
            });

            controller.DataModel.BackendsList.Add(new Backend()
            {
                BackendUrl = "http://ntt-vpn.certivox.org",
                Type = ConfigurationType.OTP,
                Name = "OTP NTT login" + i
            });

            controller.DataModel.BackendsList.Add(new Backend()
            {
                BackendUrl = "http://tcb.certivox.org",
                Type = ConfigurationType.Mobile,
                Name = "Mobile banking login" + i
            });

            controller.DataModel.BackendsList.Add(new Backend()
            {
                BackendUrl = "http://tcb.certivox.org",
                Type = ConfigurationType.Online,
                Name = "Online banking login" + i
            });

            controller.DataModel.BackendsList.Add(new Backend()
            {
                BackendUrl = "http://otp.m-pin.id",
                Type = ConfigurationType.OTP,
                Name = "VPN login" + i
            });
        }

        private void Button_Click_1(object sender, RoutedEventArgs e)
        {
            BlankPage1.controller.DataModel.BackendsList.Clear();            
        }

        private void Button_Click_2(object sender, RoutedEventArgs e)
        {
            Frame mainFrame = MainPage.Current.FindName("MainFrame") as Frame;
            mainFrame.GoBack(new List<object>() { "Service", "" });

        }
    }
}
