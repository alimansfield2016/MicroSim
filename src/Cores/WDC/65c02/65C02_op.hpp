#include "65C02.hpp"

using namespace MicroSim::WDC;

void CoreW65C02S::op_adc(CoreW65C02S::Addr op_addr)
{
	std::uint8_t v = m_memory.read_byte(op_addr);
	std::uint8_t t = A;
	std::uint16_t res = v + A + C;
	A = static_cast<std::uint8_t>(res);
	C = (res&0x100)>0;
	N = (A&0x80)>0;
	Z = (A == 0);
	V = ((A^t^v)&0x80)>0;
	_cycles++;
}
void CoreW65C02S::op_and(CoreW65C02S::Addr op_addr)
{
	std::uint8_t v = m_memory.read_byte(op_addr);
	A &= v;
	N = (A&0x80)>0;
	Z = (A == 0);
	_cycles++;
}
void CoreW65C02S::op_asl(CoreW65C02S::Addr op_addr)
{
	if(op_addr) //memory
	{
		std::uint8_t v = m_memory.read_byte(op_addr);
		C = v&0x80;
		v <<=1;
		Z = (v==0);
		N = (A&0x80)>0;
		m_memory.write_byte(op_addr, v);
		_cycles += 3;
	}
	else //Accumulator
	{
		C = A&0x80;
		A <<=1;
		Z = (A==0);
		N = (A&0x80)>0;
		_cycles++;
	}
}
void CoreW65C02S::op_bcc(CoreW65C02S::Addr op_addr)
{
	if(!C){
		PC = op_addr;
		_cycles++;
	}
}
void CoreW65C02S::op_bcs(CoreW65C02S::Addr op_addr)
{
	if(C){
		PC = op_addr;
		_cycles++;
	}
}
void CoreW65C02S::op_beq(CoreW65C02S::Addr op_addr)
{
	if(Z){
		PC = op_addr;
		_cycles++;
	}
}
void CoreW65C02S::op_bit(CoreW65C02S::Addr op_addr)
{
	std::uint8_t v = A^m_memory.read_byte(op_addr);
	_cycles++;
	P &= ~0xC0;
	P |= v&0xC0;
	Z = (v==0);
}
void CoreW65C02S::op_bmi(CoreW65C02S::Addr op_addr)
{
	if(N){
		PC = op_addr;
		_cycles++;
	}
}
void CoreW65C02S::op_bne(CoreW65C02S::Addr op_addr)
{
	if(!Z){
		PC = op_addr;
		_cycles++;
	}
}
void CoreW65C02S::op_bpl(CoreW65C02S::Addr op_addr)
{
	if(!N){
		PC = op_addr;
		_cycles++;
	}
}
void CoreW65C02S::op_bra(CoreW65C02S::Addr op_addr)
{
	PC = op_addr;
	_cycles ++;
}
void CoreW65C02S::op_brk(CoreW65C02S::Addr op_addr)
{
	std::uint8_t sig = m_memory.read_byte(op_addr);
	push_word(PC);
	push_byte(P);
	PC = m_memory.read_word(vec_brk);
	B = 1;
	D = 0;
	I = 1;
	_cycles += 6;
}
void CoreW65C02S::op_bvc(CoreW65C02S::Addr op_addr)
{
	if(!V){
		PC = op_addr;
		_cycles++;
	}
}
void CoreW65C02S::op_bvs(CoreW65C02S::Addr op_addr)
{
	if(V){
		PC = op_addr;
		_cycles++;
	}
}
void CoreW65C02S::op_clc(CoreW65C02S::Addr op_addr)
{
	C = 0;
	_cycles++;
}
void CoreW65C02S::op_cld(CoreW65C02S::Addr op_addr)
{
	D = 0;
	_cycles++;
}
void CoreW65C02S::op_cli(CoreW65C02S::Addr op_addr)
{
	I = 0;
	_cycles++;
}
void CoreW65C02S::op_clv(CoreW65C02S::Addr op_addr)
{
	V = 0;
	_cycles++;
}
void CoreW65C02S::op_cmp(CoreW65C02S::Addr op_addr)
{
	std::uint8_t m = m_memory.read_byte(op_addr);
	std::uint16_t res = A - m;
	N = res&0x80;
	Z = res==0;
	C = res&0x100;

	_cycles++;
}
void CoreW65C02S::op_cpx(CoreW65C02S::Addr op_addr)
{
	std::uint8_t m = m_memory.read_byte(op_addr);
	std::uint16_t res = X - m;
	N = res&0x80;
	Z = res==0;
	C = res&0x100;

	_cycles++;
}
void CoreW65C02S::op_cpy(CoreW65C02S::Addr op_addr)
{
	std::uint8_t m = m_memory.read_byte(op_addr);
	std::uint16_t res = Y - m;
	N = res&0x80;
	Z = res==0;
	C = res&0x100;

	_cycles++;
}
void CoreW65C02S::op_dec(CoreW65C02S::Addr op_addr)
{
	if(op_addr) //memory
	{
		std::uint8_t v = m_memory.read_byte(op_addr);
		v--;
		Z = (v==0);
		N = (A&0x80)>0;
		m_memory.write_byte(op_addr, v);
		_cycles += 3;
	}
	else //Accumulator
	{
		A--;
		Z = (A==0);
		N = (A&0x80)>0;
		_cycles++;
	}
}
void CoreW65C02S::op_dex(CoreW65C02S::Addr op_addr)
{
	X--;
	Z = (X==0);
	N = (X&0x80)>0;
	_cycles++;
}
void CoreW65C02S::op_dey(CoreW65C02S::Addr op_addr)
{
	Y--;
	Z = (Y==0);
	N = (Y&0x80)>0;
	_cycles++;
}
void CoreW65C02S::op_eor(CoreW65C02S::Addr op_addr)
{
	std::uint8_t v = m_memory.read_byte(op_addr);
	A ^= v;
	N = (A&0x80)>0;
	Z = (A == 0);
	_cycles++;
}
void CoreW65C02S::op_inc(CoreW65C02S::Addr op_addr)
{
	if(op_addr) //memory
	{
		std::uint8_t v = m_memory.read_byte(op_addr);
		v++;
		Z = (v==0);
		N = (A&0x80)>0;
		m_memory.write_byte(op_addr, v);
		_cycles += 3;
	}
	else //Accumulator
	{
		A++;
		Z = (A==0);
		N = (A&0x80)>0;
		_cycles++;
	}
}
void CoreW65C02S::op_inx(CoreW65C02S::Addr op_addr)
{
	X++;
	Z = (X==0);
	N = (X&0x80)>0;
	_cycles++;
}
void CoreW65C02S::op_iny(CoreW65C02S::Addr op_addr)
{
	Y++;
	Z = (Y==0);
	N = (Y&0x80)>0;
	_cycles++;
}
void CoreW65C02S::op_jmp(CoreW65C02S::Addr op_addr)
{
	PC = op_addr;
}
void CoreW65C02S::op_jsr(CoreW65C02S::Addr op_addr)
{
	push_word(PC);
	PC = op_addr;
	_cycles += 2;
}
void CoreW65C02S::op_lda(CoreW65C02S::Addr op_addr)
{
	A = m_memory.read_byte(op_addr);
	_cycles++;
}
void CoreW65C02S::op_ldx(CoreW65C02S::Addr op_addr)
{
	X = m_memory.read_byte(op_addr);
	_cycles++;
}
void CoreW65C02S::op_ldy(CoreW65C02S::Addr op_addr)
{
	Y = m_memory.read_byte(op_addr);
	_cycles++;
}
void CoreW65C02S::op_lsr(CoreW65C02S::Addr op_addr)
{
	if(op_addr) //memory
	{
		std::uint8_t v = m_memory.read_byte(op_addr);
		C = v&0x01;
		v >>=1;
		Z = (v==0);
		N = (A&0x80)>0;
		m_memory.write_byte(op_addr, v);
		_cycles += 3;
	}
	else //Accumulator
	{
		C = A&0x01;
		A >>=1;
		Z = (A==0);
		N = (A&0x80)>0;
		_cycles++;
	}
}
void CoreW65C02S::op_nop(CoreW65C02S::Addr op_addr)
{
	_cycles++;
}
void CoreW65C02S::op_ora(CoreW65C02S::Addr op_addr)
{
	std::uint8_t v = m_memory.read_byte(op_addr);
	A |= v;
	N = (A&0x80)>0;
	Z = (A == 0);
	_cycles++;
}
void CoreW65C02S::op_pha(CoreW65C02S::Addr op_addr)
{
	push_byte(A);
	_cycles+=2;
}
void CoreW65C02S::op_php(CoreW65C02S::Addr op_addr)
{
	push_byte(P);
	_cycles+=2;
}
void CoreW65C02S::op_phx(CoreW65C02S::Addr op_addr)
{
	push_byte(X);
	_cycles+=2;
}
void CoreW65C02S::op_phy(CoreW65C02S::Addr op_addr)
{
	push_byte(Y);
	_cycles+=2;
}
void CoreW65C02S::op_pla(CoreW65C02S::Addr op_addr)
{
	A = pop_byte();
	_cycles+=3;
}
void CoreW65C02S::op_plp(CoreW65C02S::Addr op_addr)
{
	P = pop_byte();
	_cycles+=3;
}
void CoreW65C02S::op_plx(CoreW65C02S::Addr op_addr)
{
	X = pop_byte();
	_cycles+=3;
}
void CoreW65C02S::op_ply(CoreW65C02S::Addr op_addr)
{
	Y = pop_byte();
	_cycles+=3;
}
void CoreW65C02S::op_rol(CoreW65C02S::Addr op_addr)
{
	if(op_addr) //memory
	{
		std::uint8_t v = m_memory.read_byte(op_addr);
		
		bool c = v&0x80;
		v <<=1;
		if(C)
			v |= 0x01;
		C = c;
		Z = (v==0);
		N = (A&0x80)>0;
		m_memory.write_byte(op_addr, v);
		_cycles += 3;
	}
	else //Accumulator
	{
		bool c = A&0x80;
		A <<=1;
		if(C)
			A |= 0x01;
		C = c;
		Z = (A==0);
		N = (A&0x80)>0;
		_cycles++;
	}
}
void CoreW65C02S::op_ror(CoreW65C02S::Addr op_addr)
{
	if(op_addr) //memory
	{
		std::uint8_t v = m_memory.read_byte(op_addr);
		
		bool c = v&0x01;
		v >>=1;
		if(C)
			v |= 0x80;
		C = c;
		Z = (v==0);
		N = (A&0x80)>0;
		m_memory.write_byte(op_addr, v);
		_cycles += 3;
	}
	else //Accumulator
	{
		bool c = A&0x01;
		A >>=1;
		if(C)
			A |= 0x80;
		C = c;
		Z = (A==0);
		N = (A&0x80)>0;
		_cycles++;
	}
}
void CoreW65C02S::op_rti(CoreW65C02S::Addr op_addr)
{
	P = pop_byte();
	PC = pop_word();
	_cycles += 5;
}
void CoreW65C02S::op_rts(CoreW65C02S::Addr op_addr)
{
	PC = pop_word();
	_cycles += 5;
}
void CoreW65C02S::op_sbc(CoreW65C02S::Addr op_addr)
{
	std::uint8_t v = ~m_memory.read_byte(op_addr);
	std::uint8_t t = A;
	std::uint16_t res = A + v + C;
	A = static_cast<std::uint8_t>(res);
	C = (res&0x100)>0;
	N = (A&0x80)>0;
	Z = (A == 0);
	V = ((A^t^v)&0x80)>0;
	_cycles++;
}
void CoreW65C02S::op_sec(CoreW65C02S::Addr op_addr)
{
	C = 1;
	_cycles++;
}
void CoreW65C02S::op_sed(CoreW65C02S::Addr op_addr)
{
	D = 1;
	_cycles++;
}
void CoreW65C02S::op_sei(CoreW65C02S::Addr op_addr)
{
	I = 1;
	_cycles++;
}
void CoreW65C02S::op_sta(CoreW65C02S::Addr op_addr)
{
	m_memory.write_byte(op_addr, A);
	_cycles++;
}
void CoreW65C02S::op_stp(CoreW65C02S::Addr op_addr)
{
	//unimplemented
	_cycles++;
}
void CoreW65C02S::op_stx(CoreW65C02S::Addr op_addr)
{
	m_memory.write_byte(op_addr, X);
	_cycles++;
}
void CoreW65C02S::op_sty(CoreW65C02S::Addr op_addr)
{
	m_memory.write_byte(op_addr, Y);
	_cycles++;
}
void CoreW65C02S::op_stz(CoreW65C02S::Addr op_addr)
{
	m_memory.write_byte(op_addr, 0);
	_cycles++;
}
void CoreW65C02S::op_tax(CoreW65C02S::Addr op_addr)
{
	X = A;
	_cycles++;
}
void CoreW65C02S::op_tay(CoreW65C02S::Addr op_addr)
{
	Y = A;
	_cycles++;
}
void CoreW65C02S::op_tsx(CoreW65C02S::Addr op_addr)
{
	X = SP;
	_cycles++;
}
void CoreW65C02S::op_txa(CoreW65C02S::Addr op_addr)
{
	A = X;
	_cycles++;
}
void CoreW65C02S::op_txs(CoreW65C02S::Addr op_addr)
{
	SP = X;
	_cycles++;
}
void CoreW65C02S::op_tya(CoreW65C02S::Addr op_addr)
{
	A = Y;
	_cycles++;
}
void CoreW65C02S::op_wai(CoreW65C02S::Addr op_addr)
{
	//unimplemented
	_cycles++;
}
