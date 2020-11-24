

namespace FSManager{

	export class Directory{

	}
	
	export class File{
	
	}
	
	export class UI{
		_html : HTMLDivElement;
		_bar : HTMLDivElement;
		_dragable : Dragable;
		_btnClose : HTMLButtonElement;
		_btnMax : HTMLButtonElement;
		constructor(){
			//construct the HTML view
			this._html = document.createElement("div");
			this._html.classList.add("FS", "UI", "dragable");
			document.body.append(this._html);

			this._bar = document.createElement("div");
			this._bar.classList.add("FS", "bar", "handle");
			this._html.append(this._bar);


			this._btnClose = document.createElement("button");
			this._btnClose.classList.add("FS", "close");
			this._bar.append(this._btnClose);



			this._dragable = new Dragable(this._html);
		}

		select_file() : Promise<string>{
			return Promise.resolve(".");
		}

		select_files() : Promise<string[]>{
			return Promise.resolve(["."]);
		}

		close(){
			//reject all awaiting promises.
		}
	}
}