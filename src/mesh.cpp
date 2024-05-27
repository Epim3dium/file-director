#include "mesh.h"
#include "elements_buffer.h"
#include <string>
#include <unordered_map>

void Mesh::draw(Shader& shader, Camera& cam, GLsizei count) {
    // Bind shader to be able to access uniforms
	shader.bind();
	VAO.bind();

	// Keep track of how many of each type of textures we have
    std::unordered_map<std::string, unsigned int> tex_types_count;

	for (unsigned int i = 0; i < textures.size(); i++)
	{
		std::string type = textures[i].type;
        std::string num = std::to_string(tex_types_count[type]++);
        
		textures[i].texUnit(shader, (type + num).c_str(), i);
		textures[i].bind();
	}
	// Take care of the camera Matrix
	glUniform3f(shader.u("camPos"), cam.position.x, cam.position.y, cam.position.z);
	cam.exportMatrix(shader, "camMatrix");

    if(count == 1) {
        // Draw the actual mesh
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    }else {
        glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0, count);
    }
}
Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& idxs, const std::vector<Texture>& texs) 
    : verticies(vertices), indices(idxs), textures(texs) {

	VAO.bind();
	// Generates Vertex Buffer Object and links it to vertices
	VertexBuffer VBO(vertices);
	// Generates Element Buffer Object and links it to indices
	ElementsBuffer EBO(indices);
	// Links VBO attributes such as coordinates and colors to VAO
	VAO.link(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	VAO.link(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
	VAO.link(VBO, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
	VAO.link(VBO, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(9 * sizeof(float)));
	// Unbind all to prevent accidentally modifying them
	VAO.unbind();
	VBO.unbind();
	EBO.unbind();
}
