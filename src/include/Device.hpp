#pragma once

namespace MicroSim
{
	class Device
	{
	public:
		virtual void reset() = 0;
		virtual void clock() = 0;
	};
} // namespace MicroSim
