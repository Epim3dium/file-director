#version 330 core
out vec4 FragColor;

in vec3 texCoords;

uniform samplerCube skybox;

void main()
{    
    FragColor = texture(skybox, texCoords);
    if(FragColor.r < 0.1f) {
        FragColor = vec4(1, 1, 1, 1);
    }
}
