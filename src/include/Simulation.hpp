#pragma once

#include "Core.hpp"

#ifdef WASM

#include <emscripten/bind.h>

#endif

namespace MicroSim
{
	class Simulation
	{
	public:
		Simulation(MicroSim::Core &copy);
		void step_instr(std::size_t count = 0);
		void step_cycles(std::size_t count = 0);
	};
} // namespace MicroSim
