using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.ApplicationModel;
using Windows.ApplicationModel.Activation;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Media.Animation;
using Windows.UI.Xaml.Navigation;
using HockeyApp;
using Windows.Storage;
using Windows.Phone.UI.Input;

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
            this.Resuming += App_Resuming;
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
                        
            Frame rootFrame = Window.Current.Content as Frame;

            // Do not repeat app initialization when the Window already has content,
            // just ensure that the window is active
            if (rootFrame == null)
            {
                // Create a Frame to act as the navigation context and navigate to the first page
                rootFrame = new Frame();

                // TODO: change this value to a cache size that is appropriate for your application
                rootFrame.CacheSize = 1;

                if (e.PreviousExecutionState == ApplicationExecutionState.Terminated)
                {
                    // TODO: Load state from previously suspended application
                }

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

                // When the navigation stack isn't restored navigate to the first page,
                // configuring the new page by passing required information as a navigation
                // parameter
                if (!rootFrame.Navigate(typeof(MainPage), e.Arguments))
                {
                    throw new Exception("Failed to create initial page");
                }
            }

            //RootFrame = rootFrame;
            // Ensure the current window is active
            Window.Current.Activate();

            await HockeyClient.Current.SendCrashesAsync();
#if WINDOWS_PHONE_APP
            await HockeyClient.Current.CheckForAppUpdateAsync();
#endif
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

        ///// <summary>
        ///// Handles the back button press and navigates through the history of the root frame.
        ///// </summary>
        ///// <param name="sender">The source of the event. <see cref="HardwareButtons"/></param>
        ///// <param name="e">Details about the back button press.</param>
        //private void HardwareButtons_BackPressed(object sender, BackPressedEventArgs e)
        //{
        //    Frame frame = Window.Current.Content as Frame;
        //    if (frame == null)
        //    {
        //        return;
        //    }

        //    var handler = this.BackPressed;
        //    if (handler != null)
        //    {
        //        handler(sender, e);
        //    }

        //    if (frame.CanGoBack && !e.Handled)
        //    {
        //        frame.GoBack();
        //        e.Handled = true;
        //    }
        //}

        /// <summary>
        /// Invoked when application execution is being suspended.  Application state is saved
        /// without knowing whether the application will be terminated or resumed with the contents
        /// of memory still intact.
        /// </summary>
        /// <param name="sender">The source of the suspend request.</param>
        /// <param name="e">Details about the suspend request.</param>
        private async void OnSuspending(object sender, SuspendingEventArgs e)
        {
            var deferral = e.SuspendingOperation.GetDeferral();

            Frame currentFrame = Window.Current.Content as Frame;
            if (currentFrame.SourcePageType.Equals(typeof(BlankPage1)))
            {
                BlankPage1 page = currentFrame.Content as BlankPage1;
                await page.Clear();
            }

            // TODO: Save application state and stop any background activity
            deferral.Complete();
        }
        
        async void App_Resuming(object sender, object e)
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