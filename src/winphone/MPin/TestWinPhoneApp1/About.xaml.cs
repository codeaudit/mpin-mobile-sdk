using System;
using Windows.ApplicationModel;
using Windows.ApplicationModel.Resources;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Navigation;

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkID=390556

namespace MPinDemo
{
    /// <summary>
    /// An page that displays the information about the M-Pin application.
    /// </summary>
    public sealed partial class About : Page
    {
        public About()
        {
            this.InitializeComponent();
            PackageVersion pv = Package.Current.Id.Version;
            Version version = new Version(Package.Current.Id.Version.Major,
                Package.Current.Id.Version.Minor);
            
            VersionTB.Text = ResourceLoader.GetForCurrentView().GetString("AboutVersion") + version.ToString();
            BuildTB.Text = ResourceLoader.GetForCurrentView().GetString("AboutBuild") + Package.Current.Id.Version.Build.ToString();
            Certivox.Text = string.Format(ResourceLoader.GetForCurrentView().GetString("CertivoxLtd"), Certivox.Text);
        }

        /// <summary>
        /// Invoked when this page is about to be displayed in a Frame.
        /// </summary>
        /// <param name="e">Event data that describes how this page was reached.
        /// This parameter is typically used to configure the page.</param>
        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
        }
    }
}
