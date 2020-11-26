#include "2C02.hpp"

#include <iostream>
#include <iomanip>

// #define _DEBUG
#ifdef WASM
#define DEBUG_STREAM std::cout
#else
#define DEBUG_STREAM std::cout
#endif

#ifdef _DEBUG
#define DEBUG(...) MicroSim::debug_print(__VA_ARGS__)
#else
#define DEBUG(...)
#endif

using namespace MicroSim::Ricoh;

MemoryDevice2C02::MemoryDevice2C02(unsigned long int _freq) : 
	//Currently has fixed address
	MicroSim::MemoryDevice{0x2000, 0x2000, 0x3FFF, 0x07, nullptr, 1, _freq}
	{ }

void MemoryDevice2C02::clock(){
	//rendering sequence
	//scanline 0 not visible
	if(m_scanline<=240){
		//very likely
		if((m_cycle >= 2 && m_cycle < 258) || (m_cycle >= 321 && m_cycle < 338)){
			update_shifters();
			switch((m_cycle-1)&0x07){
				case 0x00: {
					load_shifters(); 
					m_bg_next_tile_id = ppu_read(0x2000 | (m_vram_addr.reg&0x0FFF));
					break;
				}
				case 0x01: break;
				case 0x02:{
					m_bg_next_tile_attrib = ppu_read(0x23C0 | (m_vram_addr.nametabley << 11) 
															| (m_vram_addr.nametablex << 10) 
															| ((m_vram_addr.coarsey >> 2) << 3) 
															| (m_vram_addr.coarsex >> 2));			
					if (m_vram_addr.coarsey & 0x02) m_bg_next_tile_attrib >>= 4;
					if (m_vram_addr.coarsex & 0x02) m_bg_next_tile_attrib >>= 2;
					m_bg_next_tile_attrib &= 0x03;
					break;
				}
				case 0x03: break;
				case 0x04:{
					m_bg_next_tile_lsb = ppu_read((m_ppu_ctrl&0x10 << 8)
													+ (Word{m_bg_next_tile_id} << 4)
													+ (m_vram_addr.finey) + 0); 
					break;
				}
				case 0x05: break;
				case 0x06: {
					m_bg_next_tile_msb = ppu_read((m_ppu_ctrl&0x10 << 8)
													+ (Word{m_bg_next_tile_id} << 4)
													+ (m_vram_addr.finey) + 8); 
					break;
				}
				case 0x07: inc_scroll_x(); break;
			}

		}
		//doesnt clash so here.
		else if(m_scanline == 0 && m_cycle >= 280 && m_cycle < 305)
		transfer_y();
		//scanline == 0 & m_cycle == 1 : very very unlikely
		else if(!m_scanline && m_cycle == 1){
			m_ppu_status &= ~0xE0;
			//clear shifters
		}
		//very very very unlikely
		else if(m_scanline == 1 && m_cycle == 0 && m_odd_frame && (m_ppu_mask&0x18) /*rendering*/){
			m_cycle = 1;
		}
	}
	else if(m_scanline == 241){} //DO NOTHING 
	else if(m_scanline == 242 && m_cycle == 1){
		// DEBUG(DEBUG_STREAM, "vblank\n");
		// std::cout << "vblank\n";
		m_ppu_status |= 0x80;
		//emit NMI
		if(m_core && m_ppu_ctrl&0x80)
			m_core->nmi();
	}
	if(m_cycle == 256)
		inc_scroll_y();
	else if(m_cycle == 257){
		load_shifters();
		transfer_x();
	}
	else if (m_cycle == 338 || m_cycle == 340)
		m_bg_next_tile_id = ppu_read(0x2000 | (m_vram_addr.reg & 0x0FFF));
	

	Byte bg_pixel = 0, bg_palette = 0;

	if(m_ppu_mask&0x08 && //render_background
		(m_ppu_mask&0x04 || (m_cycle >= 9))){ //render left-most 8 pixels

		Word bit_mux = 0x8000 >> m_finex;

		// Select Plane pixels by extracting from the shifter 
		// at the required location. 
		Byte p0_pixel = (m_bg_shifter_pattern_lo & bit_mux) > 0;
		Byte p1_pixel = (m_bg_shifter_pattern_hi & bit_mux) > 0;

		// Combine to form pixel index
		bg_pixel = (p1_pixel << 1) | p0_pixel;

		// Get palette
		uint8_t bg_pal0 = (m_bg_shifter_attrib_lo & bit_mux) > 0;
		uint8_t bg_pal1 = (m_bg_shifter_attrib_hi & bit_mux) > 0;
		bg_palette = (bg_pal1 << 1) | bg_pal0;
	}

	Byte pixel = bg_pixel;
	Byte palette = bg_palette;

	if(m_scanline && m_scanline <= 240 && m_cycle && m_cycle <= 256)
		m_display[(m_scanline-1)*256 + (m_cycle-1)] = ppu_read(0x3F00 + (palette<<2) + pixel)&0x3F;
	m_cycle++;
	// DEBUG(DEBUG_STREAM, "incremented cycle to: ", m_cycle, '\n');
	if(m_cycle >= 341){
		m_cycle = 0;
		m_scanline++;
		if(m_scanline > 261){
			m_scanline = 0;
			//once done, call refresh_fn
			if(m_refresh_fn) (*m_refresh_fn)();
			m_odd_frame = !m_odd_frame;	
		}
	}


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
	// DEBUG(DEBUG_STREAM,
	// 		"ppu read byte ",
	// 		std::showbase ,
	// 		std::internal ,
	// 		std::setfill('0'),
	// 		std::hex,
	// 		std::setw(4),
	// 		_addr,
	// 		'\n');
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
	DEBUG(DEBUG_STREAM,
			"ppu write byte ",
			std::showbase ,
			std::internal ,
			std::setfill('0'),
			std::hex,
			std::setw(4),
			_addr,
			":",
			std::setw(2),
			(b&0xFF),
			'\n');
	switch (_addr&0x07)
	{
	case 0x00: m_ppu_ctrl = b; break; //PPUCTRL
	case 0x01: m_ppu_mask = b; break; //PPUMASK
	case 0x02: break;
	case 0x03: m_oam_addr = b; break; //OAMADDR
	case 0x04: m_oam[m_oam_addr++] = b;
	case 0x05: { //PPUSCROLL
		if(!m_addr_latch){
			m_finex = b&0x07;
			m_tram_addr.coarsex = (b>>3)&0x1F;
			m_addr_latch = 1;
		}else{
			m_tram_addr.finey = b&0x07;
			m_tram_addr.coarsey = (b>>3)&0x1F;
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
			m_vram_addr.reg += 32;
		else
			m_vram_addr.reg += 1;
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
void MemoryDevice2C02::set_core(MicroSim::MOS::Core6502 *core)
{
	m_core = core;
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
	return m_palettes[_addr&0x1F];
}
void MemoryDevice2C02::ppu_write(Addr _addr, Byte b)
{
	_addr &= 0x3FFF;
	if(_addr < 0x2000) m_chr->write_byte(_addr, b);
	else if(_addr < 0x3F00){
		if(m_mapper->mirror() == Nes::Mapper::Mirroring::H){
			m_nametables[(_addr>>11)&0x01][_addr&0x03FF] = b;
		}else{
			m_nametables[(_addr>>10)&0x01][_addr&0x03FF] = b;
		}
	}
	else /* if(_addr&0x03) */{
		m_palettes[_addr&0x1F] = b&0x3F;
	}
	/* else{
		m_palettes[0] = b&0x3F;
	} */
}


void MemoryDevice2C02::inc_scroll_x()
{
	//only if rendering is enabled
	if(m_ppu_mask&0x18){
		if(m_vram_addr.coarsex==31){
			m_vram_addr.coarsex = 0;
			m_vram_addr.nametablex = ~m_vram_addr.nametablex;
		}else{
			m_vram_addr.coarsex++;
		}
	}
}
void MemoryDevice2C02::inc_scroll_y()
{
	//only if rendering is enabled
	if(m_ppu_mask&0x18){
		if(m_vram_addr.finey < 7){
			m_vram_addr.finey++;
		}else{
			m_vram_addr.finey = 0;
			if(m_vram_addr.coarsey == 29){
				m_vram_addr.coarsey = 0;
				m_vram_addr.nametabley = ~m_vram_addr.nametabley;
			}else if(m_vram_addr.coarsey == 31){
				m_vram_addr.coarsey = 0;
			}else{
				m_vram_addr.coarsey++;
			}
		}
	}
}
void MemoryDevice2C02::transfer_x()
{
	//if rendering is enabled
	if(m_ppu_mask&0x18){
		m_vram_addr.coarsex = m_tram_addr.coarsex;
		m_vram_addr.nametablex = m_tram_addr.nametablex;
	}
}
void MemoryDevice2C02::transfer_y()
{
	//if rendering is enabled
	if(m_ppu_mask&0x18){
		m_vram_addr.finey = m_tram_addr.finey;
		m_vram_addr.coarsey = m_tram_addr.coarsey;
		m_vram_addr.nametabley = m_tram_addr.nametabley;
	}
}

void MemoryDevice2C02::load_shifters()
{
	MicroSim::low(m_bg_shifter_pattern_lo) = m_bg_next_tile_lsb;
	MicroSim::low(m_bg_shifter_pattern_hi) = m_bg_next_tile_msb;
	MicroSim::low(m_bg_shifter_attrib_lo) = (m_bg_next_tile_attrib&0x01)?0xFF:0x00;
	MicroSim::low(m_bg_shifter_attrib_hi) = (m_bg_next_tile_attrib&0x02)?0xFF:0x00;
}
void MemoryDevice2C02::update_shifters()
{
	if(m_ppu_mask&0x08){
		m_bg_shifter_pattern_lo <<= 1;
		m_bg_shifter_pattern_hi <<= 1;
		m_bg_shifter_attrib_lo <<= 1;
		m_bg_shifter_attrib_hi <<= 1;
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
		.function("set_mapper", &MemoryDevice2C02::set_mapper, emscripten::allow_raw_pointers())
		.function("set_core", &MemoryDevice2C02::set_core, emscripten::allow_raw_pointers())
		.function("ppu_read", &MemoryDevice2C02::ppu_read)
		.function("ppu_write", &MemoryDevice2C02::ppu_write)
		;
}
#endif