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
    public class PinPadControl : Control, INotifyPropertyChanged
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

        void PinPadControl_Loaded(object sender, RoutedEventArgs e)
        {
            IsEntered = false;
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
            if (this.Pass.Data.Length > 0)
                this.Pass.Data = this.Pass.Data.Substring(0, this.Pass.Data.Length - 1);

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
