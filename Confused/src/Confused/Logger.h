#pragma once
#include <memory>

#include "Core.h"
#include "Macros.h"
#include "spdlog/spdlog.h"

#pragma warning( disable : 4251 ) // needs to have dll-interface to be used by clients of class

namespace Confused
{
	class CONFUSED_API Logger
	{
	public:
		static void Initialize();

		[[nodiscard]] inline static const std::shared_ptr<_SPDLOG logger>& GetCoreLogger() noexcept { return m_pCoreLogger; }
		[[nodiscard]] inline static const std::shared_ptr<_SPDLOG logger>& GetClientLogger() noexcept { return m_pClientLogger; }

	private:
		static std::shared_ptr<_SPDLOG logger> m_pCoreLogger;
		static std::shared_ptr<_SPDLOG logger> m_pClientLogger;
	};
}