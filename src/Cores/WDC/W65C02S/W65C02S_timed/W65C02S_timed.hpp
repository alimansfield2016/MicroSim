#pragma once

#include "Cores/MOS/6502/6502.hpp"

namespace MicroSim::WDC
{
	class CoreW65C02S_timed : public MOS::Core6502
	{
	public:
		CoreW65C02S_timed(unsigned long int);
		~CoreW65C02S_timed() override;
		void clock() override;
		void reset() override;
	private:
		void decode(std::uint8_t);
		enum class State{
			FETCH,
			ADDR,
			EXEC,
		}state;
		MicroSim::Byte TMP;
		MicroSim::Word _addr;
		void (CoreW65C02S_timed::*addrm)();
		void (CoreW65C02S_timed::*op)();

		void addrm_abs();
		void addrm_abs2();
		void addrm_absx();
		void addrm_absx2();
		void addrm_absx_rmw();
		void addrm_absx2_rmw();
		void addrm_abs_x3_rmw();
		void addrm_absy();
		void addrm_absy2();
		void addrm_acc();
		void addrm_immb();
		void addrm_immw();
		void addrm_imp();
		void addrm_zp();
		void addrm_zpi();
		void addrm_zpxi();
		void addrm_zpi2();
		void addrm_zpi3();
		void addrm_zpiy();
		void addrm_zpiy2();
		void addrm_zpiy3();
		void addrm_zpiy4();
		void addrm_zpx();
		void addrm_zpy();
		void addrm_zpx_rmw();
		void addrm_zpx1_rmw();
		void addrm_pcr();
		void addrm_unused2();
		void addrm_unused();

		void op_adc();
		void op_adc2();
		void op_and();
		void op_asla();
		void op_asl();
		void op_asl_m();
		void op_bit();
		template<std::uint8_t b> 
		void op_bbr();
		template<std::uint8_t b> 
		void op_bbr2();
		template<std::uint8_t b> 
		void op_bbs();
		template<std::uint8_t b> 
		void op_bbs2();
		/**Branch on flag(F) set
		 */
		template<std::uint8_t F> 
		void op_br_fs();
		/**Branch on flag(F) clear
		 */
		template<std::uint8_t F> 
		void op_br_fc();
		void op_bra();
		void op_br();
		void op_br2();

		void op_brk();
		void op_brk2();
		void op_brk3();
		void op_brk4();

		template<std::uint8_t F> 
		void op_clf();
		template<std::uint8_t F> 
		void op_sef();

		void op_cmp();
		void op_cpx();
		void op_cpy();

		void op_dec();
		void op_dec2();
		void op_dea();
		void op_dex();
		void op_dey();

		void op_eor();

		void op_inc();
		void op_inc2();
		void op_ina();
		void op_inx();
		void op_iny();

		void op_jmp();
		void op_jmp2();

		void op_jsr();
		void op_jsr2();
		void op_jsr3();
		void op_jsr4();
		void op_jsr5();

		void op_lda();
		void op_ldx();
		void op_ldy();

		void op_lsra();
		void op_lsr();
		void op_lsr2();

		void op_nop();

		void op_ora();

		void op_pha();
		void op_php();
		void op_phx();
		void op_phy();
		void op_pla();
		void op_plp();
		void op_plx();
		void op_ply();

		template<std::uint8_t b>
		void op_rmb();
		template<std::uint8_t b>
		void op_rmb_m();
		template<std::uint8_t b>
		void op_smb();
		template<std::uint8_t b>
		void op_smb_m();
		void op_rmw_w();

		void op_rola();
		void op_rol();
		void op_rol_m();
		void op_rora();
		void op_ror();
		void op_ror_m();

		void op_rti();
		void op_rts();
		void op_rts2();
		void op_rts3();

		void op_sbc();
		void op_sbc2();

		void op_sta();
		void op_stx();
		void op_sty();
		void op_stz();

		void op_tax();
		void op_tay();
		void op_tsx();
		void op_txa();
		void op_txs();
		void op_tya();

		void op_trb();
		void op_trb_m();
		void op_tsb();
		void op_tsb_m();

		void op_vecl();
		void op_vech();

		void op_reset();
		void op_wai();
		void op_stp();
	};
	
} // namespace MicroSim::WDC
