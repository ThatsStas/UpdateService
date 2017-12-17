#include "logger.h"
#include "configurator.h"

int main(int argc, char** argv)
{
	
	auto lgLvl = attic::Logger::Info | attic::Logger::Debug | attic::Logger::Critical | attic::Logger::Warning | attic::Logger::Trace;
	attic::Logger::getInstance().setFileLogFilter(lgLvl);
	attic::Logger::getInstance().setOutputLogFilter(lgLvl);
	
	Configurator conf;
	auto param = conf.parseCommandline(argc, argv);

	if (param.find("-v") != param.end())
	{
		INFO(VER_APPLICATION_VERSION_STR);
	}

	
	//INFO(VER_APPLICATION_VERSION_STR);
	//INFO(VER_RELEASE_TYPE_STR);
	


	return 0;
}