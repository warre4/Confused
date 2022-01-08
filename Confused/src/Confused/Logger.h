#pragma once
#include <memory>

#include "Core.h"
#include "spdlog/spdlog.h"

// .h and .cpp because .ixx doesn't support #pragma warning
#pragma warning( push )
#pragma warning( disable : 4251 ) // needs to have dll-interface to be used by clients of class

namespace Confused
{
	class CONFUSED_API Logger
	{
	public:
		static void Initialize();

		inline static const std::shared_ptr<spdlog::logger>& GetCoreLogger() { return m_pCoreLogger; }
		inline static const std::shared_ptr<spdlog::logger>& GetClientLogger() { return m_pClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> m_pCoreLogger;
		static std::shared_ptr<spdlog::logger> m_pClientLogger;
	};
}

#pragma warning( pop )