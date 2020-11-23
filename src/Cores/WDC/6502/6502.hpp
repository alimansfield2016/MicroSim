#pragma once

#include "include/Core.hpp"
#include "include/MicroSim.hpp"

namespace MicroSim::WDC
{
	class Core6502 : public MicroSim::Core
	{
		using MicroSim::Core::m_memory;
	protected:
		using Addr = Word;
		Byte A;
		Byte X;
		Byte Y;
		Byte SP;
		union{
			Byte P;
			struct{
				Byte C:1;
				Byte Z:1;
				Byte I:1;
				Byte D:1;
				Byte B:1;
				Byte U:1;
				Byte V:1;
				Byte N:1;
			};
		};
		Word PC;
		
	public:
		Core6502(unsigned long int);
		~Core6502() override;
		virtual void clock() override = 0;
		virtual void reset() override = 0;
		// static std::shared_ptr<Core> createCore();
		// constexpr static CoreDescription s_description{"65C02S", createCore};
	protected:
		constexpr static Addr vec_brk = 0xFFFE;
		constexpr static Addr vec_irq = 0xFFFE;
		constexpr static Addr vec_res = 0xFFFC;
		constexpr static Addr vec_nmi = 0xFFFA;
		MicroSim::Byte read_byte(Addr);
		void write_byte(Addr, MicroSim::Byte);
		void push_byte(MicroSim::Byte);
		MicroSim::Byte pop_byte();

	};
} // namespace MicroSim::WDC
