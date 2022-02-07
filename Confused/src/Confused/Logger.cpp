#include "Logger.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Confused
{
	std::shared_ptr<_SPDLOG logger> Logger::m_pCoreLogger;
	std::shared_ptr<_SPDLOG logger> Logger::m_pClientLogger;

	void Logger::Initialize(const std::string& appName)
	{
		spdlog::set_pattern(" %n: %^%v%$");

		m_pCoreLogger = spdlog::stdout_color_mt(ENGINE_INITIALS);
		m_pCoreLogger->set_level(spdlog::level::trace);

		m_pClientLogger = spdlog::stdout_color_mt(appName);
		m_pClientLogger->set_level(spdlog::level::trace);
	}
}