#ifndef CONFIGURATOR__H__
#define CONFIGURATOR__H__

#include <string>
#include <list>
#include <map>

#include "logger.h"


class Configurator {
public:

	struct PARAMETERS {
		std::list<std::string> VALID_PARAMS;

		//std::string TYPE = "--type";
		//std::string THREADS = "-threads";
		//std::string CPUENABLE = "-cpu";
		//std::string GPUENABLE = "-gpu";
		//std::string KILLTIMER = "-kill";
	};

	Configurator()
	{
		//refParams.VALID_PARAMS.push_back("--type");
		refParams.VALID_PARAMS.push_back("-v");
		//refParams.VALID_PARAMS.push_back("--noCpu");
		//refParams.VALID_PARAMS.push_back("--noGpu");
		//refParams.VALID_PARAMS.push_back("-k");
		//refParams.VALID_PARAMS.push_back("--local");
		//refParams.VALID_PARAMS.push_back("--remote");
		//refParams.VALID_PARAMS.push_back("--kill");
		//refParams.VALID_PARAMS.push_back("-v");
		//refParams.VALID_PARAMS.push_back("--output");
		//refParams.VALID_PARAMS.push_back("-d");
		//refParams.VALID_PARAMS.push_back("-q");
		

	}

	PARAMETERS getReferenceParams()
	{
		return refParams;
	}

	std::map<std::string, std::string> parseCommandline(int argc, char** argv)
	{
		std::map<std::string, std::string> m;
		size_t pos = 0;
		std::string key;
		std::string value;

		std::string delimiter = "=";

		for (int i = 1; i < argc; i++)
		{
			auto line = std::string(argv[i]);
			pos = line.find(delimiter);
			key = line.substr(0, pos);
			line.erase(0, pos + delimiter.length());
			value = line;
			if (!key.empty())
				m.emplace(key, value);
		}
		if (verifyConfiguration(m))
			return m;
		else
			return std::map<std::string, std::string>();


	}

private:
	PARAMETERS refParams;

	bool verifyConfiguration(std::map<std::string, std::string> map)
	{
		bool valid = true;
		for (std::pair<std::string, std::string> entry : map)
		{
			bool isValid = false;
			for (std::string refKey : refParams.VALID_PARAMS)
			{
				if (entry.first.find(refKey) != entry.first.npos)
				{
					isValid = true;
				}
			}
			if (!isValid)
			{
				CRITICAL("[" + entry.first + "] is not a valid parameter");
				valid = false;
			}
		}

		return valid;
	}

};


#endif