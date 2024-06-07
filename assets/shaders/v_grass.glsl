
#version 330

//Uniform variables
uniform vec3 camPos;
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

uniform float maxGrassLength = 1.f;
uniform float maxLayer = 1.f;

// Positions/Coordinates
layout (location = 0) in vec3 aPos;
// Normals (not necessarily normalized)
layout (location = 1) in vec3 aNormal;
// Colors
layout (location = 2) in vec3 aColor;
// Texture Coordinates
layout (location = 3) in vec2 aTex;
// layout (location=1) in vec4 normal; //vertex normal vector in model space
//varying variables

out vec4 fColor;
out vec2 fTex;
out float fLayer;
out float fDistance;

void main(void) {
    fLayer = gl_InstanceID;
    vec3 nv=aPos+(fLayer*maxGrassLength/maxLayer)*normalize(aNormal);
    gl_Position=P*V*M*vec4(nv, 1);
    fDistance = gl_Position.z;
    fColor=vec4(aColor, 1);
    fTex = aTex;
}
