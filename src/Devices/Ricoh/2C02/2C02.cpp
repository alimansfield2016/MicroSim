#include "2C02.hpp"

using namespace MicroSim::Ricoh;

MemoryDevice2C02::MemoryDevice2C02(unsigned long int _freq) : 
	//Currently has fixed address
	MicroSim::MemoryDevice{0x2000, 0x3FFF, 0x07, nullptr, 1, _freq}
	{ }

void MemoryDevice2C02::clock(){
	//rendering sequence

	//once done, call refresh_fn
}
void MemoryDevice2C02::reset(){
	//zero memory
	for(auto &e : m_display) e=0x3F;
	m_finex = 0;
	m_addr_latch = 0;
	m_ppu_data_buffer = 0;
	m_scanline = 0;
	m_cycle = 0;
}

MicroSim::Byte MemoryDevice2C02::read_byte(Addr _addr)
{ 
	switch (_addr&0x07)
	{
	case 0x00: return 0; //PPUCTRL
	case 0x01: return 0; //PPUMASK
	case 0x02: {
		Byte v = m_ppu_status;
		m_ppu_status &= ~0x80;
		m_addr_latch = 0;
		return v;
	}
	case 0x03: return 0; //OAMADDR
	case 0x04: return m_oam_data;
	case 0x05: return 0; //PPUSCROLL
	case 0x06: return 0; //PPUADDR
	case 0x07: return m_ppu_data;
	}
	return 0; 
}
MicroSim::Byte MemoryDevice2C02::read_byte_const(Addr _addr) const 
{ 
	switch (_addr&0x07)
	{
	case 0x00: return 0; //PPUCTRL
	case 0x01: return 0; //PPUMASK
	case 0x02: return m_ppu_status;
	case 0x03: return 0; //OAMADDR
	case 0x04: return m_oam[m_oam_addr];
	case 0x05: return 0; //PPUSCROLL
	case 0x06: return 0; //PPUADDR
	case 0x07: return m_ppu_data;
	}
	return 0;  
}

void MemoryDevice2C02::write_byte(Addr _addr, Byte b) 
{
	switch (_addr&0x07)
	{
	case 0x00: m_ppu_ctrl = b; //PPUCTRL
	case 0x01: m_ppu_mask = b; //PPUMASK
	case 0x02: return;
	case 0x03: m_oam_addr = b; //OAMADDR
	case 0x04: m_oam[m_oam_addr++] = b;
	case 0x05: { //PPUSCROLL
		if(!m_addr_latch){
			m_finex = b&0x07;
			m_tram_addr.corsex = (b>>3)&0x1F;
			m_addr_latch = 1;
		}else{
			m_tram_addr.finey = b&0x07;
			m_tram_addr.corsey = (b>>3)&0x1F;
			m_addr_latch = 0;
		}
		break;
	}
	case 0x06: {//PPUADDR
		if(!m_addr_latch){
			//high byte
			MicroSim::high(m_tram_addr.reg) = b&0x3F;
			m_addr_latch = 1;
		}else{
			//low byte
			MicroSim::low(m_tram_addr.reg) = b;
			m_vram_addr = m_tram_addr;
			m_addr_latch = 0;
		}
		break;
	}
	case 0x07:{//PPUDATA
		ppu_write(m_vram_addr.reg, b);
		if(m_ppu_ctrl&0x04)
			m_ppu_addr += 32;
		else
			m_ppu_addr += 1;
		break;
	}
	}
}
void MemoryDevice2C02::write_byte_override(Addr _addr, Byte b) 
{
	write_byte(_addr, b);
}

void MemoryDevice2C02::set_mapper(MicroSim::Nes::Mapper *mapper)
{
	m_mapper = mapper;
	m_chr = m_mapper->chr();
}

MicroSim::Byte MemoryDevice2C02::ppu_read(Addr _addr)
{
	_addr &= 0x3FFF;
	if(_addr < 0x2000) return m_chr->read_byte(_addr);
	if(_addr < 0x3F00){
		if(m_mapper->mirror() == Nes::Mapper::Mirroring::H){
			return m_nametables[(_addr>>11)&0x01][_addr&0x03FF];
		}else{
			return m_nametables[(_addr>>10)&0x01][_addr&0x03FF];
		}
	}
	if(_addr&0x03){
		return m_palettes[_addr&0x1F];
	}else{
		return m_palettes[0];
	}
	return 0;
}
void MemoryDevice2C02::ppu_write(Addr _addr, Byte b)
{
	_addr &= 0x3FFF;
	if(_addr < 0x2000) m_chr->write_byte(_addr, b);
	if(_addr < 0x3F00){
		if(m_mapper->mirror() == Nes::Mapper::Mirroring::H){
			m_nametables[(_addr>>11)&0x01][_addr&0x03FF] = 0;
		}else{
			m_nametables[(_addr>>10)&0x01][_addr&0x03FF] = 0;
		}
	}
	if(_addr&0x03){
		m_palettes[_addr&0x1F] = 0;
	}else{
		m_palettes[0] = 0;
	}
}

void MemoryDevice2C02::inc_scroll_x()
{
	//only if rendering is enabled
	if(true){
		if(m_vram_addr.corsex==31){
			m_vram_addr.corsex = 0;
			m_vram_addr.nametablex = ~m_vram_addr.nametablex;
		}else{
			m_vram_addr.corsex++;
		}
	}
}
void MemoryDevice2C02::inc_scroll_y()
{
	//only if rendering is enabled
	if(true){
		if(m_vram_addr.finey < 7){
			m_vram_addr.finey++;
		}else{
			m_vram_addr.finey = 0;
			if(m_vram_addr.corsey == 29){
				m_vram_addr.corsey = 0;
				m_vram_addr.nametabley = ~m_vram_addr.nametabley;
			}else if(m_vram_addr.corsey == 31){
				m_vram_addr.corsey = 0;
			}else{
				m_vram_addr.corsey++;
			}
		}
	}
}


#ifdef WASM
emscripten::val MemoryDevice2C02::display()
{
    return emscripten::val(emscripten::typed_memory_view(256*240, m_display));
}
#else
MicroSim::Byte *MemoryDevice2C02::display()
{
	return m_display;
}
#endif

void MemoryDevice2C02::set_refresh_fn(uintptr_t ptr)
{
	void(*fn)() = reinterpret_cast<void(*)()>(ptr);
	m_refresh_fn = fn;
}


#ifdef WASM
#include <emscripten/bind.h>
EMSCRIPTEN_BINDINGS(2C02){
	emscripten::class_<MicroSim::Ricoh::MemoryDevice2C02,
						emscripten::base<MicroSim::MemoryDevice>>("MemoryDevice2C02")
		.constructor<unsigned long int>()
		.function("display", &MemoryDevice2C02::display)
		.function("set_refresh_fn", &MemoryDevice2C02::set_refresh_fn, emscripten::allow_raw_pointers())
		;
}
#endif