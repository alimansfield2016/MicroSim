#include "2A03.hpp"

using namespace MicroSim::Ricoh;


//DONE
void Core2A03::op_adc()
{
	TMP = read_byte(_addr);
	MicroSim::Byte t = A;
	MicroSim::Word res = TMP + A + C;
	A = static_cast<MicroSim::Byte>(res);
	C = (res&0x100)>0;
	N = (A&0x80)>0;
	Z = (A == 0);
	V = ((A^t^TMP)&0x80)>0;
	state = State::FETCH;
}

//DONE
void Core2A03::op_and()
{
	TMP = read_byte(_addr);
	A &= TMP;
	N = (A&0x80)>0;
	Z = (A == 0);
	state = State::FETCH;
}


//DONE
void Core2A03::op_asla()
{
	MicroSim::Byte &R = A;
	C = R&0x80;
	R <<=1;
	Z = (R==0);
	N = (R&0x80)>0;
}
//DONE
void Core2A03::op_asl()
{
	TMP = read_byte(_addr);
	op = &Core2A03::op_asl_m;
}
//DONE
void Core2A03::op_asl_m()
{
	write_byte(_addr, TMP);
	C = TMP&0x80;
	TMP <<=1;
	Z = (TMP==0);
	N = (TMP&0x80)>0;
	op = &Core2A03::op_rmw_w;
}

void Core2A03::op_bit()
{
	TMP = read_byte(_addr);
	P &= ~0xC0;
	P |= TMP&0xC0;
	Z = !(TMP&A);
	state = State::FETCH;
}
template<std::uint8_t F>
void Core2A03::op_br_fs()
{
	if(!(P&F))
		state = State::FETCH;
	op = &Core2A03::op_br;
}
template<std::uint8_t F>
void Core2A03::op_br_fc()
{
	if((P&F))
		state = State::FETCH;
	op = &Core2A03::op_br;
}
//DONE
void Core2A03::op_br()
{
	read_byte(PC);
	std::int8_t offset = static_cast<std::int8_t>(_addr);
	Word PC_new = PC + offset;
	low(PC) += offset;
	TMP = high(PC_new) - high(PC);
	if(!TMP)
		state = State::FETCH;
	op = &Core2A03::op_br2;
}
//DONE
void Core2A03::op_br2()
{
	read_byte(PC);
	high(PC)+=TMP;
}

void Core2A03::op_brk()
{
	read_byte(_addr);
	op = &Core2A03::op_brk2;
}
void Core2A03::op_brk2()
{
	push_byte(high(PC));
	op = &Core2A03::op_brk3;
}
void Core2A03::op_brk3()
{
	push_byte(low(PC));
	op = &Core2A03::op_brk4;
}
void Core2A03::op_brk4()
{
	push_byte(P);
	_addr = vec_brk;
	op = &Core2A03::op_vecl;
}

template<std::uint8_t F> 
void Core2A03::op_clf()
{
	P &= ~F;
	state = State::FETCH;
}
template<std::uint8_t F> 
void Core2A03::op_sef()
{
	P |= F;
	state = State::FETCH;
}

void Core2A03::op_cmp()
{
	TMP = read_byte(_addr);
	MicroSim::Word res = A + ~TMP + 1;
	N = res&0x80;
	Z = res==0;
	C = res&0x100;
	state = State::FETCH;
}
void Core2A03::op_cpx()
{
	TMP = read_byte(_addr);
	MicroSim::Word res = X + ~TMP + 1;
	N = res&0x80;
	Z = res==0;
	C = res&0x100;
	state = State::FETCH;	
}
void Core2A03::op_cpy()
{
	TMP = read_byte(_addr);
	MicroSim::Word res = Y + ~TMP + 1;
	N = res&0x80;
	Z = res==0;
	C = res&0x100;
	state = State::FETCH;	
}


void Core2A03::op_dec()
{
	TMP = read_byte(_addr);
	op = &Core2A03::op_dec2;
}
void Core2A03::op_dec2()
{
	read_byte(_addr);
	TMP--;
	Z = (TMP==0);
	N = (TMP&0x80)>0;
	op = &Core2A03::op_rmw_w;
}
void Core2A03::op_dex()
{
	X--;
	Z = (X==0);
	N = (X&0x80)>0;
	state = State::FETCH;
}
void Core2A03::op_dey()
{
	Y--;
	Z = (Y==0);
	N = (Y&0x80)>0;
	state = State::FETCH;
}

