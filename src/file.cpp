#include "file.h"

namespace fs = std::filesystem;
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
    std::pair<Texture, float> File::genTextTexture(FontRenderer& fr, std::string str, int width, int height, int font_height) {
        constexpr float SCALE = 1.f;
        constexpr float FONT_HEIGHT = 32.f * SCALE;
        constexpr float FONT_WIDTH = 32.f * SCALE;

        if(width == 0) {
            width = fr.getPixelWidth(str, FONT_HEIGHT);
        }
        if(height == 0) {
            height = FONT_HEIGHT;
            font_height = FONT_HEIGHT;
        }
        // int width = FONT_WIDTH * str.size();
        double font_ratio = width / FONT_HEIGHT ;
        return {fr.generate(str, width, height, 1, font_height), font_ratio};
    }
    std::vector<Vertex> File::genTextVerticies(float text_ratio, float text_height) {
        const std::vector<Vertex> TEXT_VERTICES = {
            {glm::vec3(-text_height*text_ratio/2, 1.2, 0), {}, {1, 0, 0}, glm::vec2(0, 1)},
            {glm::vec3( text_height*text_ratio/2, 1.2, 0) , {}, {1, 0, 0}, glm::vec2(1, 1)},
            {glm::vec3( text_height*text_ratio/2, 1.2 + text_height, 0) , {}, {1, 0, 0}, glm::vec2(1, 0)},
            {glm::vec3(-text_height*text_ratio/2, 1.2 + text_height, 0), {}, {1, 0, 0}, glm::vec2(0, 0)},
        };
        return TEXT_VERTICES;
    }
    const glm::vec3 File::TXT_COLOR = {0.8, 0.8, 0.8};
