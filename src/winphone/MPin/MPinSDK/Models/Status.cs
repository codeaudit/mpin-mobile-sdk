using MPinRC;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MPinSDK.Models
{
    public class Status
    {
        internal StatusWrapper Wrapper { get; set; }

        public Status(int statusCode, String error)
        {
            this.Wrapper = new StatusWrapper();
            this.StatusCode = (Code)Enum.GetValues(typeof(Code)).GetValue(statusCode);
            this.ErrorMessage = error;            
        }

        public Code StatusCode
        {
            get
            {
                // TODO:::: see if the conversion is proper or create a convertor for it
                return (Code)this.Wrapper.Code;
            }
            private set
            {
                this.Wrapper.Code = (int)value;
            }
        }

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

        public override string ToString()
        {
            return "Status [StatusCode=" + this.StatusCode + ", ErrorMessage='" + this.ErrorMessage + "']";
        }

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

        public override int GetHashCode()
        {
            return base.GetHashCode();
        }

        [Flags]
        public enum Code
        {
            OK,
            PIN_INPUT_CANCELED, // Local error, returned when user cancels pin entering
            CRYPTO_ERROR, // Local error in crypto functions
            STORAGE_ERROR, // Local storage related error
            NETWORK_ERROR, // Local error - cannot connect to remote server (no internet, or invalid server/port)
            RESPONSE_PARSE_ERROR, // Local error - cannot parse json response from remote server (invalid json or unexpected json structure)
            FLOW_ERROR, // Local error - unproper MPinSDK class usage
            IDENTITY_NOT_AUTHORIZED, // Remote error - the remote server refuses user registration
            IDENTITY_NOT_VERIFIED, // Remote error - the remote server refuses user registration because identity is not verified
            REQUEST_EXPIRED, // Remote error - the register/authentication request expired
            REVOKED, // Remote error - cannot get time permit (propably the user is temporary suspended)
            INCORRECT_PIN, // Remote error - user entered wrong pin
            BLOCKED, // Remote error - user entered wrong pin for more than N(3) times, user is removed and must register again
            HTTP_SERVER_ERROR, // Remote error, that was not reduced to one of the above - the remote server returned internal server error status (5xx)
            HTTP_REQUEST_ERROR // Remote error, that was not reduced to one of the above - invalid data sent to server, the remote server returned 4xx error status
        }
    }
}
