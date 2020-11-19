#pragma once

#include "Device.hpp"
#include "Memory.hpp"
#include <memory>
#include <vector>
#include <string>

namespace MicroSim
{
	class Core;
	struct CoreDescription
	{
		const char *name;
		std::shared_ptr<Core> (*create)();
	};
	class Core : public Device
	{
		const unsigned long int m_frequency;

		// registers (for GDB)
		const std::vector<
			std::tuple<
				std::string, 
				std::size_t,
				void* 
			> 
		> m_registers;

	protected:
		Core(const std::initializer_list<std::tuple<std::string, std::size_t, void*>> & _registers, unsigned long int _frequency);
		Memory m_memory;
		unsigned long int m_cycles;
		void clock_mem();

	public:
		unsigned long int cycles() const;
		Memory &memory() { return m_memory; };
		std::string registers() const;

	public:
		virtual ~Core();
		virtual void clock() override = 0;
		virtual void reset() override = 0;
	};
} // namespace MicroSim
