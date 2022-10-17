module;
#include "General.h"

#include <stdint.h>
#include <string>
#include <sstream>
#include <vector>
#include <concepts>
#include <random>
#include <fstream>
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
#undef LOG

// Passing logger and loglevel through a function
// creates variables:
//=> logger
//=> loglevel
#define LOGGER_ARGS(defaultLevel) [[maybe_unused]] const std::shared_ptr<_SPDLOG logger>& logger, [[maybe_unused]] _LOGLEVEL level_enum loglevel = _LOGLEVEL defaultLevel
// uses LOGGER_ARGS to log
#define LOG(string) logger->log(loglevel, string)

#define RESOURCE_LOCATION "Resources/"

template <typename OldT, typename NewT>
concept Constructable = requires(NewT e) { e = OldT(); };

namespace Confused
{
	class Application;

	export class CONFUSED_API Utils final : public Singleton<Utils>
	{
	public:
#pragma region Random

		// Returns a random float
		template<typename TFloat> requires std::floating_point<TFloat>
		[[nodiscard]] inline TFloat Rand(TFloat min, TFloat max) noexcept
		{
			return std::uniform_real_distribution<TFloat>{ min, max }(m_RandomGenerator);
		}
		// Returns a random int
		template<typename TInt> requires std::integral<TInt>
		[[nodiscard]] inline TInt Rand(TInt min, TInt max) noexcept
		{
			return std::uniform_int_distribution<TInt>{ min, max }(m_RandomGenerator);
		}

		// Test random
		void TestRandomNumbers(LOGGER_ARGS(info))
		{
		#ifndef NDEBUG // Only works in debug
			// Test random
			LOG("Testing random generator");
			int nrloops{ 100 };

			int minI{ 0 }, maxI{ 100 };
			LOG("int (" + std::to_string(minI) + ", " + std::to_string(maxI) + "):");
			for (int i = 0; i < nrloops; i++)
			{
				LOG('\t' + std::to_string(Rand(minI, maxI)));
			}
			LOG("");

			float minF{ 15.f }, maxF{ 15.2f };
			LOG("float (" + std::to_string(minF) + ", " + std::to_string(maxF) + "):");
			for (int i = 0; i < nrloops; i++)
			{
				LOG('\t' + std::to_string(Rand(minF, maxF)));
			}
			LOG("");
		#endif
		}

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

			return newVec;
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

			return newVec;
		}

		void PrintStrings(const std::vector<std::string>& vec, const std::string& description, LOGGER_ARGS(debug)) const
		{
			LOG("");

			LOG(description + "(" + std::to_string(vec.size()) + "):");
			for (const std::string& element : vec)
				LOG("| " + element);

			LOG("");
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
#pragma region Files

		[[nodiscard]] std::vector<Byte> ReadFile(const std::string& fileName) const
		{
			std::ifstream file{ RESOURCE_LOCATION + fileName, std::ios::ate | std::ios::binary};

			if (!file.is_open())
				RTE("failed to open file! \n" + RESOURCE_LOCATION + fileName);

			const size_t fileSize = (size_t)file.tellg();
			std::vector<Byte> buffer(fileSize);

			file.seekg(0);
			file.read(reinterpret_cast<char*>(buffer.data()), fileSize);

			file.close();

			return buffer;
		}

#pragma endregion Files
#pragma region Enums

		std::string ToString(const Layer layer)
		{
			switch (layer)
			{
				// HUD
			case LAYER_HUD_0:		return "LAYER_HUD_0";
			case LAYER_HUD_1:		return "LAYER_HUD_1";

				// Particle
			case LAYER_PARTICLE_0:	return "LAYER_PARTICLE_0";

				// Normal
			case LAYER_NORMAL_0:	return "LAYER_NORMAL_0";
			case LAYER_NORMAL_1:	return "LAYER_NORMAL_1";

				// ...


				// Debug
			case LAYER_DEBUG_0:		return "LAYER_DEBUG_0";
			case LAYER_DEBUG_1:		return "LAYER_DEBUG_1";
			case LAYER_DEBUG_2:		return "LAYER_DEBUG_2";
			case LAYER_DEBUG_3:		return "LAYER_DEBUG_3";

				// Not implemented
			default: return "UNKNOWN_LAYER(error-not-implemented)";
			}
		}

#pragma endregion Enums

	private:
		friend class Internal;
		friend class Singleton<Utils>;

		Utils()
			// Initialize random generator
			: Singleton<Utils>()
			, m_RandomGenerator{ std::random_device{}() }
		{

		}

		std::mt19937 m_RandomGenerator;
	};
}