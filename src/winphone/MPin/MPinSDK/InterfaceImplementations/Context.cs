using MPinRC;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MPinSDK
{
    public class Context : IContext
    {
        #region Members
        IPinPad pinPad { get; set; }
        IStorage storage { get; set; }
        IHttpRequest httpRequest { get; set; }
        #endregion

        #region IContext
        public IHttpRequest CreateHttpRequest()
        {
            if (this.httpRequest == null)
                this.httpRequest = new HTTPConnector();

            return this.httpRequest;
        }

        public void ReleaseHttpRequest(IHttpRequest request)
        { }

        public IStorage GetStorage(MPinRC.StorageType type)
        {
            if(this.storage == null)
                this.storage = new Storage(type);

            return this.storage;
        }

        public IPinPad GetPinPad()
        {
            if (this.pinPad == null)
                this.pinPad = new PinPad();

            return this.pinPad;
        }

        public CryptoType GetMPinCryptoType()
        {
            return CryptoType.CRYPTO_NON_TEE;
        }
        #endregion
    }
}
