using MPinSDK.Models;
using MPinSDK.Common;
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
                if (pinPadClassControl != null && doAuthenticate != null)
                {
                    PinMailTB.Text = (doAuthenticate.Value ? "Authenticate user " : "Register user ") + data[2].ToString();
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
