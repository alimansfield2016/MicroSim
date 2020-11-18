#include "include/Memory.hpp"

#include <algorithm>

#include <iostream>
#include <iomanip>

#define DEBUG


std::uint8_t MicroSim::MemoryDevice::read_byte(Addr addr)
{
	return *reinterpret_cast<std::uint8_t*>(&m_data[addr-m_low]);
}
std::uint16_t MicroSim::MemoryDevice::read_word(Addr addr)
{
	return *reinterpret_cast<std::uint16_t*>(&m_data[addr-m_low]);
}
std::uint32_t MicroSim::MemoryDevice::read_dword(Addr addr)
{
	return *reinterpret_cast<std::uint32_t*>(&m_data[addr-m_low]);
}
std::uint64_t MicroSim::MemoryDevice::read_qword(Addr addr)
{
	return *reinterpret_cast<std::uint64_t*>(&m_data[addr-m_low]);
}

void MicroSim::MemoryDevice::write_byte(Addr addr, std::uint8_t d)
{
	*reinterpret_cast<std::uint8_t*>(&m_data[addr-m_low]) = d;
}
void MicroSim::MemoryDevice::write_word(Addr addr, std::uint16_t d)
{
	*reinterpret_cast<std::uint16_t*>(&m_data[addr-m_low]) = d;
}
void MicroSim::MemoryDevice::write_dword(Addr addr, std::uint32_t d)
{
	*reinterpret_cast<std::uint32_t*>(&m_data[addr-m_low]) = d;
}
void MicroSim::MemoryDevice::write_qword(Addr addr, std::uint64_t d)
{
	*reinterpret_cast<std::uint64_t*>(&m_data[addr-m_low]) = d;
}

std::uint8_t MicroSim::DefaultMemDev::read_byte(Addr)
{
	return 0;
}
std::uint16_t MicroSim::DefaultMemDev::read_word(Addr)
{
	return 0;
}
std::uint32_t MicroSim::DefaultMemDev::read_dword(Addr)
{
	return 0;
}
std::uint64_t MicroSim::DefaultMemDev::read_qword(Addr)
{
	return 0;
}

void MicroSim::DefaultMemDev::write_byte(Addr, std::uint8_t)
{
}
void MicroSim::DefaultMemDev::write_word(Addr, std::uint16_t)
{
}
void MicroSim::DefaultMemDev::write_dword(Addr, std::uint32_t)
{
}
void MicroSim::DefaultMemDev::write_qword(Addr, std::uint64_t)
{
}
MicroSim::DefaultMemDev MicroSim::DefaultMemDev::s_defaultMemDev;

MicroSim::MemoryDevice &MicroSim::Memory::device_at(Addr _addr)
{
	for(auto &it : m_devices)
	{
		if(it->low() > _addr) continue;
		if(it->high() < _addr) continue;
		return *it;
	}
	return DefaultMemDev::s_defaultMemDev;
}

void MicroSim::Memory::add_device(std::shared_ptr<MemoryDevice> &&dev)
// void MicroSim::Memory::add_device(MemoryDevice *dev)
{
	auto priority = dev->priority();
	for(auto it = m_devices.begin(); it != m_devices.end(); it++)
	{
		if(priority > (*it)->priority()){
			// m_devices.insert(it, dev);
			m_devices.insert(it, std::move(dev));
			break;
		}
	}
	m_devices.push_back(std::move(dev));
	// m_devices.push_back(dev);

}
void MicroSim::Memory::remove_device(std::shared_ptr<MemoryDevice> &&dev)
// void MicroSim::Memory::remove_device(MemoryDevice *dev)
{
	std::remove_if(m_devices.begin(), m_devices.end(), [&](auto &e){return e == dev; });
}

std::uint8_t MicroSim::Memory::read_byte_const(Addr addr) const
{
	return 0;
}
std::uint16_t MicroSim::Memory::read_word_const(Addr addr) const
{
	return 0;
}
std::uint32_t MicroSim::Memory::read_dword_const(Addr addr) const
{
	return 0;
}
std::uint64_t MicroSim::Memory::read_qword_const(Addr addr) const
{
	return 0;
}

std::uint8_t MicroSim::Memory::read_byte(Addr addr)
{
	std::uint8_t v = device_at(addr).read_byte(addr);
	#ifdef DEBUG
	std::cout 	<< "Read Byte " 
				<< std::showbase 
				<< std::internal 
				<< std::setfill('0')
				<< std::hex
				<< std::setw(4)
				<< addr
				<< ":"
				<< std::setw(2)
				<< (v&0xFF)
				<< '\n';
	#endif
	return v;
}
std::uint16_t MicroSim::Memory::read_word(Addr addr)
{
	auto v = device_at(addr).read_word(addr);
	return v;
}
std::uint32_t MicroSim::Memory::read_dword(Addr addr)
{
	auto v = device_at(addr).read_dword(addr);
	return v;
}
std::uint64_t MicroSim::Memory::read_qword(Addr addr)
{
	auto v = device_at(addr).read_qword(addr);
	return v;
}

