module;
#include "Core.h"

#include <stdint.h>
#include <string>
#include <vector>
#include <concepts>
export module Confused.Utils;

import Confused.Application;
import Confused.Singleton;

template <typename OldT, typename NewT>
concept Constructable = requires(NewT e) { e = OldT(); };

namespace Confused
{
	export class CONFUSED_API Utils final : public Singleton<Utils>
	{
	public:
		//// Returns a random float
		//export float Rand(float min, float max, uint32_t precision = 2)
		//{

		//}
		//// Returns a random int
		//export int Rand(int min, int max)
		//{

		//}

		// Converts a pointer into a vector
		// newT is the type of the elements (newT must have a constructor that takes T)
		template <typename newT, typename T> requires Constructable<T, newT>
		std::vector<newT> ToVec(T* pData, uint32_t count)
		{
			std::vector<newT> newVec{ count };

			for (uint32_t i = 0; i < count; i++)
			{
				newVec[i] = pData[i];
			}

			return std::move(newVec);
		}

	private:
		friend class Singleton<Utils>;
		Utils() = default;

		friend class Application;
		void Initialize();
	};
}