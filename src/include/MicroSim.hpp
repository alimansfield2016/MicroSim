#pragma once

#include <cstdint>
#include <utility>
#include <iostream>

namespace MicroSim
{
	template<typename T>
	struct uint_half{ };
	// struct uint_half{ using value_type = void; };
	template<>
	struct uint_half<std::uint16_t>{ using type = std::uint8_t; };
	template<>
	struct uint_half<std::uint32_t>{ using type = std::uint16_t; };
	template<>
	struct uint_half<std::uint64_t>{ using type = std::uint32_t; };

	using Byte = std::uint8_t;
	using Word = std::uint16_t;
	using DWord = std::uint32_t;
	using QWord = std::uint64_t;

	template<typename T>
	typename uint_half<T>::type &
	low(T &word)
	{
		return reinterpret_cast<typename uint_half<T>::type*>(&word)[0];
	}

	template<typename T>
	typename uint_half<T>::type &
	high(T &word)
	{
		return reinterpret_cast<typename uint_half<T>::type*>(&word)[1];
	}

	template<typename T>
	typename uint_half<T>::type
	low(const T &word)
	{
		return reinterpret_cast<typename uint_half<T>::type*>(&word)[0];
	}

	template<typename T>
	typename uint_half<T>::type
	high(const T &word)
	{
		return reinterpret_cast<typename uint_half<T>::type*>(&word)[1];
	}

	std::ostream& debug_print(std::ostream &stream);
	template<typename First, typename ...Rest>
	std::ostream& debug_print(std::ostream &stream, First && first, Rest && ...rest)
	{
		stream << std::forward<First>(first);
		return debug_print(stream, std::forward<Rest>(rest)...);
	}

} // namespace MicroSim
