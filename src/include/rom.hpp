#pragma once

#include "Memory.hpp"

namespace MicroSim
{

	class Rom : public MemoryDevice
	{
		std::uint8_t *m_data;
	public:
		Rom(std::size_t n, Addr low) : 
			MemoryDevice{low, low+n, nullptr},
			m_data{new std::uint8_t[n]}
		{
			MemoryDevice::m_data = m_data;
		}

		void fill(std::uint8_t *data, std::size_t size, Addr offset)
		{
			offset -= low();
			for(std::size_t i = 0; i < size; i++)
			{
				m_data[i+offset] = data[i];
			}
		}

		void write_byte(Addr, MicroSim::Byte) override {};
		void write_word(Addr, MicroSim::Word) override {};
		void write_dword(Addr, MicroSim::DWord) override {};
		void write_qword(Addr, MicroSim::QWord) override {};

		void override_write_byte(Addr addr, MicroSim::Byte d) { *reinterpret_cast<std::uint8_t*>(&m_data[addr-m_low]) = d; };
		void override_write_word(Addr addr, MicroSim::Word d) { *reinterpret_cast<std::uint16_t*>(&m_data[addr-m_low]) = d;  };
		void override_write_dword(Addr addr, MicroSim::DWord d) { *reinterpret_cast<std::uint32_t*>(&m_data[addr-m_low]) = d;  };
		void override_write_qword(Addr addr, MicroSim::QWord d) { *reinterpret_cast<std::uint64_t*>(&m_data[addr-m_low]) = d;  };
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
