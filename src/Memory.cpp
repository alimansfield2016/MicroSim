#include "include/Memory.hpp"

#include <algorithm>

#include <iostream>
#include <iomanip>

// #define _DEBUG
#ifdef WASM
#define DEBUG_STREAM std::cout
#else
#define DEBUG_STREAM std::clog
#endif

#ifdef _DEBUG
#define DEBUG(...) MicroSim::debug_print(__VA_ARGS__)
#else
#define DEBUG(...)
#endif


MicroSim::Byte MicroSim::MemoryDevice::read_byte(Addr addr)
{
	return *reinterpret_cast<std::uint8_t*>(&m_data[addr&m_mask]);
}
MicroSim::Word MicroSim::MemoryDevice::read_word(Addr addr)
{
	return *reinterpret_cast<std::uint16_t*>(&m_data[addr&m_mask]);
}
MicroSim::DWord MicroSim::MemoryDevice::read_dword(Addr addr)
{
	return *reinterpret_cast<std::uint32_t*>(&m_data[addr&m_mask]);
}
MicroSim::QWord MicroSim::MemoryDevice::read_qword(Addr addr)
{
	return *reinterpret_cast<std::uint64_t*>(&m_data[addr&m_mask]);
}

void MicroSim::MemoryDevice::write_byte(Addr addr, MicroSim::Byte d)
{
	*reinterpret_cast<std::uint8_t*>(&m_data[addr&m_mask]) = d;
}
void MicroSim::MemoryDevice::write_word(Addr addr, MicroSim::Word d)
{
	*reinterpret_cast<std::uint16_t*>(&m_data[addr&m_mask]) = d;
}
void MicroSim::MemoryDevice::write_dword(Addr addr, MicroSim::DWord d)
{
	*reinterpret_cast<std::uint32_t*>(&m_data[addr&m_mask]) = d;
}
void MicroSim::MemoryDevice::write_qword(Addr addr, MicroSim::QWord d)
{
	*reinterpret_cast<std::uint64_t*>(&m_data[addr&m_mask]) = d;
}


MicroSim::Byte MicroSim::MemoryDevice::read_byte_const(Addr addr) const
{
		return *reinterpret_cast<std::uint8_t*>(&m_data[addr&m_mask]);
}
MicroSim::Word MicroSim::MemoryDevice::read_word_const(Addr addr) const
{
		return *reinterpret_cast<std::uint16_t*>(&m_data[addr&m_mask]);
}
MicroSim::DWord MicroSim::MemoryDevice::read_dword_const(Addr addr) const
{
		return *reinterpret_cast<std::uint32_t*>(&m_data[addr&m_mask]);
}
MicroSim::QWord MicroSim::MemoryDevice::read_qword_const(Addr addr) const
{
		return *reinterpret_cast<std::uint64_t*>(&m_data[addr&m_mask]);
}

void MicroSim::MemoryDevice::write_byte_override(Addr addr, MicroSim::Byte d)
{
	*reinterpret_cast<std::uint8_t*>(&m_data[addr&m_mask]) = d;
}
void MicroSim::MemoryDevice::write_word_override(Addr addr, MicroSim::Word d)
{
	*reinterpret_cast<std::uint16_t*>(&m_data[addr&m_mask]) = d;
}
void MicroSim::MemoryDevice::write_dword_override(Addr addr, MicroSim::DWord d)
{
	*reinterpret_cast<std::uint32_t*>(&m_data[addr&m_mask]) = d;
}
void MicroSim::MemoryDevice::write_qword_override(Addr addr, MicroSim::QWord d)
{
	*reinterpret_cast<std::uint64_t*>(&m_data[addr&m_mask]) = d;
}


