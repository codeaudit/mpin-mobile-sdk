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
        private const string urlKey = "BackendUrl";
        private const string requestANKey = "RequestAccessNumber";
        private const string requestOtpKey = "RequestOtp";
        private const string titleKey = "Title";
        private const string rpsKey = "rps";

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
                    this.backendUrl = value;
                    OnPropertyChanged();
                }
            }
        }

        private bool requestAN;
        public bool RequestAccessNumber
        {
            get
            {
                return this.requestAN;
            }
            set
            {
                if (this.requestAN != value)
                {
                    this.requestAN = value;
                    OnPropertyChanged();
                }
            }
        }

        private bool requestOTP;
        public bool RequestOtp
        {
            get
            {
                return requestOTP;
            }
            set
            {
                if (this.requestOTP != value)
                {
                    this.requestOTP = value;
                    OnPropertyChanged();
                }
            }
        }

        private string title;
        public string Title
        {
            get
            {
                return this.title;
            }
            set
            {
                if (this.title != value)
                {
                    this.title = value;
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
                this.RequestAccessNumber.Equals(b.RequestAccessNumber) &&
                this.RequestOtp.Equals(b.RequestOtp) &&
                this.RpsPrefix.Equals(b.RpsPrefix) &&
                this.Title.Equals(b.Title);
        }

        public override int GetHashCode()
        {
            return base.GetHashCode();
        }

        public Backend()
        {
            this.BackendUrl = string.Empty;
            this.title = string.Empty;
            this.RequestAccessNumber = false;
            this.RequestOtp = false;
            this.IsSet = false;            
        }

        public Backend(JsonObject jsonObject)
        {
            if (jsonObject != null)
            {
                this.BackendUrl = jsonObject.GetNamedString(urlKey, "");
                this.RequestAccessNumber = jsonObject.GetNamedBoolean(requestANKey, false);
                this.RequestOtp = jsonObject.GetNamedBoolean(requestOtpKey, false);
                this.Title = jsonObject.GetNamedString(titleKey, "");
                this.RpsPrefix = jsonObject.GetNamedString(rpsKey, "");
            }
        }

        public JsonObject ToJsonObject()
        {
            JsonObject backendObject = new JsonObject();
            backendObject.SetNamedValue(urlKey, JsonValue.CreateStringValue(BackendUrl));
            backendObject.SetNamedValue(requestANKey, JsonValue.CreateBooleanValue(RequestAccessNumber));
            backendObject.SetNamedValue(requestOtpKey, JsonValue.CreateBooleanValue(RequestOtp));
            backendObject.SetNamedValue(titleKey, JsonValue.CreateStringValue(Title));
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
}
