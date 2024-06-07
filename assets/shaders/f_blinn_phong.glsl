#version 330

struct Light {
    //if w = 0 then it is directional light
    vec4 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 color;
};

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

uniform Light light; 

uniform sampler2D diffuse0;
uniform sampler2D specular0;
uniform sampler2D normal0;

in vec4 fColor;
in vec2 fTex;
in vec3 fNormal;
in vec3 fPos;
in mat3 TBN;

out vec4 pixelColor;

void main(void) {
    vec3 norm = texture(normal0, fTex).rgb;
    norm = norm * 2.0 - 1.0;
    norm = normalize(TBN * norm);
    // vec3 norm = normalize(fNormal);
    vec3 lightDir = normalize(vec3(light.position) - fPos); 
    if(light.position.w == 0) { //treat as directional
        lightDir = normalize(vec3(light.position));
    }
    
    float diff = max(dot(norm, lightDir), 0.0);

    vec3 viewDir = normalize(vec3(inverse(V) * vec4(0, 0, 0, 1)) - fPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float shininess = 32.0;
    float spec = pow(max(dot(norm, halfwayDir), 0.0), shininess);

    vec4 color = texture(diffuse0, fTex);
    
    vec4 specularColor = texture(specular0, fTex);
    vec4 specular = vec4(light.specular, 1) * spec * specularColor;
    vec4 diffuse = diff * vec4(light.diffuse, 1) * color;
    vec4 ambient = vec4(light.ambient, 1) * color;
    
    vec4 result = diffuse + ambient + specular;
    pixelColor = result;
}
