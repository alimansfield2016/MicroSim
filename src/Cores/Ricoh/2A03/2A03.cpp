#include "2A03.hpp"
#include "2A03_addrm.hpp"
#include "2A03_op.hpp"
using namespace MicroSim::Ricoh;

Core2A03::Core2A03(unsigned long int _freq) : 
MicroSim::MOS::Core6502{_freq}
{}

void Core2A03::clock()
{
	switch (state)
	{
	case State::FETCH:
		//check for NMI and IRQ
		if(m_nmi){
			//nmi
			m_nmi = 0;
			_addr = vec_nmi;
			state = State::EXEC;
			op_int();
		}else if(0){
			//irq
			_addr = vec_irq;
			state = State::EXEC;
			op_int();
		}else{
			decode(read_byte(PC++));
			state = State::ADDR;
		}
		break;
	case State::ADDR:
		(this->*addrm)();
		break;
	case State::EXEC:
		(this->*op)();
		break;
	
	}
	m_cycles++;
}
void Core2A03::reset()
{
	op = &Core2A03::op_reset;
	state = State::EXEC;
}

void Core2A03::op_reset()
{
	_addr = vec_res;
	SP = 0;
	push_byte(0);
	P &= ~0x3C;
	P |= 0x34;
	op = &Core2A03::op_vecl;
	// state = State::EXEC;
}

