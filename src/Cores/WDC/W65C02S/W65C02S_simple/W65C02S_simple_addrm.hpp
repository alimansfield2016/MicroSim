#include "W65C02S_simple.hpp"

using namespace MicroSim::WDC;

CoreW65C02S_simple::Addr CoreW65C02S_simple::addrm_abs()
{
	Addr addr = read_word(PC);
	PC += 2;
	_cycles += 2;
	return addr;
}
CoreW65C02S_simple::Addr CoreW65C02S_simple::addrm_absx_ind()
{
	Addr addr = read_word(PC);
	PC += 2;
	addr += X;
	addr = read_word(addr);
	_cycles += 5;
	return addr;
}
CoreW65C02S_simple::Addr CoreW65C02S_simple::addrm_absx()
{
	Addr addr = read_word(PC);
	PC += 2;
	addr += X;
	_cycles += 2;
	return addr;
}
CoreW65C02S_simple::Addr CoreW65C02S_simple::addrm_absy()
{
	Addr addr = read_word(PC);
	PC += 2;
	addr += Y;
	return addr;
}
CoreW65C02S_simple::Addr CoreW65C02S_simple::addrm_abs_ind()
{
	Addr addr = read_word(PC);
	PC += 2;
	addr = read_word(addr);
	_cycles += 4;
	return addr;
}
CoreW65C02S_simple::Addr CoreW65C02S_simple::addrm_acc()
{
	return 0;
}
CoreW65C02S_simple::Addr CoreW65C02S_simple::addrm_immb()
{
	return PC++;
}
CoreW65C02S_simple::Addr CoreW65C02S_simple::addrm_immw()
{
	Addr addr = PC;
	PC += 2;
	return addr;
}
CoreW65C02S_simple::Addr CoreW65C02S_simple::addrm_imp()
{
	return 0;
}
CoreW65C02S_simple::Addr CoreW65C02S_simple::addrm_pcr()
{
	std::int8_t offset = read_byte(PC++);
	_cycles++;
	return PC+offset;
}
CoreW65C02S_simple::Addr CoreW65C02S_simple::addrm_zp()
{
	Addr addr = read_byte(PC++);
	_cycles++;
	return addr;
}
CoreW65C02S_simple::Addr CoreW65C02S_simple::addrm_zpx_ind()
{
	Addr addr = static_cast<MicroSim::Byte>(read_byte(PC++)+X);
	_cycles += 4;
	return read_word(addr);
}
CoreW65C02S_simple::Addr CoreW65C02S_simple::addrm_zpx()
{
	Addr addr = static_cast<MicroSim::Byte>(read_byte(PC++)+X);
	_cycles += 2;
	return addr;
}
CoreW65C02S_simple::Addr CoreW65C02S_simple::addrm_zpy()
{
	Addr addr = static_cast<MicroSim::Byte>(read_byte(PC++)+Y);
	_cycles += 2;
	return addr;
}
CoreW65C02S_simple::Addr CoreW65C02S_simple::addrm_zp_ind()
{
	Addr addr = read_byte(PC++);
	_cycles += 3;
	return read_word(addr);
}
CoreW65C02S_simple::Addr CoreW65C02S_simple::addrm_zp_indy()
{
	Addr addr = read_byte(PC++);
	_cycles += 3;
	return read_word(addr)+Y;
}

