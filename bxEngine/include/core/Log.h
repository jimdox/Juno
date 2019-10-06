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



#define BBX_ERR(...) ::bbx::Log::coreLog()->error(__VA_ARGS__);
#define BBX_INFO(...) ::bbx::Log::coreLog()->info(__VA_ARGS__);
#define BBX_WARN(...) ::bbx::Log::coreLog()->warn(__VA_ARGS__);
#define BBX_CRIT(...) ::bbx::Log::coreLog()->critical(__VA_ARGS__);
#define BBX_TRACE(...) ::bbx::Log::coreLog()->trace(__VA_ARGS__);

#define BBX_CLI_ERR(...) ::bbx::Log::clientLog()->error(__VA_ARGS__);
#define BBX_CLI_INFO(...) ::bbx::Log::clientLog()->info(__VA_ARGS__);
#define BBX_CLI_WARN(...) ::bbx::Log::clientLog()->warn(__VA_ARGS__);
#define BBX_CLI_CRIT(...) ::bbx::Log::clientLog()->critical(__VA_ARGS__);

