#pragma once

#include "Cores/MOS/6502/6502.hpp"

namespace MicroSim::Ricoh
{
	class Core2A03 : public MicroSim::MOS::Core6502
	{
	public:
		Core2A03(unsigned long int);
		void clock() override;
		void reset() override;
	private:
		constexpr static Addr vec_nmi = 0xFFFA;
		constexpr static Addr vec_res = 0xFFFC;
		constexpr static Addr vec_irq = 0xFFFE;

		void decode(Byte);
		enum class State{
			FETCH,
			ADDR,
			EXEC,
		}state;
		MicroSim::Byte TMP;
		MicroSim::Word _addr;
		void (Core2A03::*addrm)();
		void (Core2A03::*op)();

		void addrm_imp();
		void addrm_acc();
		void addrm_immb();
		void addrm_immw();
		void addrm_zp();
		void addrm_abs();
		void addrm_abs2();
		void addrm_pcr();
		void addrm_zpx();
		void addrm_zpx2();
		void addrm_zpy();
		void addrm_zpy2();
		void addrm_absx();
		void addrm_absx_rmw();
		void addrm_absx2();
		void addrm_absx2_rmw();
		void addrm_absx3();
		void addrm_absx3_rmw();
		void addrm_absy();
		void addrm_absy2();
		void addrm_absy3();
		void addrm_zpxi();
		void addrm_zpxi2();
		void addrm_zpxi3();
		void addrm_zpiy();
		void addrm_zpiy2();
		void addrm_zpiy3();
		void addrm_zpiy4();
		void addrm_unused();

		void op_adc();
		void op_and();
		void op_asla();
		void op_asl();
		void op_asl_m();
		void op_bit();
		/**Branch on flag(F) set
		 */
		template<std::uint8_t F> 
		void op_br_fs();
		/**Branch on flag(F) clear
		 */
		template<std::uint8_t F> 
		void op_br_fc();
		void op_br();
		void op_br2();
		void op_brk();

		template<std::uint8_t F> 
		void op_clf();
		template<std::uint8_t F> 
		void op_sef();
		void op_cmp();
		void op_cpx();
		void op_cpy();

		void op_dec();
		void op_dec2();
		void op_dex();
		void op_dey();
		
		void op_eor();

		void op_inc();
		void op_inc2();
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
		void op_nop2();

		void op_ora();

		void op_pha();
		void op_php();
		void op_pla();
		void op_pla2();
		void op_plp();
		void op_plp2();

		void op_rola();
		void op_rol();
		void op_rol_m();
		void op_rora();
		void op_ror();
		void op_ror_m();

		void op_rti();
		void op_rti2();
		void op_rti3();
		void op_rti4();
		void op_rts();
		void op_rts2();
		void op_rts3();
		void op_rts4();

		void op_sbc();

		void op_sta();
		void op_stx();
		void op_sty();

		void op_tax();
		void op_tay();
		void op_tsx();
		void op_txa();
		void op_txs();
		void op_tya();

		void op_rmw_w();

		void op_vecl();
		void op_vech();
		void op_reset();
		void op_int();
		void op_int2();
		void op_int3();
		void op_int4();
		void op_int5();

		//Illegal instructions:
	};
} // namespace MicroSim::Ricoh

