#include "HD44780U.hpp"
#include <iostream>

using namespace MicroSim::Hitachi;

// #define _DEBUG
#define DEBUG_STREAM std::cout

#ifdef _DEBUG
#define DEBUG(...) MicroSim::debug_print(__VA_ARGS__)
#else
#define DEBUG(...)
#endif




MemoryDeviceHD44780U::MemoryDeviceHD44780U(Addr _addr, std::size_t size) :
	MicroSim::MemoryDevice{_addr, _addr+size, nullptr, 250000}
{
	reset();
}

MicroSim::Byte MemoryDeviceHD44780U::read_byte(Addr _addr)
{
	bool rs = _addr&1;
	if(!rs){	//Read busy flag and address

	}else {		//Read data

	}
	return 0;
}
void MemoryDeviceHD44780U::write_byte(Addr _addr, MicroSim::Byte b)
{
	bool rs = _addr&1;
	if(m_busy) return;
	if(!rs){
		if(m_dataLen == DL::_8Bit){
			m_IR = b;
			m_half = DW::Low;
		} else if(m_half == DW::High){
			m_IR = b&0xF0;
			m_half = DW::Low;
			return;
		} else{
			m_IR |= b>>4;
			m_half = DW::High;
		}
	}
	if(rs){
		Byte idx = m_addr;
		m_DDRAM[idx] = b;
		if(m_id == ID::Increment)
			increment();
		else
			decrement();
		DEBUG(DEBUG_STREAM, "Addr now: ",int(getIdx()),'\n');
			
			
	}else if(!(m_IR&static_cast<Byte>(Instr::ClearDisplay))){	//clear display
		clear();
	}else if(!(m_IR&static_cast<Byte>(Instr::ReturnHome))){	//return home
		home();
	}else if(!(m_IR&static_cast<Byte>(Instr::EntryModeSet))){	//entry mode set
		DEBUG(DEBUG_STREAM, "Entry Mode Set\n");
		m_id = static_cast<ID>(!!(m_IR&static_cast<Byte>(Bits::ID)));
		if(m_id == ID::Increment)
			DEBUG(DEBUG_STREAM, "cursor increment\n");
		else
			DEBUG(DEBUG_STREAM, "cursor decrement\n");

		m_s = static_cast<S>(!!(m_IR&static_cast<Byte>(Bits::S)));
	}else if(!(m_IR&static_cast<Byte>(Instr::DisplayControl))){	//display on/off
		DEBUG(DEBUG_STREAM, "Display On/Off\n");
	
	}else if(!(m_IR&static_cast<Byte>(Instr::CursorDisplayShift))){	//cursor/display shift
		DEBUG(DEBUG_STREAM, "Cursor/Display Shift\n");
		//move cursor / shift display
		//doesnt change ddram contents
	}else if(!(m_IR&static_cast<Byte>(Instr::FunctionSet))){	//function set
		DEBUG(DEBUG_STREAM, "Function Set\n");
		
		//if we were in 8-bit mode and switching to 4-bt mode
		if(!(m_IR&0x10) && m_half == DW::Low){
			m_IR &= 0xF0;
			m_dataLen = DL::_4Bit;
			return;
		}
		//otherwise, continue as normal
		if(m_IR & static_cast<Byte>(Bits::N))
			m_numLines = NL::_2Line;
		else
			m_numLines = NL::_1Line;

		if(m_IR & static_cast<Byte>(Bits::DL))
			m_dataLen = DL::_8Bit;

		if(m_IR & static_cast<Byte>(Bits::F))
			m_font = F::Font5x10;
		else
			m_font = F::Font5x8;

	}else if(!(m_IR&static_cast<Byte>(Instr::SetCGRAMAddr))){	//set CGRAM addr
	}else{					//set DDRAM addr

	}
	//set busy timeout
}

void MemoryDeviceHD44780U::clear()
{
	DEBUG(DEBUG_STREAM, "Clear\n");
	for(auto &e : m_DDRAM)
		e = 0x20;
}
void MemoryDeviceHD44780U::home()
{
	DEBUG(DEBUG_STREAM, "Home\n");
	m_addr = 0;
	m_shift = 0;

}

MicroSim::Byte MemoryDeviceHD44780U::getIdx() const
{
	Byte addr = m_addr + m_shift;
	if(addr >= 80) addr -= 80;
	return addr;
}

void MemoryDeviceHD44780U::increment()
{
		m_addr++;

		if(m_addr >= 80)
			m_addr -= 80;

		if(m_s == S::Shift){
			m_shift++;

			if(m_shift >= 80)
				m_shift -= 80;
		} 
}
void MemoryDeviceHD44780U::decrement()
{
		if(m_addr)
			m_addr--;
		else
			m_addr += 80-1;


		if(m_s == S::Shift){
			if(m_shift)
				m_shift--;
			else
				m_shift += 80-1;
		} 
}

void MemoryDeviceHD44780U::reset()
{
	m_dataLen = DL::_8Bit;
	//10ms busy while reset

}
void MemoryDeviceHD44780U::clock()
{

}

MicroSim::Byte MicroSim::Hitachi::MemoryDeviceHD44780U::charCode(MicroSim::Byte idx)
{

	Byte addr = idx + m_shift;
	if(addr >= 80)
		addr -= 80;
	// DEBUG(DEBUG_STREAM, "getting charcode at addr: ",int(addr),'\n');
	return m_DDRAM[addr];
}

MicroSim::Byte MicroSim::Hitachi::MemoryDeviceHD44780U::genChar(MicroSim::Byte charcode, MicroSim::Byte offset)
{
	Word addr = charcode;
	if(!(charcode & ~0x0F)){
		//CGRAM
		addr &= 0x07;
		addr <<= 3;
		addr += offset;
		// DEBUG(DEBUG_STREAM, "reading CGRAM bits: ",m_CGRAM[addr],'\n');
		return m_CGRAM[addr];
	}else{
		//CGROM
		addr -= 16;
		addr <<= 4;
		addr += offset;
		// DEBUG(DEBUG_STREAM, "reading CGR0M addr: ",addr,'\n');
		// DEBUG(DEBUG_STREAM, "reading CGR0M bits: ",m_CGRAM[addr],'\n');
		return s_CGROM[addr];
	}
	return 0;
}


#ifdef WASM
#include <emscripten/bind.h>
EMSCRIPTEN_BINDINGS(HD47780U){
	emscripten::class_<	MicroSim::Hitachi::MemoryDeviceHD44780U,
						emscripten::base<MicroSim::MemoryDevice>>("MemoryDeviceHD44780U")
		.constructor<MicroSim::Addr, std::size_t>()
		.function("char_code", &MicroSim::Hitachi::MemoryDeviceHD44780U::charCode)
		.function("gen_char", &MicroSim::Hitachi::MemoryDeviceHD44780U::genChar)
		;
}
#endif