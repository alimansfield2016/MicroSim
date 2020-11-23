#pragma once

#include "include/Memory.hpp"

namespace MicroSim::Hitachi
{
	class MemoryDeviceHD44780U : public MicroSim::MemoryDevice
	{
		static MicroSim::Byte s_CGROM[240*16];
		using Addr = MicroSim::Addr;
		MicroSim::Byte m_DDRAM[80];
		MicroSim::Byte m_CGRAM[64];
		MicroSim::Byte m_IR = 0;
		MicroSim::Byte m_DR = 0;
		MicroSim::Byte m_addr = 0;
		MicroSim::Byte m_shift = 0;
		bool m_busy;
		enum class DL : bool{
			_8Bit = 1,
			_4Bit = 0,
		}m_dataLen;
		enum class DW : bool{
			High = 0,
			Low = 1,
		}m_half;

		enum class ID : bool{ //Increment/Decrement
			Decrement = 0,
			Increment = 1,
		}m_id;
		enum class S : bool{
			NoShift = 0,
			Shift = 1,
		}m_s;
		enum class SC : bool{ //Shift/Cursor
			CursorMove = 0,
			DisplayShift = 1,
		}m_sc;
		enum class RL : bool{ //Right/Left
			Left = 0,
			Right = 1,
		}m_rl;
		enum class F : bool{
			Font5x8 = 0,
			Font5x10 = 1,
		}m_font;
		enum class NL : bool{
			_1Line = 0,
			_2Line = 1,
		}m_numLines;

		enum class Instr : Byte{
			ClearDisplay = 0xFE,
			ReturnHome = 0xFC,
			EntryModeSet = 0xF8,
			DisplayControl = 0xF0,
			CursorDisplayShift = 0xE0,
			FunctionSet = 0xC0,
			SetCGRAMAddr = 0x80,
		};

		enum class Bits : Byte{
			S = 0x01,
			ID = 0x02,
			B = 0x01,
			C = 0x02,
			D = 0x04,
			RL = 0x04,
			SC = 0x08,
			F = 0x04,
			N = 0x08,
			DL = 0x10,
		};

		void clear();
		void home();

		Byte getIdx() const;

		void increment();
		void decrement();

	public:
		MemoryDeviceHD44780U(Addr, std::size_t);

		MicroSim::Byte read_byte(Addr) override;
		void write_byte(Addr, MicroSim::Byte) override;

		void reset() override;
		void clock() override;


		enum class BF : bool{
			Accepting = 0,
			Busy = 1,
		};

		//character values
		Byte charCode(Byte idx);
		Byte genChar(Byte charcode, Byte offset);
	};
} // namespace MicroSim::Hitachi
