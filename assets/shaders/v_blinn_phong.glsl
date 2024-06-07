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

out vec4 fColor;
out vec2 fTex;
out vec3 fNormal;
out vec3 fPos;

void main(void) {
    gl_Position = P*V*M* vec4(aPos, 1.0);
    fPos = vec3(M*vec4(aPos, 1.0));
    fNormal = normalize(mat3(transpose(inverse(M))) * aNormal);
    // mat4 G=mat4(inverse(transpose(mat3(M))));
    // vec4 n=normalize(V*G*normal);
    // float nl=clamp(dot(n,lightDir),0,1);
    fColor=vec4(aColor, 1);
    fTex = aTex;
}
