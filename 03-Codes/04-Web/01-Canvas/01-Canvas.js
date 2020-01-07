function main(){

	//1. To Get Canvas
	var canvas = document.getElementById("RRJ");

	if(!canvas)
		console.log("Obtaining Canvas Failed!!\n");
	else
		console.log("Obtaining Canvas Successful!!\n");


	console.log("Canvas Height: "+canvas.height+" Canvas Width: " + canvas.width);


	//2. Getting Context
	var context = canvas.getContext("2d");
	if(!context)
		console.log("Obtaining 2D Context Failed!!\n");
	else
		console.log("Obtaining 2D Context Done!!\n");


	//3.Background Black
	context.fillStyle = "black";
	context.fillRect(0, 0, canvas.width, canvas.height);


	//4. Center Text
	context.textAlign = "center";
	context.textBaseline = "middle";

	
	//5.
	var str = "Hello World!!!";


	//6. Font
	context.font = "48px sans-serif";


	//7. Display
	context.fillStyle = "white";

	context.fillText(str, canvas.width / 2, canvas.height / 2);

}
