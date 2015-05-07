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
        IStorage storageSecure { get; set; }
        IStorage storageNonsecure { get; set; }
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
