#pragma once

#include <cstdint>

namespace MicroSim
{
	template<typename T>
	struct uint_half{ };
	// struct uint_half{ using value_type = void; };
	template<>
	struct uint_half<std::uint16_t>{ using value_type = std::uint8_t; };
	template<>
	struct uint_half<std::uint32_t>{ using value_type = std::uint16_t; };
	template<>
	struct uint_half<std::uint64_t>{ using value_type = std::uint32_t; };

	using Byte = std::uint8_t;
	template<typename T>
	class _Word
	{
	public:
		using full_type = T;
		using half_type = typename uint_half<T>::value_type;
		static constexpr int full_size = sizeof(full_type);
		static constexpr int half_size = sizeof(half_type);

		constexpr _Word(full_type _v=0) : v{_v} {}
		constexpr _Word(_Word<half_type> w) : v{full_type{w}} {}

		_Word operator++() { return ++v; }
		_Word operator++(int) { return v++; }
		_Word operator--() { return --v; }
		_Word operator--(int) { return v--; }
		_Word& operator=(full_type _v) { v = _v; return *this; }

		// _Word& operator&

		// _Word& operator+=(_Word);
		// _Word& operator-=(_Word);


		operator full_type() const { return v; }
		operator full_type&() { return v; }

		_Word<half_type>& low() { return reinterpret_cast<_Word<half_type>*>(this)[0];}
		_Word<half_type>& high() { return reinterpret_cast<_Word<half_type>*>(this)[1];}

	private:
		full_type v;

	};
	template<>
	class _Word<std::uint8_t>
	{
	public:
		using full_type = std::uint8_t;
		static constexpr int full_size = sizeof(full_type);

		constexpr _Word(full_type _v=0) : v{_v} {}

		_Word operator++() { return ++v; }
		_Word operator++(int) { return v++; }
		_Word operator--() { return --v; }
		_Word operator--(int) { return v--; }
		_Word& operator=(full_type _v) { v = _v; return *this; }

		// _Word& operator&

		// _Word& operator+=(_Word);
		// _Word& operator-=(_Word);


		operator full_type() const { return v; }
		operator full_type&() { return v; }

	private:
		full_type v;

	};
	using Word = _Word<std::uint16_t>;
	using DWord = _Word<std::uint32_t>;
	using QWord = _Word<std::uint64_t>;
	
} // namespace MicroSim
