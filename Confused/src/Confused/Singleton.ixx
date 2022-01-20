module;

export module Confused.Singleton;

namespace Confused
{
	export
	template <typename T>
	class Singleton
	{
	public:
		[[nodiscard]] static T& GetInstance()
		{
			static T instance{};
			return instance;
		}

		virtual ~Singleton() = default;
		Singleton(const Singleton & other) = delete;
		Singleton(Singleton && other) = delete;
		Singleton& operator=(const Singleton & other) = delete;
		Singleton& operator=(Singleton && other) = delete;

	protected:
		Singleton() = default;
	};
}