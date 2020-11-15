#include "include/Core.hpp"

unsigned long int MicroSim::Core::cycles() const
{
	return m_cycles;
}

unsigned short int MicroSim::Core::cooldown() const
{
	return m_cooldown;
}

unsigned short int MicroSim::Core::dec_cooldown()
{
	if(m_cooldown) --m_cooldown;
	return m_cooldown;
}

void MicroSim::Core::update_cycles()
{
	m_cycles += _cycles;
	m_cooldown += _cycles;
	_cycles = 0;
}
MicroSim::Core::Core(const std::initializer_list<std::tuple<std::string, std::size_t, void*>> & _registers) : 
	m_registers{_registers}
{}

MicroSim::Core::~Core()
{

}

#include <sstream>
#include <iomanip>

std::string MicroSim::Core::registers() const
{
	std::stringstream stream;
	for(auto &e : m_registers){
		stream << std::get<0>(e);
		auto size = std::get<1>(e);
		uint64_t val = *reinterpret_cast<uint64_t*>(std::get<2>(e));
		switch(size){
			case 1: stream  << ": 0x" << std::hex << std::internal << std::setfill('0') << std::setw(2) << (val & 0xFF); break;
			case 2: stream  << ": 0x" << std::hex << std::internal << std::setfill('0') << std::setw(4) << (val & 0xFFFF); break;
			case 4: stream  << ": 0x" << std::hex << std::internal << std::setfill('0') << std::setw(8) << (val & 0xFFFFFFFF); break;
			case 8: stream  << ": 0x" << std::hex << std::internal << std::setfill('0') << std::setw(16) << (val & 0xFFFFFFFFFFFFFFFF); break;
		}
		stream << '\n';
	}
	return stream.str();
}

#ifdef WASM
#include <emscripten/bind.h>

EMSCRIPTEN_BINDINGS(core){
	emscripten::class_<MicroSim::Core>("Core")
		.function("registers", &MicroSim::Core::registers)
		.function("step", &MicroSim::Core::step)
		.function("reset", &MicroSim::Core::reset)
		.function("memory", &MicroSim::Core::memory)
		.function("cycles", &MicroSim::Core::cycles)
		.function("cooldown", &MicroSim::Core::cooldown)
		.function("dec_cooldown", &MicroSim::Core::dec_cooldown)
		;
}

#endif