module;
#include "Core.h"
#include "Logger.h"
#include "Macros.h"

#include <stdint.h>
#include <string>
#include <vector>
#include <concepts>
export module Confused.Utils;

import Confused.Singleton;

// Disable logging macros (pass logger by const ref to the functions)
#undef LOGGER
#undef LOGT
#undef LOGD
#undef LOGI
#undef LOGWARN
#undef LOGERROR
#undef LOGCRITICAL

template <typename OldT, typename NewT>
concept Constructable = requires(NewT e) { e = OldT(); };

namespace Confused
{
	class Application;

	export class CONFUSED_API Utils final : public Singleton<Utils>
	{
	public:
		class Internal final
		{
		private:
			friend class Application;

			// Initialize Utils
			static void Initialize(Utils& utils)
			{
				// Initialize random generator
				UNREFERENCED_PARAMETER(utils);
			}
		};

#pragma region Random

		//// Returns a random float
		//float Rand(float min, float max, uint32_t precision = 2) noexcept
		//{

		//}
		//// Returns a random int
		//int Rand(int min, int max) noexcept
		//{

		//}

#pragma endregion Random
#pragma region Containers

		// Converts a pointer into a vector
		// newT is the type of the elements (newT must have a constructor that takes T)
		template <typename newT, typename T> requires Constructable<T, newT>
		[[nodiscard]] std::vector<newT> ToVec(T* pData, uint32_t count) const noexcept
		{
			std::vector<newT> newVec{ count };

			for (uint32_t i = 0; i < count; i++)
			{
				newVec[i] = pData[i];
			}

			return std::move(newVec);
		}

		template <typename newT, typename oldT> requires Constructable<oldT, newT>
		[[nodiscard]] std::vector<newT> ChangeType(const std::vector<oldT>& oldVec) const noexcept
		{
			size_t count = oldVec.size();
			std::vector<newT> newVec{ count };

			for (size_t i = 0; i < count; i++)
			{
				newVec[i] = oldVec[i];
			}

			return std::move(newVec);
		}

		void PrintStrings(const std::vector<std::string>& vec, const std::string& description, const std::shared_ptr<_SPDLOG logger>& logger, _LOGLEVEL level_enum level = _LOGLEVEL debug) const
		{
			logger->log(level, "");

			logger->log(level, description + "(" + std::to_string(vec.size()) + "):");
			for (const std::string& element : vec)
				logger->log(level, "\t" + element);

			logger->log(level, "");
		}

#pragma endregion Containers
#pragma region Time

		[[nodiscard]] inline float DurationInMilliseconds(const _CHRONO steady_clock::time_point& start, const _CHRONO steady_clock::time_point& end) const noexcept
		{
			return _CHRONO duration<float, _STD nano>(end - start).count() / _STD micro::den;
		}
		[[nodiscard]] inline float DurationInSeconds(const _CHRONO steady_clock::time_point& start, const _CHRONO steady_clock::time_point& end) const noexcept
		{
			return _CHRONO duration<float, _STD nano>(end - start).count() / _STD nano::den;
		}

#pragma endregion Time

	private:
		Utils() = default;
		friend class Singleton<Utils>;
	};
}