#version 330

uniform sampler2D grass0;

in vec4 fColor;
in vec2 fTex;

out vec4 pixelColor; 

void main(void) {
	pixelColor=fColor;
    pixelColor = texture(grass0, fTex);
}
