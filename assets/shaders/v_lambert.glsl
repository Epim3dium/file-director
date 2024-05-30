#version 330

//Uniform variables
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform vec3 lightPos = vec3(0, 0, -6);

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
out vec4 fLight;//eye space
out vec4 fNormal; //eye space
out vec4 fViewer;//eye space

void main(void) {
    fLight = normalize(V * vec4(lightPos, 1) - V * M * vec4(aPos, 1)); //vector towards the light in eye space
    fViewer = normalize(vec4(0, 0, 0, 1) - V * M * vec4(aPos, 1)); //vector towards the viewer in eye space
    fNormal = normalize(V * M * vec4(aNormal, 0)); //normal vector in eye space
    gl_Position=P*V*M*vec4(aPos, 1);
    // mat4 G=mat4(inverse(transpose(mat3(M))));
    // vec4 n=normalize(V*G*normal);
    // float nl=clamp(dot(n,lightDir),0,1);
    fColor=vec4(aColor, 1);
}
