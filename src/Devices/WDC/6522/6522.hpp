#pragma once

#include "include/Memory.hpp"
#include "include/MicroSim.hpp"

#include <cstdint>


namespace MicroSim::WDC
{
	class MemoryDevice6522 : public MicroSim::MemoryDevice
	{
		using Addr = MicroSim::Addr;
	protected:
		Byte m_ORA;
		Byte m_IRA;
		Byte m_DDRA;

		Byte m_ORB;
		Byte m_IRB;
		Byte m_DDRB;

		Byte m_IFR;
		Byte m_IER;

		Word m_T1L;
		Word m_T1C;
		Byte m_T2L;
		Word m_T2C;

		Byte m_ACR;
		Byte m_PCR;

		Byte m_SR;
	public:
		MemoryDevice6522(Addr, std::size_t, unsigned long int);
		Byte read_byte(Addr) override;
		void write_byte(Addr, Byte) override;
	};
} // namespace MicroSim::WDC
