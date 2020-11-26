#include "mapper000.hpp"

using namespace MicroSim::NES;

Mapper000::Mapper000(bool NROM256, Mirroring _mirror) : 
	m_prg{
		static_cast<MicroSim::Addr>(NROM256?0x8000:0x4000), 
		0x8000, 
		0xFFFF, 
		static_cast<MicroSim::Addr>(NROM256?0x7FFF:0x3FFF)},
	m_chr{0x2000, 0x0000, 0x1FFF, 0x1FFF},
	m_mirror{_mirror}
{}

MicroSim::MemoryDevice *Mapper000::prg() 
{
	return &m_prg;
}

MicroSim::MemoryDevice *Mapper000::chr() 
{
	return &m_chr;
}

MicroSim::NES::Mapper::Mirroring MicroSim::NES::Mapper000::mirror() const
{
	return m_mirror;
}

