declare namespace Module{
	function profile_simulation(name:string, sim:Simulation):void;
	class Deletable{
		delete() : void;
	}
	class Simulation extends Deletable{
		constructor(core:Core);
		run_cycles(cycles:number) : void;
		run_seconds(s:number) : void;
		reset() : void; //, &MicroSim::Simulation::reset)
		frequency() : number; //, &MicroSim::Simulation::reset)
		set_core_frequency(_freq:number) : void; //, &MicroSim::Simulation::reset)
		update_frequency() : void; //, &MicroSim::Simulation::reset)
		profile_simulation() : void; //, &MicroSim::Simulation::reset)
		core_registers() : string; //, &MicroSim::Simulation::core_registers)
		add_device(dev:Device) : void; //, &MicroSim::Simulation::core_registers)
		add_memory_device(dev:MemoryDevice) : void; //, &MicroSim::Simulation::core_registers)
		read_byte(addr:number) : number; //, &MicroSim::Simulation::read_byte)
		read_word(addr:number) : number; //, &MicroSim::Simulation::read_word)
		read_dword(addr:number) : number; //, &MicroSim::Simulation::read_dword)
		read_qword(addr:number) : number; //, &MicroSim::Simulation::read_qword)
		write_byte(addr:number, val:number) : void; //, &MicroSim::Simulation::write_byte)
		write_word(addr:number, val:number) : void; //, &MicroSim::Simulation::write_word)
		write_dword(addr:number, val:number) : void; //, &MicroSim::Simulation::write_dword)
		write_qword(addr:number, val:number) : void; //, &MicroSim::Simulation::write_qword)
	}
	class Device extends Deletable{
		clock() : void;
		reset() : void;
		frequency() : number;
		// set_frequency(freq:number) : void;
	}
	class MemoryDevice extends Device{
		read_byte(addr:number) : number;
		read_word(addr:number) : number;
		read_dword(addr:number) : number;
		read_qword(addr:number) : number;

		write_byte(addr:number, val:number) : void;
		write_word(addr:number, val:number) : void;
		write_dword(addr:number, val:number) : void;
		write_qword(addr:number, val:number) : void;

		low() : number;
		high() : number;
		priority() : number;
	}
	class Core extends Device{
		cycles() : number;
		registers() : string;
		register_memory_device(dev:MemoryDevice) : void;
		unregister_memory_device(dev:MemoryDevice) : void;
	}
	class Core6502 extends Core{

	}
	class CoreW65C02S_timed extends Core6502{
		constructor(freq:number);
	}
	class CoreW65C02S_simple extends Core6502{
		constructor(freq:number);
	}

	class MemoryDeviceHD44780U extends MemoryDevice{
		constructor(addr: number, size:number);
		cursor_pos() : number;
		char_code(addr:number) : number;
		gen_char(addr:number, offset:number) : number;

	}

}
declare namespace MicroSim{
	interface MemoryDevice{
		memory_devices() : Module.MemoryDevice[];
	}
	interface DisplayDevice extends MemoryDevice{
		refresh() : void;
	}

	class Simulation{

	}


	// class LCD implements DisplayDevice{
	// 	memory_devices() : Module.MemoryDevice[];
	// 	refresh() : void;
	// }
	class NES2C02 implements DisplayDevice{
		memory_devices() : Module.MemoryDevice[];
		refresh() : void;
	}
}
