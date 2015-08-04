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
