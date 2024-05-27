#pragma once
#include <glm/glm.hpp>
struct Vertex {
    typedef glm::vec3 vec3;
    typedef glm::vec2 vec2;
    
    vec3 position;
    vec3 normal;
    vec3 color;
    vec2 textureUV;
    Vertex() {}
    Vertex(vec3 p, vec3 n = {0, 0, 0}, vec3 c = {1, 1, 1}, vec2 t = {0, 0}) : position(p), normal(n), color(c), textureUV(t) {}
};

