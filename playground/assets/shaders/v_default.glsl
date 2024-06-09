#version 330

//Uniform variables
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

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
uniform float backward = 0;
uniform float baloon = 0;

out vec4 fColor;
out vec2 fTex;

void main(void) {
    vec4 viewDir = normalize(vec4(0, 0, 0, 1)-V*M*vec4(aPos, 1));
    vec3 awayFromCenter = normalize(aPos);
    gl_Position=P*(V*M*(vec4(aPos + awayFromCenter * baloon, 1)) - viewDir * backward);
    // mat4 G=mat4(inverse(transpose(mat3(M))));
    // vec4 n=normalize(V*G*normal);
    // float nl=clamp(dot(n,lightDir),0,1);
    fColor=vec4(aColor, 1);
    fTex = aTex;
}
