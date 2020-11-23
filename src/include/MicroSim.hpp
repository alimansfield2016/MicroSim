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

	// template<typename T>
	// class _Word
	// {
	// public:
	// 	using full_type = T;
	// 	using half_type = typename uint_half<T>::value_type;
	// 	static constexpr int full_size = sizeof(full_type);
	// 	static constexpr int half_size = sizeof(half_type);

	// 	constexpr _Word(full_type _v=0) : v{_v} {}
	// 	constexpr _Word(_Word<half_type> w) : v{full_type{w}} {}

	// 	_Word operator++() { return ++v; }
	// 	_Word operator++(int) { return v++; }
	// 	_Word operator--() { return --v; }
	// 	_Word operator--(int) { return v--; }
	// 	_Word& operator=(full_type _v) { v = _v; return *this; }

	// 	// _Word& operator&

	// 	// _Word& operator+=(_Word);
	// 	// _Word& operator-=(_Word);


	// 	full_type val() const { return v; }
	// 	full_type& val() { return v; }
	// 	operator full_type() const { return v; }
	// 	operator full_type&() { return v; }

	// 	_Word<half_type>& low() { return reinterpret_cast<_Word<half_type>*>(this)[0];}
	// 	_Word<half_type>& high() { return reinterpret_cast<_Word<half_type>*>(this)[1];}

	// private:
	// 	full_type v;

	// };
	// template<>
	// class _Word<std::uint8_t>
	// {
	// public:
	// 	using full_type = std::uint8_t;
	// 	static constexpr int full_size = sizeof(full_type);

	// 	constexpr _Word(full_type _v=0) : v{_v} {}

	// 	_Word operator++() { return ++v; }
	// 	_Word operator++(int) { return v++; }
	// 	_Word operator--() { return --v; }
	// 	_Word operator--(int) { return v--; }
	// 	_Word& operator=(full_type _v) { v = _v; return *this; }

	// 	// _Word& operator&

	// 	// _Word& operator+=(_Word);
	// 	// _Word& operator-=(_Word);


	// 	full_type val() const { return v; }
	// 	full_type& val() { return v; }
	// 	operator full_type() const { return v; }
	// 	operator full_type&() { return v; }


	// private:
	// 	full_type v;

	// };
	// using Word = _Word<std::uint16_t>;
	// using DWord = _Word<std::uint32_t>;
	// using QWord = _Word<std::uint64_t>;
	
} // namespace MicroSim
