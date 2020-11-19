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
			std::uint8_t P;
			struct{
				std::uint8_t C:1;
				std::uint8_t Z:1;
				std::uint8_t I:1;
				std::uint8_t D:1;
				std::uint8_t B:1;
				std::uint8_t U:1;
				std::uint8_t V:1;
				std::uint8_t N:1;
			};
		};
		Word PC;
		
		Core6502();
	public:
		// ~Core6502() override;
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
