#pragma once

#include "include/Memory.hpp"
#include "mapper.hpp"

namespace MicroSim::Nes
{
	class Mapper000 : public Mapper{
		//PrgRom
		MicroSim::Rom m_prg;
		//ChrRom
		MicroSim::Rom m_chr;
		const Mirroring m_mirror;
	public:
		Mapper000(bool, Mirroring _mirror);
		MicroSim::MemoryDevice *prg() override;
		MicroSim::MemoryDevice *chr() override;
		Mirroring mirror() const override;
	};
} // namespace MicroSim::Nes
