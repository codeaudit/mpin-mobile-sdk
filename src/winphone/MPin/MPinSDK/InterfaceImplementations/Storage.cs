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
    public class Storage : IStorage
    {
        // Guidance for Local, LocalCache, Roaming, and Temporary files.
        //
        // Files are ideal for storing large data-sets, databases, or data that is
        // in a common file-format.
        //
        // Files can exist in either the Local, LocalCache, Roaming, or Temporary folders.
        //
        // Roaming files will be synchronized across machines on which the user has
        // singed in with a connected account.  Roaming of files is not instant; the
        // system weighs several factors when determining when to send the data.  Usage
        // of roaming data should be kept below the quota (available via the 
        // RoamingStorageQuota property), or else roaming of data will be suspended.
        // Files cannot be roamed while an application is writing to them, so be sure
        // to close your application's file objects when they are no longer needed.
        //
        // Local files are not synchronized, but are backed up, and can then be restored to a 
        // machine different than where they were originally written. These should be for 
        // important files that allow the feel that the user did not loose anything
        // when they restored to a new device.
        //
        // Temporary files are subject to deletion when not in use.  The system 
        // considers factors such as available disk capacity and the age of a file when
        // determining when or whether to delete a temporary file.
        //
        // LocalCache files are for larger files that can be recreated by the app, and for
        // machine specific or private files that should not be restored to a new device.
        
        #region Fields
        StorageFolder localFolder = null;
        StorageFolder localCacheFolder = null;
        StorageFolder roamingFolder = null;
        StorageFolder temporaryFolder = null;
        const string filename = "mPin.txt";
        private static readonly object LockObject = new object();
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

        public Storage(StorageType type)
            : base()
        {
            localFolder = ApplicationData.Current.LocalFolder;
            localCacheFolder = ApplicationData.Current.LocalCacheFolder;
            roamingFolder = ApplicationData.Current.RoamingFolder;
            temporaryFolder = ApplicationData.Current.TemporaryFolder;

            path = type == StorageType.SECURE ? MPIN_STORAGE : USER_STORAGE;
            this.Data = string.Empty;
        }

        #region IStorage
        public bool SetData(string data)
        {
            lock (this.Data)
            {
                Task.Run(async () => { await SetDataAsync(data); }).Wait();
                return string.IsNullOrEmpty(this.ErrorMessage);
            }
        }

        private async Task SetDataAsync(string data)
        {
            this.ErrorMessage = string.Empty;
            // Get the text data from the textbox. 
            byte[] fileBytes = System.Text.Encoding.UTF8.GetBytes(data.ToCharArray());

            var file = await GetFile();

             try
            {
                //IBuffer buffer = Windows.Security.Cryptography.CryptographicBuffer.ConvertStringToBinary(data, Windows.Security.Cryptography.BinaryStringEncoding.Utf8);
                //await Windows.Storage.FileIO.WriteBufferAsync(file, buffer);

                ////await s.Stream.WriteAsync(buffer);
                ////s.Write(fileBytes, 0, fileBytes.Length);

                await FileIO.WriteTextAsync(file, data);
            }
            catch (Exception e)
            {
                this.ErrorMessage = e.Message;
            }
        }

        private async Task<StorageFile> GetFile()
        {
            // TODO: change the location of the file depending on the StorageFile in the c'tor

            //StorageFile file = await localFolder.CreateFileAsync(filename, CreationCollisionOption.ReplaceExisting);
            //await FileIO.WriteTextAsync(file, localCounter.ToString());

            //StorageFile file = await localCacheFolder.GetFileAsync(filename);
            //string text = await FileIO.ReadTextAsync(file);

            //StorageFile file = await roamingFolder.CreateFileAsync(filename, CreationCollisionOption.ReplaceExisting);
            //await FileIO.WriteTextAsync(file, roamingCounter.ToString());

            //StorageFile file = await temporaryFolder.CreateFileAsync(filename, CreationCollisionOption.ReplaceExisting);
            //await FileIO.WriteTextAsync(file, temporaryCounter.ToString());

            //StorageFolder local = Windows.Storage.ApplicationData.Current.LocalFolder;

            //// Create a new folder name DataFolder.
            //var dataFolder = await localFolder.CreateFolderAsync("DataFolder",
            //    CreationCollisionOption.OpenIfExists);

            //// Get a file from the installation folder with the ms-appx URI scheme. => ms-appdata requires three slashes (“///”) and isostore requires only one slash (“/”).
            //var file1 = await Windows.Storage.StorageFile.GetFileFromApplicationUriAsync(new Uri("ms-appx:///AppSetup/initialSettings2.xml"));

            StorageFile file;
            if (await IsFilePresent(path))
            {
                file = await roamingFolder.GetFileAsync(path);
            }
            else
            {
                file = await roamingFolder.CreateFileAsync(path, CreationCollisionOption.ReplaceExisting);
            }

            return file;
        }

        private async Task<bool> IsFilePresent(string fileName)
        {
            var allfiles = await roamingFolder.GetFilesAsync();
            foreach (var storageFile in allfiles)
            {
                if (storageFile.Name == fileName)
                {
                    return true;
                }
            }

            return false;
        }


        public string GetData()
        {
            lock (this.Data)
            {
                Task.Run(async () => { await GetDataAsync(); }).Wait();
                return string.IsNullOrEmpty(this.Data) ? string.Empty : this.Data;
            }
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


        public string GetErrorMessage()
        {
            return this.ErrorMessage;
        }
        #endregion // IStorage
    }
}
