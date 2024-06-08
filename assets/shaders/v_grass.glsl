
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
uniform vec3 camPos;
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

struct Grass {
    float length ;
    float maxLayer ;
    float maxDistance ;
    float padding ;
};
uniform Grass grass;

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
out vec4 fPos;

out vec4 fLight;//eye space
out vec4 fNormal; //eye space
out vec4 fViewer;//eye space

void main(void) {
    fLayer = gl_InstanceID;
    vec3 nv=aPos+(fLayer*grass.length/grass.maxLayer)*normalize(aNormal);
    gl_Position=P*V*M*vec4(nv, 1);

    fPos = V * M * vec4(aPos, 1);
    fLight = V * light.position; //vector towards the light in eye space
    if(light.position.w == 0) {
        fLight = normalize(vec4(light.position));
    }
    fViewer = vec4(0, 0, 0, 1); //vector towards the viewer in eye space
    fNormal = V * M * vec4(aNormal, 0); //normal vector in eye space

    fDistance = gl_Position.z;
    fTex = aTex;
}
