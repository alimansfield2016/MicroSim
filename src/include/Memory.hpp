#pragma once

#include <cstdint>
#include <vector>
#include <memory>

namespace MicroSim
{
	using Addr = std::uintptr_t;

	class MemoryDevice
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
		virtual std::uint8_t read_byte(Addr);
		virtual std::uint16_t read_word(Addr);
		virtual std::uint32_t read_dword(Addr);
		virtual std::uint64_t read_qword(Addr);
		virtual void write_byte(Addr, std::uint8_t);
		virtual void write_word(Addr, std::uint16_t);
		virtual void write_dword(Addr, std::uint32_t);
		virtual void write_qword(Addr, std::uint64_t);

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
		virtual std::uint8_t read_byte(Addr) override;
		virtual std::uint16_t read_word(Addr) override;
		virtual std::uint32_t read_dword(Addr) override;
		virtual std::uint64_t read_qword(Addr) override;
		virtual void write_byte(Addr, std::uint8_t) override;
		virtual void write_word(Addr, std::uint16_t) override;
		virtual void write_dword(Addr, std::uint32_t) override;
		virtual void write_qword(Addr, std::uint64_t) override;

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
		[[nodiscard]] std::uint8_t read_byte_const(Addr) const;
		[[nodiscard]] std::uint16_t read_word_const(Addr) const;
		[[nodiscard]] std::uint32_t read_dword_const(Addr) const;
		[[nodiscard]] std::uint64_t read_qword_const(Addr) const;
		Memory(){}
		// void add_device(MemoryDevice*);
		void add_device(std::shared_ptr<MemoryDevice>&&);
		// void remove_device(MemoryDevice*);
		void remove_device(std::shared_ptr<MemoryDevice>&&);

		std::uint8_t read_byte(Addr);
		std::uint16_t read_word(Addr);
		std::uint32_t read_dword(Addr);
		std::uint64_t read_qword(Addr);

		void write_byte(Addr, std::uint8_t);
		void write_word(Addr, std::uint16_t);
		void write_dword(Addr, std::uint32_t);
		void write_qword(Addr, std::uint64_t);

		void clock();
	};
} // namespace MicroSim

#include "ram.hpp"
#include "rom.hpp"