MicroSim::Byte MicroSim::DefaultMemDev::read_byte(Addr)
{
	return 0;
}
MicroSim::Word MicroSim::DefaultMemDev::read_word(Addr)
{
	return 0;
}
MicroSim::DWord MicroSim::DefaultMemDev::read_dword(Addr)
{
	return 0;
}
MicroSim::QWord MicroSim::DefaultMemDev::read_qword(Addr)
{
	return 0;
}

void MicroSim::DefaultMemDev::write_byte(Addr, MicroSim::Byte)
{
}
void MicroSim::DefaultMemDev::write_word(Addr, MicroSim::Word)
{
}
void MicroSim::DefaultMemDev::write_dword(Addr, MicroSim::DWord)
{
}
void MicroSim::DefaultMemDev::write_qword(Addr, MicroSim::QWord)
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
const MicroSim::MemoryDevice &MicroSim::Memory::device_at(Addr _addr) const
{
	for(auto &it : m_devices)
	{
		if(it->low() > _addr) continue;
		if(it->high() < _addr) continue;
		return *it;
	}
	return DefaultMemDev::s_defaultMemDev;
}

// void MicroSim::Memory::add_device(std::shared_ptr<MemoryDevice> dev)
void MicroSim::Memory::add_device(MemoryDevice *dev)
{
	if(!dev) return;
	auto priority = dev->priority();
	for(auto it = m_devices.begin(); it != m_devices.end(); it++)
	{
		if(priority > (*it)->priority()){
			m_devices.insert(it, dev);
			// m_devices.insert(it, std::move(dev));
			break;
		}
	}
	// m_devices.push_back(std::move(dev));
	m_devices.push_back(dev);

}
// void MicroSim::Memory::remove_device(std::shared_ptr<MemoryDevice> dev)
void MicroSim::Memory::remove_device(MemoryDevice *dev)
{
	std::remove_if(m_devices.begin(), m_devices.end(), [&](auto &e){return e == dev; });
}

MicroSim::Byte MicroSim::Memory::read_byte(Addr addr)
{
	std::uint8_t v = device_at(addr).read_byte(addr);
	DEBUG(DEBUG_STREAM,
			"Read Byte ",
			std::showbase ,
			std::internal ,
			std::setfill('0'),
			std::hex,
			std::setw(4),
			addr,
			":",
			std::setw(2),
			(v&0xFF),
			'\n');
	return v;
}
MicroSim::Word MicroSim::Memory::read_word(Addr addr)
{
	auto v = device_at(addr).read_word(addr);
	return v;
}
MicroSim::DWord MicroSim::Memory::read_dword(Addr addr)
{
	auto v = device_at(addr).read_dword(addr);
	return v;
}
MicroSim::QWord MicroSim::Memory::read_qword(Addr addr)
{
	auto v = device_at(addr).read_qword(addr);
	return v;
}

void MicroSim::Memory::write_byte(Addr addr, MicroSim::Byte d)
{
	DEBUG(DEBUG_STREAM,
			"Write Byte " ,
			std::showbase ,
			std::internal ,
			std::setfill('0'),
			std::hex,
			std::setw(4),
			addr,
			":",
			std::setw(2),
			(d&0xFF),
			'\n');
	device_at(addr).write_byte(addr, d);
}
void MicroSim::Memory::write_word(Addr addr, MicroSim::Word d)
{
	device_at(addr).write_word(addr, d);
}
void MicroSim::Memory::write_dword(Addr addr, MicroSim::DWord d)
{
	device_at(addr).write_dword(addr, d);
}
void MicroSim::Memory::write_qword(Addr addr, MicroSim::QWord d)
{
	device_at(addr).write_qword(addr, d);
}

MicroSim::Byte MicroSim::Memory::read_byte_const(Addr addr) const
{
	return device_at(addr).read_byte_const(addr);
}
MicroSim::Word MicroSim::Memory::read_word_const(Addr addr) const
{
	return device_at(addr).read_word_const(addr);
}
MicroSim::DWord MicroSim::Memory::read_dword_const(Addr addr) const
{
	return device_at(addr).read_dword_const(addr);
}
MicroSim::QWord MicroSim::Memory::read_qword_const(Addr addr) const
{
	return device_at(addr).read_qword_const(addr);
}

