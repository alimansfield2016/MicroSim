#pragma once
#include "W65C02S_timed.hpp"

using namespace MicroSim::WDC;

void CoreW65C02S_timed::op_adc()
{
	TMP = read_byte(_addr);
	std::uint8_t t = A;
	// Decimal mode not yet supported
	// if(D){
	// 	uint8_t res1 = TMP&0x0F + A&0x0F + C;
	// 	if(res1 < 9) C = 1;
	// 	op = &CoreW65C02S_timed::op_adc1;
	// }else
	{
		std::uint16_t res = TMP + A + C;
		A = static_cast<std::uint8_t>(res);
		C = (res&0x100)>0;
		N = (A&0x80)>0;
		Z = (A == 0);
		V = ((A^t^TMP)&0x80)>0;
		state = State::FETCH;
	}
}
void CoreW65C02S_timed::op_adc2()
{
	// Decimal mode not yet supported
	state = State::FETCH;
}

void CoreW65C02S_timed::op_and()
{
	TMP = read_byte(_addr);
	A &= TMP;
	N = (A&0x80)>0;
	Z = (A == 0);
	state = State::FETCH;
}

void CoreW65C02S_timed::op_asla()
{
	std::uint8_t &R = A;
	C = R&0x80;
	R <<=1;
	Z = (R==0);
	N = (R&0x80)>0;
}
void CoreW65C02S_timed::op_asl()
{
	TMP = read_byte(_addr);
	op = &CoreW65C02S_timed::op_asl_m;
}
void CoreW65C02S_timed::op_asl_m()
{
	C = TMP&0x80;
	TMP <<=1;
	Z = (TMP==0);
	N = (TMP&0x80)>0;
	op = &CoreW65C02S_timed::op_rmw_w;
}

void CoreW65C02S_timed::op_bit()
{
	TMP = read_byte(_addr);
	P &= ~0xC0;
	P |= TMP&0xC0;
	Z = !(TMP&A);
	state = State::FETCH;
}

template<std::uint8_t b>
void CoreW65C02S_timed::op_bbr() 
{
	TMP = read_byte(_addr);
	addrm = &CoreW65C02S_timed::addrm_pcr;
	op = &CoreW65C02S_timed::op_bbr2<b>;
	state = State::ADDR;
}
template<std::uint8_t b>
void CoreW65C02S_timed::op_bbr2() 
{
	if((TMP&b))
		state = State::FETCH;
	op = &CoreW65C02S_timed::op_br;
}
template<std::uint8_t b>
void CoreW65C02S_timed::op_bbs() 
{
	TMP = read_byte(_addr);
	addrm = &CoreW65C02S_timed::addrm_pcr;
	op = &CoreW65C02S_timed::op_bbs2<b>;
	state = State::ADDR;
}
template<std::uint8_t b>
void CoreW65C02S_timed::op_bbs2() 
{
	if(!(TMP&b))
		state = State::FETCH;
	op = &CoreW65C02S_timed::op_br;
}

template<std::uint8_t F>
void CoreW65C02S_timed::op_br_fs()
{
	if(!(P&F))
		state = State::FETCH;
	op = &CoreW65C02S_timed::op_br;
}
template<std::uint8_t F>
void CoreW65C02S_timed::op_br_fc()
{
	if((P&F))
		state = State::FETCH;
	op = &CoreW65C02S_timed::op_br;
}

void CoreW65C02S_timed::op_bra()
{
	op = &CoreW65C02S_timed::op_br;
}
void CoreW65C02S_timed::op_br()
{
	read_byte(PC);
	std::uint16_t pcl = PCL + _addr;
	PCL = pcl;
	if(!(!!(pcl&0x100) ^ !!(_addr&0x80)))
		state = State::FETCH;
	op = &CoreW65C02S_timed::op_br2;
}
void CoreW65C02S_timed::op_br2()
{
	read_byte(PC);
	PCH++;
}
/*

103 + (-)80 = 83
172 + (-)c2 = e2+42 = 134
72 + c2 = 134
00 + ff = (0) ff 0 would have page change
01 + ff = (1) 00 1 wouldnt have page change
80 + ff = (1) 7f 0 wouldnt have page change
81 + ff = (1) 80 1 wouldnt have page change
00 + 7f = (0) 7f no change
01 + 7f = (0) 80 no change
80 + 7f = (0) ff no change
81 + 7f = (1) 00 page change

*/

