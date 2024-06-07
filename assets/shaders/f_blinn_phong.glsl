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
uniform sampler2D gloss0;

in vec4 fColor;
in vec2 fTex;
in vec3 fNormal;
in vec3 fPos;

out vec4 pixelColor;

void main(void) {
    vec3 norm = normalize(fNormal);
    vec3 lightDir = normalize(vec3(light.position) - fPos); 
    if(light.position.w == 0) { //treat as directional
        lightDir = normalize(vec3(-light.position));
    }
    
    float diff = max(dot(norm, lightDir), 0.0);

    vec3 viewDir = normalize(vec3(inverse(V) * vec4(0, 0, 0, 1)) - fPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    // vec3 reflectDir = reflect(-lightDir, norm);  
    float glossiness = texture(gloss0, fTex).x;
    float shininess = 16.0 * glossiness;
    float spec = pow(max(dot(fNormal, halfwayDir), 0.0), shininess);
    
    vec3 specular = light.specular * spec * vec3(texture(specular0, fTex));
    vec3 diffuse = diff * light.diffuse * vec3(texture(diffuse0, fTex));
    vec3 ambient = light.ambient * vec3(texture(diffuse0, fTex));
    
    vec4 result = vec4(diffuse + ambient + specular, 1);
    pixelColor = result;
}
