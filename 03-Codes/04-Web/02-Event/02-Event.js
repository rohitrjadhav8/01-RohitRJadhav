function main(){

	//1. Get Canvas
	var canvas = document.getElementById("RRJ-Event");
	if(!canvas)
		console.log("Getting Context Failed!\n");
	else
		console.log("Getting Context Done!!\n");

	console.log("Canvas Width: " + canvas.width + "Canvas Height: " + canvas.height);

	//2.Getting Context
	var context = canvas.getContext("2d");
	if(!context)
		console.log("Obtaining Context Failed!\n");
	else
		console.log("Context Obtained!!\n");


	//3. Black background
	context.fillStyle = "black";
	context.fillRect(0, 0, canvas.width, canvas.height);

	//4. Alignment
	context.textAlign = "center";
	context.textBaseline = "middle";

	//5.
	var str = "Hello World!!";

	
	//6. Font
	context.font = "48px sans-serif";
	context.fillStyle = "white";

	//7.Draw
	context.fillText(str, canvas.width / 2, canvas.height / 2);


	//8. Event Adding
	window.addEventListener("keydown", keyDown, false);
	window.addEventListener("click", mouseDown, false);

}

function keyDown(){
	alert("A Key is Pressed!!");
}

function mouseDown(){
	alert("Mouse is Click!!");
}

