#include "mapper.hpp"

#include "mapper000.hpp"

#include <fstream>
#include <iostream>

MicroSim::Nes::Mapper *MicroSim::Nes::Mapper::mapper_from_ines(std::string filename)
{
	std::ifstream rom{filename};
	if(!rom.is_open()){
		std::cerr << "Unable to open ines file: " << filename << '\n';
		return nullptr;
	}

	//check magic bytes
	char c;
	std::uint8_t b;
	constexpr std::uint8_t magic[4] = {0x4E, 0x45, 0x53, 0x1A};
	for(auto i = 0; i < 4; i++){
		rom.get(c); //0-3
		b = c;
		if(b != magic[i]){
			std::cerr  << filename << " is not a valid ines ROM file\n";
			return nullptr;
		}
	}
	rom.get(c); //4
	std::uint8_t prg_rom_size = c;
	rom.get(c); //5
	std::uint8_t chr_rom_size = c;
	rom.get(c); //6
	Mirroring mirror;
	bool has_persistent_ram;
	bool has_trainer;
	bool ignore_mirror;
	std::uint16_t mapper_no = 0;
	mirror = static_cast<Mirroring>(c&0x01);
	has_persistent_ram = c&0x02;
	has_trainer = c&0x04;
	ignore_mirror = c&0x08;

	mapper_no |= (c>>4)&0x0F;
	bool vs_unisystem;
	bool playchoice_10;
	rom.get(c); //7
	vs_unisystem = c&0x01;
	playchoice_10 = c&0x02;
	mapper_no |= (c)&0x0F;
	rom.get(c); //8
	rom.get(c); //9
	rom.get(c); //10
	rom.get(c); //11
	rom.get(c); //12
	rom.get(c); //13
	rom.get(c); //14
	rom.get(c); //15

	std::uint8_t trainer[512];
	if(has_trainer){
		for(auto i = 0; i < 512; i++){
			rom.get(c);
			trainer[i] = c;
		}
	}
	

	switch (mapper_no)
	{
	case 0x00:{

		auto mapper = new Mapper000(prg_rom_size>1, mirror);
		auto prg = static_cast<MicroSim::Rom*>(mapper->prg());
		prg->fill(rom);
		auto chr = static_cast<MicroSim::Rom*>(mapper->chr());
		chr->fill(rom);
		return mapper;
	}
		break;
	
	default:
		break;
	}

	return nullptr;
}

#ifdef WASM
#include <emscripten/bind.h>
EMSCRIPTEN_BINDINGS(mapper){
	emscripten::class_<MicroSim::Nes::Mapper>("mapper")
		.function("prg", &MicroSim::Nes::Mapper::prg, emscripten::allow_raw_pointers())
		.function("ram", &MicroSim::Nes::Mapper::ram, emscripten::allow_raw_pointers())
		.function("chr", &MicroSim::Nes::Mapper::chr, emscripten::allow_raw_pointers())
		.class_function("mapper", &MicroSim::Nes::Mapper::mapper_from_ines, emscripten::allow_raw_pointers())
		;
}
#endif