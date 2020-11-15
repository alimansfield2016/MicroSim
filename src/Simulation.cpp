#include "include/Simulation.hpp"

MicroSim::Simulation::Simulation(MicroSim::Core &core)
{}

void MicroSim::Simulation::step_cycles(std::size_t count)
{}

void MicroSim::Simulation::step_instr(std::size_t count)
{}

#ifdef WASM
// #if 0

EMSCRIPTEN_BINDINGS(simulation){
	emscripten::class_<MicroSim::Simulation>("Simulation")
		.constructor<MicroSim::Core&>()
		.function("step_cycles", &MicroSim::Simulation::step_cycles)
		.function("step_instruction", &MicroSim::Simulation::step_instr)
		;
}

#endif
