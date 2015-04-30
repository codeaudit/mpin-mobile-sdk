using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MPinRC;

namespace MPinSDK.Models
{
    public class User : IDisposable
    {
        private UserWrapper user;
        public enum State
        {
            INVALID,
            STARTED_REGISTRATION,
            ACTIVATED,
            REGISTERED,
            BLOCKED
        };

        internal User(UserWrapper user)
        {
            this.Wrapper = user;
        }

        public String Id
        {
            get
            {
                return this.Wrapper.GetId();
            }
        }

        public State UserState
        {
            get
            {
                switch (this.Wrapper.GetState())
                {
                    case 1:
                        return State.STARTED_REGISTRATION;
                    case 2:
                        return State.ACTIVATED;
                    case 3:
                        return State.REGISTERED;
                    case 4:
                        return State.BLOCKED;
                    default:
                        return State.INVALID;
                }
            }
        }

        //// TODO: not used... used for? see android version IsUserSelected/SetUserSelected
        //public bool IsSelected
        //{
        //    get;
        //    set;
        //}

        public override string ToString()
        {
            return this.Id;
        }

        public override bool Equals(object obj)
        {
            return this.Id.Equals(((User)obj).Id);
        }

        public override int GetHashCode()
        {
            return base.GetHashCode();
        }

        #region IDisposable
        public void Dispose()
        {
            lock (this)
            {
                this.Wrapper.Destruct();
            }
        }
        #endregion // IDisposable

        internal UserWrapper Wrapper
        {
            get
            {
                return this.user;
            }
            private set
            {
                this.user = value;
            }
        }
    }
}
