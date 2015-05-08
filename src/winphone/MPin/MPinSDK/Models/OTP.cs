using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MPinRC;

namespace MPinSDK.Models
{
    /// <summary>
    /// Defines an One-Time Password (OTP) object used for authenticating with a RADIUS serve.
    /// </summary>
    public class OTP
    {
        /// <summary>
        /// Gets or sets the issued One-Time Password.
        /// </summary>
        /// <value>
        /// The otp.
        /// </value>
        public string Otp
        {
            get
            {
                return this.Wrapper.Otp;
            }
            set
            {
                this.Wrapper.Otp = value;
            }
        }

        public long ExpireTime
        {
            get
            {
                return this.Wrapper.ExpireTime;
            }
            set
            {
                this.Wrapper.ExpireTime = value;
            }
        }

        public int TtlSeconds
        {
            get
            {
                return this.Wrapper.TtlSeconds;
            }
            set
            {
                this.Wrapper.TtlSeconds = value;
            }
        }

        public long NowTime
        {
            get
            {
                return this.Wrapper.NowTime;
            }
            set
            {
                this.Wrapper.NowTime = value;
            }
        }

        private Status _status;
        public Status Status
        {
            get
            {
                if (_status == null || !_status.Wrapper.Equals(this.Wrapper.Status))
                {
                    _status = new Status(this.Wrapper.Status.Code, this.Wrapper.Status.Error);
                }

                return _status;                
            }
            set
            {
                if (false == this.Wrapper.Status.Equals(value))
                    this.Wrapper.Status = ConvertToWrapper(value);
            }
        }

        internal OTPWrapper Wrapper
        {
            get;
            set;
        }

        public OTP()
        {
            this.Wrapper = new OTPWrapper();
        }

        private StatusWrapper ConvertToWrapper(Models.Status value)
        {
            return new StatusWrapper() { Code = (int)value.StatusCode, Error = value.ErrorMessage };
        }

    }
}
