#pragma once
#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace bbx {
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



#define BX_ERR(...) ::bbx::Log::coreLog()->error(__VA_ARGS__);
#define BX_INFO(...) ::bbx::Log::coreLog()->info(__VA_ARGS__);
#define BX_WARN(...) ::bbx::Log::coreLog()->warn(__VA_ARGS__);
#define BX_CRIT(...) ::bbx::Log::coreLog()->critical(__VA_ARGS__);
#define BX_TRACE(...) ::bbx::Log::coreLog()->trace(__VA_ARGS__);

#define BX_CLI_ERR(...) ::bbx::Log::clientLog()->error(__VA_ARGS__);
#define BX_CLI_INFO(...) ::bbx::Log::clientLog()->info(__VA_ARGS__);
#define BX_CLI_WARN(...) ::bbx::Log::clientLog()->warn(__VA_ARGS__);
#define BX_CLI_CRIT(...) ::bbx::Log::clientLog()->critical(__VA_ARGS__);

