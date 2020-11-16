#include "65C02.hpp"
#include "65C02_addrm.hpp"
#include "65C02_op.hpp"
using namespace MicroSim::WDC;

void CoreW65C02S::step()
{
	//get the next instruction
	std::uint8_t instr = m_memory.read_byte(PC++);
	_cycles++;
	switch(instr){
		case 0x00: op_brk(addrm_immb()); break;
		case 0x01: op_ora(addrm_zpx_ind()); break;
		case 0x02: break;
		case 0x03: break;
		case 0x04: /*TSB*/ break;
		case 0x05: op_ora(addrm_zp()); break;
		case 0x06: op_asl(addrm_zp()); break;
		case 0x07: /*RMB0*/ break;
		case 0x08: op_php(addrm_imp()); break;
		case 0x09: op_ora(addrm_immb()); break;
		case 0x0a: op_asla(addrm_acc()); break;
		case 0x0b: break;
		case 0x0c: /*TSB*/ break;
		case 0x0d: op_ora(addrm_abs()); break;
		case 0x0e: op_asl(addrm_abs());break;
		case 0x0f: /*BBR0*/ break;

		case 0x10: op_bpl(addrm_pcr()); break;
		case 0x11: op_ora(addrm_zp_indy()); break;
		case 0x12: op_ora(addrm_zp_ind()); break;
		case 0x13: break;
		case 0x14: /*TRB*/ break;
		case 0x15: op_ora(addrm_zpx()); break;
		case 0x16: op_asl(addrm_zpx()); break;
		case 0x17: /*RMB1*/ break;
		case 0x18: op_clc(addrm_imp()); break;
		case 0x19: op_ora(addrm_absy()); break;
		case 0x1a: op_ina(addrm_acc()); break;
		case 0x1b: break;
		case 0x1c: /*TRB*/ break;
		case 0x1d: op_ora(addrm_absx()); break;
		case 0x1e: op_asl(addrm_absx()); break;
		case 0x1f: /*BBR1*/ break;

		case 0x20: op_jsr(addrm_abs()); break;
		case 0x21: op_and(addrm_zpx_ind()); break;
		case 0x22: break;
		case 0x23: break;
		case 0x24: op_bit(addrm_zp()); break;
		case 0x25: op_and(addrm_zp()); break;
		case 0x26: op_rol(addrm_zp()); break;
		case 0x27: /*RMB*/ break;
		case 0x28: op_plp(addrm_imp()); break;
		case 0x29: op_and(addrm_immb()); break;
		case 0x2a: op_rola(addrm_acc()); break;
		case 0x2b: break;
		case 0x2c: op_bit(addrm_abs()); break;
		case 0x2d: op_and(addrm_abs()); break;
		case 0x2e: op_rol(addrm_abs()); break;
		case 0x2f: /*BBR*/ break;

		case 0x30: op_bmi(addrm_pcr()); break;
		case 0x31: op_and(addrm_zp_indy()); break;
		case 0x32: op_and(addrm_zp_ind()); break;
		case 0x33: break;
		case 0x34: op_bit(addrm_zpx()); break;
		case 0x35: op_and(addrm_zpx()); break;
		case 0x36: op_rol(addrm_zpx()); break;
		case 0x37: /*RMB*/ break;
		case 0x38: op_sec(addrm_imp()); break;
		case 0x39: op_and(addrm_absy()); break;
		case 0x3a: op_dea(addrm_acc()); break;
		case 0x3b: break;
		case 0x3c: op_bit(addrm_absx()); break;
		case 0x3d: op_and(addrm_absx()); break;
		case 0x3e: op_rol(addrm_absx()); break;
		case 0x3f: /*BBR*/ break;

		case 0x40: op_rti(addrm_imp()); break;
		case 0x41: op_eor(addrm_zpx_ind()); break;
		case 0x42: break;
		case 0x43: break;
		case 0x44: break;
		case 0x45: op_eor(addrm_zp()); break;
		case 0x46: op_lsr(addrm_zp()); break;
		case 0x47: /*RMB*/ break;
		case 0x48: op_pha(addrm_imp()); break;
		case 0x49: op_eor(addrm_immb()); break;
		case 0x4a: op_lsra(addrm_acc()); break;
		case 0x4b: break;
		case 0x4c: op_jmp(addrm_abs()); break;
		case 0x4d: op_eor(addrm_abs()); break;
		case 0x4e: op_lsr(addrm_abs()); break;
		case 0x4f: /*BBR*/ break;

		case 0x50: op_bvc(addrm_pcr()); break;
		case 0x51: op_eor(addrm_zp_indy()); break;
		case 0x52: op_eor(addrm_zp_ind()); break;
		case 0x53: break;
		case 0x54: break;
		case 0x55: op_eor(addrm_zpx()); break;
		case 0x56: op_lsr(addrm_zpx()); break;
		case 0x57: /*RMB*/ break;
		case 0x58: op_cli(addrm_imp()); break;
		case 0x59: op_eor(addrm_absy()); break;
		case 0x5a: op_phy(addrm_imp()); break;
		case 0x5b: break;
		case 0x5c: break;
		case 0x5d: op_eor(addrm_absx()); break;
		case 0x5e: op_lsr(addrm_absx()); break;
		case 0x5f: /*BBR*/ break;

		case 0x60: op_rts(addrm_imp()); break;
		case 0x61: op_adc(addrm_zpx_ind()); break;
		case 0x62: break;
		case 0x63: break;
		case 0x64: op_stz(addrm_zp()); break;
		case 0x65: op_adc(addrm_zp()); break;
		case 0x66: op_ror(addrm_zp()); break;
		case 0x67: /*RMB*/ break;
		case 0x68: op_pla(addrm_imp()); break;
		case 0x69: op_adc(addrm_immb()); break;
		case 0x6a: op_rora(addrm_acc()); break;
		case 0x6b: break;
		case 0x6c: op_jmp(addrm_abs_ind()); break;
		case 0x6d: op_adc(addrm_abs()); break;
		case 0x6e: op_ror(addrm_abs()); break;
		case 0x6f: /*BBR*/ break;

		case 0x70: op_bvs(addrm_pcr()); break;
		case 0x71: op_adc(addrm_zp_indy()); break;
		case 0x72: op_adc(addrm_zp_ind()); break;
		case 0x73: break;
		case 0x74: op_stz(addrm_zpx()); break;
		case 0x75: op_adc(addrm_zpx()); break;
		case 0x76: op_ror(addrm_zpx()); break;
		case 0x77: /*RMB*/ break;
		case 0x78: op_sei(addrm_imp()); break;
		case 0x79: op_adc(addrm_absy()); break;
		case 0x7a: op_ply(addrm_imp()); break;
		case 0x7b: break;
		case 0x7c: op_jmp(addrm_absx_ind()); break;
		case 0x7d: op_adc(addrm_absx()); break;
		case 0x7e: op_ror(addrm_absx());break;
		case 0x7f: /*BBR*/ break;

		case 0x80: op_bra(addrm_pcr()); break;
		case 0x81: op_sta(addrm_zpx_ind()); break;
		case 0x82: break;
		case 0x83: break;
		case 0x84: op_sty(addrm_zp()); break;
		case 0x85: op_sta(addrm_zp()); break;
		case 0x86: op_stx(addrm_zp()); break;
		case 0x87: /*SMB*/ break;
		case 0x88: op_dey(addrm_imp()); break;
		case 0x89: op_bit(addrm_immb()); break;
		case 0x8a: op_txa(addrm_imp()); break;
		case 0x8b: break;
		case 0x8c: op_sty(addrm_abs()); break;
		case 0x8d: op_sta(addrm_abs()); break;
		case 0x8e: op_stx(addrm_abs()); break;
		case 0x8f: /*BBS*/ break;

		case 0x90: op_bcc(addrm_pcr()); break;
		case 0x91: op_sta(addrm_zp_indy()); break;
		case 0x92: op_sta(addrm_zp_ind()); break;
		case 0x93: break;
		case 0x94: op_sty(addrm_zpx()); break;
		case 0x95: op_sta(addrm_zpx()); break;
		case 0x96: op_stz(addrm_zpy());break;
		case 0x97: /*SMB*/ break;
		case 0x98: op_tya(addrm_imp()); break;
		case 0x99: op_sta(addrm_absy()); break;
		case 0x9a: op_txs(addrm_imp()); break;
		case 0x9b: break;
		case 0x9c: op_stz(addrm_abs()); break;
		case 0x9d: op_sta(addrm_absx()); break;
		case 0x9e: op_stz(addrm_absx()); break;
		case 0x9f: /*BBS*/ break;

		case 0xa0: op_ldy(addrm_immb()); break;
		case 0xa1: op_lda(addrm_zpx_ind()); break;
		case 0xa2: op_ldx(addrm_immb()); break;
		case 0xa3: break;
		case 0xa4: op_ldy(addrm_zp()); break;
		case 0xa5: op_lda(addrm_zp()); break;
		case 0xa6: op_ldx(addrm_zp()); break;
		case 0xa7: /*SMB*/ break;
		case 0xa8: op_tay(addrm_imp()); break;
		case 0xa9: op_lda(addrm_immb()); break;
		case 0xaa: op_tax(addrm_imp()); break;
		case 0xab: break;
		case 0xac: op_ldy(addrm_abs()); break;
		case 0xad: op_lda(addrm_abs()); break;
		case 0xae: op_ldx(addrm_abs()); break;
		case 0xaf: /*BBS*/ break;

		case 0xb0: op_bcs(addrm_pcr()); break;
		case 0xb1: op_lda(addrm_zp_indy()); break;
		case 0xb2: op_lda(addrm_zp_ind()); break;
		case 0xb3: break;
		case 0xb4: op_ldy(addrm_zpx()); break;
		case 0xb5: op_lda(addrm_zpx()); break;
		case 0xb6: op_ldx(addrm_zpy()); break;
		case 0xb7: /*SMB*/ break;
		case 0xb8: op_clv(addrm_imp()); break;
		case 0xb9: op_lda(addrm_absy()); break;
		case 0xba: op_tsx(addrm_imp()); break;
		case 0xbb: break;
		case 0xbc: op_ldy(addrm_absx()); break;
		case 0xbd: op_lda(addrm_absx()); break;
		case 0xbe: op_ldx(addrm_absy()); break;
		case 0xbf: /*BBS*/ break;

		case 0xc0: op_cpy(addrm_immb()); break;
		case 0xc1: op_cmp(addrm_zpx_ind()); break;
		case 0xc2: break;
		case 0xc3: break;
		case 0xc4: op_cpy(addrm_zp()); break;
		case 0xc5: op_cmp(addrm_zp()); break;
		case 0xc6: op_dec(addrm_zp()); break;
		case 0xc7: /*SMB*/ break;
		case 0xc8: op_iny(addrm_imp()); break;
		case 0xc9: op_cmp(addrm_immb()); break;
		case 0xca: op_dex(addrm_imp()); break;
		case 0xcb: op_wai(addrm_imp()); break;
		case 0xcc: op_cpy(addrm_abs()); break;
		case 0xcd: op_cmp(addrm_abs()); break;
		case 0xce: op_dec(addrm_abs()); break;
		case 0xcf: /*BBS*/ break;

		case 0xd0: op_bne(addrm_pcr()); break;
		case 0xd1: op_cmp(addrm_zp_indy()); break;
		case 0xd2: op_cmp(addrm_zp_ind()); break;
		case 0xd3: break;
		case 0xd4: break;
		case 0xd5: op_cmp(addrm_zpx()); break;
		case 0xd6: op_dec(addrm_zpx()); break;
		case 0xd7: /*SMB*/ break;
		case 0xd8: op_cld(addrm_imp()); break;
		case 0xd9: op_cmp(addrm_absy()); break;
		case 0xda: op_phx(addrm_imp()); break;
		case 0xdb: op_stp(addrm_imp()); break;
		case 0xdc: break;
		case 0xdd: op_cmp(addrm_absx()); break;
		case 0xde: op_dec(addrm_absx()); break;
		case 0xdf: /*BBS*/ break;

		case 0xe0: op_cpx(addrm_immb()); break;
		case 0xe1: op_sbc(addrm_zpx_ind()); break;
		case 0xe2: break;
		case 0xe3: break;
		case 0xe4: op_cpx(addrm_zp()); break;
		case 0xe5: op_sbc(addrm_zp()); break;
		case 0xe6: op_inc(addrm_zp()); break;
		case 0xe7: /*SMB*/ break;
		case 0xe8: op_inx(addrm_imp()); break;
		case 0xe9: op_sbc(addrm_immb()); break;
		case 0xea: op_nop(addrm_imp()); break;
		case 0xeb: break;
		case 0xec: op_cpx(addrm_abs()); break;
		case 0xed: op_sbc(addrm_abs()); break;
		case 0xee: op_inc(addrm_abs()); break;
		case 0xef: /*BBS*/ break;

		case 0xf0: op_beq(addrm_pcr()); break;
		case 0xf1: op_sbc(addrm_zp_indy()); break;
		case 0xf2: op_sbc(addrm_zp_ind()); break;
		case 0xf3: break;
		case 0xf4: break;
		case 0xf5: op_sbc(addrm_zpx()); break;
		case 0xf6: op_inc(addrm_zpx()); break;
		case 0xf7: /*SMB*/ break;
		case 0xf8: op_sed(addrm_imp()); break;
		case 0xf9: op_sbc(addrm_absy()); break;
		case 0xfa: op_plx(addrm_imp()); break;
		case 0xfb: break;
		case 0xfc: break;
		case 0xfd: op_sbc(addrm_absx()); break;
		case 0xfe: op_inx(addrm_absx()); break;
		case 0xff: /*BBS*/ break;
	}
	update_cycles();
}

