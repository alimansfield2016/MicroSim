#pragma once

#include "include/Core.hpp"

namespace MicroSim::WDC
{
	class CoreW65C02S : public MicroSim::Core
	{
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
	public:
		CoreW65C02S();
		~CoreW65C02S() override;
		void step() override;
		void reset() override;
		static std::shared_ptr<Core> createCore();
		constexpr static CoreDescription s_description{"65C02S", createCore};
	private:
		constexpr static Addr vec_brk = 0xFFFE;
		constexpr static Addr vec_irq = 0xFFFE;
		constexpr static Addr vec_res = 0xFFFC;
		constexpr static Addr vec_nmi = 0xFFFA;
		void push_byte(std::uint8_t);
		void push_word(std::uint16_t);
		std::uint8_t pop_byte();
		std::uint16_t pop_word();

		Addr addrm_abs();
		Addr addrm_absx_ind();
		Addr addrm_absx();
		Addr addrm_absy();
		Addr addrm_abs_ind();
		Addr addrm_acc();
		Addr addrm_immb();
		Addr addrm_immw();
		Addr addrm_imp();
		Addr addrm_pcr();
		Addr addrm_zp();
		Addr addrm_zpx_ind();
		Addr addrm_zpx();
		Addr addrm_zpy();
		Addr addrm_zp_ind();
		Addr addrm_zp_indy();

		void op_adc(Addr);
		void op_and(Addr);
		void op_asl(Addr);
		void op_asla(Addr);
		void op_bcc(Addr);
		void op_bcs(Addr);
		void op_beq(Addr);
		void op_bit(Addr);
		void op_bmi(Addr);
		void op_bne(Addr);
		void op_bpl(Addr);
		void op_bra(Addr);
		void op_brk(Addr);
		void op_bvc(Addr);
		void op_bvs(Addr);
		void op_clc(Addr);
		void op_cld(Addr);
		void op_cli(Addr);
		void op_clv(Addr);
		void op_cmp(Addr);
		void op_cpx(Addr);
		void op_cpy(Addr);
		void op_dec(Addr);
		void op_dea(Addr);
		void op_dex(Addr);
		void op_dey(Addr);
		void op_eor(Addr);
		void op_inc(Addr);
		void op_ina(Addr);
		void op_inx(Addr);
		void op_iny(Addr);
		void op_jmp(Addr);
		void op_jsr(Addr);
		void op_lda(Addr);
		void op_ldx(Addr);
		void op_ldy(Addr);
		void op_lsr(Addr);
		void op_lsra(Addr);
		void op_nop(Addr);
		void op_ora(Addr);
		void op_pha(Addr);
		void op_php(Addr);
		void op_phx(Addr);
		void op_phy(Addr);
		void op_pla(Addr);
		void op_plp(Addr);
		void op_plx(Addr);
		void op_ply(Addr);
		void op_rol(Addr);
		void op_rola(Addr);
		void op_ror(Addr);
		void op_rora(Addr);
		void op_rti(Addr);
		void op_rts(Addr);
		void op_sbc(Addr);
		void op_sec(Addr);
		void op_sed(Addr);
		void op_sei(Addr);
		void op_sta(Addr);
		void op_stp(Addr);
		void op_stx(Addr);
		void op_sty(Addr);
		void op_stz(Addr);
		void op_tax(Addr);
		void op_tay(Addr);
		void op_tsx(Addr);
		void op_txa(Addr);
		void op_txs(Addr);
		void op_tya(Addr);
		void op_wai(Addr);
	};
} // namespace MicroSim::WDC