void Core2A03::op_eor()
{
	TMP = read_byte(_addr);
	A ^= TMP;
	N = (A&0x80)>0;
	Z = (A == 0);
	state = State::FETCH;
}


void Core2A03::op_inc()
{
	TMP = read_byte(_addr);
	op = &Core2A03::op_inc2;
}
void Core2A03::op_inc2()
{
	read_byte(_addr);
	TMP++;
	Z = (TMP==0);
	N = (TMP&0x80)>0;
	op = &Core2A03::op_rmw_w;
}
void Core2A03::op_inx()
{
	X++;
	Z = (X==0);
	N = (X&0x80)>0;
	state = State::FETCH;
}
void Core2A03::op_iny()
{
	Y++;
	Z = (Y==0);
	N = (Y&0x80)>0;
	state = State::FETCH;
}


void Core2A03::op_jmp()
{
	low(PC) = read_byte(_addr);
	op = &Core2A03::op_jmp2;
}
void Core2A03::op_jmp2()
{
	high(PC) = read_byte(_addr+1);
	state = State::FETCH;
}


void Core2A03::op_jsr()
{
	TMP = read_byte(_addr++);
	op = &Core2A03::op_jsr2;
}
void Core2A03::op_jsr2()
{
	read_byte(0x100|SP);
	op = &Core2A03::op_jsr3;
}
void Core2A03::op_jsr3()
{
	push_byte(high(PC));
	op = &Core2A03::op_jsr4;
}
void Core2A03::op_jsr4()
{
	push_byte(low(PC));
	op = &Core2A03::op_jsr5;
}
void Core2A03::op_jsr5()
{
	high(PC) = read_byte(_addr++);
	low(PC) = TMP;
	state = State::FETCH;
}


void Core2A03::op_lda()
{
	MicroSim::Byte &R = A;
	R = read_byte(_addr);
	Z = !R;
	N = R&0x80;
	state = State::FETCH;
}
void Core2A03::op_ldx()
{
	MicroSim::Byte &R = X;
	R = read_byte(_addr);
	Z = !R;
	N = R&0x80;
	state = State::FETCH;
}
void Core2A03::op_ldy()
{
	MicroSim::Byte &R = Y;
	R = read_byte(_addr);
	Z = !R;
	N = R&0x80;
	state = State::FETCH;
}

void Core2A03::op_lsra()
{
	MicroSim::Byte &R = A;
	C = R&0x01;
	R >>=1;
	Z = (R==0);
	N = (R&0x80)>0;
	state = State::FETCH;
}
void Core2A03::op_lsr()
{
	TMP = read_byte(_addr);
	op = &Core2A03::op_lsr2;
}
void Core2A03::op_lsr2()
{
	read_byte(_addr);
	MicroSim::Byte &R = TMP;
	C = R&0x01;
	R >>=1;
	Z = (R==0);
	N = (R&0x80)>0;
	op = &Core2A03::op_rmw_w;
}

void Core2A03::op_nop()
{
	op = &Core2A03::op_nop2;
}
void Core2A03::op_nop2()
{
	read_byte(PC);
	state = State::FETCH;
}

void Core2A03::op_ora()
{
	TMP = read_byte(_addr);
	A |= TMP;
	N = (A&0x80)>0;
	Z = (A == 0);
	state = State::FETCH;
}

void Core2A03::op_pha(){
	push_byte(A);
	state = State::FETCH;
}
void Core2A03::op_php(){
	push_byte(P);
	state = State::FETCH;
}
void Core2A03::op_pla(){
	read_byte(0x100|SP);
	op = &Core2A03::op_pla2;
}
void Core2A03::op_plp(){
	read_byte(0x100|SP);
	op = &Core2A03::op_plp2;
}
void Core2A03::op_pla2(){
	A = pop_byte();
	state = State::FETCH;
}
void Core2A03::op_plp2(){
	P = pop_byte();
	state = State::FETCH;
}

void Core2A03::op_rmw_w()
{
	write_byte(_addr, TMP);
	state = State::FETCH;
}


