#pragma once

#include "../6502.hpp"

namespace MicroSim::WDC
{
	class CoreW65C02S_simple : public Core6502
	{
	public:
		CoreW65C02S_simple(unsigned long int);
		~CoreW65C02S_simple() override;
		void reset() override;
		void clock() override;
		void step();
	protected:
		unsigned short int _cycles;
		unsigned short int m_cooldown;
		unsigned short int cooldown() const;
		unsigned short int dec_cooldown();
		void set_cooldown(unsigned short int);
		void do_cycle();
		void update_cycles();
		void push_word(Word);
		Word pop_word();
		Word read_word(Addr);
		void write_word(Addr, Word);

	private:

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
