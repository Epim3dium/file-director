#version 330

uniform sampler2D grass0;
uniform sampler2D detail0;
uniform sampler2D noise0;
uniform vec2 noiseOffset;

struct Grass {
    float length ;
    float maxLayer ;
    float maxDistance ;
    float padding ;
};
uniform Grass grass;

in vec4 fColor;
in vec2 fTex;
in float fLayer;
in float fDistance;
in vec3 fPos;

out vec4 pixelColor; 

void main(void) {
    //oszczedzanie powyzej jakiejs tam odleglosci
    if(fDistance > grass.maxDistance && fLayer != 0 && fLayer != grass.maxLayer - 1) discard;
    float noiseX=texture(noise0,(fTex + noiseOffset.xy) * 0.1).x; 
    float noiseY=texture(noise0,(fTex + noiseOffset.yx) * 0.15).x; 
    
    float swayMagnitude = 0.05;
    vec2 normalizedNoise = (vec2(noiseX, noiseY) - 0.5) * 2;
    vec2 texCoord = fTex +  normalizedNoise * swayMagnitude / grass.maxLayer * fLayer;
    vec4 grassDet=texture(detail0,texCoord); 
    vec4 grassCol=texture(grass0,texCoord); 
    
    vec4 col=grassCol;
    //zapobieganie tej dziwnej granicy
    if(fDistance > grass.maxDistance - grass.padding) grassDet.a = 1;
    if (fLayer!=0) col=vec4(grassCol.rgb,grassDet.a);
    if (grassDet.a==0 && fLayer!=0) discard;
    pixelColor=col; 
}
