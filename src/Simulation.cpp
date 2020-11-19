#include "include/Simulation.hpp"

MicroSim::Simulation::Simulation(std::unique_ptr<MicroSim::Core> &&core, unsigned long int _frequency) : 
	m_primary_core{std::move(core)},
	m_frequency{_frequency}
{}

void MicroSim::Simulation::clock_cycles(std::size_t count)
{
	while(count--){
		m_primary_core->clock();
		m_primary_core->memory().clock();
	}
}

// void MicroSim::Simulation::step_instr(std::size_t count)
// {}

// MicroSim::Core *MicroSim::Simulation::primary_core()
// {
	// return m_primary_core;
// }

#ifdef WASM
#include <emscripten/bind.h>

EMSCRIPTEN_BINDINGS(simulation){
	emscripten::class_<MicroSim::Simulation>("Simulation")
		.constructor<std::unique_ptr<MicroSim::Core>&&, unsigned long int>()
		// .smart_ptr_constructor("Simulation", &std::make_shared<MicroSim::Simulation>)
		.function("clock_cycles", &MicroSim::Simulation::clock_cycles)
		// .function("step_instruction", &MicroSim::Simulation::step_instr)
		// .function("primary_core", &MicroSim::Simulation::primary_core, emscripten::allow_raw_pointers())
		;
	// emscripten::smart_ptr_trait<std::shared_ptr<MicroSim::Core>>();
}

#endif
