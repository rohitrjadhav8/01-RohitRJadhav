var canvas = null;
var gl = null;
var bFullScreen = false;
var canvas_original_width;
var canvas_orginal_height;

var requestAnimationFrame = 
	window.requestAnimationFrame ||
	window.webkitRequestAnimationFrame ||
	window.mozRequestAnimationFrame ||
	window.oRequestAnimationFrame ||
	window.msRequestAnimationFrame ||
	null;


var cancleAnimationFrame = 
	window.cancelAnimationFrame ||
	window.webkitCancelRequestAnimationFrame || window.webkitCancelAnimationFrame ||
	window.mozCancelRequestAnimationFrame || window.mozCancelAnimationFrame ||
	window.oCancelRequestAnimationFrame || window.oCancelAnimationFrame ||
	window.msCancelRequestAnimationFrame || window.msCancelAnimationRequest ||
	null;

function main(){


	canvas = document.getElementById("RRJ-Color");
	if(!canvas)
		console.log("Obtaining Canvas Failed!!\n");
	else
		console.log("Canvas Obtained!!\n");

	canvas_original_width = canvas.width;
	canvas_original_height = canvas.height;

	window.addEventListener("keydown", keyDown, false);
	window.addEventListener("click", mouseDown, false);
	window.addEventListener("resize", resize, false);


	initialize();

	resize();
	draw();

}




function toggleFullScreen(){

	var fullscreen_element = 
		document.fullscreenElement || 
		document.webkitFullscreenElement ||
		document.mozFullScreenElement ||
		document.msFullscreenElement ||
		null;

	if(fullscreen_element == null){

		if(canvas.requestFullscreen)
			canvas.requestFullscreen();
		else if(canvas.mozRequestFullScreen)
			canvas.mozRequestFullScreen();
		else if(canvas.webkitRequestFullscreen)
			canvas.webkitRequestFullscreen();
		else if(canvas.msRequestFullscreen)
			canvas.msRequestFullscreen();

		bFullScreen = true;
	}
	else{
		if(document.exitFullscreen)
			document.exitFullscreen();
		else if(document.mozCancelFullScreen)
			document.mozCancelFullScreen();
		else if(document.webkitExitFullscreen)
			document.webkitExitFullscreen();
		else if(document.msExitFullscreen)
			document.msExitFullscreen();

		bFullScreen = false;
	}
}


function initialize(){

	gl = canvas.getContext("webgl2");
	if(gl == null){
		console.log("Failed to get Rendering Context!!\n");
		return;
	}

	gl.viewportWidth = canvas.width;
	gl.viewportHeight = canvas.height;

	gl.clearColor(0.0, 0.0, 1.0, 1.0);
}

function resize(){

	if(bFullScreen == true){
		canvas.width = window.innerWidth;
		canvas.height = window.innerHeight;
	}
	else{
		canvas.width = canvas_original_width;
		canvas.height = canvas_original_height;
	}

	gl.viewport(0, 0, canvas.width, canvas.height);
}

function draw(){

	gl.clear(gl.COLOR_BUFFER_BIT);

	requestAnimationFrame(draw, canvas);
}

function keyDown(event){

	switch(event.keyCode){
		case 70:
			toggleFullScreen();
			break;
	}
}

function mouseDown(){


}
