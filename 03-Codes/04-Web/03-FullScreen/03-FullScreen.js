var context = null;
var canvas = null;

function main(){

	//1.
	canvas = document.getElementById("RRJ-FullScreen");
	if(!canvas)
		console.log("Canvas Obtaining Failed!!\n");
	else 
		console.log("Canvas Obtained!!\n");

	console.log("Width: " + canvas.width + " Height: " + canvas.height);

	//2.
	context = canvas.getContext("2d");
	if(!context)
		console.log("Context Obtaining Failed!!\n");
	else
		console.log("Contex Obtained!!\n");


	//3.
	context.fillStyle = "black";
	context.fillRect(0, 0, canvas.width, canvas.height);

	//4 - 7.
	drawText("Hello World!!");


	//8.Add Event
	window.addEventListener("keydown", keyDown, false);
	window.addEventListener("click", mouseDown, false);
}

function drawText(str){

	//5.
	context.textAlign = "center";
	context.textBaseline = "middle";

	//7.
	context.font = "48px sans-serif";
	context.fillStyle = "white";

	context.fillText(str, canvas.width / 2, canvas.height / 2);
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

	}
	else{
		if(document.exitFullScreen)
			document.exitFullscreen();
		else if(document.mozCancelFullScreen)
			document.mozCancelFullScreen();
		else if(document.webkitExitFullscreen)
			document.webkitExitFullscreen();
		else if(document.msExitFullscreen)
			document.msExitFullscreen();
	}

}


function keyDown(event){


	switch(event.keyCode){
		case 70:
		toggleFullScreen();

		drawText("Hello World!!");
		break;
	}
}

function mouseDown(){

}

