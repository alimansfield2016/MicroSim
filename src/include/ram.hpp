#pragma once

#include "Memory.hpp"

namespace MicroSim
{
	class Ram : public MicroSim::MemoryDevice
	{
		std::uint8_t *m_data;
	public:
		Ram(std::size_t n, Addr low) : 
			MemoryDevice{low, low+n, nullptr},
			m_data{new std::uint8_t[n]}
		{
			MemoryDevice::m_data = m_data;
		}
	};
} // namespace MicroSim

