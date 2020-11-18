#include "include/Simulation.hpp"

MicroSim::Simulation::Simulation(MicroSim::Core *core, unsigned long int _frequency) : 
	m_primary_core{core},
	m_frequency{_frequency}
{}

void MicroSim::Simulation::step_cycles(std::size_t count)
{
	while(count--){
		if(!m_primary_core->dec_cooldown())
			m_primary_core->step();
	}
}

void MicroSim::Simulation::step_instr(std::size_t count)
{}

MicroSim::Core *MicroSim::Simulation::primary_core()
{
	return m_primary_core;
}

#ifdef WASM
#include <emscripten/bind.h>

EMSCRIPTEN_BINDINGS(simulation){
	emscripten::class_<MicroSim::Simulation>("Simulation")
		.constructor<MicroSim::Core*, unsigned long int>()
		// .smart_ptr_constructor("Simulation", &std::make_shared<MicroSim::Simulation>)
		.function("step_cycles", &MicroSim::Simulation::step_cycles)
		.function("step_instruction", &MicroSim::Simulation::step_instr)
		.function("primary_core", &MicroSim::Simulation::primary_core, emscripten::allow_raw_pointers())
		;
	// emscripten::smart_ptr_trait<std::shared_ptr<MicroSim::Core>>();
}

#endif
