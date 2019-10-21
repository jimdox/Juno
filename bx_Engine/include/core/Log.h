#pragma once
#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace bx {
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



#define BX_ERR(...) ::bx::Log::coreLog()->error(__VA_ARGS__);
#define BX_INFO(...) ::bx::Log::coreLog()->info(__VA_ARGS__);
#define BX_WARN(...) ::bx::Log::coreLog()->warn(__VA_ARGS__);
#define BX_CRIT(...) ::bx::Log::coreLog()->critical(__VA_ARGS__);
#define BX_TRACE(...) ::bx::Log::coreLog()->trace(__VA_ARGS__);

#define BX_CLI_ERR(...) ::bx::Log::clientLog()->error(__VA_ARGS__);
#define BX_CLI_INFO(...) ::bx::Log::clientLog()->info(__VA_ARGS__);
#define BX_CLI_WARN(...) ::bx::Log::clientLog()->warn(__VA_ARGS__);
#define BX_CLI_CRIT(...) ::bx::Log::clientLog()->critical(__VA_ARGS__);

