using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.UI.Xaml.Controls;

namespace MPinSDK.Common
{
    public static class Extensions
    {
        private static object Data;

        /// <summary>
        /// Causes the Frame to load content represented by the specified Page, also
        /// passing a data to be interpreted by the target of the navigation.        ///  
        /// </summary>
        /// <param name="frame">The frame itself.</param>
        /// <param name="sourcePageType">The URI of the content to navigate to.</param>
        /// <param name="data">The data that you need to pass to the other page 
        /// specified in URI.</param>
        public static bool Navigate(this Frame frame, Type sourcePageType, object data)
        {
            Data = data;
            return frame.Navigate(sourcePageType);
        }

        /// <summary>
        /// Navigates to the most recent item in back navigation history, if a Frame
        ///  manages its own navigation history.
        /// </summary>
        /// <param name="frame">The frame itself.</param>
        /// <param name="data">The data that you need to pass to the other page 
        /// specified in URI.</param>
        public static void GoBack(this Frame frame, object data)
        {
            Data = data;
            frame.GoBack();
        }

        /// <summary>
        /// Gets the navigation data passed from the previous page.
        /// </summary>
        /// <param name="service">The service.</param>
        /// <returns>System.Object.</returns>
        public static object GetNavigationData(this Frame service)
        {
            return Data;
        }
    }
}
