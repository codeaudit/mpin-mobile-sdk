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

using MPinSDK.Models;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using Windows.ApplicationModel.Resources;
using Windows.Data.Json;
using Windows.Storage;


namespace MPinDemo.Models
{
    public class AppDataModel : INotifyPropertyChanged
    {
        #region Fields & Members

        private const string FileName =
#if DEBUG
            "SampleData_Debug.json";
#elif MPinConnect
            "SampleData_MPinConnect.json";
#else
 "SampleData.json";
#endif

        private const string FilePath = "ms-appx:///Resources/" + FileName;
        private const string PredefinedServicesCountString = "PredefinedServicesCount";

        private bool setPredefinedConfigurationCount = false;
        private StorageFolder localFolder = Windows.Storage.ApplicationData.Current.LocalFolder;
        private const string backendsKey = "Backends";
        private static AppDataModel _appDataModel = new AppDataModel();

        public AppDataModel()
        {
            //CreateBackends();
        }

        private ObservableCollection<Backend> services;
        public ObservableCollection<Backend> BackendsList
        {
            get
            {
                return services;
            }
            set
            {
                if (services != value)
                {
                    services = value;
                    OnPropertyChanged();
                }
            }
        }

        private ObservableCollection<User> users;
        public ObservableCollection<User> UsersList
        {
            get
            {
                return users;
            }
            set
            {
                if (users != value)
                {
                    users = value;
                    OnPropertyChanged();
                }
            }
        }

        #region CurrentUser
        private User _currentUser;
        public User CurrentUser
        {
            get
            {
                return _currentUser;
            }
            set
            {
                _currentUser = value;
                this.OnPropertyChanged();
            }
        }
        #endregion // CurrentUser

        #region CurrentService
        private Backend _currentService;
        public Backend CurrentService
        {
            get
            {
                return _currentService;
            }
            set
            {
                _currentService = value;
                this.OnPropertyChanged();
            }
        }

        #endregion // CurrentService

        public static int PredefinedServicesCount
        {
            get
            {
                return BlankPage1.RoamingSettings.Values[PredefinedServicesCountString] == null ? 0 : int.Parse(BlankPage1.RoamingSettings.Values[PredefinedServicesCountString].ToString());
            }
        } 

        #endregion

        #region Methods

        public static async Task<ObservableCollection<Backend>> GetBackendsAsync()
        {
            await _appDataModel.GetSampleDataAsync();
            return _appDataModel.BackendsList;
        }

        public static async Task<Backend> GetBackendAsync(string uniqueId)
        {
            await _appDataModel.GetSampleDataAsync();
            var matches = _appDataModel.BackendsList.Where((backend) => backend.BackendUrl.Equals(uniqueId));
            return matches.FirstOrDefault();
        }

        private async Task GetSampleDataAsync()
        {
            if (this.BackendsList == null)
            {
                this.BackendsList = new ObservableCollection<Backend>();
            }

            if (this.BackendsList.Count != 0)
                return;

            StorageFile file = null;
            try
            {
                file = await GetConfigurationFile();
            }
            catch (Exception sewe)
            {
                System.Diagnostics.Debug.WriteLine(sewe.Message);
                return;
            }

            await LoadBackendsFromFile(file);
        }

        internal async Task LoadBackendsFromFile(StorageFile file)
        {
            string jsonText = await FileIO.ReadTextAsync(file);
            if (string.IsNullOrEmpty(jsonText))
            {
                System.Diagnostics.Debug.WriteLine("Empty json file!");
                return;
            }

            List<Backend> newBackends = GetBackendsFromJson(jsonText);
            foreach (Backend backend in newBackends)
            {
                this.BackendsList.Add(backend);
            }

            if (setPredefinedConfigurationCount)
                BlankPage1.RoamingSettings.Values[PredefinedServicesCountString] = this.BackendsList.Count;
        }

