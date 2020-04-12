#include "core/Log.h"
using namespace juno;

std::shared_ptr<spdlog::logger> Log::coreLogger;
std::shared_ptr<spdlog::logger> Log::clientLogger;

void Log::init()
{
	spdlog::set_pattern("%^[%T] %n: %v%$");
	coreLogger = spdlog::stdout_color_mt("Core");
	coreLogger->set_level(spdlog::level::trace);

	clientLogger = spdlog::stdout_color_mt("User");
	clientLogger->set_level(spdlog::level::trace);

}