void Core2A03::op_rola()
{
	MicroSim::Byte &R = A;
	bool c = R&0x80;
	R <<=1;
	if(C)
		R |= 0x01;
	C = c;
	Z = (R==0);
	N = (R&0x80);
	state = State::FETCH;
}
void Core2A03::op_rol()
{
	TMP = read_byte(_addr);
	op = &Core2A03::op_rol_m;
}
void Core2A03::op_rol_m()
{
	MicroSim::Byte &R = TMP;
	bool c = R&0x80;
	R <<=1;
	if(C)
		R |= 0x01;
	C = c;
	Z = (R==0);
	N = (R&0x80);

	op = &Core2A03::op_rmw_w;
}

void Core2A03::op_rora()
{
	MicroSim::Byte &R = A;
	bool c = R&0x01;
	R >>=1;
	if(C)
		R |= 0x80;
	C = c;
	Z = (R==0);
	N = (R&0x80);
	state = State::FETCH;
}
void Core2A03::op_ror()
{
	TMP = read_byte(_addr);
	op = &Core2A03::op_ror_m;
}
void Core2A03::op_ror_m()
{
	MicroSim::Byte &R = TMP;
	bool c = R&0x01;
	R >>=1;
	if(C)
		R |= 0x80;
	C = c;
	Z = (R==0);
	N = (R&0x80);

	op = &Core2A03::op_rmw_w;
}


void Core2A03::op_rti()
{
	read_byte(0x100|SP);
	op = &Core2A03::op_rti2;
}
void Core2A03::op_rti2()
{
	P = pop_byte();
	op = &Core2A03::op_rti3;
}
void Core2A03::op_rti3()
{
	low(_addr) = pop_byte();
	op = &Core2A03::op_rti4;
}
void Core2A03::op_rti4()
{
	high(_addr) = pop_byte();
	PC = _addr;
	state = State::FETCH;
}
void Core2A03::op_rts()
{
	read_byte(0x100|SP);
	op = &Core2A03::op_rts2;
}
void Core2A03::op_rts2()
{
	low(_addr) = pop_byte();
	op = &Core2A03::op_rts3;
}
void Core2A03::op_rts3()
{
	high(_addr) = pop_byte();
	op = &Core2A03::op_rts4;
}
void Core2A03::op_rts4()
{
	//Not quite sure about this one?
	read_byte(PC);
	PC = _addr;
	state = State::FETCH;
}

void Core2A03::op_sbc()
{
	TMP = read_byte(_addr);
	MicroSim::Byte t = A;
	MicroSim::Word res = A + TMP + C;
	A = static_cast<MicroSim::Byte>(res);
	C = (res&0x100)>0;
	N = (A&0x80)>0;
	Z = (A == 0);
	V = ((A^t^TMP)&0x80)>0;
	state = State::FETCH;
}

void Core2A03::op_sta()
{
	MicroSim::Byte &R = A;
	write_byte(_addr, R);
	state = State::FETCH;
}
void Core2A03::op_stx()
{
	MicroSim::Byte &R = X;
	write_byte(_addr, R);
	state = State::FETCH;
}
void Core2A03::op_sty()
{
	MicroSim::Byte &R = Y;
	write_byte(_addr, R);
	state = State::FETCH;
}


void Core2A03::op_tax()
{
	X = A;
	N = X&0x80;
	Z = !X;
	state = State::FETCH;
}
void Core2A03::op_tay()
{
	Y = A;
	N = Y&0x80;
	Z = !Y;
	state = State::FETCH;
}
void Core2A03::op_tsx()
{
	X = SP;
	N = X&0x80;
	Z = !X;
	state = State::FETCH;
}
void Core2A03::op_txa()
{
	A = X;
	N = A&0x80;
	Z = !A;
	state = State::FETCH;
}
void Core2A03::op_txs()
{
	SP = X;
	state = State::FETCH;
}
void Core2A03::op_tya()
{
	A = Y;
	N = A&0x80;
	Z = !A;
	state = State::FETCH;
}




void Core2A03::op_vecl()
{
	low(PC) = read_byte(_addr);
	op = &Core2A03::op_vech;
}
void Core2A03::op_vech()
{
	high(PC) = read_byte(_addr+1);
	state = State::FETCH;
}
