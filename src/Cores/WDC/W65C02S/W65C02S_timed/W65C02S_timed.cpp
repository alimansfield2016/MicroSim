#include "W65C02S_timed.hpp"
#include "W65C02S_timed_addrm.hpp"
#include "W65C02S_timed_op.hpp"

#include <iostream>

using namespace MicroSim::WDC;

CoreW65C02S_timed::CoreW65C02S_timed(unsigned long int _freq) : 
	Core6502{_freq} 
{}

CoreW65C02S_timed::~CoreW65C02S_timed(){

}

void CoreW65C02S_timed::clock()
{
	switch(state){
		case State::FETCH:
			decode(read_byte(PC++));
			state = State::ADDR;
			break;
		case State::ADDR:
			(this->*addrm)();
			break;
		case State::EXEC:
			(this->*op)();
			break;
	}
	m_cycles++;
	clock_mem();
}

void CoreW65C02S_timed::reset()
{
	// std::cout << "CoreW65C02S_timed::reset()\n";
	state = State::EXEC;
	op = &CoreW65C02S_timed::op_reset;
}


void CoreW65C02S_timed::op_reset()
{
	_addr = vec_res;
	SP = 0;
	push_byte(0);
	P &= ~0x3C;
	P |= 0x34;
	op = &CoreW65C02S_timed::op_vecl;
}

