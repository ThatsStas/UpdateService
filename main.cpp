#include <Windows.h>
#include <tchar.h>

#include "remoteFile.h"
#include "logger.h"
#include "configurator.h"
#include "settings.h"
#include "datamanager.h"
#include "json.hpp"

#pragma comment(lib, "urlmon.lib")
#pragma comment(lib,"wininet.lib")

using json = nlohmann::json;

static const std::string JSONFILE = "jnk.json";
static const std::string REMOTEURL = "http://zeroblood.square7.ch/";

void setConsoleVisibility(bool isOn)
{
	HWND hwnd = GetConsoleWindow();
	ShowWindow(hwnd, isOn ? 1 : 0);
}

bool getFile(std::string source, std::string target)
{
	HRESULT hr;
	LPCTSTR Url = _T(source.c_str()), File = _T(target.c_str());
	
	// can't overwrite stuff
	if (Datamanager::FileExists(target))
		remove(target.c_str());

	hr = URLDownloadToFile(0, Url, File, 0, 0);

	
	switch (hr)
	{
	case S_OK:
		INFO("Successful download to [" + target + "]");
		break;
	case E_OUTOFMEMORY:
		DEBUG("Out of memory error");
		break;
	case INET_E_DOWNLOAD_FAILURE:
		DEBUG("Cannot access server data");
		break;
	default:
		DEBUG("Unknown error");
		break;
	}

	if (hr)
	{
		remove(target.c_str());
	}

	return !hr;
}

int main(int argc, char** argv)
{
	auto lgLvl = attic::Logger::Info | attic::Logger::Debug | attic::Logger::Critical | attic::Logger::Warning | attic::Logger::Trace;
	attic::Logger::getInstance().setFileLogFilter(lgLvl);
	attic::Logger::getInstance().setOutputLogFilter(lgLvl);

	Configurator conf;
	auto param = conf.parseCommandline(argc, argv);

	if (param.find("-v") == param.end())
	{
		setConsoleVisibility(false);
	}


	std::string applicationRootPath = Datamanager::TruncateRootPath(argv[0]) + "config.ini";
	INFO(VER_APPLICATION_VERSION_STR);
	INFO(VER_RELEASE_TYPE_STR);

	if (!Datamanager::FileExists(applicationRootPath))
	{
		CRITICAL("Settingsfile not found.");
		MessageBox(NULL, "Settings file could not be found.", "Error", MB_OK | MB_ICONERROR);
		return -1;
	}

	Settings::settingsMap& fSettings = *Settings::ParseSettings(applicationRootPath);

	if (fSettings.find("applicationName") == fSettings.end())
	{
		WARNING("Application name not set in settings.");
		MessageBox(NULL, "Application not set in settings.", "Error", MB_OK | MB_ICONERROR);
		return -1;
	}

	std::string remotePath;
	std::string remoteName;
	std::string version;
	std::string args;

	if (getFile(REMOTEURL + "?request=appver&application=" + fSettings["applicationName"], JSONFILE))
	{
		std::ifstream i(JSONFILE.c_str());
		json j;
		i >> j;

		remotePath = j.at("applicationPath").get<std::string>();
		remoteName = j.at("applicationName").get<std::string>();
		version = j.at("version").get<std::string>();
		remove(JSONFILE.c_str());
	}
	else
	{
		WARNING("Could not get json from remote host. Starting in default mode.");
	}
	

	int remoteVer = std::stoi(version);
	int localVer = std::stoi(fSettings.find("version") == fSettings.end() ? "0" : fSettings["version"]);

	std::string targetApplication = Datamanager::TruncateRootPath(argv[0]) + fSettings["applicationName"];
	if (remoteVer > localVer)
	{
		fSettings["version"] = version;
		getFile(REMOTEURL + remotePath, targetApplication);
		Settings::SaveSettings(applicationRootPath);
	}

	if (fSettings.find("args") == fSettings.end())
	{
		WARNING("No arguments provided for this file.");
	}
	else
		args = fSettings["args"];

	INFO("\n****\nAPPLICATION OUTPUT\n****\n")

	std::string fullAppPath = targetApplication + " " + args;
	system(fullAppPath.c_str());

	return 0;
}

