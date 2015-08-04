using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using Windows.Data.Json;

namespace MPinDemo.Models
{
    public class Backend: INotifyPropertyChanged
    {
        public const string DEFAULT_RPS_PREFIX = "rps";
        private const string urlKey = "url";
        private const string nameKey = "name";
        private const string rpsKey = "rps";
        private const string typeKey = "type";

        private string backendUrl;
        public string BackendUrl
        {
            get
            {
                return this.backendUrl;
            }
            set
            {
                if (this.backendUrl != value)
                {
                    this.backendUrl = value.Trim();
                    OnPropertyChanged();
                }
            }
        }

        private string name;
        public string Name
        {
            get
            {
                return this.name;
            }
            set
            {
                if (this.name != value)
                {
                    this.name = value.Trim();
                    OnPropertyChanged();
                }
            }
        }

        private string rpsPrefix;
        public string RpsPrefix
        {
            get
            {
                if (string.IsNullOrEmpty(this.rpsPrefix))
                    return DEFAULT_RPS_PREFIX;

                return this.rpsPrefix;
            }
            set
            {
                if (this.rpsPrefix != value)
                {
                    this.rpsPrefix = value;
                    OnPropertyChanged();
                }
            }
        }

        private ConfigurationType type;
        public ConfigurationType Type
        {
            get
            {
                return this.type;
            }
            set
            {
                if (value != this.type)
                {
                    this.type = value;
                    OnPropertyChanged();
                }
            }
        }

        private bool? isSet;
        public bool? IsSet
        {
            get
            {
                return this.isSet;
            }
            set
            {
                if (value != this.isSet)
                {
                    this.isSet = value;
                    OnPropertyChanged();
                }
            }
        }

        public override bool Equals(object obj)
        {
            Backend b = obj as Backend;
            if (b == null)
                return false;

            return !string.IsNullOrEmpty(this.BackendUrl) &&
                this.BackendUrl.Equals(b.BackendUrl) &&
                this.Type.Equals(b.Type) &&
                this.Name.Equals(b.Name);
        }

        public override int GetHashCode()
        {
            return base.GetHashCode();
        }

        public Backend()
        {
            this.BackendUrl = string.Empty;
            this.name = string.Empty;
            this.IsSet = false;            
        }

        public Backend(JsonObject jsonObject)
        {
            if (jsonObject != null)
            {
                this.BackendUrl = jsonObject.GetNamedString(urlKey, "");
                this.Name = jsonObject.GetNamedString(nameKey, "");
                this.Type = ParseType(jsonObject.GetNamedString(typeKey, ""));
                this.RpsPrefix = jsonObject.GetNamedString(rpsKey, "");                
            }
        }

        private ConfigurationType ParseType(string typeString)
        {
            if (string.IsNullOrEmpty(typeString))
                return ConfigurationType.Mobile;

            switch (typeString)
            {
                case "online":
                    return ConfigurationType.Online;
                case "otp":
                    return ConfigurationType.OTP;
                default:
                    return ConfigurationType.Mobile;
            }
        }

        public JsonObject ToJsonObject()
        {
            JsonObject backendObject = new JsonObject();
            backendObject.SetNamedValue(urlKey, JsonValue.CreateStringValue(BackendUrl));
            backendObject.SetNamedValue(typeKey, JsonValue.CreateStringValue(Type.ToString().ToLower()));
            backendObject.SetNamedValue(nameKey, JsonValue.CreateStringValue(Name));
            backendObject.GetNamedValue(rpsKey, JsonValue.CreateStringValue(RpsPrefix));

            return backendObject;
        }

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

    public enum ConfigurationType
    {
        Mobile,
        Online,
        OTP
    }
}
