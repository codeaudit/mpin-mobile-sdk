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
using Windows.Storage;
using Windows.Storage.Streams;

namespace MPinSDK
{
    class Storage : IStorage
    {        
        #region Fields
        StorageFolder localFolder = null;
        public const string MPIN_STORAGE = "tokens.json"; 
        public const string USER_STORAGE = "users.json";  

        private string path;
        private string Data { get; set; }

        public string ErrorMessage
        {
            private set;
            get;
        }
        #endregion

        #region C'tor
        public Storage(StorageType type) : base()
        {
            localFolder = ApplicationData.Current.LocalFolder;
            
            path = type == StorageType.SECURE ? MPIN_STORAGE : USER_STORAGE;
            this.Data = string.Empty;
        }
        #endregion // C'tor

        #region IStorage
        public bool SetData(string data)
        {
            lock (this.Data)
            {
                Task.Run(async () => { await SetDataAsync(data); }).Wait();
                return string.IsNullOrEmpty(this.ErrorMessage);
            }
        }

        public string GetData()
        {
            lock (this.Data)
            {
                Task.Run(async () => { await GetDataAsync(); }).Wait();
                return string.IsNullOrEmpty(this.Data) ? string.Empty : this.Data;
            }
        }

        public string GetErrorMessage()
        {
            return this.ErrorMessage;
        }
        #endregion // IStorage

        #region Methods

        private async Task SetDataAsync(string data)
        {
            this.ErrorMessage = string.Empty;
            byte[] fileBytes = System.Text.Encoding.UTF8.GetBytes(data.ToCharArray());
            var file = await GetFile();

            try
            {
                await FileIO.WriteTextAsync(file, data);
            }
            catch (Exception e)
            {
                this.ErrorMessage = e.Message;
            }
        }

        private async Task<StorageFile> GetFile()
        {
            StorageFile file;
            if (await IsFilePresent(path))
            {
                file = await localFolder.GetFileAsync(path);
            }
            else
            {
                file = await localFolder.CreateFileAsync(path, CreationCollisionOption.ReplaceExisting);
            }

            return file;
        }

        private async Task<bool> IsFilePresent(string fileName)
        {
            var allfiles = await localFolder.GetFilesAsync();
            foreach (var storageFile in allfiles)
            {
                if (storageFile.Name == fileName)
                {
                    return true;
                }
            }

            return false;
        }

        private async Task GetDataAsync()
        {
            this.ErrorMessage = string.Empty;
            try
            {
                var file = await GetFile();
                this.Data = await FileIO.ReadTextAsync(file);
            }
            catch (Exception e)
            {
                this.ErrorMessage = e.Message;
            }
        }

        #endregion // Methods
    }
}
