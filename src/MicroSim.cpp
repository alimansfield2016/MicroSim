#include "include/Core.hpp"
#include "include/Simulation.hpp"
#include "Cores/WDC/6502/W65C02S_simple/W65C02S_simple.hpp"
#include "Cores/WDC/6502/W65C02S_timed/W65C02S_timed.hpp"

#include <array>
#include <fstream>
#include <iostream>
#include <thread>
#include <chrono>

#ifdef WASM
#include <emscripten.h>
#endif

using namespace std::chrono_literals;

const std::vector<MicroSim::CoreDescription> Cores{
	// MicroSim::WDC::CoreW65C02S_simple::s_description,
};


#ifdef WASM
#define sleep(_ms) emscripten_sleep(_ms);
#else
#define sleep(_ms) std::this_thread::sleep_for(_ms##ms);
#endif

MicroSim::Simulation *g_simulation;
MicroSim::Core *g_core;

int main(int, char *[])
{

	//load the core for simulation
	//get the desired clock frequency
	g_core = new MicroSim::WDC::CoreW65C02S_timed();
	MicroSim::Core *core = g_core;

	auto ram = std::make_unique<MicroSim::Ram>(0x4000, 0x0000);
	// auto ram = new MicroSim::Ram<0x4000>(0x0000);
	auto rom = std::make_unique<MicroSim::Rom>(0x8000, 0x8000);
	// auto rom = new MicroSim::Rom<0x8000>(0x8000);
	std::ifstream romFile{"./test"};
	if(!romFile.is_open()){
		std::cout << "Failed to open ./test\n";
		return 0;
	}
	std::array<char, 0x8000> rom_data;
	for(std::size_t i = 0; i < 0x8000; i++)
		romFile.get(rom_data[i]);

	rom->fill(reinterpret_cast<std::uint8_t*>(rom_data.begin()), rom_data.size(), 0x8000);

	core->memory().add_device(std::move(ram));
	core->memory().add_device(std::move(rom));
	core->reset();

	g_simulation = new MicroSim::Simulation{core, 1000000};
	MicroSim::Simulation &sim = *g_simulation;

	// std::cout << "sleeping for 1\n";
	// sleep(1000);
	// std::cout << "starting\n";
	
	// auto start = std::chrono::high_resolution_clock::now();
	// sim.step_cycles(10000000);
	// auto end = std::chrono::high_resolution_clock::now();
	// std::cout << std::chrono::duration<double, std::ratio<1, 1000000>>(end - start).count() << '\n';

	for(auto i = 0; i < 0x100; i++){
		// std::cout << sim.primary_core()->registers() << '\n';
		// std::cout << sim.primary_core()->cycles() << ' '; 
		sim.step_cycles();
	}


	return 0;
}

MicroSim::Simulation *global_simulation() { return g_simulation; }

#ifdef WASM
#include <emscripten/bind.h>

EMSCRIPTEN_BINDINGS(Main)
{
	emscripten::function("global_simulation", &global_simulation, emscripten::allow_raw_pointers());
}

#endif