#pragma once

#include "Device.hpp"
#include "MicroSim.hpp"

#include <cstdint>
#include <vector>
#include <memory>

namespace MicroSim
{
	using Addr = std::uintptr_t;

	class MemoryDevice : public MicroSim::Device
	{
	protected:
		const Addr m_low, m_high;
		const std::uint8_t m_priority;
		std::uint8_t *m_data;
	public:
		constexpr MemoryDevice(Addr _low, Addr _high, std::uint8_t *_data) :
			m_low{_low},
			m_high{_high},
			m_priority{0},
			m_data{_data} {}
		virtual ~MemoryDevice(){}
		constexpr Addr low() const { return m_low; }
		constexpr Addr high() const { return m_high; }
		constexpr Addr priority() const { return m_priority; }
		virtual Byte read_byte(Addr);
		virtual Word read_word(Addr);
		virtual DWord read_dword(Addr);
		virtual QWord read_qword(Addr);
		virtual void write_byte(Addr, Byte);
		virtual void write_word(Addr, Word);
		virtual void write_dword(Addr, DWord);
		virtual void write_qword(Addr, QWord);

		void clock() override{};
		void reset() override{};

	};
	class DefaultMemDev : public MemoryDevice
	{
	public:
		DefaultMemDev(	Addr _low = static_cast<Addr>(0),
						Addr _high = static_cast<Addr>(-1)) : 
						MemoryDevice{	
							_low, 
							_high, 
							nullptr
						}{}
		virtual Byte read_byte(Addr) override;
		virtual Word read_word(Addr) override;
		virtual DWord read_dword(Addr) override;
		virtual QWord read_qword(Addr) override;
		virtual void write_byte(Addr, Byte) override;
		virtual void write_word(Addr, Word) override;
		virtual void write_dword(Addr, DWord) override;
		virtual void write_qword(Addr, QWord) override;

		static DefaultMemDev s_defaultMemDev;
	};


	class Memory
	{
		std::vector<
			std::shared_ptr<MemoryDevice>
			// MemoryDevice*
		> m_devices;

		MemoryDevice &device_at(Addr);
		//some kind of tree structure 
		//to hold all of the 
		//memory devices
	
	public:
		[[nodiscard]] Byte read_byte_const(Addr) const;
		[[nodiscard]] Word read_word_const(Addr) const;
		[[nodiscard]] DWord read_dword_const(Addr) const;
		[[nodiscard]] QWord read_qword_const(Addr) const;
		Memory(){}
		// void add_device(MemoryDevice*);
		void add_device(std::shared_ptr<MemoryDevice>&&);
		// void remove_device(MemoryDevice*);
		void remove_device(std::shared_ptr<MemoryDevice>&&);

		Byte read_byte(Addr);
		Word read_word(Addr);
		DWord read_dword(Addr);
		QWord read_qword(Addr);

		void write_byte(Addr, Byte);
		void write_word(Addr, Word);
		void write_dword(Addr, DWord);
		void write_qword(Addr, QWord);

		void clock();
	};
} // namespace MicroSim

#include "ram.hpp"
#include "rom.hpp"
