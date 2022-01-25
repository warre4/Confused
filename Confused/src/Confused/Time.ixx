module;
#include "Core.h"
#include "Macros.h"
#include <chrono>
#include <iostream>
export module Confused.Time;

import Confused.Application;
import Confused.Singleton;

namespace Confused
{
	export class CONFUSED_API Time final : public Singleton<Time>
	{
	public:
		[[nodiscard]] inline _CHRONO steady_clock::time_point GetCurrent() const noexcept
		{
			return std::chrono::high_resolution_clock::now();
		}

		[[nodiscard]] float GetElapsed() const noexcept
		{
			return m_ElapsedSec;
		}

	private:
		friend class Singleton<Time>;
		Time() = default;

		friend void Application::Run();
		void SetElapsed(float elapsedSec) noexcept
		{
			m_ElapsedSec = elapsedSec;
		}

		float m_ElapsedSec;
	};
}