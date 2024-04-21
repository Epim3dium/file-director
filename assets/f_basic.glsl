#version 330 core

in Vertex {
    vec4 color;
} in_vertex;

out vec4 out_color;

void main()
{
    out_color = in_vertex.color;
}
