using MPinSDK.Models;
using MPinSDK.Common; // navigation extensions
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using System.Threading;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;
using Windows.ApplicationModel.Resources;

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkID=390556

namespace MPinSDK.Controls
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    [Windows.Foundation.Metadata.WebHostHidden]
    sealed partial class PinPadPage : Page
    {
        PinPad pinPadClassControl;

        public PinPadPage()
        {
            this.InitializeComponent();
            this.PinPad.PinEntered += PinPad_PinEntered;
            Windows.Phone.UI.Input.HardwareButtons.BackPressed += HardwareButtons_BackPressed;
        }

        private void HardwareButtons_BackPressed(object sender, Windows.Phone.UI.Input.BackPressedEventArgs e)
        {
            string param = (Window.Current.Content as Frame).GetNavigationData() as string;
            if (e.Handled && param != null && param.Equals("HardwareBack"))
            {
                // need to set the IsEntered property so we could pass the pin (empty string in that case) as a result of the PinPad.Show method
                pinPadClassControl.Pin = string.Empty;
                if (!this.PinPad.IsEntered)
                    this.PinPad.IsEntered = true;
            }          
        }

        public User CurrentUser
        {
            get;
            set;
        }

        /// <summary>
        /// Invoked when this page is about to be displayed in a Frame.
        /// </summary>
        /// <param name="e">Event data that describes how this page was reached.
        /// This parameter is typically used to configure the page.</param>
        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            List<object> data = e.Parameter as List<object>;
            if (data != null && data.Count == 3)
            {
                pinPadClassControl = data[0] as PinPad;
                bool? doAuthenticate = data[1] as bool?;
                string userId = data[2].ToString();

                if (pinPadClassControl != null && doAuthenticate != null)
                {
                    PinMailTB.Text = ResourceLoader.GetForCurrentView("MPinSDK/Resources").GetString(doAuthenticate.Value ? "PinPadAuthentication" : "PinPadRegistration") + userId; 
                }
            }         
        }

        void PinPad_PinEntered(object sender, PinPadEventArgs e)
        {
            pinPadClassControl.Pin = e.Pin;
            Frame rootFrame = Window.Current.Content as Frame;
            rootFrame.GoBack("PinEntered");
        }

    }
}
