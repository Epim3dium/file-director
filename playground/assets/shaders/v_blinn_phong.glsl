#version 330

struct Light {
    //if w = 0 then it is directional light
    vec4 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 color;
};
uniform Light light; 
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

out vec3 fLightDir;
out vec3 fViewDir;
out mat4 TBN;

void main(void) {
    //workaround
    mat4 NormalMatrix = mat4(mat3(transpose(inverse(M))));
    vec4 T = normalize(NormalMatrix * vec4(aTangent.xyz, 0.0));
    vec4 N = normalize(NormalMatrix * vec4(aNormal, 0.0));
    T = normalize(T - dot(T, N) * N);
    vec4 B = normalize(vec4(cross(vec3(N), vec3(T)), 0));

    TBN = mat4(T, B, N, vec4(0, 0, 0, 1));

    vec3 VertPos=  vec3(M * vec4(aPos, 1));
    fViewDir=  vec3(inverse(V)*vec4(0, 0, 0, 1)) - VertPos;
    fLightDir= vec3(light.position) - VertPos;

    fNormal = normalize(mat3(transpose(inverse(M))) * aNormal);

    gl_Position = P*V*M* vec4(aPos, 1.0);
    fPos = vec3(M*vec4(aPos, 1.0));
    fColor=vec4(aColor, 1);
    fTex = aTex;
}
