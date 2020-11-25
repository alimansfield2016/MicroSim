#include "2C02.hpp"

using namespace MicroSim::Ricoh;

Core2C02::Core2C02(unsigned long int _freq) : 
	MicroSim::Core{{},_freq},
	//Currently has fixed address
	MicroSim::MemoryDevice{0x2000, 0x4000, 0x07, nullptr}
	{ }

void Core2C02::clock(){}
void Core2C02::reset(){}

MicroSim::Byte Core2C02::read_byte(Addr _addr){ return 0; }
MicroSim::Byte Core2C02::read_byte_const(Addr _addr) const { return 0; }

void Core2C02::write_byte(Addr _addr, Byte b) {}
void Core2C02::write_byte_override(Addr _addr, Byte b) {}

#ifdef WASM
#include <emscripten/bind.h>
EMSCRIPTEN_BINDINGS(2C02){
	emscripten::class_<MicroSim::Ricoh::Core2C02,
						emscripten::base<MicroSim::Core>>("Core2C02")
		
		;
}
#endif