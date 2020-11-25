#include "include/Core.hpp"
#include "include/Simulation.hpp"
#include "Cores/WDC/W65C02S/W65C02S_simple/W65C02S_simple.hpp"
#include "Cores/WDC/W65C02S/W65C02S_timed/W65C02S_timed.hpp"
#include "Devices/Hitachi/HD44780U/HD44780U.hpp"

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

// using clock = std::chrono::time_point<std::chrono::high_resolution_clock>;

struct Timer{
	std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now();
	const char* name;
	Timer(const char* _name) : name{_name}{

	}
	~Timer(){
		auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration<double, std::ratio<1, 1000>>(end - start).count();
		std::cout << name << " took: " << duration << "ms\n";
	}
};

void profile(std::string name, MicroSim::Simulation *sim)
{
	constexpr int num_profiles = 7;
	constexpr int num_avg = 10;
	double times[num_profiles] = {};

	for(auto i = 0; i < num_profiles; i++){
		auto steps = 1;
		for(auto j = 0; j < i; j++)
			steps *= 10;
		for(auto avg = 0; avg < num_avg; avg++){
			sim->reset();

			auto start = std::chrono::high_resolution_clock::now();
			sim->run_cycles(steps);
			auto end = std::chrono::high_resolution_clock::now();
			auto time = std::chrono::duration<double, std::ratio<1, 1000>>(end - start).count();
			times[i] += time;
		}
		times[i] /= num_avg;
	}
	auto steps = 1;
	for(auto i = 0; i < num_profiles; i++)
	{
		std::cout << name << " (" << steps << "): " << times[i] << "ms\n";
		steps *= 10;
	}
}

int main(int, char *[])
{
	Timer  main_timer{"Main"};
	// std::unique_ptr<MicroSim::WDC::CoreW65C02S_simple> simple_core;
	std::unique_ptr<MicroSim::Core> simple_core;
	std::unique_ptr<MicroSim::Core> timed_core;
	//load the core for simulation
	simple_core = std::make_unique<MicroSim::WDC::CoreW65C02S_simple>(1000000);
	timed_core = std::make_unique<MicroSim::WDC::CoreW65C02S_timed>(1000000);
	//create the memory devices
	auto ram = std::make_unique<MicroSim::Ram>(0x4000, 0x0000, 0x3FFF, 0x3FFF);
	auto rom = std::make_unique<MicroSim::Rom>(0x8000, 0x8000, 0xFFFF, 0x7FFF);
	auto lcd = std::make_unique<MicroSim::Hitachi::MemoryDeviceHD44780U>(0x4000, 2);
	std::ifstream romFile{"./Roms/65xx/test.rom"};
	if(!romFile.is_open()){
		std::cout << "Failed to open ./Roms/65xx/test.rom\n";
		return 0;
	}
	rom->fill(romFile);
	std::unique_ptr<MicroSim::Simulation>simple_sim = std::make_unique<MicroSim::Simulation>(simple_core.get());
	std::unique_ptr<MicroSim::Simulation>timed_sim = std::make_unique<MicroSim::Simulation>(timed_core.get());
	
	simple_core->register_memory_device(ram.get());
	simple_sim->add_device(ram.get());
	simple_core->register_memory_device(rom.get());
	simple_sim->add_device(rom.get());
	simple_core->register_memory_device(lcd.get());
	simple_sim->add_device(lcd.get());
	
	timed_core->register_memory_device(ram.get());
	timed_sim->add_device(ram.get());
	timed_core->register_memory_device(rom.get());
	timed_sim->add_device(rom.get());
	timed_core->register_memory_device(lcd.get());
	timed_sim->add_device(lcd.get());

	// rom.release();
	// ram.release();
	// lcd.release();
	simple_sim->reset();
	timed_sim->reset();

	// profile("simple", simple_sim.get());
	// profile("timed", timed_sim.get());

	return 0;
}

MicroSim::Simulation *global_simulation() { return g_simulation; }
std::ostream& MicroSim::debug_print(std::ostream &stream){ return stream; }
#ifdef WASM
#include <emscripten/bind.h>

EMSCRIPTEN_BINDINGS(main)
{
	emscripten::function("global_simulation", &global_simulation, emscripten::allow_raw_pointers());
	emscripten::function("profile_simulation", &profile, emscripten::allow_raw_pointers());
}


EMSCRIPTEN_BINDINGS(MicroSim){
}

#endif