void CoreW65C02S::reset()
{
	Addr _rst_vec = m_memory.read_word(vec_res);
	P &= ~0x3C;
	P |= 0x34;
	SP = 0xFF;
	PC = _rst_vec;
	_cycles += 7;
}

CoreW65C02S::CoreW65C02S() : 
	Core{{
		{"A", 1, &A},
		{"X", 1, &X},
		{"Y", 1, &Y},
		{"P", 1, &P},
		{"SP", 1, &SP},
		{"PC", 2, &PC}
	}}
{

}

CoreW65C02S::~CoreW65C02S()
{}

std::shared_ptr<MicroSim::Core> CoreW65C02S::createCore()
{
	return std::make_shared<CoreW65C02S>();
}

void CoreW65C02S::push_byte(std::uint8_t b)
{
	m_memory.write_byte(0x100|SP--, b);
}
void CoreW65C02S::push_word(std::uint16_t w)
{
	push_byte(w>>8);
	push_byte(w&0xFF);
}
std::uint8_t CoreW65C02S::pop_byte()
{
	return m_memory.read_byte(0x100|++SP);
}
std::uint16_t CoreW65C02S::pop_word()
{
	std::uint16_t w = 0;
	w = pop_byte();
	w |= pop_byte()<<8;
	return w;
}

#ifdef WASM

#include <emscripten/bind.h>

EMSCRIPTEN_BINDINGS(65C02){
	emscripten::class_<MicroSim::WDC::CoreW65C02S, emscripten::base<MicroSim::Core>>("CoreW65C02S")
		.constructor<>()
		.class_function("createCore", &MicroSim::WDC::CoreW65C02S::createCore)
		// .function("step", &MicroSim::WDC::CoreW65C02S::step)
		// .function("reset", &MicroSim::WDC::CoreW65C02S::reset)

		;
}

#endif