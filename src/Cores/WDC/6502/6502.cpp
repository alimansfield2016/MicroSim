#include "6502.hpp"

using namespace MicroSim::WDC;


Core6502::Core6502() : 
	Core{{
		{"A", 1, &A},
		{"X", 1, &X},
		{"Y", 1, &Y},
		{"P", 1, &P},
		{"SP", 1, &SP},
		{"PC", 2, &PC}
	},
	1000000}
{

}
std::uint8_t Core6502::read_byte(Addr addr)
{
	return m_memory.read_byte(addr);
}
void Core6502::write_byte(Addr addr, std::uint8_t b)
{
	return m_memory.write_byte(addr, b);
}


void Core6502::push_byte(std::uint8_t b)
{
	m_memory.write_byte(0x100|SP--, b);
}

std::uint8_t Core6502::pop_byte()
{
	return m_memory.read_byte(0x100|++SP);
}