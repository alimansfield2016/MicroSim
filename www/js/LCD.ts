/// <reference path="./microsim.d.ts" />

namespace MicroSim{

	export class LCD implements MicroSim.DisplayDevice
	{
		_controllers : Module.MemoryDeviceHD44780U[] = [];
		_html : HTMLDivElement;
		_width = 16;
		_height = 4;
		_char_width = 5;
		_char_height = 8;
		_chars : HTMLDivElement[][][] = [];
		_dragable : Dragable;
		constructor(addr:number , x:number = 16, y:number = 4){
			let unsupported : boolean = false 
			this._controllers[0] = new Module.MemoryDeviceHD44780U(addr, 2);
			if(y == 1){
				if(x > 80) unsupported = true;
			}else if(y == 2){
				if(x > 40) unsupported = true;
				// if(x > 40)
					// this._controllers[1] = new Module.MemoryDeviceHD44780U(addr+2, 2);
			}else if(y == 4){
				if(x > 40) unsupported = true;
				if(x > 20){
					this._controllers[1] = new Module.MemoryDeviceHD44780U(addr+2, 2);
				}
			}
			if(unsupported)
				throw `Unsupported dimensions: ${x}*${y}`;
			this._width = x;
			this._height = y;

			this._html = document.createElement("div");
			this._html.classList.add("lcd", "handle", "dragable");
			// this._html.setAttribute("lcd-w", `${this._width}`);
			// this._html.setAttribute("lcd-h", `${this._height}`);
			this._html.setAttribute("style", `--lcd-width: ${this._width}; --lcd-height: ${this._height};`);
			this._html.style.top="0";
			this._html.style.left="0";
			document.body.appendChild(this._html);
			this._dragable = new Dragable(this._html);
			for(let i = 0; i < this._height; i++){
				for(let j = 0; j < this._width; j++){
					let character = document.createElement("div");
					character.classList.add("char");
					character.setAttribute("style", `--char-width: ${this._char_width}; --char-height: ${this._char_height};`)
					this._html.append(character);

					let char = [];

					for(let cy = 0; cy < this._char_height; cy++){
						char[cy] = [];
						for(let cx = 0; cx < this._char_width; cx++){
							let pixel = document.createElement("div");
							pixel.className = "pixel";
							
							character.append(pixel);
							char[cy][cx] = pixel;
						}
					}
					this._chars.push(char);
				}
			}

			// this._html.addEventListener("contextmenu")

		}

		destroy(){
			for(let controller of this._controllers)
				controller.delete();
			this._html.parentElement.removeChild(this._html);
		}

		// get controllers(){
			// return this._controllers;
		// }

		memory_devices(){
			return this._controllers;
		}
		

		refresh(){
			let idx = 0;
			for(let y = 0; y < this._height; y++){
				let addr = y * 40;
				let controller;
				if(y > 1 && this._width <= 20) {
					addr -= 20 + 40; //1.5 rows back
					controller = this._controllers[0];
				}else
				if(y > 1 && this._width > 20) {
					addr -= 80;
					controller = this._controllers[1];
				}else{
					controller = this._controllers[0];
				}

				for(let x = 0; x < this._width; x++){
					let charcode = controller.char_code(addr);
					this.refresh_char(controller, this._chars[idx++], charcode);
					addr++;
				}
			}
		}

		refresh_char(controller : Module.MemoryDeviceHD44780U, char : HTMLDivElement[][], charcode : number){
			let offset = 0;
			for(let cy = 0; cy < this._char_height; cy++){
				let row = char[cy];
				let bits = controller.gen_char(charcode, offset);
				for(let cx = 0; cx < this._char_width; cx++){
					let pixel = row[cx];
					let b = (bits >> (this._char_width - cx - 1)) & 0x01;
					if(b)
						pixel.style.visibility = "visible";
					else
						pixel.style.visibility = "hidden";
				}
				offset++;
			}
		}

		draw_cursor(){
			for(let i = 0; i < this._controllers.length; i++){
				let controller = this._controllers[i];
				let cursor_pos = controller.cursor_pos();
				//check if cursor visible
				//map index to character
				if(cursor_pos > 40){}
			}
		}

		add_to_core(core : Module.Core){
			for(let controller of this._controllers)
				core.register_memory_device(controller);
		}

	}
}