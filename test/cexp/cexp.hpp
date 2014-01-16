/* http://stackoverflow.com/a/19877048/778858 */

#include <cstdint>
#include <cmath>
#include <type_traits>
#include <limits>


namespace cexp
{
	constexpr float modulo_fn(float a, float b)
	{
		return std::fmod(a, b);
	}

	template <typename T, typename U>
	constexpr T modulo_impl(T a, U b, std::true_type, std::true_type)
	{ return a % b; }

	template <typename T, typename U>
	constexpr U modulo_impl(T a, U b, std::true_type, std::false_type)
	{ return modulo_fn(static_cast<U>(a), b); }

	template <typename T, typename U>
	constexpr T modulo_impl(T a, U b, std::false_type, std::true_type)
	{ return modulo_fn(a, static_cast<T>(b)); }

	template <typename T, typename U>
	constexpr T modulo_impl(T a, U b, std::false_type, std::false_type)
	{ return modulo_fn(a, b); }

	template <typename T, typename U>
	constexpr bool modulo(T a, U b)
	{
		return modulo_impl<T, U>(a, b, std::is_integral<T>(), std::is_integral<U>());
	}


	template <typename T, typename U>
	constexpr auto pow(T base, U exponent) -> decltype(base + exponent)
	{
		return (exponent < 0)
				? base * (base * (1.0f / pow(base, (exponent*-1)+2)))
			: (exponent == 0)
				? 1
			: (modulo(exponent, 2) == 0)
				? pow(base, exponent/2) * pow(base, exponent/2)
			: base * pow(base, exponent-1);
	}

	template <unsigned int N>
	constexpr auto exp2() -> decltype(1 << N)
	{
		static_assert(sizeof(unsigned int) < 1 << N, "lol");
		return 1 << N;
	}
}