void CoreW65C02S_timed::decode(std::uint8_t instr)
{
	switch (instr)
	{
	case 0x00: op = &CoreW65C02S_timed::op_brk; addrm = &CoreW65C02S_timed::addrm_immb; break;
	case 0x01: op = &CoreW65C02S_timed::op_ora; addrm = &CoreW65C02S_timed::addrm_zpxi; break;
	case 0x02: break;
	case 0x03: break;
	case 0x04: op = &CoreW65C02S_timed::op_tsb; addrm = &CoreW65C02S_timed::addrm_zp; break;
	case 0x05: op = &CoreW65C02S_timed::op_ora; addrm = &CoreW65C02S_timed::addrm_zp; break;
	case 0x06: op = &CoreW65C02S_timed::op_asl; addrm = &CoreW65C02S_timed::addrm_zp; break;
	case 0x07: op = &CoreW65C02S_timed::op_rmb<0x01>; addrm = &CoreW65C02S_timed::addrm_zp; break;
	case 0x08: op = &CoreW65C02S_timed::op_php; addrm = &CoreW65C02S_timed::addrm_unused; break;
	case 0x09: op = &CoreW65C02S_timed::op_ora; addrm = &CoreW65C02S_timed::addrm_immb; break;
	case 0x0a: op = &CoreW65C02S_timed::op_asla; addrm = &CoreW65C02S_timed::addrm_acc; break;
	case 0x0b: break;
	case 0x0c: op = &CoreW65C02S_timed::op_tsb; addrm = &CoreW65C02S_timed::addrm_abs; break;
	case 0x0d: op = &CoreW65C02S_timed::op_ora; addrm = &CoreW65C02S_timed::addrm_abs; break;
	case 0x0e: op = &CoreW65C02S_timed::op_asl; addrm = &CoreW65C02S_timed::addrm_abs; break;
	case 0x0f: op = &CoreW65C02S_timed::op_bbr<0x01>; addrm = &CoreW65C02S_timed::addrm_zp; break;

	case 0x10: op = &CoreW65C02S_timed::op_br_fc<0x80>; addrm = &CoreW65C02S_timed::addrm_pcr; break; //bpl
	case 0x11: op = &CoreW65C02S_timed::op_ora; addrm = &CoreW65C02S_timed::addrm_zpiy; break;
	case 0x12: op = &CoreW65C02S_timed::op_ora; addrm = &CoreW65C02S_timed::addrm_zpi; break;
	case 0x13: break;
	case 0x14: op = &CoreW65C02S_timed::op_trb; addrm = &CoreW65C02S_timed::addrm_zp; break;
	case 0x15: op = &CoreW65C02S_timed::op_ora; addrm = &CoreW65C02S_timed::addrm_zpx; break;
	case 0x16: op = &CoreW65C02S_timed::op_asl; addrm = &CoreW65C02S_timed::addrm_zpx; break;
	case 0x17: op = &CoreW65C02S_timed::op_rmb<0x02>; addrm = &CoreW65C02S_timed::addrm_zp; break;
	case 0x18: op = &CoreW65C02S_timed::op_clf<0x01>; addrm = &CoreW65C02S_timed::addrm_imp; break;
	case 0x19: op = &CoreW65C02S_timed::op_ora; addrm = &CoreW65C02S_timed::addrm_absy; break;
	case 0x1a: op = &CoreW65C02S_timed::op_ina; addrm = &CoreW65C02S_timed::addrm_acc; break;
	case 0x1b: break;
	case 0x1c: op = &CoreW65C02S_timed::op_trb; addrm = &CoreW65C02S_timed::addrm_abs; break;
	case 0x1d: op = &CoreW65C02S_timed::op_ora; addrm = &CoreW65C02S_timed::addrm_absx; break;
	case 0x1e: op = &CoreW65C02S_timed::op_asl; addrm = &CoreW65C02S_timed::addrm_absx; break;
	case 0x1f: op = &CoreW65C02S_timed::op_bbr<0x02>; addrm = &CoreW65C02S_timed::addrm_zp; break;

	case 0x20: op = &CoreW65C02S_timed::op_jsr; addrm = &CoreW65C02S_timed::addrm_immw; break;
	case 0x21: op = &CoreW65C02S_timed::op_and; addrm = &CoreW65C02S_timed::addrm_zpxi; break;
	case 0x22: break;
	case 0x23: break;
	case 0x24: op = &CoreW65C02S_timed::op_bit; addrm = &CoreW65C02S_timed::addrm_zp; break;
	case 0x25: op = &CoreW65C02S_timed::op_and; addrm = &CoreW65C02S_timed::addrm_zp; break;
	case 0x26: op = &CoreW65C02S_timed::op_rol; addrm = &CoreW65C02S_timed::addrm_zp; break;
	case 0x27: op = &CoreW65C02S_timed::op_rmb<0x04>; addrm = &CoreW65C02S_timed::addrm_zp; break;
	case 0x28: op = &CoreW65C02S_timed::op_plp; addrm = &CoreW65C02S_timed::addrm_unused2; break;
	case 0x29: op = &CoreW65C02S_timed::op_and; addrm = &CoreW65C02S_timed::addrm_immb; break;
	case 0x2a: op = &CoreW65C02S_timed::op_rola; addrm = &CoreW65C02S_timed::addrm_acc; break;
	case 0x2b: break;
	case 0x2c: op = &CoreW65C02S_timed::op_bit; addrm = &CoreW65C02S_timed::addrm_abs; break;
	case 0x2d: op = &CoreW65C02S_timed::op_and; addrm = &CoreW65C02S_timed::addrm_abs; break;
	case 0x2e: op = &CoreW65C02S_timed::op_rol; addrm = &CoreW65C02S_timed::addrm_abs; break;
	case 0x2f: op = &CoreW65C02S_timed::op_bbr<0x04>; addrm = &CoreW65C02S_timed::addrm_zp; break;

	case 0x30: op = &CoreW65C02S_timed::op_br_fs<0x80>; addrm = &CoreW65C02S_timed::addrm_pcr; break; //bmi
	case 0x31: op = &CoreW65C02S_timed::op_and; addrm = &CoreW65C02S_timed::addrm_zpiy; break;
	case 0x32: op = &CoreW65C02S_timed::op_and; addrm = &CoreW65C02S_timed::addrm_zpi; break;
	case 0x33: break;
	case 0x34: op = &CoreW65C02S_timed::op_bit; addrm = &CoreW65C02S_timed::addrm_zpx; break;
	case 0x35: op = &CoreW65C02S_timed::op_and; addrm = &CoreW65C02S_timed::addrm_zpx; break;
	case 0x36: op = &CoreW65C02S_timed::op_rol; addrm = &CoreW65C02S_timed::addrm_zpx; break;
	case 0x37: op = &CoreW65C02S_timed::op_rmb<0x08>; addrm = &CoreW65C02S_timed::addrm_zp; break;
	case 0x38: op = &CoreW65C02S_timed::op_sef<0x01>; addrm = &CoreW65C02S_timed::addrm_imp; break;
	case 0x39: op = &CoreW65C02S_timed::op_and; addrm = &CoreW65C02S_timed::addrm_absy; break;
	case 0x3a: op = &CoreW65C02S_timed::op_dea; addrm = &CoreW65C02S_timed::addrm_acc; break;
	case 0x3b: break;
	case 0x3c: op = &CoreW65C02S_timed::op_bit; addrm = &CoreW65C02S_timed::addrm_absx; break;
	case 0x3d: op = &CoreW65C02S_timed::op_and; addrm = &CoreW65C02S_timed::addrm_absx; break;
	case 0x3e: op = &CoreW65C02S_timed::op_rol; addrm = &CoreW65C02S_timed::addrm_absx; break;
	case 0x3f: op = &CoreW65C02S_timed::op_bbr<0x08>; addrm = &CoreW65C02S_timed::addrm_zp; break;

	case 0x40: op = &CoreW65C02S_timed::op_rti; addrm = &CoreW65C02S_timed::addrm_unused; break;
	case 0x41: op = &CoreW65C02S_timed::op_eor; addrm = &CoreW65C02S_timed::addrm_zpxi; break;
	case 0x42: break;
	case 0x43: break;
	case 0x44: break;
	case 0x45: op = &CoreW65C02S_timed::op_eor; addrm = &CoreW65C02S_timed::addrm_zp; break;
	case 0x46: op = &CoreW65C02S_timed::op_lsr; addrm = &CoreW65C02S_timed::addrm_zp; break;
	case 0x47: op = &CoreW65C02S_timed::op_rmb<0x10>; addrm = &CoreW65C02S_timed::addrm_zp; break;
	case 0x48: op = &CoreW65C02S_timed::op_pha; addrm = &CoreW65C02S_timed::addrm_unused; break;
	case 0x49: op = &CoreW65C02S_timed::op_eor; addrm = &CoreW65C02S_timed::addrm_immb; break;
	case 0x4a: op = &CoreW65C02S_timed::op_lsra; addrm = &CoreW65C02S_timed::addrm_acc; break;
	case 0x4b: break;
	case 0x4c: op = &CoreW65C02S_timed::op_jmp; addrm = &CoreW65C02S_timed::addrm_immw; break;
	case 0x4d: op = &CoreW65C02S_timed::op_eor; addrm = &CoreW65C02S_timed::addrm_abs; break;
	case 0x4e: op = &CoreW65C02S_timed::op_lsr; addrm = &CoreW65C02S_timed::addrm_abs; break;
	case 0x4f: op = &CoreW65C02S_timed::op_bbr<0x10>; addrm = &CoreW65C02S_timed::addrm_zp; break;

	case 0x50: op = &CoreW65C02S_timed::op_br_fc<0x40>; addrm = &CoreW65C02S_timed::addrm_pcr; break; //bvc
	case 0x51: op = &CoreW65C02S_timed::op_eor; addrm = &CoreW65C02S_timed::addrm_zpiy; break;
	case 0x52: op = &CoreW65C02S_timed::op_eor; addrm = &CoreW65C02S_timed::addrm_zpi; break;
	case 0x53: break;
	case 0x54: break;
	case 0x55: op = &CoreW65C02S_timed::op_eor; addrm = &CoreW65C02S_timed::addrm_zpx; break;
	case 0x56: op = &CoreW65C02S_timed::op_lsr; addrm = &CoreW65C02S_timed::addrm_zpx; break;
	case 0x57: op = &CoreW65C02S_timed::op_rmb<0x20>; addrm = &CoreW65C02S_timed::addrm_zp; break;
	case 0x58: op = &CoreW65C02S_timed::op_clf<0x04>; addrm = &CoreW65C02S_timed::addrm_imp; break; //cli
	case 0x59: op = &CoreW65C02S_timed::op_eor; addrm = &CoreW65C02S_timed::addrm_absy; break;
	case 0x5a: op = &CoreW65C02S_timed::op_phy; addrm = &CoreW65C02S_timed::addrm_unused; break;
	case 0x5b: break;
	case 0x5c: break;
	case 0x5d: op = &CoreW65C02S_timed::op_eor; addrm = &CoreW65C02S_timed::addrm_absx; break;
	case 0x5e: op = &CoreW65C02S_timed::op_lsr; addrm = &CoreW65C02S_timed::addrm_absx; break;
	case 0x5f: op = &CoreW65C02S_timed::op_bbr<0x20>; addrm = &CoreW65C02S_timed::addrm_zp; break;

	case 0x60: op = &CoreW65C02S_timed::op_rts; addrm = &CoreW65C02S_timed::addrm_unused2; break;
	case 0x61: op = &CoreW65C02S_timed::op_adc; addrm = &CoreW65C02S_timed::addrm_zpxi; break;
	case 0x62: break;
	case 0x63: break;
	case 0x64: op = &CoreW65C02S_timed::op_stz; addrm = &CoreW65C02S_timed::addrm_zp; break;
	case 0x65: op = &CoreW65C02S_timed::op_adc; addrm = &CoreW65C02S_timed::addrm_zp; break;
	case 0x66: op = &CoreW65C02S_timed::op_ror; addrm = &CoreW65C02S_timed::addrm_zp; break;
	case 0x67: op = &CoreW65C02S_timed::op_rmb<0x40>; addrm = &CoreW65C02S_timed::addrm_zp; break;
	case 0x68: op = &CoreW65C02S_timed::op_pla; addrm = &CoreW65C02S_timed::addrm_unused2; break;
	case 0x69: op = &CoreW65C02S_timed::op_adc; addrm = &CoreW65C02S_timed::addrm_immb; break;
	case 0x6a: op = &CoreW65C02S_timed::op_rora; addrm = &CoreW65C02S_timed::addrm_acc; break;
	case 0x6b: break;
	case 0x6c: op = &CoreW65C02S_timed::op_jmp; addrm = &CoreW65C02S_timed::addrm_abs; break;
	case 0x6d: op = &CoreW65C02S_timed::op_adc; addrm = &CoreW65C02S_timed::addrm_abs; break;
	case 0x6e: op = &CoreW65C02S_timed::op_ror; addrm = &CoreW65C02S_timed::addrm_abs; break;
	case 0x6f: op = &CoreW65C02S_timed::op_bbr<0x40>; addrm = &CoreW65C02S_timed::addrm_zp; break;

	case 0x70: op = &CoreW65C02S_timed::op_br_fs<0x40>; addrm = &CoreW65C02S_timed::addrm_pcr; break; //bvs
	case 0x71: op = &CoreW65C02S_timed::op_adc; addrm = &CoreW65C02S_timed::addrm_zpiy; break;
	case 0x72: op = &CoreW65C02S_timed::op_adc; addrm = &CoreW65C02S_timed::addrm_zpi; break;
	case 0x73: break;
	case 0x74: op = &CoreW65C02S_timed::op_stz; addrm = &CoreW65C02S_timed::addrm_zpx; break;
	case 0x75: op = &CoreW65C02S_timed::op_adc; addrm = &CoreW65C02S_timed::addrm_zpx; break;
	case 0x76: op = &CoreW65C02S_timed::op_ror; addrm = &CoreW65C02S_timed::addrm_zpx; break;
	case 0x77: op = &CoreW65C02S_timed::op_rmb<0x80>; addrm = &CoreW65C02S_timed::addrm_zp; break;
	case 0x78: op = &CoreW65C02S_timed::op_sef<0x04>; addrm = &CoreW65C02S_timed::addrm_imp; break; //sei
	case 0x79: op = &CoreW65C02S_timed::op_adc; addrm = &CoreW65C02S_timed::addrm_absy; break;
	case 0x7a: op = &CoreW65C02S_timed::op_ply; addrm = &CoreW65C02S_timed::addrm_unused2; break;
	case 0x7b: break;
	case 0x7c: op = &CoreW65C02S_timed::op_jmp; addrm = &CoreW65C02S_timed::addrm_absx; break;
	case 0x7d: op = &CoreW65C02S_timed::op_adc; addrm = &CoreW65C02S_timed::addrm_absx; break;
	case 0x7e: op = &CoreW65C02S_timed::op_ror; addrm = &CoreW65C02S_timed::addrm_absx; break;
	case 0x7f: op = &CoreW65C02S_timed::op_bbr<0x80>; addrm = &CoreW65C02S_timed::addrm_zp; break;

	case 0x80: op = &CoreW65C02S_timed::op_bra; addrm = &CoreW65C02S_timed::addrm_pcr; break;
	case 0x81: op = &CoreW65C02S_timed::op_sta; addrm = &CoreW65C02S_timed::addrm_zpxi; break;
	case 0x82: break;
	case 0x83: break;
	case 0x84: op = &CoreW65C02S_timed::op_sty; addrm = &CoreW65C02S_timed::addrm_zp; break;
	case 0x85: op = &CoreW65C02S_timed::op_sta; addrm = &CoreW65C02S_timed::addrm_zp; break;
	case 0x86: op = &CoreW65C02S_timed::op_stx; addrm = &CoreW65C02S_timed::addrm_zp; break;
	case 0x87: op = &CoreW65C02S_timed::op_smb<0x01>; addrm = &CoreW65C02S_timed::addrm_zp; break;
	case 0x88: op = &CoreW65C02S_timed::op_dey; addrm = &CoreW65C02S_timed::addrm_imp; break;
	case 0x89: op = &CoreW65C02S_timed::op_bit; addrm = &CoreW65C02S_timed::addrm_immb; break;
	case 0x8a: op = &CoreW65C02S_timed::op_txa; addrm = &CoreW65C02S_timed::addrm_imp; break;
	case 0x8b: break;
	case 0x8c: op = &CoreW65C02S_timed::op_sty; addrm = &CoreW65C02S_timed::addrm_abs; break;
	case 0x8d: op = &CoreW65C02S_timed::op_sta; addrm = &CoreW65C02S_timed::addrm_abs; break;
	case 0x8e: op = &CoreW65C02S_timed::op_stx; addrm = &CoreW65C02S_timed::addrm_abs; break;
	case 0x8f: op = &CoreW65C02S_timed::op_bbs<0x01>; addrm = &CoreW65C02S_timed::addrm_zp; break;

	case 0x90: op = &CoreW65C02S_timed::op_br_fc<0x01>; addrm = &CoreW65C02S_timed::addrm_pcr; break; //bcc
	case 0x91: op = &CoreW65C02S_timed::op_sta; addrm = &CoreW65C02S_timed::addrm_zpiy; break;
	case 0x92: op = &CoreW65C02S_timed::op_sta; addrm = &CoreW65C02S_timed::addrm_zpi; break;
	case 0x93: break;
	case 0x94: op = &CoreW65C02S_timed::op_sty; addrm = &CoreW65C02S_timed::addrm_zpx; break;
	case 0x95: op = &CoreW65C02S_timed::op_sta; addrm = &CoreW65C02S_timed::addrm_zpx; break;
	case 0x96: op = &CoreW65C02S_timed::op_stx; addrm = &CoreW65C02S_timed::addrm_zpy; break;
	case 0x97: op = &CoreW65C02S_timed::op_smb<0x02>; addrm = &CoreW65C02S_timed::addrm_zp; break;
	case 0x98: op = &CoreW65C02S_timed::op_tya; addrm = &CoreW65C02S_timed::addrm_imp; break;
	case 0x99: op = &CoreW65C02S_timed::op_sta; addrm = &CoreW65C02S_timed::addrm_absy; break;
	case 0x9a: op = &CoreW65C02S_timed::op_txs; addrm = &CoreW65C02S_timed::addrm_imp; break;
	case 0x9b: break;
	case 0x9c: op = &CoreW65C02S_timed::op_stz; addrm = &CoreW65C02S_timed::addrm_abs; break;
	case 0x9d: op = &CoreW65C02S_timed::op_sta; addrm = &CoreW65C02S_timed::addrm_absx; break;
	case 0x9e: op = &CoreW65C02S_timed::op_stx; addrm = &CoreW65C02S_timed::addrm_absx; break;
	case 0x9f: op = &CoreW65C02S_timed::op_bbs<0x02>; addrm = &CoreW65C02S_timed::addrm_zp; break;

	case 0xa0: op = &CoreW65C02S_timed::op_ldy; addrm = &CoreW65C02S_timed::addrm_immb; break;
	case 0xa1: op = &CoreW65C02S_timed::op_lda; addrm = &CoreW65C02S_timed::addrm_zpxi; break;
	case 0xa2: op = &CoreW65C02S_timed::op_ldx; addrm = &CoreW65C02S_timed::addrm_immb; break;
	case 0xa3: break;
	case 0xa4: op = &CoreW65C02S_timed::op_ldy; addrm = &CoreW65C02S_timed::addrm_zp; break;
	case 0xa5: op = &CoreW65C02S_timed::op_lda; addrm = &CoreW65C02S_timed::addrm_zp; break;
	case 0xa6: op = &CoreW65C02S_timed::op_ldx; addrm = &CoreW65C02S_timed::addrm_zp; break;
	case 0xa7: op = &CoreW65C02S_timed::op_smb<0x04>; addrm = &CoreW65C02S_timed::addrm_zp; break;
	case 0xa8: op = &CoreW65C02S_timed::op_tay; addrm = &CoreW65C02S_timed::addrm_imp; break;
	case 0xa9: op = &CoreW65C02S_timed::op_lda; addrm = &CoreW65C02S_timed::addrm_immb; break;
	case 0xaa: op = &CoreW65C02S_timed::op_tax; addrm = &CoreW65C02S_timed::addrm_imp; break;
	case 0xab: break;
	case 0xac: op = &CoreW65C02S_timed::op_ldy; addrm = &CoreW65C02S_timed::addrm_abs; break;
	case 0xad: op = &CoreW65C02S_timed::op_lda; addrm = &CoreW65C02S_timed::addrm_abs; break;
	case 0xae: op = &CoreW65C02S_timed::op_ldx; addrm = &CoreW65C02S_timed::addrm_abs; break;
	case 0xaf: op = &CoreW65C02S_timed::op_bbs<0x04>; addrm = &CoreW65C02S_timed::addrm_zp; break;

	case 0xb0: op = &CoreW65C02S_timed::op_br_fs<0x01>; addrm = &CoreW65C02S_timed::addrm_pcr; break; //bcs
	case 0xb1: op = &CoreW65C02S_timed::op_lda; addrm = &CoreW65C02S_timed::addrm_zpiy; break;
	case 0xb2: op = &CoreW65C02S_timed::op_lda; addrm = &CoreW65C02S_timed::addrm_zpi; break;
	case 0xb3: break;
	case 0xb4: op = &CoreW65C02S_timed::op_ldy; addrm = &CoreW65C02S_timed::addrm_zpx; break;
	case 0xb5: op = &CoreW65C02S_timed::op_lda; addrm = &CoreW65C02S_timed::addrm_zpx; break;
	case 0xb6: op = &CoreW65C02S_timed::op_ldx; addrm = &CoreW65C02S_timed::addrm_zpy; break;
	case 0xb7: op = &CoreW65C02S_timed::op_smb<0x08>; addrm = &CoreW65C02S_timed::addrm_zp; break;
	case 0xb8: op = &CoreW65C02S_timed::op_clf<0x40>; addrm = &CoreW65C02S_timed::addrm_imp; break; //clv
	case 0xb9: op = &CoreW65C02S_timed::op_lda; addrm = &CoreW65C02S_timed::addrm_absy; break;
	case 0xba: op = &CoreW65C02S_timed::op_tsx; addrm = &CoreW65C02S_timed::addrm_imp; break;
	case 0xbb: break;
	case 0xbc: op = &CoreW65C02S_timed::op_ldy; addrm = &CoreW65C02S_timed::addrm_absx; break;
	case 0xbd: op = &CoreW65C02S_timed::op_lda; addrm = &CoreW65C02S_timed::addrm_absx; break;
	case 0xbe: op = &CoreW65C02S_timed::op_ldx; addrm = &CoreW65C02S_timed::addrm_absx; break;
	case 0xbf: op = &CoreW65C02S_timed::op_bbs<0x08>; addrm = &CoreW65C02S_timed::addrm_zp; break;

	case 0xc0: op = &CoreW65C02S_timed::op_cpy; addrm = &CoreW65C02S_timed::addrm_immb; break;
	case 0xc1: op = &CoreW65C02S_timed::op_cmp; addrm = &CoreW65C02S_timed::addrm_zpxi; break;
	case 0xc2: break;
	case 0xc3: break;
	case 0xc4: op = &CoreW65C02S_timed::op_cpy; addrm = &CoreW65C02S_timed::addrm_zp; break;
	case 0xc5: op = &CoreW65C02S_timed::op_cmp; addrm = &CoreW65C02S_timed::addrm_zp; break;
	case 0xc6: op = &CoreW65C02S_timed::op_dec; addrm = &CoreW65C02S_timed::addrm_zp; break;
	case 0xc7: op = &CoreW65C02S_timed::op_smb<0x10>; addrm = &CoreW65C02S_timed::addrm_zp; break;
	case 0xc8: op = &CoreW65C02S_timed::op_iny; addrm = &CoreW65C02S_timed::addrm_imp; break;
	case 0xc9: op = &CoreW65C02S_timed::op_cmp; addrm = &CoreW65C02S_timed::addrm_immb; break;
	case 0xca: op = &CoreW65C02S_timed::op_dex; addrm = &CoreW65C02S_timed::addrm_imp; break;
	case 0xcb: op = &CoreW65C02S_timed::op_wai; addrm = &CoreW65C02S_timed::addrm_imp; break;
	case 0xcc: op = &CoreW65C02S_timed::op_cpy; addrm = &CoreW65C02S_timed::addrm_abs; break;
	case 0xcd: op = &CoreW65C02S_timed::op_cmp; addrm = &CoreW65C02S_timed::addrm_abs; break;
	case 0xce: op = &CoreW65C02S_timed::op_dec; addrm = &CoreW65C02S_timed::addrm_abs; break;
	case 0xcf: op = &CoreW65C02S_timed::op_bbs<0x10>; addrm = &CoreW65C02S_timed::addrm_zp; break;

	case 0xd0: op = &CoreW65C02S_timed::op_br_fc<0x02>; addrm = &CoreW65C02S_timed::addrm_pcr; break; //bne/bnz
	case 0xd1: op = &CoreW65C02S_timed::op_cmp; addrm = &CoreW65C02S_timed::addrm_zpiy; break;
	case 0xd2: op = &CoreW65C02S_timed::op_cmp; addrm = &CoreW65C02S_timed::addrm_zpi; break;
	case 0xd3: break;
	case 0xd4: break;
	case 0xd5: op = &CoreW65C02S_timed::op_cmp; addrm = &CoreW65C02S_timed::addrm_zpx; break;
	case 0xd6: op = &CoreW65C02S_timed::op_dec; addrm = &CoreW65C02S_timed::addrm_zpx; break;
	case 0xd7: op = &CoreW65C02S_timed::op_smb<0x20>; addrm = &CoreW65C02S_timed::addrm_zp; break;
	case 0xd8: op = &CoreW65C02S_timed::op_clf<0x08>; addrm = &CoreW65C02S_timed::addrm_imp; break; //cld
	case 0xd9: op = &CoreW65C02S_timed::op_cmp; addrm = &CoreW65C02S_timed::addrm_absy; break;
	case 0xda: op = &CoreW65C02S_timed::op_phx; addrm = &CoreW65C02S_timed::addrm_unused; break;
	case 0xdb: op = &CoreW65C02S_timed::op_stp; addrm = &CoreW65C02S_timed::addrm_imp; break;
	case 0xdc: break;
	case 0xdd: op = &CoreW65C02S_timed::op_cmp; addrm = &CoreW65C02S_timed::addrm_absx; break;
	case 0xde: op = &CoreW65C02S_timed::op_dec; addrm = &CoreW65C02S_timed::addrm_absx; break;
	case 0xdf: op = &CoreW65C02S_timed::op_bbs<0x20>; addrm = &CoreW65C02S_timed::addrm_zp; break;

	case 0xe0: op = &CoreW65C02S_timed::op_cpx; addrm = &CoreW65C02S_timed::addrm_immb; break;
	case 0xe1: op = &CoreW65C02S_timed::op_sbc; addrm = &CoreW65C02S_timed::addrm_zpxi; break;
	case 0xe2: break;
	case 0xe3: break;
	case 0xe4: op = &CoreW65C02S_timed::op_cpx; addrm = &CoreW65C02S_timed::addrm_zp; break;
	case 0xe5: op = &CoreW65C02S_timed::op_sbc; addrm = &CoreW65C02S_timed::addrm_zp; break;
	case 0xe6: op = &CoreW65C02S_timed::op_inc; addrm = &CoreW65C02S_timed::addrm_zp; break;
	case 0xe7: op = &CoreW65C02S_timed::op_smb<0x40>; addrm = &CoreW65C02S_timed::addrm_zp; break;
	case 0xe8: op = &CoreW65C02S_timed::op_inx; addrm = &CoreW65C02S_timed::addrm_imp; break;
	case 0xe9: op = &CoreW65C02S_timed::op_sbc; addrm = &CoreW65C02S_timed::addrm_immb; break;
	case 0xea: op = &CoreW65C02S_timed::op_nop; addrm = &CoreW65C02S_timed::addrm_imp; break;
	case 0xeb: break;
	case 0xec: op = &CoreW65C02S_timed::op_cpx; addrm = &CoreW65C02S_timed::addrm_abs; break;
	case 0xed: op = &CoreW65C02S_timed::op_sbc; addrm = &CoreW65C02S_timed::addrm_abs; break;
	case 0xee: op = &CoreW65C02S_timed::op_inc; addrm = &CoreW65C02S_timed::addrm_abs; break;
	case 0xef: op = &CoreW65C02S_timed::op_bbs<0x40>; addrm = &CoreW65C02S_timed::addrm_zp; break;

	case 0xf0: op = &CoreW65C02S_timed::op_br_fs<0x02>; addrm = &CoreW65C02S_timed::addrm_pcr; break; //beq/bz
	case 0xf1: op = &CoreW65C02S_timed::op_sbc; addrm = &CoreW65C02S_timed::addrm_zpiy; break;
	case 0xf2: op = &CoreW65C02S_timed::op_sbc; addrm = &CoreW65C02S_timed::addrm_zpi; break;
	case 0xf3: break;
	case 0xf4: break;
	case 0xf5: op = &CoreW65C02S_timed::op_sbc; addrm = &CoreW65C02S_timed::addrm_zpx; break;
	case 0xf6: op = &CoreW65C02S_timed::op_inc; addrm = &CoreW65C02S_timed::addrm_zpx; break;
	case 0xf7: op = &CoreW65C02S_timed::op_smb<0x80>; addrm = &CoreW65C02S_timed::addrm_zp; break;
	case 0xf8: op = &CoreW65C02S_timed::op_sef<0x08>; addrm = &CoreW65C02S_timed::addrm_imp; break; //sed
	case 0xf9: op = &CoreW65C02S_timed::op_sbc; addrm = &CoreW65C02S_timed::addrm_absy; break;
	case 0xfa: op = &CoreW65C02S_timed::op_plx; addrm = &CoreW65C02S_timed::addrm_unused2; break;
	case 0xfb: break;
	case 0xfc: break;
	case 0xfd: op = &CoreW65C02S_timed::op_sbc; addrm = &CoreW65C02S_timed::addrm_absx; break;
	case 0xfe: op = &CoreW65C02S_timed::op_inc; addrm = &CoreW65C02S_timed::addrm_absx; break;
	case 0xff: op = &CoreW65C02S_timed::op_bbs<0x80>; addrm = &CoreW65C02S_timed::addrm_zp; break;
	
	}
}

#ifdef WASM
#include <emscripten/bind.h>
EMSCRIPTEN_BINDINGS(CoreW65C02S_timed){
	emscripten::class_<	MicroSim::WDC::CoreW65C02S_timed,
						emscripten::base<MicroSim::MOS::Core6502>
						>("CoreW65C02S_timed")
		.constructor<unsigned long int>()
		// .constructor<unsigned long int>(&std::make_unique<MicroSim::WDC::CoreW65C02S_timed>)
		;
}
#endif