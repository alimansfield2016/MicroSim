#pragma once

#include "include/Core.hpp"

namespace MicroSim::WDC
{
	class Core6502 : public MicroSim::Core
	{
		using MicroSim::Core::m_memory;
	protected:
		using Addr = std::uint16_t;
		std::uint8_t A;
		std::uint8_t X;
		std::uint8_t Y;
		std::uint8_t SP;
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
		union{
			Addr PC;
			struct{
				std::uint8_t PCL;
				std::uint8_t PCH;
			};
		};
		
		Core6502();
	public:
		// ~Core6502() override;
		virtual void step() override = 0;
		virtual void reset() override = 0;
		// static std::shared_ptr<Core> createCore();
		// constexpr static CoreDescription s_description{"65C02S", createCore};
	protected:
		constexpr static Addr vec_brk = 0xFFFE;
		constexpr static Addr vec_irq = 0xFFFE;
		constexpr static Addr vec_res = 0xFFFC;
		constexpr static Addr vec_nmi = 0xFFFA;
		std::uint8_t read_byte(Addr);
		void write_byte(Addr, std::uint8_t);
		void push_byte(std::uint8_t);
		// void push_word(std::uint16_t);
		std::uint8_t pop_byte();
		// std::uint16_t pop_word();

	};
} // namespace MicroSim::WDC
