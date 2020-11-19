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

int main(int, char *[])
{
	Timer  main_timer{"Main"};
	std::unique_ptr<MicroSim::WDC::CoreW65C02S_simple> simple_core;
	std::unique_ptr<MicroSim::WDC::CoreW65C02S_timed> timed_core;
	//load the core for simulation
	simple_core = std::make_unique<MicroSim::WDC::CoreW65C02S_simple>();
	timed_core = std::make_unique<MicroSim::WDC::CoreW65C02S_timed>();
	//get the desired clock frequency
	auto ram = std::make_unique<MicroSim::Ram>(0x4000, 0x0000);
	auto rom = std::make_unique<MicroSim::Rom>(0x8000, 0x8000);
	std::ifstream romFile{"./test"};
	if(!romFile.is_open()){
		std::cout << "Failed to open ./test\n";
		return 0;
	}
	std::array<char, 0x8000> rom_data;
	for(std::size_t i = 0; i < 0x8000; i++)
		romFile.get(rom_data[i]);

	rom->fill(reinterpret_cast<std::uint8_t*>(rom_data.begin()), rom_data.size(), 0x8000);
	simple_core->memory().add_device(std::move(ram));
	timed_core->memory().add_device(std::move(ram));
	simple_core->reset();
	timed_core->reset();
	// g_simulation = new MicroSim::Simulation{std::move(core), 1000000};
	// MicroSim::Simulation &sim = *g_simulation;
	std::unique_ptr<MicroSim::Simulation>simple_sim = std::make_unique<MicroSim::Simulation>(std::move(simple_core), 1000000);
	std::unique_ptr<MicroSim::Simulation>timed_sim = std::make_unique<MicroSim::Simulation>(std::move(timed_core), 1000000);
	double timed_times[9];
	double simple_times[9];

	for(auto i = 0; i < 8; i++){
		auto steps = 1;
		for(auto j = 0; j < i; j++)
			steps *= 10;
		for(auto avg = 0; avg < 10; avg++){
			{
				auto start = std::chrono::high_resolution_clock::now();
				simple_sim->clock_cycles(steps);
				
				auto end = std::chrono::high_resolution_clock::now();
				auto simple_time = std::chrono::duration<double, std::ratio<1, 1000>>(end - start).count();
				simple_times[i] *= avg;
				simple_times[i] += simple_time;
				simple_times[i] /= (avg+1);
			}
			{
				auto start = std::chrono::high_resolution_clock::now();
				timed_sim->clock_cycles(steps);
				
				auto end = std::chrono::high_resolution_clock::now();
				auto timed_time = std::chrono::duration<double, std::ratio<1, 1000>>(end - start).count();
				timed_times[i] *= avg;
				timed_times[i] += timed_time;
				timed_times[i] /= (avg+1);
			}
		}
	}
	for(auto i = 0; i < 8; i++)
	{
		auto steps = 1;
		for(auto j = 0; j < i; j++)
			steps *= 10;
		std::cout << "Simple (" << steps << "): " << simple_times[i] << "ms\n";
		std::cout << "Timed (" << steps << "): " << timed_times[i] << "ms\n";
	}

	return 0;
}

MicroSim::Simulation *global_simulation() { return g_simulation; }

#ifdef WASM
#include <emscripten/bind.h>

EMSCRIPTEN_BINDINGS(main)
{
	emscripten::function("global_simulation", &global_simulation, emscripten::allow_raw_pointers());
}

#endif