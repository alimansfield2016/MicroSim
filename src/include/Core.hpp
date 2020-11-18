#pragma once

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
	class Core
	{
		const unsigned long int m_frequency;
		unsigned long int m_cycles;
		unsigned short int m_cooldown;

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
		unsigned short int _cycles;
		void update_cycles();

	public:
		unsigned long int cycles() const;
		unsigned short int cooldown() const;
		void set_cooldown(unsigned short int);
		unsigned short int dec_cooldown();
		Memory &memory() { return m_memory; };
		std::string registers() const;

	public:
		virtual ~Core();
		virtual void step() = 0;
		virtual void reset() = 0;
	};
} // namespace MicroSim
