#pragma once

namespace MicroSim
{
	class Device
	{
		unsigned long int m_frequency;
	public:
		constexpr Device(unsigned long int _freq) : m_frequency{_freq} {};
		constexpr unsigned long int frequency() const { return m_frequency; }
		void set_frequency(unsigned long int _freq) { m_frequency = _freq; }
		virtual void reset() = 0;
		virtual void clock() = 0;
		virtual ~Device(){};
	};
} // namespace MicroSim
