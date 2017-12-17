#ifndef SETTINGS__H__
#define SETTINGS__H__

#include <Windows.h>
#include <map>
#include <string>
#include <mutex>

#include "logger.h"
#include "datamanager.h"

#define LOCK(x) std::lock_guard<std::mutex> g(x);
#define GETSETTING(x) Settings::GetSettings(x)
class Settings
{
public:
	typedef std::map<std::string, std::string> settingsMap;
	
	static bool LoadSettings(std::string settingsFile);
	static bool SaveSettings(std::string settingsFile);
	//static std::string GetSettings(std::string key);
	static settingsMap* ParseSettings(std::string filePath, std::string delimiter = "=", char commentIndicator = '#');

private:
	std::string m_settingsFileName;
	settingsMap* m_settings = nullptr;
	bool m_settingsLoaded = false;

	Settings();
	~Settings();
	Settings(Settings& s) { static_assert(true, "SINGLETON CANNOT BE COPIED."); };
	Settings& operator= (const Settings& rhs) {};
	
	static Settings& GetInstance() { static Settings s; return s; };
	void SettingsFileWatchdog();
	void ReloadFile();
};

#endif