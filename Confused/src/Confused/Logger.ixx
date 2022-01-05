module;
#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include <memory>
export module Confused.Logger;

namespace Confused
{
	export class CONFUSED_API Logger
	{
	public:
		static void Initialize()
		{
			spdlog::set_pattern("%^[%T] %n: %v%$");

			m_pCoreLogger = spdlog::stdout_color_mt("CF");
			m_pCoreLogger->set_level(spdlog::level::trace);

			m_pClientLogger = spdlog::stdout_color_mt("APP");
			m_pClientLogger->set_level(spdlog::level::trace);
		}

		inline static const std::shared_ptr<spdlog::logger>& GetCoreLogger() { return m_pCoreLogger; }
		inline static const std::shared_ptr<spdlog::logger>& GetClientLogger() { return m_pClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> m_pCoreLogger;
		static std::shared_ptr<spdlog::logger> m_pClientLogger;
	};

	std::shared_ptr<spdlog::logger> Logger::m_pCoreLogger = nullptr;
	std::shared_ptr<spdlog::logger> Logger::m_pClientLogger = nullptr;
}