        internal List<Backend> GetBackendsFromJson(string jsonText)
        {
            if (string.IsNullOrEmpty(jsonText))
            {
                throw new ArgumentNullException(ResourceLoader.GetForCurrentView().GetString("EmptyJSON"));
            }

            JsonArray jsonArray;
            try
            {
                jsonArray = JsonArray.Parse(jsonText);
            }
            catch
            {
                throw new ArgumentException(ResourceLoader.GetForCurrentView().GetString("InvalidJSON"));
            }

            List<Backend> backendsList = new List<Backend>();
            foreach (JsonValue groupValue in jsonArray)
            {
                JsonObject groupObject = groupValue.GetObject();
                backendsList.Add(new Backend(groupObject));
            }

            return backendsList;
        }

        internal async Task SaveServices()
        {
            JsonArray jsonArray = new JsonArray();
            foreach (Backend backend in BackendsList)
            {
                jsonArray.Add(backend.ToJsonObject());
            }

            StorageFile file = await GetConfigurationFile(true);

            try
            {
                CachedFileManager.DeferUpdates(file);

                string data = jsonArray.Stringify();

                await FileIO.WriteTextAsync(file, data);

                Windows.Storage.Provider.FileUpdateStatus status = await CachedFileManager.CompleteUpdatesAsync(file);

                if (status == Windows.Storage.Provider.FileUpdateStatus.Complete)
                {
                    // File was saved
                }
                else
                {
                    // File was not saved
                }
            }
            catch (Exception wer)
            {
                System.Diagnostics.Debug.WriteLine(wer.Message);
            }
        }

        /// <summary>
        /// Gets the configuration file. The InstalledLocation storage folder is readonly, which is why 
        /// when modify the predefined configuration we should use the LocalStorage.
        /// /// </summary>
        /// <param name="forceGetFromLocal">if set to <c>true</c> [force get from local].</param>
        /// <returns></returns>
        private async Task<StorageFile> GetConfigurationFile(bool forceGetFromLocal = false)
        {
            this.setPredefinedConfigurationCount = false;
            bool isPresent = await IsFilePresentInLocalStorage(FileName);
            if (isPresent)
            {
                // if the file is present in the LocalFolder -> the predefined configuration have been changed and save in the local storage
                return await localFolder.GetFileAsync(FileName);
            }
            else if (forceGetFromLocal)
            {
                return await localFolder.CreateFileAsync(FileName, CreationCollisionOption.ReplaceExisting);
            }
            else
            {
                // the configurations have not been modified -> get the predefined configuration from the local installed location
                Uri dataUri = new Uri(FilePath, UriKind.Absolute);
                this.setPredefinedConfigurationCount = true;
                return await StorageFile.GetFileFromApplicationUriAsync(dataUri);
            }
        }

        private async Task<bool> IsFilePresentInLocalStorage(string fileName)
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

        internal async Task MergeConfigurations(List<Backend> newBackends)
        {
            foreach (Backend backend in newBackends)
            {
                var currentBackend = this.BackendsList.FirstOrDefault(item => item.Name.Equals(backend.Name));
                if (currentBackend != null)
                {
                    // loaded configurations overwrite the existing ones with matching names.
                    this.BackendsList[this.BackendsList.IndexOf(currentBackend)].BackendUrl = backend.BackendUrl;
                    this.BackendsList[this.BackendsList.IndexOf(currentBackend)].Type = backend.Type;
                    this.BackendsList[this.BackendsList.IndexOf(currentBackend)].RpsPrefix = backend.RpsPrefix;
                    this.BackendsList[this.BackendsList.IndexOf(currentBackend)].Name = backend.Name;
                }
                else
                {
                    this.BackendsList.Add(backend);
                }
            }

            await SaveServices();
        }
        #endregion

        #region INotifyPropertyChanged
        public event PropertyChangedEventHandler PropertyChanged;
        void OnPropertyChanged([CallerMemberName]string name = "")
        {
            PropertyChangedEventHandler handler = PropertyChanged;
            if (handler != null)
            {
                handler(this, new PropertyChangedEventArgs(name));
            }
        }
        #endregion // INotifyPropertyChanged
        
    }
}
