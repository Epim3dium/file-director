#version 330

uniform sampler2D diffuse0;
uniform vec4 uColor = vec4(0, 0, 0, 0);
out vec4 pixelColor; //Output variable of the fragment shader. (Almost) final pixel color.
//Varying variables
in vec4 fColor;
in vec2 fTex;

void main(void) {
    if(uColor.a == 0) {
        pixelColor=texture(diffuse0, fTex);
    }else {
        pixelColor = uColor;
    }
}
