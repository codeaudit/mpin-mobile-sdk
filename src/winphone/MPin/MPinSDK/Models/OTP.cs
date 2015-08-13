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
