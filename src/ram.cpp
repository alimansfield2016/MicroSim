#include "include/ram.hpp"

MicroSim::Ram::Ram(std::size_t n, Addr low, Addr high, Addr mask) : 
	MemoryDevice{n, low, high, mask, nullptr},
	m_data{new std::uint8_t[n]}
{
	MemoryDevice::m_data = m_data;
}
MicroSim::Ram::~Ram(){
	delete[] m_data;
}

#ifdef WASM
#include <emscripten/bind.h>

EMSCRIPTEN_BINDINGS(Ram){
	emscripten::class_<MicroSim::Ram, emscripten::base<MicroSim::MemoryDevice>>("Ram")
		.constructor<std::size_t, MicroSim::Addr, MicroSim::Addr, MicroSim::Addr>()
		;
}

#endif