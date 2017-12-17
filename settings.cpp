#include "settings.h"

bool Settings::LoadSettings(std::string pathToFile)
{
	GetInstance().m_settingsFileName = pathToFile;
	
	HANDLE file = CreateFile(pathToFile.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file == INVALID_HANDLE_VALUE)
	{
		CRITICAL("Settingsfile does not exist.");
		return false;
	}
	else
	{
		//GetFileTime(file, NULL, NULL, &GetInstance().m_accessTime);
		*GetInstance().m_settings = *ParseSettings(pathToFile);
	}
	CloseHandle(file);
	return false;
}

bool Settings::SaveSettings(std::string pathToFile)
{
	if (remove(pathToFile.c_str()) != 0)
		return false;
	std::ofstream settingsFile(pathToFile, std::ios::out);

	for (auto itm : *GetInstance().m_settings)
	{
		//properly save "string"
		if (itm.second.find(' ') != std::string::npos)
		{
			settingsFile << itm.first << "=\"" << itm.second << "\"\n";
		}
		else
			settingsFile << itm.first << "=" << itm.second << "\n";

	}
	settingsFile.flush();
	settingsFile.close();

	return false;
}

//std::string Settings::GetSettings(std::string key)
//{
//	auto set = GetInstance().m_settings;
//	if (set->find(key) == set->end())
//	{
//		CRITICAL("Requested setting could not be found.");
//		return "0";
//	}
//	return (*set)[key];
//}

Settings::Settings()
{
	m_settings = new settingsMap;
}

Settings::~Settings() 
{ 
	if (!m_settings)
		delete m_settings; 
}

void Settings::SettingsFileWatchdog()
{
	
	//HANDLE file = CreateFile(m_settingsFileName.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	//if (file == INVALID_HANDLE_VALUE)
	//{
	//	WARNING("Could not open file for reload.");
	//	return;
	//}
	//FILETIME lastAccess;
	//GetFileTime(file, NULL, NULL, &lastAccess);

	//if ((lastAccess.dwHighDateTime != GetInstance().m_accessTime.dwHighDateTime) || (lastAccess.dwLowDateTime != GetInstance().m_accessTime.dwLowDateTime))
	//{
	//	GetInstance().m_accessTime = lastAccess;
	//	//DEBUG("Settingsfile change detected. Reload settings.");
	//	Settings::LoadSettings(m_settingsFileName);
	//}
	//CloseHandle(file);
}

void Settings::ReloadFile()
{
}


/*
	Reads and parse a given settings file creating a map of key/value pairs as <string,string>.
	The delimiter must be a '='. Lines starting with a '#' will be threated as a comment.
	Keys/values will be overwritten so appending a parameter to the end of the file will override
	any parameter declared previously.
*/
Settings::settingsMap* Settings::ParseSettings(std::string filePath, std::string delimiter, char commentIndicator) {

	//std::string delimiter = "=";
	//char comment = '#';

	std::map<std::string, std::string>* settings = GetInstance().m_settings;
	settings->clear();
	std::ifstream settingsFile(filePath, std::ios::in);

	std::string line;
	while (!settingsFile.eof() && !settingsFile.bad() && !settingsFile.fail())
	{
		std::getline(settingsFile, line);
		if (line.empty() || (line.at(0)) == commentIndicator) // Here the comment is found.
			continue;

		size_t pos = 0;
		std::string tmpKey;
		std::string tmpValue;

		std::string key;
		std::string value;

		pos = line.find(delimiter);
		tmpKey = line.substr(0, pos);
		line.erase(0, pos + delimiter.length());
		tmpValue = line;
		
		if (tmpValue.empty())
			continue;

		for (int i = 0; i < tmpKey.length(); i++)
		{
			//if (tmpKey.at(i) == commentIndicator)
			//	break;
			if (tmpKey.at(i) != ' ' && tmpKey.at(i) != '\t')
				key.push_back(tmpKey.at(i));
		}

		for (int i = 0; i < tmpValue.length(); i++)
		{
			if (tmpValue.at(i) == commentIndicator)
				break;
			//string in 'as is' condition
			if (tmpValue.at(i) == '"')
			{
				value = tmpValue.substr(1 , tmpValue.size() - 2);
				break;
			}
			if (tmpValue.at(i) != ' ' && tmpValue.at(i) != '\t')
				value.push_back(tmpValue.at(i));
		}

		// Overwrite existing value
		auto it = settings->find(key);
		if (it == settings->end())
			settings->emplace(key, value);
		else
			it->second = value;
	}

	settingsFile.close();
	//GetInstance().m_settings = &settings;
	return settings;
}