using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MPinSDK.Controls
{
    class PinPadEventArgs : EventArgs
    {
        public PinPadEventArgs(string pin)
        {
            this.Pin = pin;
        }

        public string Pin
        {
            get;
            set;
        }
    }
}
