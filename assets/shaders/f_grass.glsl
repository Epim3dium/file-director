#version 330

uniform sampler2D grass0;
uniform sampler2D detail0;
uniform sampler2D noise0;
uniform vec2 noiseOffset;

uniform float maxGrassLength = 1.f;
uniform float maxLayer = 1.f;

in vec4 fColor;
in vec2 fTex;
in float fLayer;

out vec4 pixelColor; 

void main(void) {
    float noiseX=texture(noise0,(fTex + noiseOffset.xy) * 0.2).x; //fur color
    float noiseY=texture(noise0,(fTex + noiseOffset.yx) * 0.1).x; //fur color
    
    float swayMagnitude = 0.05;
    vec2 normalizedNoise = (vec2(noiseX, noiseY) - 0.5) * 2;
    vec2 texCoord = fTex +  normalizedNoise * swayMagnitude / maxLayer * fLayer;
    vec4 fur=texture(detail0,texCoord); //fur (is there a hair?)
    vec4 furCol=texture(grass0,texCoord); //fur color
    
    vec4 col=furCol;
	// pixelColor=fColor;
    if (fLayer!=0) col=vec4(furCol.rgb,fur.a);
    // Discard invisible fragment. Z Buffer will be unchanged
    if (fur.a==0 && fLayer!=0) discard;
    pixelColor=col/* *vec4(nl,nl,nl,1) */; 
    // pixelColor = texture(grass0, fTex);
}
