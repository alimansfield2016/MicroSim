
class Dragable{
	dx : number = 0;
	dy : number = 0;
	px : number = 0;
	py : number = 0;
	element : HTMLElement;
	handle : HTMLElement;
	constructor(elmnt : HTMLElement){
		this.element = elmnt;
		this.handle = this.element.querySelector<HTMLElement>(".handle");

		this.handle = this.handle || this.element;
		this.handle.onmousedown = this.mouseDown.bind(this);
	}

	mouseDown(e : MouseEvent){
		e.preventDefault();
		this.px = e.clientX;
		this.py = e.clientY;
		document.onmouseup = this.mouseUp.bind(this);
		// call a function whenever the cursor moves:
		document.onmousemove = this.mouseMove.bind(this);
	}
	mouseUp(){
		document.onmouseup = null;
		// call a function whenever the cursor moves:
		document.onmousemove = null;
	}
	mouseMove(e : MouseEvent){
		this.dx = this.px - e.clientX;
		this.dy = this.py - e.clientY;
		this.px = e.clientX;
		this.py = e.clientY;
		this.element.style.top = (this.element.offsetTop - this.dy) + "px";
		this.element.style.left = (this.element.offsetLeft - this.dx) + "px";
	}
}

// function dragElement(elmnt : HTMLElement){

// 	var pos1 = 0, pos2 = 0, pos3 = 0, pos4 = 0;
// 	let elems = elmnt.getElementsByClassName("handle");
// 	if(elems.length){
// 		for(let elem in elems)
// 			elems[elem].onmousedown = dragMouseDown;
// 	}else{
// 		// otherwise, move the DIV from anywhere inside the DIV:
// 		elmnt.onmousedown = dragMouseDown;
// 	}
  
// 	function dragMouseDown(e) {
// 	  e = e || window.event;
// 	  e.preventDefault();
// 	  // get the mouse cursor position at startup:
// 	  pos3 = e.clientX;
// 	  pos4 = e.clientY;
// 	  document.onmouseup = closeDragElement;
// 	  // call a function whenever the cursor moves:
// 	  document.onmousemove = elementDrag;
// 	}
  
// 	function elementDrag(e) {
// 	  e = e || window.event;
// 	  e.preventDefault();
// 	  // calculate the new cursor position:
// 	  pos1 = pos3 - e.clientX;
// 	  pos2 = pos4 - e.clientY;
// 	  pos3 = e.clientX;
// 	  pos4 = e.clientY;
// 	  // set the element's new position:
// 	  elmnt.style.top = (elmnt.offsetTop - pos2) + "px";
// 	  elmnt.style.left = (elmnt.offsetLeft - pos1) + "px";
// 	}
  
// 	function closeDragElement() {
// 	  // stop moving when mouse button is released:
// 	  document.onmouseup = null;
// 	  document.onmousemove = null;
// 	}
// }