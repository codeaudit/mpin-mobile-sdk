using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;

namespace MPinSDK.Controls
{
    public class PinPadPassword : Control
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
