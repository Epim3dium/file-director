#include "mesh.h"
#include "elements_buffer.h"
#include <iostream>
#include <string>
#include <assert.h>
#include <unordered_map>

void Mesh::addTexture(const Texture& tex) {
    textures.push_back(tex);
}
void Mesh::removeTexture(unsigned int ID) {
    // auto itr = std::find(textures.begin(), textures.end(), [&](const Texture& tex) { return tex.ID == ID; });
    // if(itr != textures.end()) {
    //     textures.erase(itr);
    // }
}
void Mesh::generateTangents() {
    for(auto& v : verticies) {
        v.tangent = glm::vec4(0, 0, 0, 0);
    }
    std::vector<glm::vec3> tan1(verticies.size(), glm::vec3(0, 0, 0));
    std::vector<glm::vec3> tan2(verticies.size(), glm::vec3(0, 0, 0));
    for(int i = 0; i < indices.size(); i += 3) {
        size_t i1 = indices[i];
        size_t i2 = indices[i + 1];
        size_t i3 = indices[i + 2];
        glm::vec3 v1 = verticies[i1].position;
        glm::vec3 v2 = verticies[i2].position;
        glm::vec3 v3 = verticies[i3].position;
        glm::vec2 w1 = verticies[i1].textureUV;
        glm::vec2 w2 = verticies[i2].textureUV;
        glm::vec2 w3 = verticies[i3].textureUV;
        float x1 = v2.x - v1.x;
        float x2 = v3.x - v1.x;
        float y1 = v2.y - v1.y;
        float y2 = v3.y - v1.y;
        float z1 = v2.z - v1.z;
        float z2 = v3.z - v1.z;
        
        float s1 = w2.x - w1.x;
        float s2 = w3.x - w1.x;
        float t1 = w2.y - w1.y;
        float t2 = w3.y - w1.y;
        float r = 1.0F / (s1 * t2 - s2 * t1);
        if(abs(s1 * t2 - s2 * t1) < 0.01f || r == 0.) {
            r = 1.f;
        }
        glm::vec3 sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r,
                (t2 * z1 - t1 * z2) * r);
        glm::vec3 tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r,
                (s1 * z2 - s2 * z1) * r);

        tan1[i1] += sdir;
        tan1[i2] += sdir;
        tan1[i3] += sdir;
        
        tan2[i1] += tdir;
        tan2[i2] += tdir;
        tan2[i3] += tdir;
    }
    for(int i = 0; i < verticies.size(); i++) {
        const glm::vec3& n = verticies[i].normal;
        const glm::vec3& t = tan1[i];
        
        // Gram-Schmidt orthogonalize
        verticies[i].tangent = glm::vec4(glm::normalize(t - n * glm::dot(n, t)), 0);
        if(isnan(verticies[i].tangent.x) || isnan(verticies[i].tangent.y) || isnan(verticies[i].tangent.z)) {
            verticies[i].tangent = verticies[i - 1].tangent;
        }

        
        // Calculate handedness
        verticies[i].tangent.w = (glm::dot(glm::cross(n, t), tan2[i]) < 0.0F) ? -1.0F : 1.0F;
    }
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
Mesh::Mesh(std::string obj_filepath, const std::vector<Texture>& textures) : textures(textures){
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
                //if reading quads
                if(i > 3) {
                    // std::cerr << "more than Quads not supported\n"; 
                    break;
                }
                
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
                
                if(i == 3) {
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
    generateTangents();
    initVAO();
}
void Mesh::initVAO() {
	VAO.bind();
	// Generates Vertex Buffer Object and links it to vertices
	VertexBuffer VBO(verticies);
	// Generates Element Buffer Object and links it to indices
	ElementsBuffer EBO(indices);
	// Links VBO attributes such as coordinates and colors to VAO
	VAO.link(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	VAO.link(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
	VAO.link(VBO, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
	VAO.link(VBO, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(9 * sizeof(float)));
    VAO.link(VBO, 4, 4, GL_FLOAT, sizeof(Vertex), (void*)(11 * sizeof(float)));
	// Unbind all to prevent accidentally modifying them
	VAO.unbind();
	VBO.unbind();
	EBO.unbind();
}

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& idxs, const std::vector<Texture>& texs) 
    : verticies(vertices), indices(idxs), textures(texs) 
{
    generateTangents();
    initVAO();
}
Mesh Mesh::Plane(float size, std::vector<Texture> texs){
    const std::vector<Vertex> planeVerticies = { 
        Vertex({size,  0, size},  {0, 1, 0}, {1, 0, 1}, {size, size}),
        Vertex({-size, 0, size},  {0, 1, 0}, {1, 0, 1}, {0, size}),
        Vertex({-size, 0, -size}, {0, 1, 0}, {1, 0, 1}, {0, 0}),
        Vertex({size,  0, -size}, {0, 1, 0}, {1, 0, 1}, {size, 0})
    };
    return Mesh(planeVerticies, {0, 1, 2, 2, 3, 0}, texs);
}
Mesh Mesh::UniformConvex(float radius, size_t sides, std::vector<Texture> textures) {
    std::vector<Vertex> planeVerticies;
    std::vector<GLuint> indicies;
    planeVerticies.push_back(Vertex({0, 0, 0}, {0, 1, 0}, {1, 0, 1}, glm::vec2(0.5, 0.5) * radius));
    for(int i = 0; i < sides + 1; i++) {
        float angle = 2.f * M_PI / static_cast<float>(sides) * i;
        glm::vec3 dir = {cos(angle), 0, sin(angle)};
        glm::vec2 texCoord = glm::vec2(dir.x, dir.z) * 0.5f + 0.5f;
        glm::vec3 pos = dir * radius;
        planeVerticies.push_back(Vertex(pos, {0, 1, 0}, {1, 0, 1}, texCoord * radius));
        if(i > 0) {
            indicies.push_back(0);
            indicies.push_back(planeVerticies.size() - 1);
            indicies.push_back(planeVerticies.size() - 2);
        }
    }
    return Mesh(planeVerticies, indicies, textures);
}
Mesh Mesh::UniformConvexCutout(float large_size, float radius, size_t sides, std::vector<Texture> textures) {
    std::vector<Vertex> planeVerticies;
    std::vector<GLuint> indicies;
    const int BL = 0;
    const int BR = 1;
    const int TL = 2;
    const int TR = 3;
    planeVerticies.push_back(Vertex({-radius, 0, -radius}, {0, 1, 0}, {1, 0, 1}, glm::vec2(0, 0) * radius));
    planeVerticies.push_back(Vertex({radius, 0, -radius}, {0, 1, 0}, {1, 0, 1}, glm::vec2(1, 0) * radius));
    planeVerticies.push_back(Vertex({-radius, 0, radius}, {0, 1, 0}, {1, 0, 1}, glm::vec2(0, 1) * radius));
    planeVerticies.push_back(Vertex({radius, 0, radius}, {0, 1, 0}, {1, 0, 1}, glm::vec2(1, 1) * radius));
    
    if(large_size != radius) {
        planeVerticies.push_back(Vertex({-large_size, 0, -large_size}, {0, 1, 0}, {1, 0, 1}, glm::vec2(-1, -1) * large_size));
        planeVerticies.push_back(Vertex({large_size, 0, -large_size}, {0, 1, 0}, {1, 0, 1}, glm::vec2(1, -1) * large_size));
        planeVerticies.push_back(Vertex({-large_size, 0, large_size}, {0, 1, 0}, {1, 0, 1}, glm::vec2(-1, 1) * large_size));
        planeVerticies.push_back(Vertex({large_size, 0, large_size}, {0, 1, 0}, {1, 0, 1}, glm::vec2(1, 1) * large_size));
        for( auto [a, b] : std::vector<std::pair<int, int>>{ {TL, TR}, {TR, BR}, {BL, BR}, {TL, BL}}) {
            indicies.push_back(a);
            indicies.push_back(b);
            indicies.push_back(a + 4);

            indicies.push_back(b);
            indicies.push_back(a + 4);
            indicies.push_back(b + 4);
        }
    }
    
    int prevMaster = TR;
    float prevAngle = 0.f;
    for(int i = 0; i < sides + 1; i++) {
        float angle = 2.f * M_PI / static_cast<float>(sides) * i;
        glm::vec3 dir = {cos(angle), 0, sin(angle)};
        glm::vec2 texCoord = glm::vec2(dir.x, dir.z) * 0.5f + 0.5f;
        glm::vec3 pos = dir * radius;
        planeVerticies.push_back(Vertex(pos, {0, 1, 0}, {1, 0, 1}, texCoord * radius));
        if(i > 0) {
            float avgang = (angle + prevAngle) * 0.5f;
            int master = TR;
            if(avgang >= 2.f * M_PI * 0.75f) {
                master = BR;
            }else if(avgang >= M_PI) {
                master = BL;
            }else if(avgang >= 2.f * M_PI * 0.25f) {
                master = TL;
            }else {
                master = TR;
            }
            indicies.push_back(master);
            indicies.push_back(planeVerticies.size() - 1);
            indicies.push_back(planeVerticies.size() - 2);
            if(master != prevMaster) {
                indicies.push_back(master);
                indicies.push_back(prevMaster);
                indicies.push_back(planeVerticies.size() - 2);
            }
            prevMaster = master;
        }    
        prevAngle = angle;
    }
    return Mesh(planeVerticies, indicies, textures);
}
