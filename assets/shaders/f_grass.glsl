#version 330

uniform sampler2D grass0;
uniform sampler2D detail0;
uniform sampler2D noise0;
uniform vec2 noiseOffset;

uniform float maxGrassLength = 1.f;
uniform float maxLayer = 1.f;
uniform float maxGrassingDistance = 5;

in vec4 fColor;
in vec2 fTex;
in float fLayer;
in float fDistance;

out vec4 pixelColor; 

void main(void) {
    if(fDistance > maxGrassingDistance && fLayer != 0) discard;
    float noiseX=texture(noise0,(fTex + noiseOffset.xy) * 0.1).x; 
    float noiseY=texture(noise0,(fTex + noiseOffset.yx) * 0.15).x; 
    
    float swayMagnitude = 0.05;
    vec2 normalizedNoise = (vec2(noiseX, noiseY) - 0.5) * 2;
    vec2 texCoord = fTex +  normalizedNoise * swayMagnitude / maxLayer * fLayer;
    vec4 grass=texture(detail0,texCoord); 
    vec4 grassCol=texture(grass0,texCoord); 
    
    vec4 col=grassCol;
    if (fLayer!=0) col=vec4(grassCol.rgb,grass.a);
    if (grass.a==0 && fLayer!=0) discard;
    pixelColor=col; 
}