void Core2A03::decode(Byte b)
{
	switch (b)
	{
	case 0x00: op = &Core2A03::op_brk; addrm = &Core2A03::addrm_immb; break;
	case 0x01: op = &Core2A03::op_ora; addrm = &Core2A03::addrm_zpxi; break;
	case 0x02: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0x03: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0x04: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0x05: op = &Core2A03::op_ora; addrm = &Core2A03::addrm_zp; break;
	case 0x06: op = &Core2A03::op_asl; addrm = &Core2A03::addrm_zp; break;
	case 0x07: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0x08: op = &Core2A03::op_php; addrm = &Core2A03::addrm_unused; break;
	case 0x09: op = &Core2A03::op_ora; addrm = &Core2A03::addrm_immb; break;
	case 0x0a: op = &Core2A03::op_asla; addrm = &Core2A03::addrm_acc; break;
	case 0x0b: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0x0c: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0x0d: op = &Core2A03::op_ora; addrm = &Core2A03::addrm_abs; break;
	case 0x0e: op = &Core2A03::op_asl; addrm = &Core2A03::addrm_abs; break;
	case 0x0f: std::cout << "unknown opcode: " << int{b} << '\n'; break;

	case 0x10: op = &Core2A03::op_br_fc<0x80>; addrm = &Core2A03::addrm_pcr; break; //bpl
	case 0x11: op = &Core2A03::op_ora; addrm = &Core2A03::addrm_zpiy; break;
	case 0x12: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0x13: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0x14: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0x15: op = &Core2A03::op_ora; addrm = &Core2A03::addrm_zpx; break;
	case 0x16: op = &Core2A03::op_asl; addrm = &Core2A03::addrm_zpx; break;
	case 0x17: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0x18: op = &Core2A03::op_clf<0x01>; addrm = &Core2A03::addrm_imp; break;
	case 0x19: op = &Core2A03::op_ora; addrm = &Core2A03::addrm_absy; break;
	case 0x1a: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0x1b: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0x1c: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0x1d: op = &Core2A03::op_ora; addrm = &Core2A03::addrm_absx; break;
	case 0x1e: op = &Core2A03::op_asl; addrm = &Core2A03::addrm_absx; break;
	case 0x1f: std::cout << "unknown opcode: " << int{b} << '\n'; break;

	case 0x20: op = &Core2A03::op_jsr; addrm = &Core2A03::addrm_immw; break;
	case 0x21: op = &Core2A03::op_and; addrm = &Core2A03::addrm_zpxi; break;
	case 0x22: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0x23: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0x24: op = &Core2A03::op_bit; addrm = &Core2A03::addrm_zp; break;
	case 0x25: op = &Core2A03::op_and; addrm = &Core2A03::addrm_zp; break;
	case 0x26: op = &Core2A03::op_rol; addrm = &Core2A03::addrm_zp; break;
	case 0x27: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0x28: op = &Core2A03::op_plp; addrm = &Core2A03::addrm_unused; break;
	case 0x29: op = &Core2A03::op_and; addrm = &Core2A03::addrm_immb; break;
	case 0x2a: op = &Core2A03::op_rola; addrm = &Core2A03::addrm_acc; break;
	case 0x2b: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0x2c: op = &Core2A03::op_bit; addrm = &Core2A03::addrm_abs; break;
	case 0x2d: op = &Core2A03::op_and; addrm = &Core2A03::addrm_abs; break;
	case 0x2e: op = &Core2A03::op_rol; addrm = &Core2A03::addrm_abs; break;
	case 0x2f: std::cout << "unknown opcode: " << int{b} << '\n'; break;

	case 0x30: op = &Core2A03::op_br_fs<0x80>; addrm = &Core2A03::addrm_pcr; break; //bmi
	case 0x31: op = &Core2A03::op_and; addrm = &Core2A03::addrm_zpiy; break;
	case 0x32: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0x33: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0x34: op = &Core2A03::op_bit; addrm = &Core2A03::addrm_zpx; break;
	case 0x35: op = &Core2A03::op_and; addrm = &Core2A03::addrm_zpx; break;
	case 0x36: op = &Core2A03::op_rol; addrm = &Core2A03::addrm_zpx; break;
	case 0x37: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0x38: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0x39: op = &Core2A03::op_and; addrm = &Core2A03::addrm_absy; break;
	case 0x3a: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0x3b: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0x3c: op = &Core2A03::op_bit; addrm = &Core2A03::addrm_absx; break;
	case 0x3d: op = &Core2A03::op_and; addrm = &Core2A03::addrm_absx; break;
	case 0x3e: op = &Core2A03::op_rol; addrm = &Core2A03::addrm_absx; break;
	case 0x3f: std::cout << "unknown opcode: " << int{b} << '\n'; break;

	case 0x40: op = &Core2A03::op_rti; addrm = &Core2A03::addrm_unused; break;
	case 0x41: op = &Core2A03::op_eor; addrm = &Core2A03::addrm_zpxi; break;
	case 0x42: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0x43: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0x44: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0x45: op = &Core2A03::op_eor; addrm = &Core2A03::addrm_zp; break;
	case 0x46: op = &Core2A03::op_lsr; addrm = &Core2A03::addrm_zp; break;
	case 0x47: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0x48: op = &Core2A03::op_pha; addrm = &Core2A03::addrm_unused; break;
	case 0x49: op = &Core2A03::op_eor; addrm = &Core2A03::addrm_immb; break;
	case 0x4a: op = &Core2A03::op_lsra; addrm = &Core2A03::addrm_acc; break;
	case 0x4b: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0x4c: op = &Core2A03::op_jmp; addrm = &Core2A03::addrm_immw; break;
	case 0x4d: op = &Core2A03::op_eor; addrm = &Core2A03::addrm_abs; break;
	case 0x4e: op = &Core2A03::op_lsr; addrm = &Core2A03::addrm_abs; break;
	case 0x4f: std::cout << "unknown opcode: " << int{b} << '\n'; break;

	case 0x50: op = &Core2A03::op_br_fc<0x40>; addrm = &Core2A03::addrm_pcr; break; //bvc
	case 0x51: op = &Core2A03::op_eor; addrm = &Core2A03::addrm_zpiy; break;
	case 0x52: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0x53: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0x54: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0x55: op = &Core2A03::op_eor; addrm = &Core2A03::addrm_zpx; break;
	case 0x56: op = &Core2A03::op_lsr; addrm = &Core2A03::addrm_zpx; break;
	case 0x57: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0x58: op = &Core2A03::op_clf<0x04>; addrm = &Core2A03::addrm_imp; break; //cli
	case 0x59: op = &Core2A03::op_eor; addrm = &Core2A03::addrm_absy; break;
	case 0x5a: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0x5b: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0x5c: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0x5d: op = &Core2A03::op_eor; addrm = &Core2A03::addrm_absx; break;
	case 0x5e: op = &Core2A03::op_lsr; addrm = &Core2A03::addrm_absx; break;
	case 0x5f: std::cout << "unknown opcode: " << int{b} << '\n'; break;

	case 0x60: op = &Core2A03::op_rts; addrm = &Core2A03::addrm_unused; break;
	case 0x61: op = &Core2A03::op_adc; addrm = &Core2A03::addrm_zpxi; break;
	case 0x62: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0x63: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0x64: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0x65: op = &Core2A03::op_adc; addrm = &Core2A03::addrm_zp; break;
	case 0x66: op = &Core2A03::op_ror; addrm = &Core2A03::addrm_zp; break;
	case 0x67: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0x68: op = &Core2A03::op_pla; addrm = &Core2A03::addrm_unused; break;
	case 0x69: op = &Core2A03::op_adc; addrm = &Core2A03::addrm_immb; break;
	case 0x6a: op = &Core2A03::op_rora; addrm = &Core2A03::addrm_acc; break;
	case 0x6b: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0x6c: op = &Core2A03::op_jmp; addrm = &Core2A03::addrm_abs; break;
	case 0x6d: op = &Core2A03::op_adc; addrm = &Core2A03::addrm_abs; break;
	case 0x6e: op = &Core2A03::op_ror; addrm = &Core2A03::addrm_abs; break;
	case 0x6f: std::cout << "unknown opcode: " << int{b} << '\n'; break;

	case 0x70: op = &Core2A03::op_br_fs<0x40>; addrm = &Core2A03::addrm_pcr; break; //bvs
	case 0x71: op = &Core2A03::op_adc; addrm = &Core2A03::addrm_zpiy; break;
	case 0x72: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0x73: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0x74: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0x75: op = &Core2A03::op_adc; addrm = &Core2A03::addrm_zpx; break;
	case 0x76: op = &Core2A03::op_ror; addrm = &Core2A03::addrm_zpx; break;
	case 0x77: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0x78: op = &Core2A03::op_sef<0x04>; addrm = &Core2A03::addrm_imp; break; //sei
	case 0x79: op = &Core2A03::op_adc; addrm = &Core2A03::addrm_absy; break;
	case 0x7a: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0x7b: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0x7c: op = &Core2A03::op_jmp; addrm = &Core2A03::addrm_absx; break;
	case 0x7d: op = &Core2A03::op_adc; addrm = &Core2A03::addrm_absx; break;
	case 0x7e: op = &Core2A03::op_ror; addrm = &Core2A03::addrm_absx; break;
	case 0x7f:break;

	case 0x80: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0x81: op = &Core2A03::op_sta; addrm = &Core2A03::addrm_zpxi; break;
	case 0x82: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0x83: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0x84: op = &Core2A03::op_sty; addrm = &Core2A03::addrm_zp; break;
	case 0x85: op = &Core2A03::op_sta; addrm = &Core2A03::addrm_zp; break;
	case 0x86: op = &Core2A03::op_stx; addrm = &Core2A03::addrm_zp; break;
	case 0x87: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0x88: op = &Core2A03::op_dey; addrm = &Core2A03::addrm_imp; break;
	case 0x89: op = &Core2A03::op_bit; addrm = &Core2A03::addrm_immb; break;
	case 0x8a: op = &Core2A03::op_txa; addrm = &Core2A03::addrm_imp; break;
	case 0x8b: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0x8c: op = &Core2A03::op_sty; addrm = &Core2A03::addrm_abs; break;
	case 0x8d: op = &Core2A03::op_sta; addrm = &Core2A03::addrm_abs; break;
	case 0x8e: op = &Core2A03::op_stx; addrm = &Core2A03::addrm_abs; break;
	case 0x8f: std::cout << "unknown opcode: " << int{b} << '\n'; break;

	case 0x90: op = &Core2A03::op_br_fc<0x01>; addrm = &Core2A03::addrm_pcr; break; //bcc
	case 0x91: op = &Core2A03::op_sta; addrm = &Core2A03::addrm_zpiy; break;
	case 0x92: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0x93: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0x94: op = &Core2A03::op_sty; addrm = &Core2A03::addrm_zpx; break;
	case 0x95: op = &Core2A03::op_sta; addrm = &Core2A03::addrm_zpx; break;
	case 0x96: op = &Core2A03::op_stx; addrm = &Core2A03::addrm_zpy; break;
	case 0x97: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0x98: op = &Core2A03::op_tya; addrm = &Core2A03::addrm_imp; break;
	case 0x99: op = &Core2A03::op_sta; addrm = &Core2A03::addrm_absy; break;
	case 0x9a: op = &Core2A03::op_txs; addrm = &Core2A03::addrm_imp; break;
	case 0x9b: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0x9c: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0x9d: op = &Core2A03::op_sta; addrm = &Core2A03::addrm_absx; break;
	case 0x9e: op = &Core2A03::op_stx; addrm = &Core2A03::addrm_absx; break;
	case 0x9f: std::cout << "unknown opcode: " << int{b} << '\n'; break;

	case 0xa0: op = &Core2A03::op_ldy; addrm = &Core2A03::addrm_immb; break;
	case 0xa1: op = &Core2A03::op_lda; addrm = &Core2A03::addrm_zpxi; break;
	case 0xa2: op = &Core2A03::op_ldx; addrm = &Core2A03::addrm_immb; break;
	case 0xa3: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0xa4: op = &Core2A03::op_ldy; addrm = &Core2A03::addrm_zp; break;
	case 0xa5: op = &Core2A03::op_lda; addrm = &Core2A03::addrm_zp; break;
	case 0xa6: op = &Core2A03::op_ldx; addrm = &Core2A03::addrm_zp; break;
	case 0xa7: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0xa8: op = &Core2A03::op_tay; addrm = &Core2A03::addrm_imp; break;
	case 0xa9: op = &Core2A03::op_lda; addrm = &Core2A03::addrm_immb; break;
	case 0xaa: op = &Core2A03::op_tax; addrm = &Core2A03::addrm_imp; break;
	case 0xab: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0xac: op = &Core2A03::op_ldy; addrm = &Core2A03::addrm_abs; break;
	case 0xad: op = &Core2A03::op_lda; addrm = &Core2A03::addrm_abs; break;
	case 0xae: op = &Core2A03::op_ldx; addrm = &Core2A03::addrm_abs; break;
	case 0xaf: std::cout << "unknown opcode: " << int{b} << '\n'; break;

	case 0xb0: op = &Core2A03::op_br_fs<0x01>; addrm = &Core2A03::addrm_pcr; break; //bcs
	case 0xb1: op = &Core2A03::op_lda; addrm = &Core2A03::addrm_zpiy; break;
	case 0xb2: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0xb3: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0xb4: op = &Core2A03::op_ldy; addrm = &Core2A03::addrm_zpx; break;
	case 0xb5: op = &Core2A03::op_lda; addrm = &Core2A03::addrm_zpx; break;
	case 0xb6: op = &Core2A03::op_ldx; addrm = &Core2A03::addrm_zpy; break;
	case 0xb7: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0xb8: op = &Core2A03::op_clf<0x40>; addrm = &Core2A03::addrm_imp; break; //clv
	case 0xb9: op = &Core2A03::op_lda; addrm = &Core2A03::addrm_absy; break;
	case 0xba: op = &Core2A03::op_tsx; addrm = &Core2A03::addrm_imp; break;
	case 0xbb: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0xbc: op = &Core2A03::op_ldy; addrm = &Core2A03::addrm_absx; break;
	case 0xbd: op = &Core2A03::op_lda; addrm = &Core2A03::addrm_absx; break;
	case 0xbe: op = &Core2A03::op_ldx; addrm = &Core2A03::addrm_absx; break;
	case 0xbf: std::cout << "unknown opcode: " << int{b} << '\n'; break;

	case 0xc0: op = &Core2A03::op_cpy; addrm = &Core2A03::addrm_immb; break;
	case 0xc1: op = &Core2A03::op_cmp; addrm = &Core2A03::addrm_zpxi; break;
	case 0xc2: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0xc3: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0xc4: op = &Core2A03::op_cpy; addrm = &Core2A03::addrm_zp; break;
	case 0xc5: op = &Core2A03::op_cmp; addrm = &Core2A03::addrm_zp; break;
	case 0xc6: op = &Core2A03::op_dec; addrm = &Core2A03::addrm_zp; break;
	case 0xc7: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0xc8: op = &Core2A03::op_iny; addrm = &Core2A03::addrm_imp; break;
	case 0xc9: op = &Core2A03::op_cmp; addrm = &Core2A03::addrm_immb; break;
	case 0xca: op = &Core2A03::op_dex; addrm = &Core2A03::addrm_imp; break;
	case 0xcb: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0xcc: op = &Core2A03::op_cpy; addrm = &Core2A03::addrm_abs; break;
	case 0xcd: op = &Core2A03::op_cmp; addrm = &Core2A03::addrm_abs; break;
	case 0xce: op = &Core2A03::op_dec; addrm = &Core2A03::addrm_abs; break;
	case 0xcf: std::cout << "unknown opcode: " << int{b} << '\n'; break;

	case 0xd0: op = &Core2A03::op_br_fc<0x02>; addrm = &Core2A03::addrm_pcr; break; //bne/bnz
	case 0xd1: op = &Core2A03::op_cmp; addrm = &Core2A03::addrm_zpiy; break;
	case 0xd2: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0xd3: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0xd4: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0xd5: op = &Core2A03::op_cmp; addrm = &Core2A03::addrm_zpx; break;
	case 0xd6: op = &Core2A03::op_dec; addrm = &Core2A03::addrm_zpx; break;
	case 0xd7: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0xd8: op = &Core2A03::op_clf<0x08>; addrm = &Core2A03::addrm_imp; break; //cld
	case 0xd9: op = &Core2A03::op_cmp; addrm = &Core2A03::addrm_absy; break;
	case 0xda: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0xdb: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0xdc: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0xdd: op = &Core2A03::op_cmp; addrm = &Core2A03::addrm_absx; break;
	case 0xde: op = &Core2A03::op_dec; addrm = &Core2A03::addrm_absx; break;
	case 0xdf: std::cout << "unknown opcode: " << int{b} << '\n'; break;

	case 0xe0: op = &Core2A03::op_cpx; addrm = &Core2A03::addrm_immb; break;
	case 0xe1: op = &Core2A03::op_sbc; addrm = &Core2A03::addrm_zpxi; break;
	case 0xe2: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0xe3: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0xe4: op = &Core2A03::op_cpx; addrm = &Core2A03::addrm_zp; break;
	case 0xe5: op = &Core2A03::op_sbc; addrm = &Core2A03::addrm_zp; break;
	case 0xe6: op = &Core2A03::op_inc; addrm = &Core2A03::addrm_zp; break;
	case 0xe7: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0xe8: op = &Core2A03::op_inx; addrm = &Core2A03::addrm_imp; break;
	case 0xe9: op = &Core2A03::op_sbc; addrm = &Core2A03::addrm_immb; break;
	case 0xea: op = &Core2A03::op_nop; addrm = &Core2A03::addrm_imp; break;
	case 0xeb: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0xec: op = &Core2A03::op_cpx; addrm = &Core2A03::addrm_abs; break;
	case 0xed: op = &Core2A03::op_sbc; addrm = &Core2A03::addrm_abs; break;
	case 0xee: op = &Core2A03::op_inc; addrm = &Core2A03::addrm_abs; break;
	case 0xef: std::cout << "unknown opcode: " << int{b} << '\n'; break;

	case 0xf0: op = &Core2A03::op_br_fs<0x02>; addrm = &Core2A03::addrm_pcr; break; //beq/bz
	case 0xf1: op = &Core2A03::op_sbc; addrm = &Core2A03::addrm_zpiy; break;
	case 0xf2: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0xf3: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0xf4: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0xf5: op = &Core2A03::op_sbc; addrm = &Core2A03::addrm_zpx; break;
	case 0xf6: op = &Core2A03::op_inc; addrm = &Core2A03::addrm_zpx; break;
	case 0xf7: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0xf8: op = &Core2A03::op_sef<0x08>; addrm = &Core2A03::addrm_imp; break; //sed
	case 0xf9: op = &Core2A03::op_sbc; addrm = &Core2A03::addrm_absy; break;
	case 0xfa: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0xfb: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0xfc: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	case 0xfd: op = &Core2A03::op_sbc; addrm = &Core2A03::addrm_absx; break;
	case 0xfe: op = &Core2A03::op_inc; addrm = &Core2A03::addrm_absx; break;
	case 0xff: std::cout << "unknown opcode: " << int{b} << '\n'; break;
	
	}

}

#ifdef WASM
#include <emscripten/bind.h>
EMSCRIPTEN_BINDINGS(2A03){
	emscripten::class_<MicroSim::Ricoh::Core2A03,
						emscripten::base<MicroSim::MOS::Core6502>>("Core2A03")
		.constructor<unsigned long int>()
		;
}
#endif