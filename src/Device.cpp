#include "include/Device.hpp"


#ifdef WASM

#include <emscripten/bind.h>
EMSCRIPTEN_BINDINGS(Device){
	emscripten::class_<MicroSim::Device>("Device")
		.function("clock", &MicroSim::Device::clock)
		.function("reset", &MicroSim::Device::reset)
		.function("frequency", &MicroSim::Device::frequency)
		;
}

#endif