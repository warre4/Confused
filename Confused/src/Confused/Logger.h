#pragma once
#include <memory>

#include "Core.h"
#include "spdlog/spdlog.h"

#pragma warning( disable : 4251 ) // needs to have dll-interface to be used by clients of class

namespace Confused
{
	class CONFUSED_API Logger
	{
	public:
		static void Initialize();

		[[nodiscard]] inline static const std::shared_ptr<spdlog::logger>& GetCoreLogger() { return m_pCoreLogger; }
		[[nodiscard]] inline static const std::shared_ptr<spdlog::logger>& GetClientLogger() { return m_pClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> m_pCoreLogger;
		static std::shared_ptr<spdlog::logger> m_pClientLogger;
	};
}