constexpr float txt_ratio = 1.5f;
    const std::vector<Vertex> File::VERTICES_TXT = {
        {glm::vec3(-1.0f, 1.0f,              0.01f), glm::vec3(0, 0, 1), TXT_COLOR, glm::vec2(0.0f, 0.0f)}, //top-left corner
        {glm::vec3(1.0f,  1.0f,              0.01f), glm::vec3(0, 0, 1), TXT_COLOR, glm::vec2(1.0f, 0.0f)},
        {glm::vec3(1.0f,  -1.0f * txt_ratio, 0.01f), glm::vec3(0, 0, 1), TXT_COLOR, glm::vec2(1.0f, 1.0f)},
        {glm::vec3(-1.0f, -1.0f * txt_ratio, 0.01f), glm::vec3(0, 0, 1), TXT_COLOR, glm::vec2(0.0f, 1.0f)},

        {glm::vec3(-1.0f, 1.0f,             -0.01f), glm::vec3(0, 0,-1), TXT_COLOR, glm::vec2(1.0f, 0.0f)}, //top-left corner
        {glm::vec3(1.0f,  1.0f,             -0.01f), glm::vec3(0, 0,-1), TXT_COLOR, glm::vec2(0.0f, 0.0f)},
        {glm::vec3(1.0f,  -1.0f * txt_ratio,-0.01f), glm::vec3(0, 0,-1), TXT_COLOR, glm::vec2(0.0f, 1.0f)},
        {glm::vec3(-1.0f, -1.0f * txt_ratio,-0.01f), glm::vec3(0, 0,-1), TXT_COLOR, glm::vec2(1.0f, 1.0f)},
    };
    const std::vector<GLuint> File::INDICES_TXT = {
        0, 1, 2,
        0, 2, 3,
        4, 5, 6,
        4, 6, 7
    };
    const std::vector<Vertex> File::VERTICES_IMG = {
        {glm::vec3(-1.0f, 1.0f,  0.01f), glm::vec3(0, 0, 1), {1, 0, 1}, glm::vec2(0.0f, 1.0f)}, //top-left corner
        {glm::vec3(1.0f,  1.0f,  0.01f), glm::vec3(0, 0, 1), {1, 0, 1}, glm::vec2(1.0f, 1.0f)},
        {glm::vec3(1.0f,  -1.0f, 0.01f), glm::vec3(0, 0, 1), {1, 0, 1}, glm::vec2(1.0f, 0.0f)},
        {glm::vec3(-1.0f, -1.0f, 0.01f), glm::vec3(0, 0, 1), {1, 0, 1}, glm::vec2(0.0f, 0.0f)},

        {glm::vec3(-1.0f, 1.0f, -0.01f), glm::vec3(0, 0,-1), {1, 0, 1}, glm::vec2(0.0f, 1.0f)}, //top-left corner
        {glm::vec3(1.0f,  1.0f, -0.01f), glm::vec3(0, 0,-1), {1, 0, 1}, glm::vec2(1.0f, 1.0f)},
        {glm::vec3(1.0f,  -1.0f,-0.01f), glm::vec3(0, 0,-1), {1, 0, 1}, glm::vec2(1.0f, 0.0f)},
        {glm::vec3(-1.0f, -1.0f,-0.01f), glm::vec3(0, 0,-1), {1, 0, 1}, glm::vec2(0.0f, 0.0f)},
    };
    const std::vector<GLuint> File::INDICES_IMG = {
        0, 1, 2,
        0, 2, 3,
        4, 5, 6,
        4, 6, 7
    };
    const glm::vec3 File::LINK_COLOR = {0.0, 0.8, 0.0};
    const std::vector<Vertex> File::VERTICES_LINK = {
        {glm::vec3(-1.0f, 0.3f,  0.01f), glm::vec3(0, 0, 1), LINK_COLOR, glm::vec2(0.0f, 1.0f)}, //top-left corner
        {glm::vec3(-0.1f,  0.3f,  0.01f), glm::vec3(0, 0, 1), LINK_COLOR, glm::vec2(1.0f, 1.0f)},
        {glm::vec3(-0.1f,  -0.3f, 0.01f), glm::vec3(0, 0, 1), LINK_COLOR, glm::vec2(1.0f, 0.0f)},
        {glm::vec3(-1.0f, -0.3f, 0.01f), glm::vec3(0, 0, 1), LINK_COLOR, glm::vec2(0.0f, 0.0f)},
        {glm::vec3(-0.1f,  -1.0f, 0.01f), glm::vec3(0, 0, 1), LINK_COLOR, glm::vec2(1.0f, 0.0f)},
        {glm::vec3(-0.1f,  1.0f,  0.01f), glm::vec3(0, 0, 1), LINK_COLOR, glm::vec2(1.0f, 0.0f)},
        {glm::vec3(1.0f,  0.0f,  0.01f), glm::vec3(0, 0, 1), LINK_COLOR, glm::vec2(1.0f, 0.0f)},

        {glm::vec3(-1.0f, 0.3f, -0.01f), glm::vec3(0, 0,-1), LINK_COLOR, glm::vec2(0.0f, 1.0f)}, //top-left corner
        {glm::vec3(-0.1f,  0.3f, -0.01f), glm::vec3(0, 0,-1), LINK_COLOR, glm::vec2(1.0f, 1.0f)},
        {glm::vec3(-0.1f,  -0.3f,-0.01f), glm::vec3(0, 0,-1), LINK_COLOR, glm::vec2(1.0f, 0.0f)},
        {glm::vec3(-1.0f, -0.3f,-0.01f), glm::vec3(0, 0,-1), LINK_COLOR, glm::vec2(0.0f, 0.0f)},
        {glm::vec3(-0.1f,  -1.0f,-0.01f), glm::vec3(0, 0,-1), LINK_COLOR, glm::vec2(1.0f, 0.0f)},
        {glm::vec3(-0.1f,  1.0f, -0.01f), glm::vec3(0, 0,-1), LINK_COLOR, glm::vec2(1.0f, 0.0f)},
        {glm::vec3(1.0f,  0.0f, -0.01f), glm::vec3(0, 0,-1), LINK_COLOR, glm::vec2(1.0f, 0.0f)},
    };
    const std::vector<GLuint> File::INDICES_LINK = {
        0, 1, 2,
        0, 2, 3,
        4, 5, 6,
        7, 8, 9,
        7, 9, 10,
        11, 12, 13,
    };
    const glm::vec3 File::FILE_COLOR = {1, 215.f / 255.f, 119.f / 255.f};
    const std::vector<Vertex> File::VERTICES_DIR = {
        {glm::vec3(0.0f,  0.0f,  0.01f), glm::vec3(0, 0, 1), FILE_COLOR, glm::vec2(0.0f, 0.0f)}, //center
        {glm::vec3(-1.0f, 1.0f,  0.01f), glm::vec3(0, 0, 1), FILE_COLOR, glm::vec2(0.0f, 1.0f)}, //top-left corner
        {glm::vec3(-0.4f, 1.0f,  0.01f), glm::vec3(0, 0, 1), FILE_COLOR, glm::vec2(0.3f, 1.0f)}, //end of the sticking out stub
        {glm::vec3(-0.2f, 0.8f,  0.01f), glm::vec3(0, 0, 1), FILE_COLOR, glm::vec2(0.4f, 0.9f)}, //downslope of sticking out stub
        {glm::vec3(1.0f,  0.8f,  0.01f), glm::vec3(0, 0, 1), FILE_COLOR, glm::vec2(1.0f, 0.9f)}, //top-right corner
        {glm::vec3(1.0f,  -1.0f, 0.01f), glm::vec3(0, 0, 1), FILE_COLOR, glm::vec2(1.0f, 0.0f)}, //bottom-right
        {glm::vec3(-1.0f, -1.0f, 0.01f), glm::vec3(0, 0, 1), FILE_COLOR, glm::vec2(0.0f, 0.0f)}, //bottom-left

        
        {glm::vec3(0.0f,  0.0f, -0.01f), glm::vec3(0, 0,-1), FILE_COLOR, glm::vec2(0.0f, 0.0f)},  //center
        {glm::vec3(-1.0f, 1.0f, -0.01f), glm::vec3(0, 0,-1), FILE_COLOR, glm::vec2(0.0f, 1.0f)},  //top-left corner
        {glm::vec3(-0.4f, 1.0f, -0.01f), glm::vec3(0, 0,-1), FILE_COLOR, glm::vec2(0.3f, 1.0f)},  //end of the sticking out stub
        {glm::vec3(-0.2f, 0.8f, -0.01f), glm::vec3(0, 0,-1), FILE_COLOR, glm::vec2(0.4f, 0.9f)},  //downslope of sticking out stub
        {glm::vec3(1.0f,  0.8f, -0.01f), glm::vec3(0, 0,-1), FILE_COLOR, glm::vec2(1.0f, 0.9f)},  //top-right corner
        {glm::vec3(1.0f,  -1.0f,-0.01f), glm::vec3(0, 0,-1), FILE_COLOR, glm::vec2(1.0f, 0.0f)}, //bottom-right
        {glm::vec3(-1.0f, -1.0f,-0.01f), glm::vec3(0, 0,-1), FILE_COLOR, glm::vec2(0.0f, 0.0f)}, //bottom-left
    };
    const std::vector<GLuint> File::INDICES_DIR = {
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
constexpr int maxSize = 30;
File::File(glm::vec3 pos, glm::vec3 scale, glm::mat4 rot, FontRenderer &fr, fs::path path)
    // : text_mesh({}, {}, {}), main_mesh(VERTICES, INDICES, {}), myPath(path) 
    : my_path(path), position(pos), scale(scale), rotation(rot), text_mesh({}, {}, {}), main_mesh({}, {}, {}) 
{
    std::string path_str = path.string();
    std::string ext = path_str.substr(path_str.find_last_of(".") + 1);
    std::transform(ext.begin(), ext.end(), ext.begin(),
        [](unsigned char c){ return std::tolower(c); });

    auto is_contained = [](std::string str, std::vector<std::string> vec) {
        for(auto& s : vec)
            if(s == str)  return true;
        return false;
    };

    if(fs::is_directory(path)) {
        main_mesh = Mesh(VERTICES_DIR, INDICES_DIR, {});
        type = FileType::Dir;
    }else if(is_contained(ext, {"png", "jpg", "jpeg", "tga", "bmp", "psd", "gif", "hdr", "pic"})){
        main_mesh = Mesh(VERTICES_IMG, INDICES_IMG, {Texture(path_str.c_str(), "diffuse", 1)});
        type = FileType::Image;
    }else if(fs::is_symlink(path)) {
        main_mesh = Mesh(VERTICES_LINK, INDICES_LINK, {});
        type = FileType::Link;
    }else if(is_contained(ext, {"txt", "json", "glsl", "cpp", "hpp", "h", "c", "log", "cmake"})){
        auto contents = dumpStringFromFile(path);
        int s = 256;
        int w = 16;
        contents = contents.substr(0, (s / w) * (s/w));
        auto tex = fr.generate(contents, s, s * txt_ratio, 2, w); 
        main_mesh = Mesh(VERTICES_TXT, INDICES_TXT, {tex});
        type = FileType::Readable;
    }else {
        main_mesh = Mesh(VERTICES_TXT, INDICES_TXT, {});
        type = FileType::Other;
    }
    //text
    if(path_str.size() > maxSize) {
        path_str = "..." + path_str.substr(path_str.size() - maxSize - 3);
    }
    auto [tex, ratio] = genTextTexture(fr, path_str);
    auto verts = genTextVerticies(ratio);
    text_mesh = Mesh(verts, TEXT_INDICES, {tex});
    tex.bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    tex.unbind();
}
void File::decomission() {
    for(auto& t : main_mesh.textures) {
        t.remove();
    }
    for(auto& t : text_mesh.textures) {
        t.remove();
    }
}
void File::draw(Shader& folder_shader, Shader& aura_shader, Shader& text_shader, Camera& camera) {
    if(!isActive)
        return;
    glm::mat4 Mf = glm::mat4(1);
    
    Mf = glm::translate(Mf, position);
    auto Mt = Mf;
    Mt = glm::scale(Mt, scale);
    Mt = glm::scale(Mt, glm::vec3(1,-1,1));

    Mf = Mf * rotation;
    Mf = glm::scale(Mf, scale);
    
    
    auto proj = glm::vec3(camera.position.x, position.y, camera.position.z);
    Mt = Mt * glm::lookAt(glm::vec3(0, 0, 0), proj - position, -vec3(0, 1, 0));
    

    glm::vec4 aura_color(0, 0, 0, 0);
    if(selected) {
        aura_color = glm::vec4(1, 1, 0, 1);
    }
    if(awaiting_deletion) {
        aura_color = glm::vec4(1, 0, 0, 1);
    }
    if(aura_color != glm::vec4(0, 0, 0, 0)) {
        auto MfA = glm::scale(Mf, vec3(1.2, 1.2, 1.2)); 
        aura_shader.setUniformMatrix4fv("M", MfA);
        aura_shader.setUniform1f("backward", 0.1f);
        aura_shader.setUniform4f("uColor", aura_color);
        main_mesh.draw(aura_shader, camera);
        aura_shader.setUniform1f("backward", 0.f);
        aura_shader.setUniform4f("uColor", glm::vec4(0, 0, 0, 0));
    }

    //if has textures, render them
    Shader* shader_used;
    switch(type) {
        case FileType::Readable:
            text_shader.setUniform4f("uColorBg", glm::vec4(1, 1, 1, 1));
            text_shader.setUniform4f("uColorFg", glm::vec4(0, 0, 0, 1));
            text_shader.setUniformMatrix4fv("M", Mf);
            shader_used = &text_shader;
            main_mesh.draw(text_shader, camera);
            break;
        case FileType::Image:
            folder_shader.setUniform1f("useTexture", 1);
            shader_used = &folder_shader;
            break;
        case FileType::Dir:
        case FileType::Link:
        case FileType::Other:
            {
                folder_shader.setUniform1f("useTexture", 0);
            }
            break;
    }
    folder_shader.setUniformMatrix4fv("M", Mf);
    main_mesh.draw(folder_shader, camera);

    text_shader.setUniformMatrix4fv("M", Mt);
    text_shader.setUniform4f("uColorFg", glm::vec4(1, 1, 1, 1));
    text_shader.setUniform4f("uColorBg", glm::vec4(0, 0, 0, 1));
    text_mesh.draw(text_shader, camera);
}
