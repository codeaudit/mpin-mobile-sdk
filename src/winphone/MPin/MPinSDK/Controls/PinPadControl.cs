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
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using Windows.System.Threading;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;

namespace MPinSDK.Controls
{
    class PinPadControl : Control, INotifyPropertyChanged
    {
        #region Members
        private PinPadPassword Pass;
        private PinPadButton One;
        private PinPadButton Two;
        private PinPadButton Three;
        private PinPadButton Four;
        private PinPadButton Five;
        private PinPadButton Six;
        private PinPadButton Seven;
        private PinPadButton Eight;
        private PinPadButton Nine;
        private PinPadButton Zero;
        private PinPadButton Clear;
        private PinPadButton Sign;

        internal const byte MPinLength = 4;

        public event EventHandler<PinPadEventArgs> PinEntered;
        #endregion // Members

        #region Constructor
        public PinPadControl()
        {
            this.DefaultStyleKey = typeof(PinPadControl);
            this.Loaded += PinPadControl_Loaded;
        }

        #endregion // Constructor

        #region Overrides
        protected override void OnApplyTemplate()
        {
            base.OnApplyTemplate();
            this.Pass = this.GetTemplateChild("pass") as PinPadPassword;
           
            RegisterButton(ref this.One, "one", One_Click);
            RegisterButton(ref this.Two, "two", Two_Click);
            RegisterButton(ref this.Three, "three", Three_Click);
            RegisterButton(ref this.Four, "four", Four_Click);
            RegisterButton(ref this.Five, "five", Five_Click);
            RegisterButton(ref this.Six, "six", Six_Click);
            RegisterButton(ref this.Seven, "seven", Seven_Click);
            RegisterButton(ref this.Eight, "eight", Eight_Click);
            RegisterButton(ref this.Nine, "nine", Nine_Click);
            RegisterButton(ref this.Zero, "zero", Zero_Click);
            RegisterButton(ref this.Clear, "clear", Clear_Click);
            RegisterButton(ref this.Sign, "sign", Sign_Click);
        }
                
        #endregion // Overrides

        #region handlers
        void PinPadControl_Loaded(object sender, RoutedEventArgs e)
        {
            IsEntered = false;
        }
        void One_Click(object sender, Windows.UI.Xaml.RoutedEventArgs e)
        {
            AddDigitToPin("1");
        }
        void Two_Click(object sender, Windows.UI.Xaml.RoutedEventArgs e)
        {
            AddDigitToPin("2");
        }
        void Three_Click(object sender, Windows.UI.Xaml.RoutedEventArgs e)
        {
            AddDigitToPin("3");
        }
        void Four_Click(object sender, Windows.UI.Xaml.RoutedEventArgs e)
        {
            AddDigitToPin("4");
        }
        void Five_Click(object sender, Windows.UI.Xaml.RoutedEventArgs e)
        {
            AddDigitToPin("5"); 
        }
        void Six_Click(object sender, Windows.UI.Xaml.RoutedEventArgs e)
        {
            AddDigitToPin("6");
        }
        void Seven_Click(object sender, Windows.UI.Xaml.RoutedEventArgs e)
        {
            AddDigitToPin("7");
        }
        void Eight_Click(object sender, Windows.UI.Xaml.RoutedEventArgs e)
        {
            AddDigitToPin("8");
        }
        void Nine_Click(object sender, Windows.UI.Xaml.RoutedEventArgs e)
        {
            AddDigitToPin("9");
        }
        void Zero_Click(object sender, Windows.UI.Xaml.RoutedEventArgs e)
        {
            AddDigitToPin("0");
        }
        void Clear_Click(object sender, Windows.UI.Xaml.RoutedEventArgs e)
        {
            //if (this.Pass.Data.Length > 0)
            //    this.Pass.Data = this.Pass.Data.Substring(0, this.Pass.Data.Length - 1);
            this.Pass.Data = string.Empty;
            ValidateSignButton();
        }
        void Sign_Click(object sender, Windows.UI.Xaml.RoutedEventArgs e)
        {   
            if (this.PinEntered != null)   
                this.PinEntered(this, new PinPadEventArgs(this.Pass.Data));

            IsEntered = true;
        }
        #endregion // handlers

        #region Methods

        private bool isEntered;
        internal bool IsEntered
        {
            get
            {
                return this.isEntered;
            }
            set
            {
                this.isEntered = value;
                this.OnPropertyChanged();
            }
        }
        
        private void RegisterButton(ref PinPadButton button, string name, RoutedEventHandler clickEvent)
        {
            if (button != null)
                button.Click -= clickEvent;

            button = this.GetTemplateChild(name) as PinPadButton;

            if (button != null)
                button.Click += clickEvent;
        }

        private void AddDigitToPin(string digit)
        {
            if (this.Pass.Data.Length < MPinLength)
                this.Pass.Data += digit;

            ValidateSignButton();
        }

        private void ValidateSignButton()
        {
            this.Sign.IsEnabled = this.Pass.Data.Length == MPinLength;
            if (this.Sign.IsEnabled)                
                this.Sign.Focus(FocusState.Pointer);
        }

        #region INotifyPropertyChanged
        public event PropertyChangedEventHandler PropertyChanged;
        void OnPropertyChanged([CallerMemberName]string name = "")
        {
            PropertyChangedEventHandler handler = PropertyChanged;
            if (handler != null)
            {
                handler(this, new PropertyChangedEventArgs(name));
            }
        }
        #endregion // INotifyPropertyChanged


        #endregion // Methods
    }

}
