#pragma once

#include "include/Core.hpp"

namespace MicroSim::AVR
{
	class CoreAVR5 : public MicroSim::Core
	{
		union{
			std::uint8_t m_registerFile[32];
			std::uint16_t m_registerPairs[16];
		};
		#pragma region Registers
		std::uint8_t &r0 = m_registerFile[0];
		std::uint8_t &r1 = m_registerFile[1];
		std::uint8_t &r2 = m_registerFile[2];
		std::uint8_t &r3 = m_registerFile[3];
		std::uint8_t &r4 = m_registerFile[4];
		std::uint8_t &r5 = m_registerFile[5];
		std::uint8_t &r6 = m_registerFile[6];
		std::uint8_t &r7 = m_registerFile[7];
		std::uint8_t &r8 = m_registerFile[8];
		std::uint8_t &r9 = m_registerFile[9];
		std::uint8_t &r10 = m_registerFile[10];
		std::uint8_t &r11 = m_registerFile[11];
		std::uint8_t &r12 = m_registerFile[12];
		std::uint8_t &r13 = m_registerFile[13];
		std::uint8_t &r14 = m_registerFile[14];
		std::uint8_t &r15 = m_registerFile[15];
		std::uint8_t &r16 = m_registerFile[16];
		std::uint8_t &r17 = m_registerFile[17];
		std::uint8_t &r18 = m_registerFile[18];
		std::uint8_t &r19 = m_registerFile[19];
		std::uint8_t &r20 = m_registerFile[20];
		std::uint8_t &r21 = m_registerFile[21];
		std::uint8_t &r22 = m_registerFile[22];
		std::uint8_t &r23 = m_registerFile[23];
		std::uint8_t &r24 = m_registerFile[24];
		std::uint8_t &r25 = m_registerFile[25];
		std::uint8_t &r26 = m_registerFile[26];
		std::uint8_t &r27 = m_registerFile[27];
		std::uint8_t &r28 = m_registerFile[28];
		std::uint8_t &r29 = m_registerFile[29];
		std::uint8_t &r30 = m_registerFile[30];
		std::uint8_t &r31 = m_registerFile[31];
		std::uint16_t &r1r0 = m_registerPairs[0];
		std::uint16_t &r3r2 = m_registerPairs[1];
		std::uint16_t &r5r4 = m_registerPairs[2];
		std::uint16_t &r7r6 = m_registerPairs[3];
		std::uint16_t &r9r8 = m_registerPairs[4];
		std::uint16_t &r11r10 = m_registerPairs[5];
		std::uint16_t &r13r12 = m_registerPairs[6];
		std::uint16_t &r15r14 = m_registerPairs[7];
		std::uint16_t &r17r16 = m_registerPairs[8];
		std::uint16_t &r19r18 = m_registerPairs[9];
		std::uint16_t &r21r20 = m_registerPairs[10];
		std::uint16_t &r23r22 = m_registerPairs[11];
		std::uint16_t &r25r24 = m_registerPairs[12];
		std::uint16_t &r27r26 = m_registerPairs[13];
		std::uint16_t &r29r28 = m_registerPairs[14];
		std::uint16_t &r31r30 = m_registerPairs[15];
		std::uint16_t &x = r27r26;
		std::uint16_t &y = r29r28;
		std::uint16_t &z = r31r30;
		#pragma endregion

	
	};
} // namespace MicroSim::AVR
