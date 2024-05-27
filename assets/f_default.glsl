
#version 330


out vec4 pixelColor; //Output variable of the fragment shader. (Almost) final pixel color.

//Varying variables
in vec4 fColor;

void main(void) {
	pixelColor=fColor;
}
