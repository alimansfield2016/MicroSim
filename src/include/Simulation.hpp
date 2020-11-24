#pragma once

#include "Core.hpp"
#include <memory>
#include <list>
#include <utility>

namespace MicroSim
{
	class Simulation
	{
		// std::unique_ptr<MicroSim::Core> m_primary_core;
		MicroSim::Core *m_primary_core;
		unsigned long int m_frequency;
		std::list<std::tuple<float, float, MicroSim::Device*>> m_clocked_devices;
		std::list<MicroSim::Device*> m_unclocked_devices;
	public:
		// Simulation(std::unique_ptr<MicroSim::Core> &&core);
		Simulation(MicroSim::Core *core);
		Simulation(Simulation&) = delete;
		void run_cycles(std::size_t count = 1);
		void run_time(double s);
		void reset();
		unsigned long int frequency() const;
		void update_frequency();
		void set_core_frequency(unsigned long int);
		std::string core_registers() const;

		void add_device(MicroSim::Device*);
		void add_memory_device(MicroSim::MemoryDevice*);


		MicroSim::Byte read_byte(MicroSim::Addr) const;
		MicroSim::Word read_word(MicroSim::Addr) const;
		MicroSim::DWord read_dword(MicroSim::Addr) const;
		MicroSim::QWord read_qword(MicroSim::Addr) const;
	
		void write_byte(MicroSim::Addr, MicroSim::Byte);
		void write_word(MicroSim::Addr, MicroSim::Word);
		void write_dword(MicroSim::Addr, MicroSim::DWord);
		void write_qword(MicroSim::Addr, MicroSim::QWord);

	

	};
} // namespace MicroSim
