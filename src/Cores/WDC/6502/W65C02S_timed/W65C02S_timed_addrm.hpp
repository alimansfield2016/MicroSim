#pragma once
#include "W65C02S_timed.hpp"

using namespace MicroSim::WDC;

void CoreW65C02S_timed::addrm_abs()
{
	_addr.low() = read_byte(PC++);
	addrm = &CoreW65C02S_timed::addrm_abs2;
}
void CoreW65C02S_timed::addrm_abs2()
{
	_addr.high() = read_byte(PC++);
	state = State::EXEC;
}

void CoreW65C02S_timed::addrm_absx()
{
	_addr.low() = read_byte(PC++);
	addrm = &CoreW65C02S_timed::addrm_absx2;
}
void CoreW65C02S_timed::addrm_absx2()
{
	_addr.high() = read_byte(PC++);
	_addr += X;
	state = State::EXEC;
}

void CoreW65C02S_timed::addrm_absx_rmw()
{
	_addr.low() = read_byte(PC++);
	addrm = &CoreW65C02S_timed::addrm_absx2_rmw;
}
void CoreW65C02S_timed::addrm_absx2_rmw()
{
	_addr.high() = read_byte(PC++);
	addrm = &CoreW65C02S_timed::addrm_abs_x3_rmw;
}
void CoreW65C02S_timed::addrm_abs_x3_rmw()
{
	read_byte(_addr.high()<<8 | (_addr.low()+X));
	_addr += X;
	state = State::EXEC;
}

void CoreW65C02S_timed::addrm_absy()
{
	_addr.low() = read_byte(PC++);
	addrm = &CoreW65C02S_timed::addrm_absy2;
}
void CoreW65C02S_timed::addrm_absy2()
{
	_addr.high() = read_byte(PC++);
	_addr += Y;
	state = State::EXEC;
}

void CoreW65C02S_timed::addrm_acc()
{
	state = State::EXEC;
	(this->*op)();
}

void CoreW65C02S_timed::addrm_immb()
{
	_addr = PC;
	PC++;
	state = State::EXEC;
	(this->*op)();
}
void CoreW65C02S_timed::addrm_immw()
{
	_addr = PC;
	PC += 2;
	state = State::EXEC;
	(this->*op)();
}

void CoreW65C02S_timed::addrm_imp()
{
	read_byte(PC);
	state = State::EXEC;
	(this->*op)();
}

void CoreW65C02S_timed::addrm_zp()
{
	_addr = read_byte(PC++);
	state = State::EXEC;
}

void CoreW65C02S_timed::addrm_zpi()
{
	_addr = read_byte(PC++);
	addrm = &CoreW65C02S_timed::addrm_zpi2;
}
void CoreW65C02S_timed::addrm_zpxi()
{
	_addr = read_byte(PC++);
	_addr.low() += X;
	addrm = &CoreW65C02S_timed::addrm_zpi2;
}
void CoreW65C02S_timed::addrm_zpi2()
{
	_addr.high() = read_byte(_addr.low());
	addrm = &CoreW65C02S_timed::addrm_zpi3;
}
void CoreW65C02S_timed::addrm_zpi3()
{
	_addr.low() = read_byte(_addr.low()+1);
	std::swap(_addr.low(), _addr.high());
	state = State::EXEC;
}

void CoreW65C02S_timed::addrm_zpiy()
{
	_addr = read_byte(PC++);
	addrm = &CoreW65C02S_timed::addrm_zpiy2;
}
void CoreW65C02S_timed::addrm_zpiy2()
{
	_addr.high() = read_byte(_addr.low());
	addrm = &CoreW65C02S_timed::addrm_zpiy3;
}
void CoreW65C02S_timed::addrm_zpiy3()
{
	_addr.low() = read_byte(_addr.low()+1);
	std::swap(_addr.low(), _addr.high());
	_addr.low() += Y;
	if(_addr.low() >= Y)
		state = State::EXEC;
	addrm = &CoreW65C02S_timed::addrm_zpiy4;
}
void CoreW65C02S_timed::addrm_zpiy4()
{
	read_byte(_addr);
	_addr.high()++;
	state = State::EXEC;
}

void CoreW65C02S_timed::addrm_zpx()
{
	_addr = read_byte(PC++);
	_addr.low() += X;
	state = State::EXEC;
}
void CoreW65C02S_timed::addrm_zpy()
{
	_addr = read_byte(PC++);
	_addr.low() += Y;
	state = State::EXEC;
}

void CoreW65C02S_timed::addrm_zpx_rmw()
{
	_addr = read_byte(PC);
	addrm = &CoreW65C02S_timed::addrm_zpx1_rmw;
}
void CoreW65C02S_timed::addrm_zpx1_rmw()
{
	read_byte(PC++);
	state = State::EXEC;
}

void CoreW65C02S_timed::addrm_pcr()
{
	_addr = read_byte(PC++);
	state = State::EXEC;
	(this->*op)();
}

void CoreW65C02S_timed::addrm_unused2(){
	read_byte(PC);
	addrm = &CoreW65C02S_timed::addrm_unused;
}
void CoreW65C02S_timed::addrm_unused(){
	read_byte(PC);
	state = State::EXEC;
}




