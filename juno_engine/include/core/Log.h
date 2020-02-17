#pragma once
#include "pch.h"


namespace juno {
class Log
{
public:
	Log();
	~Log();
	static void	init();

	inline static std::shared_ptr<spdlog::logger>& coreLog() { return coreLogger; }
	inline static std::shared_ptr<spdlog::logger>& clientLog() { return clientLogger; }

private:
	static std::shared_ptr<spdlog::logger> coreLogger;
	static std::shared_ptr<spdlog::logger> clientLogger;


};
}



#define JN_ERR(...) ::juno::Log::coreLog()->error(__VA_ARGS__)
#define JN_INFO(...) ::juno::Log::coreLog()->info(__VA_ARGS__)
#define JN_WARN(...) ::juno::Log::coreLog()->warn(__VA_ARGS__)
#define JN_CRIT(...) ::juno::Log::coreLog()->critical(__VA_ARGS__)
#define JN_TRACE(...) ::juno::Log::coreLog()->trace(__VA_ARGS__)

#define JN_CLI_ERR(...) ::juno::Log::clientLog()->error(__VA_ARGS__)
#define JN_CLI_INFO(...) ::juno::Log::clientLog()->info(__VA_ARGS__)
#define JN_CLI_WARN(...) ::juno::Log::clientLog()->warn(__VA_ARGS__)
#define JN_CLI_CRIT(...) ::juno::Log::clientLog()->critical(__VA_ARGS__)