void CoreW65C02S_timed::op_brk()
{
	read_byte(_addr);
	op = &CoreW65C02S_timed::op_brk2;
}
void CoreW65C02S_timed::op_brk2()
{
	push_byte(PCH);
	op = &CoreW65C02S_timed::op_brk3;
}
void CoreW65C02S_timed::op_brk3()
{
	push_byte(PCL);
	op = &CoreW65C02S_timed::op_brk4;
}
void CoreW65C02S_timed::op_brk4()
{
	push_byte(P);
	_addr = vec_brk;
	op = &CoreW65C02S_timed::op_vecl;
}


template<std::uint8_t F> 
void CoreW65C02S_timed::op_clf()
{
	P &= ~F;
	state = State::FETCH;
}
template<std::uint8_t F> 
void CoreW65C02S_timed::op_sef()
{
	P |= F;
	state = State::FETCH;
}

void CoreW65C02S_timed::op_cmp()
{
	TMP = read_byte(_addr);
	std::uint16_t res = A + ~TMP + 1;
	N = res&0x80;
	Z = res==0;
	C = res&0x100;
	state = State::FETCH;
}
void CoreW65C02S_timed::op_cpx()
{
	TMP = read_byte(_addr);
	std::uint16_t res = X + ~TMP + 1;
	N = res&0x80;
	Z = res==0;
	C = res&0x100;
	state = State::FETCH;	
}
void CoreW65C02S_timed::op_cpy()
{
	TMP = read_byte(_addr);
	std::uint16_t res = Y + ~TMP + 1;
	N = res&0x80;
	Z = res==0;
	C = res&0x100;
	state = State::FETCH;	
}

void CoreW65C02S_timed::op_dec()
{
	TMP = read_byte(_addr);
	op = &CoreW65C02S_timed::op_dec2;
}
void CoreW65C02S_timed::op_dec2()
{
	read_byte(_addr);
	TMP--;
	Z = (TMP==0);
	N = (TMP&0x80)>0;
	op = &CoreW65C02S_timed::op_rmw_w;
}
void CoreW65C02S_timed::op_dea()
{
	A--;
	Z = (A==0);
	N = (A&0x80)>0;
	state = State::FETCH;
}
void CoreW65C02S_timed::op_dex()
{
	X--;
	Z = (X==0);
	N = (X&0x80)>0;
	state = State::FETCH;
}
void CoreW65C02S_timed::op_dey()
{
	Y--;
	Z = (Y==0);
	N = (Y&0x80)>0;
	state = State::FETCH;
}

void CoreW65C02S_timed::op_eor()
{
	TMP = read_byte(_addr);
	A ^= TMP;
	N = (A&0x80)>0;
	Z = (A == 0);
	state = State::FETCH;
}

void CoreW65C02S_timed::op_inc()
{
	TMP = read_byte(_addr);
	op = &CoreW65C02S_timed::op_inc2;
}
void CoreW65C02S_timed::op_inc2()
{
	read_byte(_addr);
	TMP++;
	Z = (TMP==0);
	N = (TMP&0x80)>0;
	op = &CoreW65C02S_timed::op_rmw_w;
}
void CoreW65C02S_timed::op_ina()
{
	A++;
	Z = (A==0);
	N = (A&0x80)>0;
	state = State::FETCH;
}
void CoreW65C02S_timed::op_inx()
{
	X++;
	Z = (X==0);
	N = (X&0x80)>0;
	state = State::FETCH;
}
void CoreW65C02S_timed::op_iny()
{
	Y++;
	Z = (Y==0);
	N = (Y&0x80)>0;
	state = State::FETCH;
}

void CoreW65C02S_timed::op_jmp()
{
	PCL = read_byte(_addr);
	op = &CoreW65C02S_timed::op_jmp2;
}
void CoreW65C02S_timed::op_jmp2()
{
	PCH = read_byte(_addr+1);
	state = State::FETCH;
}

void CoreW65C02S_timed::op_jsr()
{
	TMP = read_byte(_addr++);
	op = &CoreW65C02S_timed::op_jsr2;
}
void CoreW65C02S_timed::op_jsr2()
{
	read_byte(0x100|SP);
	op = &CoreW65C02S_timed::op_jsr3;
}
void CoreW65C02S_timed::op_jsr3()
{
	push_byte(PCH);
	op = &CoreW65C02S_timed::op_jsr4;
}
void CoreW65C02S_timed::op_jsr4()
{
	push_byte(PCL);
	op = &CoreW65C02S_timed::op_jsr5;
}
void CoreW65C02S_timed::op_jsr5()
{
	PCH = read_byte(_addr++);
	PCL = TMP;
	state = State::FETCH;
}

