#include "6522.hpp"

using namespace MicroSim::WDC;

MemoryDevice6522::MemoryDevice6522(Addr _addr, std::size_t size, unsigned long int _freq = 0) : 
	MicroSim::MemoryDevice{size, _addr, _addr+size, 0x0F, nullptr, 1, _freq}
	{}

MicroSim::Byte MemoryDevice6522::read_byte(Addr _addr)
{
	switch(_addr & 0x0F){
		case 0x00: return m_IRB;
		case 0x01: return m_IRA;
		case 0x02: return m_DDRB;
		case 0x03: return m_DDRA;
		case 0x04: return MicroSim::low(m_T1C);
		case 0x05: return MicroSim::high(m_T1C);
		case 0x06: return MicroSim::low(m_T1L);
		case 0x07: return MicroSim::high(m_T1L);
		case 0x08: return MicroSim::low(m_T2C);
		case 0x09: return MicroSim::high(m_T2C);
		case 0x0A: return m_SR;
		case 0x0B: return m_ACR;
		case 0x0C: return m_PCR;
		case 0x0D: return m_IFR;
		case 0x0E: return m_IER;
		case 0x0F: return m_IRA;
	}
	return 0;
}

void MemoryDevice6522::write_byte(Addr _addr, MicroSim::Byte b)
{
	switch(_addr & 0x0F){
		case 0x00: m_ORB = b;
		case 0x01: m_ORA = b;
		case 0x02: m_DDRB = b;
		case 0x03: m_DDRA = b;
		case 0x04: MicroSim::low(m_T1L) = b;
		case 0x05: MicroSim::high(m_T1C) = b;
		case 0x06: MicroSim::low(m_T1L) = b;
		case 0x07: MicroSim::high(m_T1L) = b;
		case 0x08: m_T2L = b;
		case 0x09: MicroSim::high(m_T2C) = b;
		case 0x0A: m_SR = b;
		case 0x0B: m_ACR = b;
		case 0x0C: m_PCR = b;
		case 0x0D: m_IFR = b;
		case 0x0E: m_IER = b;
		case 0x0F: m_ORA = b;
	}
}