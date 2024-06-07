#include "file.h"

    std::string File::get_homedir(void) {
        std::string homedir;
#ifdef _WIN32
        homedir = getenv("HOMEDRIVE");
        homedir += getenv("HOMEPATH");
#else
        homedir = getenv("HOME");
#endif
        return homedir;
    }
    std::pair<Texture, float> File::genTextTexture(FontRenderer& fr, std::string str, glm::vec3 color) {
        constexpr float SCALE = 2.f;
        constexpr float FONT_HEIGHT = 32.f * SCALE;
        constexpr float FONT_WIDTH = 32.f * SCALE;

        int width = fr.getPixelWidth(str, FONT_HEIGHT);
        // int width = FONT_WIDTH * str.size();
        double font_ratio = width / FONT_HEIGHT ;
        return {fr.generate(str, color, width, FONT_HEIGHT , 1), font_ratio};
    }
    std::vector<Vertex> File::genTextVerticies(float text_ratio, float text_height) {
        const std::vector<Vertex> TEXT_VERTICES = {
            {glm::vec3(-text_height*text_ratio/2, 1.1, 0), {}, {1, 0, 0}, glm::vec2(0, 1)},
            {glm::vec3( text_height*text_ratio/2, 1.1, 0) , {}, {1, 0, 0}, glm::vec2(1, 1)},
            {glm::vec3( text_height*text_ratio/2, 1.1 + text_height, 0) , {}, {1, 0, 0}, glm::vec2(1, 0)},
            {glm::vec3(-text_height*text_ratio/2, 1.1 + text_height, 0), {}, {1, 0, 0}, glm::vec2(0, 0)},
        };
        return TEXT_VERTICES;
    }
    const glm::vec3 File::FILE_COLOR = {1, 215.f / 255.f, 119.f / 255.f};
    const std::vector<Vertex> File::VERTICES = {
        {glm::vec3(0.0f,  0.0f,  0.001f), glm::vec3(0, 0, 1), FILE_COLOR, glm::vec2(0.0f, 0.0f)}, //center
        {glm::vec3(-1.0f, 1.0f,  0.001f), glm::vec3(0, 0, 1), FILE_COLOR, glm::vec2(0.0f, 1.0f)}, //top-left corner
        {glm::vec3(-0.4f, 1.0f,  0.001f), glm::vec3(0, 0, 1), FILE_COLOR, glm::vec2(0.3f, 1.0f)}, //end of the sticking out stub
        {glm::vec3(-0.2f, 0.8f,  0.001f), glm::vec3(0, 0, 1), FILE_COLOR, glm::vec2(0.4f, 0.9f)}, //downslope of sticking out stub
        {glm::vec3(1.0f,  0.8f,  0.001f), glm::vec3(0, 0, 1), FILE_COLOR, glm::vec2(1.0f, 0.9f)}, //top-right corner
        {glm::vec3(1.0f,  -1.0f, 0.001f), glm::vec3(0, 0, 1), FILE_COLOR, glm::vec2(1.0f, 0.0f)}, //bottom-right
        {glm::vec3(-1.0f, -1.0f, 0.001f), glm::vec3(0, 0, 1), FILE_COLOR, glm::vec2(0.0f, 0.0f)}, //bottom-left

        
        {glm::vec3(0.0f,  0.0f, -0.001f), glm::vec3(0, 0,-1), FILE_COLOR, glm::vec2(0.0f, 0.0f)},  //center
        {glm::vec3(-1.0f, 1.0f, -0.001f), glm::vec3(0, 0,-1), FILE_COLOR, glm::vec2(0.0f, 1.0f)},  //top-left corner
        {glm::vec3(-0.4f, 1.0f, -0.001f), glm::vec3(0, 0,-1), FILE_COLOR, glm::vec2(0.3f, 1.0f)},  //end of the sticking out stub
        {glm::vec3(-0.2f, 0.8f, -0.001f), glm::vec3(0, 0,-1), FILE_COLOR, glm::vec2(0.4f, 0.9f)},  //downslope of sticking out stub
        {glm::vec3(1.0f,  0.8f, -0.001f), glm::vec3(0, 0,-1), FILE_COLOR, glm::vec2(1.0f, 0.9f)},  //top-right corner
        {glm::vec3(1.0f,  -1.0f,-0.001f), glm::vec3(0, 0,-1), FILE_COLOR, glm::vec2(1.0f, 0.0f)}, //bottom-right
        {glm::vec3(-1.0f, -1.0f,-0.001f), glm::vec3(0, 0,-1), FILE_COLOR, glm::vec2(0.0f, 0.0f)}, //bottom-left
    };
    const std::vector<GLuint> File::INDICES = {
            0, 1, 2,
            0, 2, 3,
            0, 3, 4,
            0, 4, 5,
            0, 5, 6,
            0, 6, 1,
            
            7, 8, 9,
            7, 9, 10,
            7, 10, 11,
            7, 11, 12,
            7, 12, 13,
            7, 13, 8,
    };
    const std::vector<GLuint> File::TEXT_INDICES = {
        0, 1, 2,
        2, 3, 0
    };
File::File(glm::vec3 pos, glm::vec3 scale, glm::mat4 rot, FontRenderer &fr,
     std::filesystem::path path_)
    // : text_mesh({}, {}, {}), main_mesh(VERTICES, INDICES, {}), myPath(path) 
    : position(pos), scale(scale), rotation(rot), text_mesh({}, {}, {}), main_mesh(VERTICES, INDICES, {}), path(path_) 
{
    auto home = get_homedir();
    auto curpath = path.string();
    if(curpath.find(home) != std::string::npos) {
        curpath.erase(curpath.find(home), home.size());
    }
    auto [tex, ratio] = genTextTexture(fr, curpath, vec3(1, 1, 1));
    auto verts = genTextVerticies(ratio);
    text_mesh = Mesh(verts, TEXT_INDICES, {tex});
}
void File::draw(Shader& folder_shader, Shader& text_shader, Camera& camera) {
    glm::mat4 Mf = glm::mat4(1);
    
    Mf = glm::translate(Mf, position);
    Mf = Mf * rotation;
    Mf = glm::scale(Mf, scale);
    
    auto Mt = Mf;
    Mt = glm::scale(Mt, glm::vec3(1,-1,1));
    
    auto proj = glm::vec3(camera.position.x, position.y, camera.position.z);
    Mt = Mt * glm::lookAt(glm::vec3(0, 0, 0), proj - position, -vec3(0, 1, 0));
    
    folder_shader.setUniformMatrix4fv("M", Mf);
    main_mesh.draw(folder_shader, camera);

    text_shader.setUniformMatrix4fv("M", Mt);
    text_mesh.draw(text_shader, camera);
}
