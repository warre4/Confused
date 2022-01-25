module;

export module Confused.Singleton;

namespace Confused
{
	export
	template <typename T>
	class Singleton
	{
	public:
		[[nodiscard]] static T& GetInstance() noexcept
		{
			static T instance{};
			return instance;
		}

		virtual ~Singleton() = default;
		Singleton(const Singleton & other) = delete;
		Singleton(Singleton && other) noexcept = delete;
		Singleton& operator=(const Singleton & other) = delete;
		Singleton& operator=(Singleton && other) noexcept = delete;

	protected:
		Singleton() = default;
	};
}