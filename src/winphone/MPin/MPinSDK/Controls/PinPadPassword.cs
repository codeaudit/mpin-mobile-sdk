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
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;

namespace MPinSDK.Controls
{
    class PinPadPassword : Control
    {
        #region fields
        private PasswordBox passwordBox;
        #endregion // fields

        #region constructor        
        public PinPadPassword()
        {
            this.DefaultStyleKey = typeof(PinPadPassword);
        }
        #endregion // constructor

        #region members
       
        #region Data

        /// <summary>
        /// Gets or sets the password data of the control.
        /// </summary>
        /// <value>The password data text.</value>
        public string Data
        {
            get { return (string)GetValue(DataProperty); }
            set { SetValue(DataProperty, value); }
        }

        /// <summary>
        /// Identifies the <see cref="Data"/> dependency property.
        /// </summary>
        public static readonly DependencyProperty DataProperty =
            DependencyProperty.Register("Data", typeof(string), typeof(PinPadPassword),            
            new PropertyMetadata(string.Empty, OnDataChanged));

        private static void OnDataChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            ((PinPadPassword)d).OnDataChanged((string)e.OldValue, (string)e.NewValue);
        }

        /// <summary>
        /// EmptyTextProperty property changed handler.
        /// </summary>
        /// <param name="oldValue">The old value.</param>
        /// <param name="newValue">The new value.</param>
        private void OnDataChanged(string oldValue, string newValue)
        {
            if (this.passwordBox != null && Validate(newValue))
            {
                this.passwordBox.Password = newValue;
            }
        }

        #endregion // EmptyText

        internal bool IsValid
        {
            get;
            set;
        }
        #endregion // members

        #region overrides
        protected override void OnApplyTemplate()
        {
            base.OnApplyTemplate();
            this.passwordBox = this.GetTemplateChild("PasswordBox") as PasswordBox;
        }
        #endregion // overrides

    
        #region methods

        private bool Validate(string newValue)
        {
            this.IsValid = newValue.Length <= PinPadControl.MPinLength;
            return this.IsValid;
        }
        #endregion // methods
    }
}
