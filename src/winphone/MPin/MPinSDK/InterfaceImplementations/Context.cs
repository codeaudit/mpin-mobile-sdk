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

namespace MPinSDK
{
    /// <summary>
    /// A Context class implementing the <see cref="T:MPinRC.IContext">IContext</see> interface which "bundles" all the rest of the interfaces. An instance of this class is provided to the Core and the others are used/accessed through it. 
    /// </summary>
    public class Context : IContext
    {
        #region Members
        IPinPad pinPad { get; set; }
        IStorage storageSecure { get; set; }
        IStorage storageNonsecure { get; set; }
        IHttpRequest httpRequest { get; set; }
        #endregion
        
        #region IContext
        /// <summary>
        /// Creates a new HTTP request instance that conforms with <see cref="T:MPinRC.IHttpRequest">IHttpRequest</see> interface.
        /// </summary>
        /// <returns>An instance of a class, implementing <see cref="T:MPinRC.IHttpRequest">IHttpRequest</see> interface. </returns>
        public IHttpRequest CreateHttpRequest()
        {
            if (this.httpRequest == null)
                this.httpRequest = new HTTPConnector();

            return this.httpRequest;
        }

        /// <summary>
        /// Destroys/releases a previously created HTTP request instance.
        /// </summary>
        /// <param name="request">The request.</param>
        public void ReleaseHttpRequest(IHttpRequest request)
        { }

        /// <summary>
        /// Creates a Storage class implementation, which conforms to <see cref="T:MPinRC.IStorage">IStorage</see> interface,
        /// depending on the specified type.
        /// </summary>
        /// <param name="type">The <see cref="T:MPinRC.StorageType">Storage type</see>.</param>
        /// <returns>A Storage class implementation.</returns>
        public IStorage GetStorage(MPinRC.StorageType type)
        {
            if (type == StorageType.SECURE)
            {
                if (storageSecure == null)
                {
                    storageSecure = new Storage(StorageType.SECURE);
                }

                return storageSecure;
            }

            if (storageNonsecure == null)
            {
                storageNonsecure = new Storage(StorageType.NONSECURE);
            }

            return storageNonsecure;
        }

        /// <summary>
        /// Provides a PIN Pad UI interface. The Core will use this class to trigger the display of the PIN Pad.
        /// </summary>
        /// <returns>Instance of a class, implementing the <see cref="T:MPinRC.IPinPad">IPinPad</see> interface.</returns>
        public IPinPad GetPinPad()
        {
            if (this.pinPad == null)
                this.pinPad = new PinPad();

            return this.pinPad;
        }

        /// <summary>
        /// This method provides an information regarding the supported Crypto Type on the specific platform. Currently, only on the Android platform this method might return something different than Non-TEE Crypto. Other platforms always returns Non-TEE Crypto. 
        /// </summary>
        /// <returns></returns>
        public CryptoType GetMPinCryptoType()
        {
            return CryptoType.CRYPTO_NON_TEE;
        }
        #endregion
    }
}