void CoreW65C02S_timed::op_lda()
{
	std::uint8_t &R = A;
	R = read_byte(_addr);
	Z = !R;
	N = R&0x80;
	state = State::FETCH;
}
void CoreW65C02S_timed::op_ldx()
{
	std::uint8_t &R = X;
	R = read_byte(_addr);
	Z = !R;
	N = R&0x80;
	state = State::FETCH;
}
void CoreW65C02S_timed::op_ldy()
{
	std::uint8_t &R = Y;
	R = read_byte(_addr);
	Z = !R;
	N = R&0x80;
	state = State::FETCH;
}

void CoreW65C02S_timed::op_lsra()
{
	std::uint8_t &R = A;
	C = R&0x01;
	R >>=1;
	Z = (R==0);
	N = (R&0x80)>0;
	state = State::FETCH;
}
void CoreW65C02S_timed::op_lsr()
{
	TMP = read_byte(_addr);
	op = &CoreW65C02S_timed::op_lsr2;
}
void CoreW65C02S_timed::op_lsr2()
{
	read_byte(_addr);
	std::uint8_t &R = TMP;
	C = R&0x01;
	R >>=1;
	Z = (R==0);
	N = (R&0x80)>0;
	op = &CoreW65C02S_timed::op_rmw_w;
}

void CoreW65C02S_timed::op_nop()
{
	state = State::FETCH;
}

void CoreW65C02S_timed::op_ora()
{
	TMP = read_byte(_addr);
	A |= TMP;
	N = (A&0x80)>0;
	Z = (A == 0);
	state = State::FETCH;
}


void CoreW65C02S_timed::op_pha(){
	push_byte(A);
	state = State::FETCH;
}
void CoreW65C02S_timed::op_php(){
	push_byte(P);
	state = State::FETCH;
}
void CoreW65C02S_timed::op_phx(){
	push_byte(X);
	state = State::FETCH;
}
void CoreW65C02S_timed::op_phy(){
	push_byte(Y);
	state = State::FETCH;
}
void CoreW65C02S_timed::op_pla(){
	A = pop_byte();
	state = State::FETCH;
}
void CoreW65C02S_timed::op_plp(){
	P = pop_byte();
	state = State::FETCH;
}
void CoreW65C02S_timed::op_plx(){
	X = pop_byte();
	state = State::FETCH;
}
void CoreW65C02S_timed::op_ply(){
	Y = pop_byte();
	state = State::FETCH;
}


template<std::uint8_t b>
void CoreW65C02S_timed::op_rmb()
{
	TMP = read_byte(_addr);
	op = &CoreW65C02S_timed::op_rmb_m<b>;
}
template<std::uint8_t b>
void CoreW65C02S_timed::op_rmb_m()
{
	TMP &= ~b;
	op = &CoreW65C02S_timed::op_rmw_w;
}
template<std::uint8_t b>
void CoreW65C02S_timed::op_smb()
{
	TMP = read_byte(_addr);
	op = &CoreW65C02S_timed::op_smb_m<b>;
}
template<std::uint8_t b>
void CoreW65C02S_timed::op_smb_m()
{
	TMP |= b;
	op = &CoreW65C02S_timed::op_rmw_w;
}

void CoreW65C02S_timed::op_rmw_w()
{
	write_byte(_addr, TMP);
	state = State::FETCH;
}

void CoreW65C02S_timed::op_rola()
{
	std::uint8_t &R = A;
	bool c = R&0x80;
	R <<=1;
	if(C)
		R |= 0x01;
	C = c;
	Z = (R==0);
	N = (R&0x80);
	state = State::FETCH;
}
void CoreW65C02S_timed::op_rol()
{
	TMP = read_byte(_addr);
	op = &CoreW65C02S_timed::op_rol_m;
}
void CoreW65C02S_timed::op_rol_m()
{
	std::uint8_t &R = TMP;
	bool c = R&0x80;
	R <<=1;
	if(C)
		R |= 0x01;
	C = c;
	Z = (R==0);
	N = (R&0x80);

	op = &CoreW65C02S_timed::op_rmw_w;
}

