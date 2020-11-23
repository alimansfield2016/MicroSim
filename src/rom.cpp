#include "include/rom.hpp"


MicroSim::Rom::Rom(std::size_t n, Addr low) : 
	MemoryDevice{low, low+n, nullptr},
	m_data{new std::uint8_t[n]}
{
	MemoryDevice::m_data = m_data;
}
MicroSim::Rom::~Rom(){
	delete[] m_data;
}

void MicroSim::Rom::fill(std::uint8_t *data, std::size_t size, Addr offset)
{
	offset -= low();
	for(std::size_t i = 0; i < size; i++)
	{
		m_data[i+offset] = data[i];
	}
}
void MicroSim::Rom::fill(std::string filename){
	std::ifstream romFile{filename};
	if(!romFile.is_open()){
		std::cout << "Failed to open ./test\n";
		return;
	}
	for(std::size_t i = 0; i < size(); i++){
		char b;
		romFile.get(b);
		m_data[i] = b;
	}
}

void MicroSim::Rom::write_byte(Addr, MicroSim::Byte) {};
void MicroSim::Rom::write_word(Addr, MicroSim::Word) {};
void MicroSim::Rom::write_dword(Addr, MicroSim::DWord) {};
void MicroSim::Rom::write_qword(Addr, MicroSim::QWord) {};


#ifdef WASM
#include <emscripten/bind.h>

EMSCRIPTEN_BINDINGS(Rom){
	emscripten::class_<MicroSim::Rom, emscripten::base<MicroSim::MemoryDevice>>("Rom")
		.constructor<std::size_t, MicroSim::Addr>()
		.function("fill", emscripten::select_overload<void(std::string)>(&MicroSim::Rom::fill))
		;

}

#endif