void MicroSim::Memory::write_byte_override(Addr addr, MicroSim::Byte v)
{
	return device_at(addr).write_byte_override(addr, v);
}
void MicroSim::Memory::write_word_override(Addr addr, MicroSim::Word v)
{
	return device_at(addr).write_word_override(addr, v);
}
void MicroSim::Memory::write_dword_override(Addr addr, MicroSim::DWord v)
{
	return device_at(addr).write_dword_override(addr, v);
}
void MicroSim::Memory::write_qword_override(Addr addr, MicroSim::QWord v)
{
	return device_at(addr).write_qword_override(addr, v);
}

void MicroSim::Memory::clock()
{
	
}

#ifdef WASM
#include <emscripten/bind.h>

EMSCRIPTEN_BINDINGS(Memory){
	emscripten::class_<MicroSim::Memory>("Memory")
		.constructor<>()
		// .smart_ptr_constructor("Simulation", &std::make_unique<MicroSim::Memory>)
		.function("read_byte", &MicroSim::Memory::read_byte)
		.function("read_word", &MicroSim::Memory::read_word)
		.function("read_dword", &MicroSim::Memory::read_dword)
		.function("read_qword", &MicroSim::Memory::read_qword)
		.function("write_byte", &MicroSim::Memory::write_byte)
		.function("write_word", &MicroSim::Memory::write_word)
		.function("write_dword", &MicroSim::Memory::write_dword)
		.function("write_qword", &MicroSim::Memory::write_qword)
		.function("read_byte_const", &MicroSim::Memory::read_byte_const)
		.function("read_word_const", &MicroSim::Memory::read_word_const)
		.function("read_dword_const", &MicroSim::Memory::read_dword_const)
		.function("read_qword_const", &MicroSim::Memory::read_qword_const)
		.function("write_byte_override", &MicroSim::Memory::write_byte_override)
		.function("write_word_override", &MicroSim::Memory::write_word_override)
		.function("write_dword_override", &MicroSim::Memory::write_dword_override)
		.function("write_qword_override", &MicroSim::Memory::write_qword_override)
		.function("add_device", &MicroSim::Memory::add_device, emscripten::allow_raw_pointers())
		.function("remove_device", &MicroSim::Memory::remove_device, emscripten::allow_raw_pointers())
		;
	emscripten::class_<MicroSim::MemoryDevice,
						emscripten::base<MicroSim::Device>>("MemoryDevice")
		.constructor<MicroSim::Addr, MicroSim::Addr, MicroSim::Addr, MicroSim::Addr, std::uint8_t*, std::uint8_t, unsigned long int>()
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
		.function("read_byte_const", &MicroSim::MemoryDevice::read_byte_const)
		.function("read_word_const", &MicroSim::MemoryDevice::read_word_const)
		.function("read_dword_const", &MicroSim::MemoryDevice::read_dword_const)
		.function("read_qword_const", &MicroSim::MemoryDevice::read_qword_const)
		.function("write_byte_override", &MicroSim::MemoryDevice::write_byte_override)
		.function("write_word_override", &MicroSim::MemoryDevice::write_word_override)
		.function("write_dword_override", &MicroSim::MemoryDevice::write_dword_override)
		.function("write_qword_override", &MicroSim::MemoryDevice::write_qword_override)
		.smart_ptr<std::shared_ptr<MicroSim::MemoryDevice>>("MemoryDevice")
		;
	emscripten::class_<MicroSim::DefaultMemDev, emscripten::base<MicroSim::MemoryDevice>>("DefaultMemoryDevice")
		.constructor<>()
		.constructor<MicroSim::Addr, MicroSim::Addr>()
		;

}

#endif