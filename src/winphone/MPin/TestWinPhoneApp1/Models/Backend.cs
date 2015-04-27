using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TestWinPhoneApp1.Models
{
    public struct Backend
    {
        public const string DEFAULT_RPS_PREFIX = "rps";
        public string BackendUrl
        {
            get;
            set;
        }

        public bool RequestAccessNumber
        {
            get;
            set;
        }

        public bool RequestOtp
        {
            get;
            set;
        }

        public string Title
        {
            get;
            set;
        }

        private string rpsPrefix;
        public string RpsPrefix
        {
            get
            {
                if (string.IsNullOrEmpty(this.rpsPrefix))
                    return DEFAULT_RPS_PREFIX;

                return this.rpsPrefix;
            }
            set
            {
                this.rpsPrefix = value;
            }
        }

    }
}
