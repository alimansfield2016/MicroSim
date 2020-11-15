#include "include/Core.hpp"
// #include "include/Simulation.hpp"
#include "Cores/WDC/65c02/65C02.hpp"

#include <array>
#include <fstream>
#include <iostream>
#include <thread>
#include <chrono>

using namespace std::chrono_literals;

const std::vector<MicroSim::CoreDescription> Cores{
	MicroSim::WDC::CoreW65C02S::s_description,
};

int main(int, char *[])
{

	//load the core for simulation
	//get the desired clock frequency
	MicroSim::Core *core;

	core = new MicroSim::WDC::CoreW65C02S();

	auto ram = std::make_unique<MicroSim::Ram<0x4000>>(0x0000);
	auto rom = std::make_unique<MicroSim::Rom<0x8000>>(0x8000);
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
	
	for(;;)
	{
		if(core->dec_cooldown() == 0){
			std::cout << core->registers() << '\n';
			core->step();
			std::this_thread::sleep_for(1s);
		}

	}


	return 0;
}