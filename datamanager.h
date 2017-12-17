#ifndef DATAMANAGER__H__
#define DATAMANAGER__H__

#include <string>
#include <io.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <Windows.h>
#include <vector>
#include <filesystem>

#include "logger.h"


class Datamanager
{
public:
	static bool DirectoryExists(const std::string pathToDirectory)
	{
		if (_access(pathToDirectory.c_str(), 0) == 0)
		{
			struct stat status;
			stat(pathToDirectory.c_str(), &status);

			if (status.st_mode & S_IFDIR)
				return true;
		}
		return false;
	}
	
	static bool FileExists(const std::string pathToFile)
	{
		struct stat buffer;
		return (stat(pathToFile.c_str(), &buffer) == 0);
	}

	static std::vector<std::string> GenerateFileList(std::string pathToFiles)
	{
		WIN32_FIND_DATA data;
		HANDLE hFind;
		std::vector<std::string> list;

		hFind = FindFirstFile((pathToFiles + "*.png").c_str(), &data);

		if (hFind == INVALID_HANDLE_VALUE)
			return list;


		do
		{
			list.push_back(std::string(data.cFileName));
		} while (FindNextFile(hFind, &data));

		DEBUG("Found\t" + TO_STR(list.size()) + "\t files in: \t" + pathToFiles);

		return list;
	}


	/*
	Create directory if it doesn't exist yet.
	*/
	static bool MyCreateDirectory(std::string pathToDirectory)
	{
		if (DirectoryExists(pathToDirectory))
		return false;
		const size_t cSize = pathToDirectory.size() + 1;
		wchar_t *wc = new wchar_t[cSize];
		mbstowcs(wc, pathToDirectory.c_str(), cSize);
		_wmkdir(wc);
		delete wc;
		return true;
	}

	static std::string TruncateRootPath(std::string rootPath)
	{
		if (rootPath.length() < 1)
			return "./";

		size_t pos = rootPath.find_last_of("/\\");
		return rootPath.substr(0, pos + 1); // keep the last '\' before the application name	
	}

};

#endif