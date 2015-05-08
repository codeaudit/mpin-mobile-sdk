﻿using MPinRC;
using MPinSDK.Controls;
using MPinSDK.Models;
using MPinSDK.Common;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using Windows.Foundation;
using Windows.System.Threading;
using Windows.UI.Core;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Media;
using System.Reflection;
using System.Runtime.InteropServices.WindowsRuntime;

namespace MPinSDK
{
    /// <summary>
    /// A class implementing the programming interface for the PIN Pad UI. The PIN Pad UI is invoked by the Core when the platform doesn't support TEE. When TEE is present, it might/will drive the UI itself in its secure environment and outside Android.
    /// </summary>
    class PinPad : IPinPad
    {
        #region Members
        private PinPadControl ctrl = null;
        private int getAttemptCount = 0;
        private ManualResetEvent mre;
        private CoreDispatcher Dispatcher { get; set; }

        internal string Pin = string.Empty;
        #endregion // Members 

        #region IPinPad

        public void SetUiDispatcher(CoreDispatcher dispatcher)
        {
            this.Dispatcher = dispatcher;
            UIDispatcher.Initialize(dispatcher);
        }

        public string Show()
        {
            this.Pin = string.Empty;
            mre = new ManualResetEvent(false);

            Task.Run(async () => { await DoAll(); }).Wait();
            
            return this.Pin;
        }
   
        private async Task DoAll()
        {
            await DisplayPinPadAsync();
            Task.WaitAll();
            await Task.Delay(2000);
            await TakePinPad();
            mre.WaitOne();
        }

        public async Task DisplayPinPadAsync()
        {
            UIDispatcher.Initialize(this.Dispatcher);
            await ThreadPool.RunAsync(operation => UIDispatcher.Execute(() =>
            {
                Frame rootFrame = Window.Current.Content as Frame;
                rootFrame.Navigate(typeof(PinPadPage), this);
                Window.Current.Activate();
            }));
        }
          
        private async Task TakePinPad()
        {
            await ThreadPool.RunAsync(operation => UIDispatcher.Execute(() =>
                {
                    Frame frame = Window.Current.Content as Frame;
                    this.getAttemptCount = 0;
                    ctrl = LookForPinPad(frame);
                    if (ctrl != null)
                    {
                        ctrl.PropertyChanged += ctrl_PropertyChanged;
                    }
                }));
        }

        void ctrl_PropertyChanged(object sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            if (e.PropertyName == "IsEntered")
                mre.Set();
        }

        private PinPadControl LookForPinPad(Frame frame)
        {
            ctrl = FindPinPad(frame);
            if (ctrl == null && getAttemptCount++ < 10)
            {
                Task.Delay(500);
                ctrl = LookForPinPad(frame);
            }

            return ctrl;
        }
              
        private PinPadControl FindPinPad(DependencyObject startNode)
        {
            int count = VisualTreeHelper.GetChildrenCount(startNode);
            PinPadControl result = null;
            for (int i = 0; i < count; i++)
            {
                DependencyObject current = VisualTreeHelper.GetChild(startNode, i);
                if ((current.GetType()).Equals(typeof(PinPadControl)) || (current.GetType().GetTypeInfo().IsSubclassOf(typeof(PinPadControl))))
                {
                    result = (PinPadControl)current;
                    return result;
                }

                result = FindPinPad(current);
            }

            return result;
        }
       
        #endregion // IPinPad
    }    
}
