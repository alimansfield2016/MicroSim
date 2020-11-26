#pragma once

#include "include/Memory.hpp"

namespace MicroSim::NES
{
	class Mapper{
	public:
		enum class Mirroring : bool {
			H = 0,
			V = 1,
		};
		virtual ~Mapper(){}
		virtual MicroSim::MemoryDevice *prg() = 0;
		virtual MicroSim::MemoryDevice *ram() { return nullptr; };
		virtual MicroSim::MemoryDevice *chr() = 0;
		virtual Mirroring mirror() const = 0;


		static Mapper *mapper_from_ines(std::string);
	};
} // namespace MicroSim::NES
