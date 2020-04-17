#pragma once
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Juno {
class Log
{
public:
	Log();
	~Log();
	static void	Init();

	inline static std::shared_ptr<spdlog::logger>& CoreLog() { return coreLogger; }
	inline static std::shared_ptr<spdlog::logger>& ClientLog() { return clientLogger; }

private:
	static std::shared_ptr<spdlog::logger> coreLogger;
	static std::shared_ptr<spdlog::logger> clientLogger;
};
}



#define JN_ERR(...) ::Juno::Log::CoreLog()->error(__VA_ARGS__)
#define JN_INFO(...) ::Juno::Log::CoreLog()->info(__VA_ARGS__)
#define JN_WARN(...) ::Juno::Log::CoreLog()->warn(__VA_ARGS__)
#define JN_CRIT(...) ::Juno::Log::CoreLog()->critical(__VA_ARGS__)
#define JN_TRACE(...) ::Juno::Log::CoreLog()->trace(__VA_ARGS__)

#define JN_CLI_ERR(...) ::Juno::Log::ClientLog()->error(__VA_ARGS__)
#define JN_CLI_INFO(...) ::Juno::Log::ClientLog()->info(__VA_ARGS__)
#define JN_CLI_WARN(...) ::Juno::Log::ClientLog()->warn(__VA_ARGS__)
#define JN_CLI_CRIT(...) ::Juno::Log::ClientLog()->critical(__VA_ARGS__)
#define JN_CLI_TRACE(...) ::Juno::Log::ClientLog()->trace(__VA_ARGS__)
