#version 330 core

layout(location = 0) in vec3 in_position;

out Vertex {
    vec4 color;
} out_vertex;

void main()
{
    out_vertex.color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    gl_Position = vec4(in_position, 1.0);
}
