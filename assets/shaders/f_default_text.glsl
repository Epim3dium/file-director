#version 330

uniform sampler2D text0;
out vec4 pixelColor; //Output variable of the fragment shader. (Almost) final pixel color.
//Varying variables
in vec4 fColor;
in vec2 fTex;

void main(void) {
	pixelColor=texture(text0,fTex);
}
