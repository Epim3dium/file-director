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
//tangent
layout (location = 4) in vec4 aTangent;
// layout (location=1) in vec4 normal; //vertex normal vector in model space
//varying variables
uniform sampler2D normal0;

out vec4 fColor;
out vec2 fTex;
out vec3 fNormal;
out vec3 fPos;
out mat3 TBN;

void main(void) {
    vec3 T = normalize(vec3(M * vec4(aTangent.xyz, 0.0)));
    vec3 N = normalize(vec3(M * vec4(aNormal, 0.0)));
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    TBN = mat3(T, B, N);

    fNormal = normalize(mat3(transpose(inverse(M))) * aNormal);
    // mat4 G=mat4(inverse(transpose(mat3(M))));
    // vec4 n=normalize(V*G*normal);
    // float nl=clamp(dot(n,lightDir),0,1);
    gl_Position = P*V*M* vec4(aPos, 1.0);
    fPos = vec3(M*vec4(aPos, 1.0));
    fColor=vec4(aColor, 1);
    fTex = aTex;
}
