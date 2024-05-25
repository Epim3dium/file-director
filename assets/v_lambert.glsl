#version 330

//Uniform variables
uniform mat4 cam_mat;
uniform mat4 M;

uniform vec4 color=vec4(1,1,1,1);
// uniform vec4 lightDir=vec4(-1,0,0,0);

//Attributes
layout (location=0) in vec4 vertex; //vertex coordinates in model space
// layout (location=1) in vec4 normal; //vertex normal vector in model space
//varying variables
out vec4 i_color;

void main(void) {
    gl_Position=cam_mat*M*vertex;
    // mat4 G=mat4(inverse(transpose(mat3(M))));
    // vec4 n=normalize(V*G*normal);
    // float nl=clamp(dot(n,lightDir),0,1);
    i_color=color;
}
