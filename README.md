# CallAdmin  
CallAdmin is an system to allow inGame reports for HL2-Games and Mods.  

The system is based on 3 parts.  
* [An sourcemod-plugin](https://github.com/Impact123/CallAdmin/) to report players inGame  
* An Desktop-client to notify admins  when a new report was made  
* [An webscript](https://github.com/Impact123/CallAdmin/) to fetch data from the database for the client  


## Installation #SourceMod
1. Get the optional [Plugin updater](http://forums.alliedmods.net/showthread.php?t=169095), we highly recommend you to use it  
2. Open `../addons/sourcemod/config/databases.cfg` and add a new entry with the key `CallAdmin`  
3. Restart your server or change the map to reload the databases.cfg file  
3. Put `calladmin.smx` in your `../addons/sourcemod/plugins` directory  
5. Put `plugin.calladmin.cfg` in your `../cfg/sourcemod` folder or load the plugin, it will automatically create it  
6. Load the plugin or change the map  
7. Edit the config to your purposes  


## Installation #Webscript
1. Put the notice.php and the app.config file somewhere you can refer to, this can be an subdomain for example  
2. Open the app.config file and edit the database settings


### Requirements #Client
The client is written in python 3.x, therefore you need to install it.


## Installation #Client
1. Download the client including all files into some folder  
2. Open up the `calladmin-client_settings.ini` file and set the url to where your notice.php is reachable (http://report.clanname.com/notice.php)  
3. Open the client and wait for new reports  


## Notes
* This system is currently an alpha software, use it on your own risk  
