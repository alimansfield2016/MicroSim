#pragma once

#include "Memory.hpp"

namespace MicroSim
{
	class Ram : public MicroSim::MemoryDevice
	{
		std::uint8_t *m_data;
	public:
		Ram(std::size_t n, Addr low, Addr high, Addr mask);
		~Ram() override;
	};
} // namespace MicroSim

