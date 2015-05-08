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
        /// The One-Time Password (OTP).
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

        /// <summary>
        /// Gets or sets the system time on the M-Pin System when the OTP is due to expire.
        /// </summary>
        /// <value>
        /// The system time on the M-Pin System when the OTP is due to expire.
        /// </value>
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

        /// <summary>
        /// Gets or sets the expiration period in seconds.
        /// </summary>
        /// <value>
        /// The expiration period in seconds.
        /// </value>
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

        /// <summary>
        /// Gets or sets the current system time of the M-Pin system.
        /// </summary>
        /// <value>
        /// The current system time of the M-Pin system.
        /// </value>
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
        /// <summary>
        /// Gets or sets the current One-Time Password (OTP) object status.
        /// </summary>
        /// <value>
        /// The status of the current One-Time Password (OTP) object.
        /// </value>
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

        /// <summary>
        /// Initializes a new instance of the <see cref="OTP"/> class.
        /// </summary>
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
