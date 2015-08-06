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
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MPinSDK.Models
{
    /// <summary>
    /// Status class used to indicate whether an operation is successful or not.
    /// </summary>
    public class Status
    {
        internal StatusWrapper Wrapper { get; set; }

        /// <summary>
        /// Initializes a new instance of the <see cref="Status"/> class.
        /// </summary>
        /// <param name="statusCode">The status code.</param>
        /// <param name="error">The error message.</param>
        public Status(int statusCode, String error)
        {
            this.Wrapper = new StatusWrapper();
            this.StatusCode = (Code)Enum.GetValues(typeof(Code)).GetValue(statusCode);
            this.ErrorMessage = error;            
        }

        /// <summary>
        /// Gets the status code returned from the server.
        /// </summary>
        /// <value>
        /// The status code returned from the server.
        /// </value>
        public Code StatusCode
        {
            get
            {
                return (Code)this.Wrapper.Code;
            }
            private set
            {
                this.Wrapper.Code = (int)value;
            }
        }

        /// <summary>
        /// Gets or sets the message of the error if there is such one.
        /// </summary>
        /// <value>
        /// The error message.
        /// </value>
        public String ErrorMessage
        {
            get
            {
                return this.Wrapper.Error;
            }
            set
            {
                this.Wrapper.Error = value;
            }
        }

        /// <summary>
        /// Returns a <see cref="System.String" /> that represents this instance.
        /// </summary>
        /// <returns>
        /// A <see cref="System.String" /> that represents this instance.
        /// </returns>
        public override string ToString()
        {
            return "Status [StatusCode=" + this.StatusCode + ", ErrorMessage='" + this.ErrorMessage + "']";
        }

        /// <summary>
        /// Determines whether the specified <see cref="System.Object" />, is equal to this instance.
        /// </summary>
        /// <param name="obj">The <see cref="System.Object" /> to compare with this instance.</param>
        /// <returns>
        ///   <c>true</c> if the specified <see cref="System.Object" /> is equal to this instance; otherwise, <c>false</c>.
        /// </returns>
        public override bool Equals(object obj)
        {
            Status objToCompare = (Status)obj;
            if (objToCompare == null)
                return false;

            if (false == this.StatusCode.Equals(objToCompare.StatusCode))
                return false;

            if (false == this.ErrorMessage.Equals(objToCompare.ErrorMessage))
                return false;

            return true;
        }

        /// <summary>
        /// Returns a hash code for this instance.
        /// </summary>
        /// <returns>
        /// A hash code for this instance, suitable for use in hashing algorithms and data structures like a hash table. 
        /// </returns>
        public override int GetHashCode()
        {
            return base.GetHashCode();
        }

        [Flags]
        public enum Code
        {
            /// <summary>
            /// Successful authentication.
            /// </summary>
            OK,
            /// <summary>
            /// Local error, returned when user cancels pin entering
            /// </summary>
            PinInputCanceled,
            /// <summary>
            /// Local error in crypto functions
            /// </summary>
            CryptoError,
            /// <summary>
            /// Local storage related error
            /// </summary>
            StorageError,
            /// <summary>
            /// Local error - cannot connect to remote server (no internet, or invalid server/port)
            /// </summary>
            NetworkError,
            /// <summary>
            /// Local error - cannot parse json response from remote server (invalid json or unexpected json structure)
            /// </summary>
            ResponseParseError,
            /// <summary>
            /// Local error - unproper MPinSDK class usage
            /// </summary>
            FlowError,
            /// <summary>
            /// Remote error - the remote server refuses user registration
            /// </summary>
            IdentityNotAuthorized,
            /// <summary>
            /// Remote error - the remote server refuses user registration because identity is not verified
            /// </summary>
            IdentityNotVerified,
            /// <summary>
            /// Remote error - the register/authentication request expired
            /// </summary>
            RequestExpired,
            /// <summary>
            /// Remote error - cannot get time permit (propably the user is temporary suspended)
            /// </summary>
            Revoked,
            /// <summary>
            /// Remote error - user entered wrong pin
            /// </summary>
            IncorrectPIN,
            /// <summary>
            /// Remote/local error - wrong access number (checksum failed or RPS returned 412)
            /// </summary>
            IncorrectAccessNumber,
            /// <summary>
            /// Remote error, that was not reduced to one of the above - the remote server returned internal server error status (5xx)
            /// </summary>
            HttpServerError,
            /// <summary>
            /// Remote error, that was not reduced to one of the above - invalid data sent to server, the remote server returned 4xx error status
            /// </summary>
            HttpRequestError
        }
    }
}
