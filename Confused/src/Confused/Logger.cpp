#include "Logger.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Confused
{
	std::shared_ptr<spdlog::logger> Logger::m_pCoreLogger;
	std::shared_ptr<spdlog::logger> Logger::m_pClientLogger;

	void Logger::Initialize()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");

		m_pCoreLogger = spdlog::stdout_color_mt("Confused");
		m_pCoreLogger->set_level(spdlog::level::trace);

		m_pClientLogger = spdlog::stdout_color_mt("App");
		m_pClientLogger->set_level(spdlog::level::trace);
	}
}