using MPinSDK.Models;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using Windows.Data.Json;
using Windows.Storage;

namespace MPinDemo.Models
{
    public class AppDataModel : INotifyPropertyChanged
    {
        #region Members
        private const string FilePath = "ms-appx:///Resources/SampleData.json";
        private static AppDataModel _appDataModel = new AppDataModel();

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
                if (_currentUser != value)
                {
                    _currentUser = value;
                    this.OnPropertyChanged();
                }
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
                if (!_currentService.Equals(value))
                {
                    _currentService = value;
                    this.OnPropertyChanged();
                }
            }
        }

        #endregion // CurrentService

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

            Uri dataUri = new Uri(FilePath);

            StorageFile file = await StorageFile.GetFileFromApplicationUriAsync(dataUri);
            string jsonText = await FileIO.ReadTextAsync(file);
            JsonObject jsonObject = JsonObject.Parse(jsonText);
            JsonArray jsonArray = jsonObject["Backends"].GetArray();

            foreach (JsonValue groupValue in jsonArray)
            {
                JsonObject groupObject = groupValue.GetObject();
                Backend backend = new Backend()
                {
                    BackendUrl = groupObject["BackendUrl"].GetString(),
                    RequestAccessNumber = groupObject["RequestAccessNumber"].GetBoolean(),
                    RequestOtp = groupObject["RequestOtp"].GetBoolean(),
                    Title = groupObject["Title"].GetString()
                };

                this.BackendsList.Add(backend);
            }
        }
       
        internal async Task SaveServices()
        {
            // TODO
            //System.Runtime.Serialization.Json.DataContractJsonSerializer ser = new System.Runtime.Serialization.Json.DataContractJsonSerializer(typeof())
 
            //JsonObject jsonObject = JsonObject.Parse(jsonText);
            //JsonArray jsonArray = jsonObject["Backends"].GetArray();
            //JsonSerializer

            //foreach (JsonValue groupValue in jsonArray)
            //foreach(var backend in BackendsList)
            //{

            //    JsonObject groupObject = groupValue.GetObject();
            //    Backend backend = new Backend()
            //    {
            //        BackendUrl = groupObject["BackendUrl"].GetString(),
            //        RequestAccessNumber = groupObject["RequestAccessNumber"].GetBoolean(),
            //        RequestOtp = groupObject["RequestOtp"].GetBoolean(),
            //        Title = groupObject["Title"].GetString()
            //    };
            //}

            Uri dataUri = new Uri(FilePath);
            StorageFile file = await StorageFile.GetFileFromApplicationUriAsync(dataUri);
            //string jsonText = await FileIO.WriteTextAsync(file, .ReadTextAsync(file);

            //JsonObject jsonObject = JsonObject.Parse(jsonText);
            //JsonArray jsonArray = jsonObject["Backends"].GetArray();

            //foreach (JsonValue groupValue in jsonArray)
            //{
            //    JsonObject groupObject = groupValue.GetObject();
            //    Backend backend = new Backend()
            //    {
            //        BackendUrl = groupObject["BackendUrl"].GetString(),
            //        RequestAccessNumber = groupObject["RequestAccessNumber"].GetBoolean(),
            //        RequestOtp = groupObject["RequestOtp"].GetBoolean(),
            //        Title = groupObject["Title"].GetString()
            //    };

            //    // TODO: User is a readonly class 
            //    //foreach (JsonValue itemValue in groupObject["Items"].GetArray())
            //    //{
            //    //    JsonObject itemObject = itemValue.GetObject();
            //    //    backend.Users.Add(new User(itemObject["UniqueId"].GetString(),
            //    //                                       itemObject["Title"].GetString(),
            //    //                                       itemObject["Subtitle"].GetString(),
            //    //                                       itemObject["ImagePath"].GetString(),
            //    //                                       itemObject["Description"].GetString(),
            //    //                                       itemObject["Content"].GetString()));
            //    //}

            //    this.BackendsList.Add(backend);
            //}
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
