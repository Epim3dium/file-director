#include "mesh.h"
#include "elements_buffer.h"
#include <iostream>
#include <string>
#include <assert.h>
#include <unordered_map>
#include <unordered_set>

void Mesh::addTexture(const Texture& tex) {
    textures.push_back(tex);
}
void Mesh::removeTexture(unsigned int ID) {
    // auto itr = std::find(textures.begin(), textures.end(), [&](const Texture& tex) { return tex.ID == ID; });
    // if(itr != textures.end()) {
    //     textures.erase(itr);
    // }
}

void Mesh::draw(Shader& shader, Camera& cam, GLsizei count) {
    // Bind shader to be able to access uniforms
	shader.bind();
	VAO.bind();

	// Keep track of how many of each type of textures we have
    std::unordered_map<std::string, unsigned int> tex_types_count;
    // std::unordered_set<int> slots;

	for (unsigned int i = 0; i < textures.size(); i++)
	{
		std::string type = textures[i].type;
        std::string num = std::to_string(tex_types_count[type]++);
        // assert(slots.find(textures[i].unit) == slots.end());
		textures[i].texUnit(shader, (type + num).c_str());
        // slots.insert(textures[i].unit);
		textures[i].bind();
	}
	// Take care of the camera Matrix
	cam.exportMatrix(shader);

    if(count == 1) {
        // Draw the actual mesh
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    }else {
        glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0, count);
    }
}
static int cantor(int a, int b) {
   return (a + b + 1) * (a + b) / 2 + b;
}

static int hash_set(int a, int b, int c) {
   return cantor(a, cantor(b, c));
}
Mesh::Mesh(std::string obj_filepath, const std::vector<Texture>& textures)  {
    std::ifstream in(obj_filepath);
    std::string line;
    //v
    //vt
    //vn
    //g
    //#
    std::vector<glm::vec3> vert_pos;
    std::vector<glm::vec3> vert_norm;
    std::vector<glm::vec2> vert_tex;

    std::unordered_map<int, int> indexSet_to_index;
    while(std::getline(in, line)) {
        
        std::stringstream ss(line);
        std::string token;
        ss >> token;
        if(token == "#") {
            continue;
        }
        if(token == "vn") {
            glm::vec3 normal;
            ss >> normal.x;
            ss >> normal.y;
            ss >> normal.z;
            vert_norm.push_back(normal);
        }
        if(token == "vt") {
            glm::vec2 tex;
            ss >> tex.x;
            ss >> tex.y;
            vert_tex.push_back(tex);
        }
        if(token == "v") {
            glm::vec3 pos;
            ss >> pos.x;
            ss >> pos.y;
            ss >> pos.z;
            vert_pos.push_back(pos);
        }
        if(token == "f") {
            char dummy;
            int i = 0;
            while(ss.good()){
                int pos_idx, tex_idx, norm_idx;
                ss>>pos_idx>>dummy>>tex_idx>>dummy>>norm_idx;
                pos_idx--; tex_idx--; norm_idx--;
                
                int hash = hash_set(pos_idx, tex_idx, norm_idx);
                auto itr = indexSet_to_index.find(hash);
                
                size_t index = 0;
                if(itr != indexSet_to_index.end()) {
                    index = itr->second;
                }else {
                    Vertex vert(vert_pos[pos_idx], vert_norm[norm_idx], {1, 1, 0}, vert_tex[tex_idx]);
                    index = verticies.size();
                    verticies.push_back(vert);
                }
                
                //if reading quads
                if(i > 3) {
                    std::cerr << "more than Quads not supported\n"; 
                }
                else if(i == 3) {
                    size_t index0 = indices[indices.size() - 1];
                    size_t index1 = indices[indices.size() - 3];
                    indices.push_back(index);
                    indices.push_back(index0);
                    indices.push_back(index1);
                }else {
                    indices.push_back(index);
                }

                indexSet_to_index[hash] = index;
                i++;
            }
        }
    }
    std::cerr << "vert: " << verticies.size() << "\n";
    std::cerr << "idxs: " << indices.size() << "\n";
    *this = Mesh(verticies, indices, textures);
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
