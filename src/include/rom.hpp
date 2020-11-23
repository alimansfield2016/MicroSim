#pragma once

#include "Memory.hpp"
#include <fstream>
#include <iostream>

namespace MicroSim
{

	class Rom : public MemoryDevice
	{
		std::uint8_t *m_data;
	public:
		Rom(std::size_t n, Addr low);
		~Rom() override;

		void fill(std::uint8_t *data, std::size_t size, Addr offset);
		void fill(std::string filename);

		void write_byte(Addr, MicroSim::Byte) override;
		void write_word(Addr, MicroSim::Word) override;
		void write_dword(Addr, MicroSim::DWord) override;
		void write_qword(Addr, MicroSim::QWord) override;

	};

	// template<std::size_t N>
	// class Rom : public _Rom
	// {
	// 	friend class _Rom;
	// 	std::uint8_t m_data[N];
	// public:
	// 	Rom(Addr low) : _Rom{low, low+N, m_data}{}

	// };


} // namespace MicroSim
