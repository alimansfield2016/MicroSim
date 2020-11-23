#include "include/Core.hpp"

unsigned long int MicroSim::Core::cycles() const
{
	return m_cycles;
}

MicroSim::Core::Core(const std::initializer_list<std::tuple<std::string, std::size_t, void*>> & _registers, unsigned long int _frequency) : 
	MicroSim::Device{_frequency},
	m_cycles{0},
	// m_cooldown{0},
	m_registers{_registers}
{}

MicroSim::Core::~Core()
{

}

void MicroSim::Core::clock_mem()
{
	m_memory.clock();
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

// void MicroSim::Core::register_memory_device(std::shared_ptr<MicroSim::MemoryDevice> dev)
void MicroSim::Core::register_memory_device(MicroSim::MemoryDevice *dev)
{
	m_memory.add_device(dev);
}
// void MicroSim::Core::unregister_memory_device(std::shared_ptr<MicroSim::MemoryDevice> dev)
void MicroSim::Core::unregister_memory_device(MicroSim::MemoryDevice *dev)
{
	m_memory.remove_device(dev);
}

#ifdef WASM
#include <emscripten/bind.h>

EMSCRIPTEN_BINDINGS(core){
	emscripten::class_<MicroSim::Core, emscripten::base<MicroSim::Device> >("Core")
		.function("registers", &MicroSim::Core::registers)
		.function("cycles", &MicroSim::Core::cycles)
		.function("register_memory_device", &MicroSim::Core::register_memory_device, emscripten::allow_raw_pointers())
		// .smart_ptr<std::shared_ptr<MicroSim::Core>>("Core")
		// .function("memory", &MicroSim::Core::memory)
		// .smart_ptr<std::unique_ptr<MicroSim::Core>>("Core")
		;
	emscripten::smart_ptr_trait<std::unique_ptr<MicroSim::Core>>();
	// emscripten::internal::_embind_register_smart_ptr(
	// 	emscripten::internal::TypeID<std::unique_ptr<MicroSim::Core>>::get(),
	// 	emscripten::internal::TypeID<MicroSim::Core>::get(),
	// 	"std::unique_ptr<MicroSim::Core>",
	// 	emscripten::sharing_policy::NONE,
	// 	"Core",
	// 	&std::unique_ptr<MicroSim::Core>::get)
}

#endif