void CoreW65C02S_timed::op_rora()
{
	std::uint8_t &R = A;
	bool c = R&0x01;
	R >>=1;
	if(C)
		R |= 0x80;
	C = c;
	Z = (R==0);
	N = (R&0x80);
	state = State::FETCH;
}
void CoreW65C02S_timed::op_ror()
{
	TMP = read_byte(_addr);
	op = &CoreW65C02S_timed::op_ror_m;
}
void CoreW65C02S_timed::op_ror_m()
{
	std::uint8_t &R = TMP;
	bool c = R&0x01;
	R >>=1;
	if(C)
		R |= 0x80;
	C = c;
	Z = (R==0);
	N = (R&0x80);

	op = &CoreW65C02S_timed::op_rmw_w;
}

void CoreW65C02S_timed::op_rti()
{
	P = pop_byte();
	op = &CoreW65C02S_timed::op_rts;
}
void CoreW65C02S_timed::op_rts()
{
	_addr_l = pop_byte();
	op = &CoreW65C02S_timed::op_rts2;
}
void CoreW65C02S_timed::op_rts2()
{
	_addr_h = pop_byte();
	op = &CoreW65C02S_timed::op_rts3;
}
void CoreW65C02S_timed::op_rts3()
{
	read_byte(PC);
	PC = _addr;
	state = State::FETCH;
}

void CoreW65C02S_timed::op_sbc()
{
	TMP = read_byte(_addr);
	std::uint8_t t = A;
	std::uint16_t res = A + TMP + C;
	A = static_cast<std::uint8_t>(res);
	C = (res&0x100)>0;
	N = (A&0x80)>0;
	Z = (A == 0);
	V = ((A^t^TMP)&0x80)>0;
	state = State::FETCH;
}
void CoreW65C02S_timed::op_sbc2()
{
	state = State::FETCH;
}


void CoreW65C02S_timed::op_sta()
{
	std::uint8_t &R = A;
	write_byte(_addr, R);
	state = State::FETCH;
}
void CoreW65C02S_timed::op_stx()
{
	std::uint8_t &R = X;
	write_byte(_addr, R);
	state = State::FETCH;
}
void CoreW65C02S_timed::op_sty()
{
	std::uint8_t &R = Y;
	write_byte(_addr, R);
	state = State::FETCH;
}
void CoreW65C02S_timed::op_stz()
{
	write_byte(_addr, 0);
	state = State::FETCH;
}

void CoreW65C02S_timed::op_tax()
{
	X = A;
	N = X&0x80;
	Z = !X;
	state = State::FETCH;
}
void CoreW65C02S_timed::op_tay()
{
	Y = A;
	N = Y&0x80;
	Z = !Y;
	state = State::FETCH;
}
void CoreW65C02S_timed::op_tsx()
{
	X = SP;
	N = X&0x80;
	Z = !X;
	state = State::FETCH;
}
void CoreW65C02S_timed::op_txa()
{
	A = X;
	N = A&0x80;
	Z = !A;
	state = State::FETCH;
}
void CoreW65C02S_timed::op_txs()
{
	SP = X;
	state = State::FETCH;
}
void CoreW65C02S_timed::op_tya()
{
	A = Y;
	N = A&0x80;
	Z = !A;
	state = State::FETCH;
}

void CoreW65C02S_timed::op_trb()
{
	TMP = read_byte(_addr);
	op = &CoreW65C02S_timed::op_trb_m;
}
void CoreW65C02S_timed::op_trb_m()
{
	TMP &= ~A;
	Z = !TMP;
	op = &CoreW65C02S_timed::op_rmw_w;
}
void CoreW65C02S_timed::op_tsb()
{
	TMP = read_byte(_addr);
	op = &CoreW65C02S_timed::op_tsb_m;
}
void CoreW65C02S_timed::op_tsb_m()
{
	TMP |= A;
	Z = !TMP;
	op = &CoreW65C02S_timed::op_rmw_w;
}


void CoreW65C02S_timed::op_vecl()
{
	PCL = read_byte(_addr);
	op = &CoreW65C02S_timed::op_vech;
}
void CoreW65C02S_timed::op_vech()
{
	PCH = read_byte(_addr+1);
	state = State::FETCH;
}

void CoreW65C02S_timed::op_wai()
{
	state = State::FETCH;
}
void CoreW65C02S_timed::op_stp()
{
	std::exit(0);
}

