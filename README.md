#CertiVox M-Pin Mobile SDK

##Building the M-Pin Mobile SDK and the M-Pin Sample App for Android with Eclipse

###Prerequisites

1. Download Android NDK and configure Eclipse with its location
1. Download Google's *appcompat_v7* and import it to Eclipse

###Building the M-Pin Mobile SDK

1. Import the project from `<mpin-mobile-sdk>/src/android/MPinLib`
1. Go to the project *Properties->Android* and add dependency to the *appcompat_v7* with its correct path 
1. Build the Project

###Building the M-Pin Sample App

1. Import the project from `<mpin-mobile-sdk>/project/eclipse/MPinSDK`. This project already has dependancy to the previously built **M-Pin Mobile SDK**
1. Build the Project

For further details, see [M-Pin Mobile SDK for Android Online Help](http://docs.certivox.com/m-pin-mobile-sdk-for-android)

##Building the M-Pin Mobile SDK and the M-Pin Sample App for iOS

###Prerequisites

1. Download and install the latest version of XCode
1. Download or Clone the project

###Building the M-Pin Mobile SDK

1. Navigate to `<mpin-mobile-sdk>/project/xcode/MPinSDK`
1. Open `MPinSDK.xcodeproj` and select *Product->Build* from the menu
 
###Building the M-Pin Sample App

1. Navigate to `<mpin-mobile-sdk>/project/xcode`
1. Open `MPin.xcworkspace`. This project already has dependancy to the previously built **M-Pin Mobile SDK**
1. Set the active scheme to `Certivox MPin`
1. Select *Product->Build* from the menu

For further details, see [M-Pin Mobile SDK for iOS Online Help](http://docs.certivox.com/m-pin-mobile-sdk-for-ios)
