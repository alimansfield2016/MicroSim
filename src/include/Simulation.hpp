#pragma once

#include "Core.hpp"
#include <memory>

namespace MicroSim
{
	class Simulation
	{
		MicroSim::Core *m_primary_core;
		unsigned long int m_frequency;
	public:
		Simulation(MicroSim::Core *core, unsigned long int _frequency);
		void step_instr(std::size_t count = 0);
		void step_cycles(std::size_t count = 1);
		MicroSim::Core *primary_core();
	};
} // namespace MicroSim
