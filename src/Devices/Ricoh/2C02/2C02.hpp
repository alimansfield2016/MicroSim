#pragma once

#include "include/Memory.hpp"
#include "include/Core.hpp"
#include "Cores/MOS/6502/6502.hpp"
#include "Devices/NES/Mapper/mapper.hpp"

#ifdef WASM
#include <emscripten/val.h>
#include <emscripten/bind.h>
#endif

namespace MicroSim::Ricoh
{
	class MemoryDevice2C02 : public MicroSim::MemoryDevice{
		Byte m_ppu_ctrl;
		Byte m_ppu_mask;
		Byte m_ppu_status;
		
		
		Byte m_oam_addr;
		Byte m_oam_data;
		Byte m_ppu_scroll_x;
		Byte m_ppu_scroll_y;
		Byte m_ppu_addr;
		Byte m_ppu_data;
		Byte m_ppu_data_buffer;

		Byte m_oam[256];
		Byte m_palettes[0x20];
		Byte m_nametables[2][0x0400];

		Byte m_finex;
		union LoopyRegister
		{
			struct{
				Word coarsex: 5;
				Word coarsey: 5;
				Word nametablex: 1;
				Word nametabley: 1;
				Word finey: 3;
				Word unused: 1;
			};
			Word reg;
		};
		LoopyRegister m_vram_addr, m_tram_addr;
		Word m_scanline, m_cycle;
		Byte 	m_bg_next_tile_id, m_bg_next_tile_attrib, 
				m_bg_next_tile_lsb, m_bg_next_tile_msb;
		Word 	m_bg_shifter_pattern_lo, m_bg_shifter_pattern_hi, 
				m_bg_shifter_attrib_lo, m_bg_shifter_attrib_hi;

		MicroSim::Nes::Mapper *m_mapper;
		MicroSim::MemoryDevice *m_chr;

		bool m_addr_latch;
		bool m_odd_frame;

		void (*m_refresh_fn)();

	public:
		Byte m_display[256*240];
		MemoryDevice2C02(unsigned long int);
		//Core functions
		void clock() override;
		void reset() override;
		//MemoryDevice functions
		Byte read_byte(Addr) override;
		Byte read_byte_const(Addr) const override;
		void write_byte(Addr,Byte) override;
		void write_byte_override(Addr,Byte) override;
		//Class specific functions
		void set_mapper(MicroSim::Nes::Mapper *);
		void set_core(MicroSim::MOS::Core6502 *core);
		#ifdef WASM
		emscripten::val display();
		#else
		Byte *display();
		#endif
		void set_refresh_fn(uintptr_t);

		Byte ppu_read(Addr);
		void ppu_write(Addr, Byte);
	private:
		//Class specific functions
		MicroSim::MOS::Core6502 *m_core;


		void inc_scroll_x();
		void inc_scroll_y();
		void transfer_x();
		void transfer_y();
		void load_shifters();
		void update_shifters();
	};
} // namespace MicroSim::Ricoh
