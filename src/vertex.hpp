#pragma once
#include <glm/glm.hpp>
struct Vertex {
    typedef glm::vec3 vec3;
    typedef glm::vec2 vec2;
    vec3 position;
    vec3 normal;
    vec3 color;
    vec2 textureUV;
};

