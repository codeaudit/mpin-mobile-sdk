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

using HockeyApp;
using System;
using Windows.ApplicationModel;
using Windows.ApplicationModel.Activation;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Media.Animation;
using Windows.UI.Xaml.Navigation;

// The Blank Application template is documented at http://go.microsoft.com/fwlink/?LinkId=391641

namespace MPinDemo
{
    /// <summary>
    /// Provides application-specific behavior to supplement the default Application class.
    /// </summary>
    public sealed partial class App : Application
    {
        private TransitionCollection transitions;
        
        /// <summary>
        /// Initializes the singleton application object.  This is the first line of authored code
        /// executed, and as such is the logical equivalent of main() or WinMain().
        /// </summary>
        public App()
        {
            this.InitializeComponent();
            this.Suspending += this.OnSuspending;
            this.Resuming += this.OnResuming;
            HockeyClient.Current.Configure("584408f872a0f7e10991ddb9954b3eb3");
        }
                
        //internal static Frame RootFrame;
        /// <summary>
        /// Invoked when the application is launched normally by the end user.  Other entry points
        /// will be used when the application is launched to open a specific file, to display
        /// search results, and so forth.
        /// </summary>
        /// <param name="e">Details about the launch request and process.</param>
        protected async override void OnLaunched(LaunchActivatedEventArgs e)
        {
#if DEBUG
            if (System.Diagnostics.Debugger.IsAttached)
            {
                this.DebugSettings.EnableFrameRateCounter = true;
            }
#endif
            Frame rootFrame = CreateRootFrame();

            // When the navigation stack isn't restored navigate to the first page,
            // configuring the new page by passing required information as a navigation
            // parameter
            if (rootFrame != null && !rootFrame.Navigate(typeof(MainPage), e.Arguments))
            {
                throw new Exception("Failed to create initial page");
            }

            //RootFrame = rootFrame;
            // Ensure the current window is active
            Window.Current.Activate();

            await HockeyClient.Current.SendCrashesAsync();
#if WINDOWS_PHONE_APP
            await HockeyClient.Current.CheckForAppUpdateAsync();
#endif
        }

        protected override void OnActivated(IActivatedEventArgs args)
        {
            base.OnActivated(args);
            if (args.Kind == ActivationKind.Protocol)
            {
                ProtocolActivatedEventArgs protocolArgs = args as ProtocolActivatedEventArgs;

                // TODO: Handle URI activation
                // The received URI is eventArgs.Uri.AbsoluteUri

                Frame rootFrame = CreateRootFrame();
                if (rootFrame.Content == null)
                {
                    if (!rootFrame.Navigate(typeof(MainPage)))
                    {
                        throw new Exception("Failed to create initial page");
                    }
                }

                //var p = rootFrame.Content as UriAssociations;
                ////p.FileEvent = null;
                //p.ProtocolEvent = eventArgs;
                //p.NavigateToProtocolPage(typeof(UserID));
                ////rootFrame.Navigate(typeof(UserID), eventArgs.Uri);

                var p = rootFrame.Content as MainPage;
                p.ProtocolEvent = protocolArgs;
                p.NavigateToProtocolPage();

                // Ensure the current window is active
                Window.Current.Activate();

            }
        }

        private Frame CreateRootFrame()
        {
            Frame rootFrame = Window.Current.Content as Frame;

            // Do not repeat app initialization when the Window already has content,
            // just ensure that the window is active
            if (rootFrame == null)
            {
                // Create a Frame to act as the navigation context and navigate to the first page
                rootFrame = new Frame();

                // TODO: change this value to a cache size that is appropriate for your application
                rootFrame.CacheSize = 1;

                //if (e.PreviousExecutionState == ApplicationExecutionState.Terminated)
                //{
                //    // TODO: Load state from previously suspended application
                //}

                // Place the frame in the current Window
                Window.Current.Content = rootFrame;
            }

            if (rootFrame.Content == null)
            {
                // Removes the turnstile navigation for startup.
                if (rootFrame.ContentTransitions != null)
                {
                    this.transitions = new TransitionCollection();
                    foreach (var c in rootFrame.ContentTransitions)
                    {
                        this.transitions.Add(c);
                    }
                }

                rootFrame.ContentTransitions = null;
                rootFrame.Navigated += this.RootFrame_FirstNavigated;
            }

            return rootFrame;
        }

        /// <summary>
        /// Restores the content transitions after the app has launched.
        /// </summary>
        /// <param name="sender">The object where the handler is attached.</param>
        /// <param name="e">Details about the navigation event.</param>
        private void RootFrame_FirstNavigated(object sender, NavigationEventArgs e)
        {
            var rootFrame = sender as Frame;
            rootFrame.ContentTransitions = this.transitions ?? new TransitionCollection() { new NavigationThemeTransition() };
            rootFrame.Navigated -= this.RootFrame_FirstNavigated;
        }

        /// <summary>
        /// Invoked when application execution is being suspended.  Application state is saved
        /// without knowing whether the application will be terminated or resumed with the contents
        /// of memory still intact.
        /// </summary>
        /// <param name="sender">The source of the suspend request.</param>
        /// <param name="e">Details about the suspend request.</param>
        private void OnSuspending(object sender, SuspendingEventArgs e)
        {
            var deferral = e.SuspendingOperation.GetDeferral();

            Frame currentFrame = Window.Current.Content as Frame;
            if (currentFrame.SourcePageType.Equals(typeof(BlankPage1)))
            {
                BlankPage1 page = currentFrame.Content as BlankPage1;
                page.Clear();
            }

            // TODO: Save application state and stop any background activity
            deferral.Complete();
        }
        
        async void OnResuming(object sender, object e)
        {
            Frame currentFrame = Window.Current.Content as Frame;
            if (currentFrame.SourcePageType.Equals(typeof(BlankPage1)))
            {
                BlankPage1 page = currentFrame.Content as BlankPage1;
                await page.InitCamera();
            }
        }
    }
}