void MicroSim::Memory::write_byte(Addr addr, std::uint8_t d)
{
	#ifdef DEBUG
	std::cout 	<< "Write Byte " 
				<< std::showbase 
				<< std::internal 
				<< std::setfill('0')
				<< std::hex
				<< std::setw(4)
				<< addr
				<< ":"
				<< std::setw(2)
				<< (d&0xFF)
				<< '\n';
	#endif
	device_at(addr).write_byte(addr, d);
}
void MicroSim::Memory::write_word(Addr addr, std::uint16_t d)
{
	device_at(addr).write_word(addr, d);
}
void MicroSim::Memory::write_dword(Addr addr, std::uint32_t d)
{
	device_at(addr).write_dword(addr, d);
}
void MicroSim::Memory::write_qword(Addr addr, std::uint64_t d)
{
	device_at(addr).write_qword(addr, d);
}

#ifdef WASM
#include <emscripten/bind.h>

EMSCRIPTEN_BINDINGS(Memory){
	emscripten::class_<MicroSim::Memory>("Memory")
		// .constructor<>()
		.smart_ptr_constructor("Simulation", &std::make_unique<MicroSim::Memory>)
		.function("read_byte", &MicroSim::Memory::read_byte)
		.function("read_word", &MicroSim::Memory::read_word)
		.function("read_dword", &MicroSim::Memory::read_dword)
		.function("read_qword", &MicroSim::Memory::read_qword)
		.function("read_byte_const", &MicroSim::Memory::read_byte_const)
		.function("read_word_const", &MicroSim::Memory::read_word_const)
		.function("read_dword_const", &MicroSim::Memory::read_dword_const)
		.function("read_qword_const", &MicroSim::Memory::read_qword_const)
		.function("write_byte", &MicroSim::Memory::write_byte)
		.function("write_word", &MicroSim::Memory::write_word)
		.function("write_dword", &MicroSim::Memory::write_dword)
		.function("write_qword", &MicroSim::Memory::write_qword)
		.function("add_device", &MicroSim::Memory::add_device, emscripten::allow_raw_pointers())
		.function("remove_device", &MicroSim::Memory::remove_device, emscripten::allow_raw_pointers())
		;
	emscripten::class_<MicroSim::MemoryDevice>("MemoryDevice")
		.constructor<MicroSim::Addr, MicroSim::Addr, std::uint8_t*>()
		.function("low", &MicroSim::MemoryDevice::low)
		.function("high", &MicroSim::MemoryDevice::high)
		.function("priority", &MicroSim::MemoryDevice::priority)
		.function("read_byte", &MicroSim::MemoryDevice::read_byte)
		.function("read_word", &MicroSim::MemoryDevice::read_word)
		.function("read_dword", &MicroSim::MemoryDevice::read_dword)
		.function("read_qword", &MicroSim::MemoryDevice::read_qword)
		.function("write_byte", &MicroSim::MemoryDevice::write_byte)
		.function("write_word", &MicroSim::MemoryDevice::write_word)
		.function("write_dword", &MicroSim::MemoryDevice::write_dword)
		.function("write_qword", &MicroSim::MemoryDevice::write_qword)
		;
	emscripten::class_<MicroSim::DefaultMemDev, emscripten::base<MicroSim::MemoryDevice>>("DefaultMemoryDevice")
		.constructor<>()
		.constructor<MicroSim::Addr, MicroSim::Addr>()
		;

	emscripten::class_<MicroSim::Rom, emscripten::base<MicroSim::MemoryDevice>>("Rom")
		.constructor<std::size_t, MicroSim::Addr>()
		.function("fill", &MicroSim::Rom::fill, emscripten::allow_raw_pointers())
		.function("override_write_byte", &MicroSim::Rom::override_write_byte)
		.function("override_write_word", &MicroSim::Rom::override_write_word)
		.function("override_write_dword", &MicroSim::Rom::override_write_dword)
		.function("override_write_qword", &MicroSim::Rom::override_write_qword)
		;

	emscripten::class_<MicroSim::Ram, emscripten::base<MicroSim::MemoryDevice>>("Ram")
		.constructor<std::size_t, MicroSim::Addr>()
		;
}

#endif