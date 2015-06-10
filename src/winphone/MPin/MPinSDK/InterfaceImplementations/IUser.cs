using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MPinSDK
{
    /// <summary>
    /// Interface to be implemented in order the selected user identity to be passed to the pin pad screen.
    /// </summary>
    public interface IUser
    {
        /// <summary>
        /// Gets the selected user name which will be displayed at the pin pad control screen.
        /// </summary>
        /// <returns>The selected user identity.</returns>
        string GetUserId();
    }
}
