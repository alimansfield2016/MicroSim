/// <reference path="./microsim.d.ts"/>

class Simulation{
	interval : number = 0.1; //ms
	interval_id : number = -1;
	simulation : Module.Simulation;
	core : Module.Core;
	devices : Module.Device[];
	display_devices : MicroSim.DisplayDevice[];
	_html : HTMLDivElement;
	_reset_buttton : HTMLButtonElement;
	_run_button : HTMLButtonElement;
	_dragable : Dragable;
	constructor(core : Module.Core){
		this.core = core;
		this.simulation = new Module.Simulation(this.core);
		this.devices = [];
		this.display_devices = [];
		//create html interface
		this._html = document.createElement("div");
		this._html.classList.add("simulation", "handle", "dragable");

		let title = document.createElement("span");
		title.innerText = "Simulation";
		this._html.append(title);


		//button to reset simulation (literally just a reset button)
		this._reset_buttton = document.createElement("button");
		this._reset_buttton.classList.add("reset");
		this._reset_buttton.onclick = this.reset.bind(this);
		this._reset_buttton.innerText = "reset";
		this._html.append(this._reset_buttton);
		//button to run/stop simulation
		this._run_button = document.createElement("button");
		this._run_button.classList.add("run");
		this._run_button.onclick = this.run.bind(this);
		this._run_button.innerText = "run";
		this._html.append(this._run_button);

		document.body.append(this._html);


		this._dragable = new Dragable(this._html);
	}
	add_device(device: Module.Device){
		this.devices.push(device);
		this.simulation.add_device(device);
	}
	add_memory_device(device: Module.MemoryDevice){
		this.devices.push(device);
		this.simulation.add_memory_device(device);
	}
	add_display_device(device : MicroSim.DisplayDevice){
		this.display_devices.push(device);
		// for(let dev of device.devices())
			// this.add_device(dev);
		for(let dev of device.memory_devices())
			this.add_memory_device(dev);
	}
	set_core_frequency(freq:number){
		this.simulation.set_core_frequency(freq);
	}

	runner(){
		//run for amount of time specified by interval
		let ns = this.interval/1000; //ms to ns
		this.simulation.run_seconds(ns);
		for(let dev of this.display_devices)
			dev.refresh();
	}
	run(){
		if(this.interval_id == -1){
			this.interval_id = setInterval(this.runner.bind(this), this.interval)
		}
		this._run_button.innerText = "stop";
		this._run_button.onclick = this.stop.bind(this);

	}
	stop(){
		if(this.interval_id != -1){
			clearInterval(this.interval_id);
			this.interval_id = -1;
		}
		this._run_button.innerText = "run";
		this._run_button.onclick = this.run.bind(this);
	}
	run_stop(){
		if(this.interval_id == -1)
			this.run();
		else
			stop();
	}
	reset(){
		this.core.reset();
		for(let dev of this.devices)
			dev.reset();
	}
	profile(name:string){
		Module.profile_simulation(name, this.simulation);
	}
	destroy(){

	}
}