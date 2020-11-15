#include "65C02.hpp"

using namespace MicroSim::WDC;

CoreW65C02S::Addr CoreW65C02S::addrm_abs()
{
	Addr addr = m_memory.read_word(PC);
	PC += 2;
	_cycles += 2;
	return addr;
}
CoreW65C02S::Addr CoreW65C02S::addrm_absx_ind()
{
	Addr addr = m_memory.read_word(PC);
	PC += 2;
	addr += X;
	addr = m_memory.read_word(addr);
	_cycles += 5;
	return addr;
}
CoreW65C02S::Addr CoreW65C02S::addrm_absx()
{
	Addr addr = m_memory.read_word(PC);
	PC += 2;
	addr += X;
	_cycles += 2;
	return addr;
}
CoreW65C02S::Addr CoreW65C02S::addrm_absy()
{
	Addr addr = m_memory.read_word(PC);
	PC += 2;
	addr += Y;
	return addr;
}
CoreW65C02S::Addr CoreW65C02S::addrm_abs_ind()
{
	Addr addr = m_memory.read_word(PC);
	PC += 2;
	addr = m_memory.read_word(addr);
	_cycles += 4;
	return addr;
}
CoreW65C02S::Addr CoreW65C02S::addrm_acc()
{
	return 0;
}
CoreW65C02S::Addr CoreW65C02S::addrm_immb()
{
	return PC++;
}
CoreW65C02S::Addr CoreW65C02S::addrm_immw()
{
	Addr addr = PC;
	PC += 2;
	return addr;
}
CoreW65C02S::Addr CoreW65C02S::addrm_imp()
{
	return 0;
}
CoreW65C02S::Addr CoreW65C02S::addrm_pcr()
{
	std::int8_t offset = m_memory.read_byte(PC++);
	_cycles++;
	return PC+offset;
}
CoreW65C02S::Addr CoreW65C02S::addrm_zp()
{
	Addr addr = m_memory.read_byte(PC++);
	_cycles++;
	return addr;
}
CoreW65C02S::Addr CoreW65C02S::addrm_zpx_ind()
{
	Addr addr = static_cast<std::uint8_t>(m_memory.read_byte(PC++)+X);
	_cycles += 4;
	return m_memory.read_word(addr);
}
CoreW65C02S::Addr CoreW65C02S::addrm_zpx()
{
	Addr addr = static_cast<std::uint8_t>(m_memory.read_byte(PC++)+X);
	_cycles += 2;
	return addr;
}
CoreW65C02S::Addr CoreW65C02S::addrm_zpy()
{
	Addr addr = static_cast<std::uint8_t>(m_memory.read_byte(PC++)+Y);
	_cycles += 2;
	return addr;
}
CoreW65C02S::Addr CoreW65C02S::addrm_zp_ind()
{
	Addr addr = m_memory.read_byte(PC++);
	_cycles += 3;
	return m_memory.read_word(addr);
}
CoreW65C02S::Addr CoreW65C02S::addrm_zp_indy()
{
	Addr addr = m_memory.read_byte(PC++);
	_cycles += 3;
	return m_memory.read_word(addr)+Y;
}

