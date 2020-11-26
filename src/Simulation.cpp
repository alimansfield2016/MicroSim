#include "include/Simulation.hpp"

MicroSim::Simulation::Simulation(/* MicroSim::Core *core */) : 
	// m_primary_core{core},
	m_frequency{0},
	m_clocked_devices{},
	m_timed_dispatch{},
	m_devices{}
{
	// add_device(m_primary_core);
	// update_frequency();
}

void MicroSim::Simulation::update_frequency()
{
	//set frequency to some multiple of the highest frequency
	unsigned long int max_freq = 0;
	for(auto &[timeout, max_timeout, dev] : m_clocked_devices)
		max_freq = std::max(max_freq, dev->frequency());

	m_frequency = max_freq;

	for(auto &[timeout, max_timeout, dev] : m_clocked_devices){
		//set the max timeouts
		max_timeout = max_freq/dev->frequency();
		timeout = 0;
	}


	
}

// MicroSim::Simulation::Simulation(std::unique_ptr<MicroSim::Core> &&core) : 
// 	m_primary_core{std::move(core)},
// 	m_frequency{0}
// {}

void MicroSim::Simulation::run_cycles(std::size_t count)
{
	// debug_print(std::cout, "running for ", count, " cycles\n");
	while(count--){
		float s = -1.0;
		while(true){
			auto e = m_timed_dispatch[0];
			auto &[timeout, max_timeout, dev] = *e;
			if((timeout+=s) >= 0)
				break;
			s = timeout;
			
			dev->clock();

			timeout = max_timeout;
			bool spliced = false;
			auto idx = 1;
			for(; idx < m_timed_dispatch.size(); idx++){
				auto _e = m_timed_dispatch[idx];
				auto &[_timeout, _max_timeout, _dev] = *_e;
				if((timeout - _timeout) >= 0){
					m_timed_dispatch[idx-1] = m_timed_dispatch[idx];
					timeout -= _timeout;
					continue;
				}
				m_timed_dispatch[idx-1] = e;
				_timeout -= timeout;
				spliced = true;
				break;
			}
			if(!spliced)
				m_timed_dispatch[idx-1] = e;
			/* 
				for(auto _e = std::next(m_clocked_devices.begin()); _e != m_clocked_devices.end(); _e++){
					auto &[_timeout, _max_timeout, _dev] = *_e;
					if((timeout - _timeout) <= 0){
						m_clocked_devices.splice(_e, m_clocked_devices, e);
						_timeout -= timeout;
						spliced = true;
						break;
					}
					else
						timeout -= _timeout;
				}
			
				if(!spliced){
					m_clocked_devices.splice(m_clocked_devices.end(), m_clocked_devices, e);
				} */
			
		}
		// m_primary_core->clock();
		// m_primary_core->memory().clock();
	}
}

void MicroSim::Simulation::run_time(double s)
{
	// debug_print(std::cout, "running for ", s, " s\n");
	unsigned long int cycles = (m_frequency * s);
	run_cycles(cycles);
}

// void MicroSim::Simulation::step_instr(std::size_t count)
// {}

void MicroSim::Simulation::reset()
{
	// m_primary_core->reset();
	for(auto &d : m_devices) d->reset();
}

// void MicroSim::Simulation::set_core_frequency(unsigned long int _freq)
// {
// 	m_primary_core->set_frequency(_freq);
// 	update_frequency();
// }

// std::string MicroSim::Simulation::core_registers() const
// {
// 	return m_primary_core->registers();
// }

unsigned long int MicroSim::Simulation::frequency() const
{
	return m_frequency;
}

void MicroSim::Simulation::add_device(MicroSim::Device *dev)
{
	if(dev->frequency()){
		std::cout << "adding clocked device\n";
		auto &t = m_clocked_devices.emplace_back(0, 0, dev);
		m_timed_dispatch.emplace_back(&t);
		update_frequency();
	}
	m_devices.emplace_back(dev);
}
// void MicroSim::Simulation::add_memory_device(MicroSim::MemoryDevice *dev)
// {
// 	if(dev->frequency()){
// 		m_clocked_devices.emplace_back(0, 0, dev);
// 		update_frequency();
// 	}else{
// 		m_unclocked_devices.push_back(dev);
// 	}
// 	m_primary_core->register_memory_device(dev);
// }

// MicroSim::Core *MicroSim::Simulation::primary_core()
// {
// 	return m_primary_core;
// 	// return m_primary_core.get();
// }

// MicroSim::Memory *MicroSim::Simulation::core_memory()
// {
// 	return &m_primary_core->memory();
// }

// MicroSim::Byte MicroSim::Simulation::read_byte(MicroSim::Addr _addr) const
// {
// 	return m_primary_core->memory().read_byte_const(_addr);
// }
// MicroSim::Word MicroSim::Simulation::read_word(MicroSim::Addr _addr) const
// {
// 	return m_primary_core->memory().read_word_const(_addr);
// }
// MicroSim::DWord MicroSim::Simulation::read_dword(MicroSim::Addr _addr) const
// {
// 	return m_primary_core->memory().read_dword_const(_addr);
// }
// MicroSim::QWord MicroSim::Simulation::read_qword(MicroSim::Addr _addr) const
// {
// 	return m_primary_core->memory().read_qword_const(_addr);
// }

// void MicroSim::Simulation::write_byte(MicroSim::Addr _addr, MicroSim::Byte v)
// {
// 	m_primary_core->memory().write_byte_override(_addr, v);
// }
// void MicroSim::Simulation::write_word(MicroSim::Addr _addr, MicroSim::Word v)
// {
// 	m_primary_core->memory().write_word_override(_addr, v);
// }
// void MicroSim::Simulation::write_dword(MicroSim::Addr _addr, MicroSim::DWord v)
// {
// 	m_primary_core->memory().write_dword_override(_addr, v);
// }
// void MicroSim::Simulation::write_qword(MicroSim::Addr _addr, MicroSim::QWord v)
// {
// 	m_primary_core->memory().write_qword_override(_addr, v);
// }

#ifdef WASM
#include <emscripten/bind.h>

EMSCRIPTEN_BINDINGS(simulation){
	emscripten::class_<MicroSim::Simulation>("Simulation")
		// .constructor<std::unique_ptr<MicroSim::Core>&&>()
		.constructor</* MicroSim::Core* */>()
		.function("run_cycles", &MicroSim::Simulation::run_cycles)
		.function("run_seconds", &MicroSim::Simulation::run_time)
		.function("reset", &MicroSim::Simulation::reset)
		.function("frequency", &MicroSim::Simulation::frequency)
		.function("update_frequency", &MicroSim::Simulation::update_frequency)
		// .function("set_core_frequency", &MicroSim::Simulation::set_core_frequency)
		// .function("core_registers", &MicroSim::Simulation::core_registers)
		.function("add_device", &MicroSim::Simulation::add_device, emscripten::allow_raw_pointers())
		// .function("add_memory_device", &MicroSim::Simulation::add_memory_device, emscripten::allow_raw_pointers())
		// .function("read_byte", &MicroSim::Simulation::read_byte)
		// .function("read_word", &MicroSim::Simulation::read_word)
		// .function("read_dword", &MicroSim::Simulation::read_dword)
		// .function("read_qword", &MicroSim::Simulation::read_qword)
		// .function("write_byte", &MicroSim::Simulation::write_byte)
		// .function("write_word", &MicroSim::Simulation::write_word)
		// .function("write_dword", &MicroSim::Simulation::write_dword)
		// .function("write_qword", &MicroSim::Simulation::write_qword)
		;
}

#endif
