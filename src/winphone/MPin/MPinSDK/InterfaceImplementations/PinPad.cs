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

using MPinRC;
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
        private static readonly object LockObject = new object();
        static CountdownEvent countDownEvent = new CountdownEvent(1);
        private CoreDispatcher Dispatcher { get; set; }

        internal string Pin = string.Empty;
        #endregion // Members

        #region IPinPad

        public void SetUiDispatcher(CoreDispatcher dispatcher)
        {
            this.Dispatcher = dispatcher;
            UIDispatcher.Initialize(dispatcher);
        }

        public string Show(UserWrapper user, Mode mode)
        {
            this.Pin = string.Empty;
            lock (LockObject)
            {
                countDownEvent = new CountdownEvent(1);
                Task.Run(async () => { await DoAll(user, mode); }).Wait();
            }

            return this.Pin;
        }

        private async Task DoAll(UserWrapper user, Mode mode)
        {
            await DisplayPinPadAsync(user, mode);
            Task.WaitAll();
            await Task.Delay(2000);
            await TakePinPad();
            countDownEvent.Wait();
        }

        public async Task DisplayPinPadAsync(UserWrapper user, Mode mode)
        {
            UIDispatcher.Initialize(this.Dispatcher);
            await ThreadPool.RunAsync(operation => UIDispatcher.Execute(() =>
            {
                Frame rootFrame = Window.Current.Content as Frame;                
                rootFrame.Navigate(typeof(PinPadPage), new List<object> { this, mode == Mode.AUTHENTICATE, user == null ? string.Empty : user.GetId() });
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
                countDownEvent.Signal();
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
