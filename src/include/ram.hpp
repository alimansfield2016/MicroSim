#pragma once

#include "Memory.hpp"

namespace MicroSim
{
	template<std::size_t N>
	class Ram : public MicroSim::MemoryDevice
	{
		std::uint8_t m_data[N];
	public:
		Ram(Addr low) : MemoryDevice{low, low+N, m_data}{}
	};
} // namespace MicroSim

