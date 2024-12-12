###Esp32_Viessm_DatBase_Bridge

This is an App for an Esp32 Dev Board which retrieves sensor data via the Viessmann Api from the Viessmann Cloud, stores the data in Azure Storage Tables in a special way that they can be graphically visualized with the iPhone App 'Charts4Azure'

![gallery](https://github.com/RoSchmi/Esp32_Viessm_DatBase_Bridge/blob/master/pictures/Heating_Graph.png)

This App is based on the software of my App https://github.com/RoSchmi/Esp32_WiFiManager_HeatingSurvey
This other App for Esp32 monitors the activity of the burner of an oil-heating
(or any other noisy thing) by measuring the sound of e.g. the heating burner
For details visit the Github Repo of this App. 

###The Story:
Recently I relocated to another city into a house with a Viessmann Vitodens 333F
heating. 
For this heating it was easyly possible to mount a VITOCONNECT 100 OPTO1
module which frequently sends sensor data of the heating via WiFi and internet
to the Viessmann Cloud and makes it possible to control and monitor the heating 
via the 'Vicare' App from Viessmann.
Using this App one can see actual sensor data and change settings, but it is not
possible to maintain historic sensor data and to display sensor data graphically 
as a timeline graphs.
As Viessmann provides access to the actual heating sensor data via an API
I decided to make an Esp32 application to download the sensor data via the API around every minute (1440 downloads per day are free) and store the values of several sensors on Azure Storage Tables. This requires a Microsoft Azure Account, is not completely free, but very cheap (only several cents per month).
So I made this App which loads the sensor data from the Viessmann Cloud
and stores the data in Azure Storage Tables in a certain way.
To display these data graphically on an Apple iPhone I use the iPhone App
'Charts4Azure' (Android version and a Microsoft Store App with minor functionality
exist as well). The App 'Charts4Azure' allways displays a set of
4 analog timeline graphs (e.g. 4 temperature curves) and 4 On/Off timeline graphs 
(e.g. for burner or pump activity) on one page.

For this Esp App Router-WiFi Credentials, Azure Credentials and Viessmann Credentials can be entered via a Captive Portal page which is provided for one minute by the Esp32 after powering up the device. 
After having entered the credentials one time they stay permanently on the Esp32 and need not to be entered every time.

This App uses an adaption of the 'Async_ConfigOnStartup.ino' as WiFi Mangager
from: -https://github.com/khoih-prog/ESPAsync_WiFiManager 
More infos about the functions and Licenses see:

https://github.com/khoih-prog/ESPAsync_WiFiManager/blob/master/examples/Async_ConfigOnDoubleReset_Multi

// -https://github.com/khoih-prog/ESPAsync_WiFiManager/tree/master/examples/Async_ConfigOnStartup
 
The WiFiManager will open a configuration portal in the WLAN Access-Point for 60 seconds when powered up if the boards has stored WiFi Credentials, otherwise, the Access Point will stay indefinitely in ConfigPortal until the Router WiFi Credentials and other Credentials are entered.
When the Esp32 starts up it provides an Access Point after about one minute to which you must connect with your iPhone. 
The SSID of this Access point is like e.g. ESP32_xxxxxx, if a password is requested (only the first time), the passwort is "My" + the SSID-Name, e.g. MyESP32_xxxxxx.
Some settings for the Esp32 App have to be made in the files config.h and config_secret.h.
The special config_secret.h for your system must be provided by copying and renaming of config_secret_template.h to config_secret.h

To access the Viessmann Api through this Esp32 App you to need a Client-Id
and a 'Refresh Token' (valid for some months), which have to be entered in config_secret.h or in the Captive Portal Page. The Client-Id and an Access-Token (valid for 1 hour) can be retrieved from: https://developer.viessmann.com/start/pricing.html (Basic -> Get started)

To get a Refresh-Token you can use my C# Windows Application:
https://github.com/RoSchmi/RoSchmiViessmannApiTest
Valueble info can be found here: 
https://www.rustimation.eu/index.php/a-viessmann-api-und-node-red/

How to get an Azure Account and there a Storage Account have a
look here: https://azureiotcharts.home.blog/getting-started-guide/
