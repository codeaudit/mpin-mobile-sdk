
using MPinDemo.Models;
using System;
using Windows.ApplicationModel.Resources;
using Windows.UI.Xaml.Data;
namespace MPinDemo
{
    public class ConfigurationConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, string language)
        {
            try
            {
                ConfigurationType type = (ConfigurationType)value;
                switch (type)
                {
                    case ConfigurationType.Mobile:
                        return ResourceLoader.GetForCurrentView().GetString("MobileLogin");
                    case ConfigurationType.Online:
                        return ResourceLoader.GetForCurrentView().GetString("OnlineLogin");
                    case ConfigurationType.OTP:
                        return ResourceLoader.GetForCurrentView().GetString("OTPLogin");
                }
            }
            catch
            { }

            return ResourceLoader.GetForCurrentView().GetString("MobileLogin");
        }

        public object ConvertBack(object value, Type targetType, object parameter, string language)
        {
            return value;
        }
    }

    public class ConfigurationTypeEnumToBooleanConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, string language)
        {
            return value.ToString().Equals(parameter.ToString());
        }

        public object ConvertBack(object value, Type targetType, object parameter, string language)
        {
            return (bool)value ? Enum.Parse(typeof(ConfigurationType), parameter.ToString(), true) : null;            
        }
    }
}
