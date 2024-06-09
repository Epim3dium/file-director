#pragma once
#include "font_renderer.h"
#include "glad/glad.h"
#include "mesh.h"
#include "shader.h"

#include <filesystem>

enum class FileType
{
    Dir,
    Readable,
    Image,
    Link,
    Other
};
struct File  {
    static std::string get_homedir(void);
    static std::pair<Texture, float> genTextTexture(FontRenderer& fr, std::string str, int width = 0, int height = 0, int font_height = 0);
    static std::vector<Vertex> genTextVerticies(float text_ratio, float text_height = 0.3) ;
    static float textTextureRatio(FontRenderer& fr, std::string str);

    static const glm::vec3 FILE_COLOR;
    static const std::vector<Vertex> VERTICES_DIR;
    static const std::vector<GLuint> INDICES_DIR;

    static const glm::vec3 TXT_COLOR;
    static const std::vector<Vertex> VERTICES_TXT;
    static const std::vector<GLuint> INDICES_TXT;

    static const std::vector<Vertex> VERTICES_IMG;
    static const std::vector<GLuint> INDICES_IMG;

    static const glm::vec3 LINK_COLOR;
    static const std::vector<Vertex> VERTICES_LINK;
    static const std::vector<GLuint> INDICES_LINK;

    static const std::vector<GLuint> TEXT_INDICES;
    
    std::filesystem::path my_path;
    Mesh main_mesh;
    Mesh text_mesh;
    glm::vec3 position;
    glm::vec3 scale;
    glm::mat4 rotation = glm::mat4(1);
    FileType type;

    bool isActive = true;
    bool awaiting_deletion = false;
    bool selected = false;
    
    void decomission();
    File(glm::vec3 pos, glm::vec3 scale, glm::mat4 rot, FontRenderer &fr, std::filesystem::path path);
    void draw(Shader& folder_shader, Shader& area_shader, Shader& text_shader, Camera& camera);
};
