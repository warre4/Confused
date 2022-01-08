module;
#include "Core.h"
#include <chrono>
#include <iostream>
export module Confused.Time;

import Confused.Application;
import Confused.Singleton;

using namespace std::chrono;

namespace Confused
{
	export class CONFUSED_API Time final : public Singleton<Time>
	{
	public:
		steady_clock::time_point GetCurrent() const
		{
			return high_resolution_clock::now();
		}

		void PrintDuration(const std::chrono::steady_clock::time_point& start, const std::chrono::steady_clock::time_point& end, const std::string& text) const
		{
			const float ms = duration_cast<microseconds>(end - start).count() / 1000.f;
			std::cout << "'" << text << "': " << ms << " ms\n";
		}

		float GetElapsed() const
		{
			return m_ElapsedSec;
		}

	private:
		friend class Singleton<Time>;
		Time() = default;

		friend void Application::Run();
		void SetElapsed(float elapsedSec)
		{
			m_ElapsedSec = elapsedSec;
		}

		float m_ElapsedSec;
	};
}