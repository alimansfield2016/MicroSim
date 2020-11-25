#pragma once

#include "include/Memory.hpp"
#include "include/Core.hpp"

namespace MicroSim::Ricoh
{
	class Core2C02 : public MicroSim::Core, public MicroSim::MemoryDevice{
		Byte m_ppu_ctrl;
		Byte m_ppu_mask;
		Byte m_ppu_status;
		Byte m_oam_addr;
		Byte m_oam_data;
		Byte m_ppu_scroll_x;
		Byte m_ppu_scroll_y;
		Byte m_ppu_addr;
		Byte m_ppu_data;
		Byte m_oam_dma;

		Byte m_display[256*240];
		Byte m_nametables[2][0x1000];

	public:
		Core2C02(unsigned long int);
		//Core functions
		void clock() override;
		void reset() override;
		//MemoryDevice functions
		Byte read_byte(Addr) override;
		Byte read_byte_const(Addr) const override;
		void write_byte(Addr,Byte) override;
		void write_byte_override(Addr,Byte) override;
	private:
		//Class specific functions
		void emit_nmi(); //emit cpu NMI
		void emit_suspend(); //suspend CPU
		void emit_release(); //release CPU (ie no more suspend)
	};
} // namespace MicroSim::Ricoh
