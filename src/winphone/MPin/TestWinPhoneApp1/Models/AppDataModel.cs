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
        #region Members
        
        private const string FileName =
#if DEBUG
            "SampleData_Debug.json";
#elif MPinConnect
            "SampleData_MPinConnect.json";
#else
            "SampleData.json";
#endif

        private const string FilePath = "ms-appx:///Resources/" + FileName;

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
            get; 
            private set; 
        }

        #endregion

        #region Methods

        //private void CreateBackends()
        //{
        //    //Backend backends[] = 
        //    //{
        //    //    {"https://m-pindemo.certivox.org"},
        //    //    {"http://ec2-54-77-232-113.eu-west-1.compute.amazonaws.com", "/rps/"},
        //    //    {"https://mpindemo-qa-v3.certivox.org", "rps"},
        //    //};
        //    //TODO:: leave only the last three services
        //    BackendsList = new ObservableCollection<Backend>();
        //    BackendsList.Add(new Backend()
        //    {
        //        BackendUrl = "https://m-pindemo.certivox.org",
        //        RequestAccessNumber = false,
        //        RequestOtp = false,
        //        Title = "Basic"
        //    });

        //    //BackendsList.Add(new Backend()
        //    //{
        //    //    BackendUrl = "http://ec2-54-77-232-113.eu-west-1.compute.amazonaws.com",
        //    //    RequestAccessNumber = false,
        //    //    RequestOtp = false,
        //    //    Title = "M-Pin Connect"
        //    //});

        //    BackendsList.Add(new Backend()
        //    {
        //        BackendUrl = "http://ec2-52-28-120-46.eu-central-1.compute.amazonaws.com",
        //        RequestAccessNumber = false,
        //        RequestOtp = false,
        //        Title = "Force Activation"
        //    });

        //    BackendsList.Add(new Backend()
        //    {
        //        BackendUrl = "https://mpindemo-qa-v3.certivox.org",
        //        RequestAccessNumber = true,
        //        RequestOtp = false,
        //        Title = "Bank service"
        //    });

        //    BackendsList.Add(new Backend()
        //    {
        //        BackendUrl = "http://otp.m-pin.id/rps",
        //        RequestAccessNumber = false,
        //        RequestOtp = true,
        //        Title = "Longest Journey Service"
        //    });

        //    BackendsList.Add(new Backend()
        //    {
        //        BackendUrl = "http://risso.certivox.org/",
        //        RequestAccessNumber = false,
        //        RequestOtp = true,
        //        Title = "OTP login"
        //    });

        //    BackendsList.Add(new Backend()
        //    {
        //        BackendUrl = "http://ntt-vpn.certivox.org",
        //        RequestAccessNumber = false,
        //        RequestOtp = true,
        //        Title = "OTP NTT login"
        //    });

        //    BackendsList.Add(new Backend()
        //    {
        //        BackendUrl = "http://tcb.certivox.org",
        //        RequestAccessNumber = false,
        //        RequestOtp = false,
        //        Title = "Mobile banking login"
        //    });

        //    BackendsList.Add(new Backend()
        //    {
        //        BackendUrl = "http://tcb.certivox.org",
        //        RequestAccessNumber = true,
        //        RequestOtp = false,
        //        Title = "Online banking login"
        //    });

        //    BackendsList.Add(new Backend()
        //    {
        //        BackendUrl = "http://otp.m-pin.id",
        //        RequestAccessNumber = false,
        //        RequestOtp = true,
        //        Title = "VPN login"
        //    });
        //}

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

            LoadBackendsFromDataString(jsonText);
        }

        internal void LoadBackendsFromDataString(string jsonText, bool readFromQR = false)
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

            List<Backend> existingOnes = new List<Backend>();
            foreach (JsonValue groupValue in jsonArray)
            {
                JsonObject groupObject = groupValue.GetObject();
                Backend backend = new Backend(groupObject);
                var currentBackend = this.BackendsList.FirstOrDefault(item => item.Name.Equals(backend.Name));

                if (currentBackend != null)
                {
                    existingOnes.Add(backend);
                    // loaded configurations overwrite the existing ones with matching names.
                    this.BackendsList[this.BackendsList.IndexOf(currentBackend)] = backend;
                }
                else
                {
                    this.BackendsList.Add(backend);
                }
            }

            if (setPredefinedConfigurationCount)
                AppDataModel.PredefinedServicesCount = this.BackendsList.Count;
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
            catch(Exception wer)
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
