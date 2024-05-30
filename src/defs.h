#ifndef FD_DEFS
#define FD_DEFS

#define FD_ASSET_DIR FD_PROJECT_DIR"/assets"
#define FD_TEXTURE_DIR FD_PROJECT_DIR"/assets/textures"
#define FD_SHADER_DIR FD_PROJECT_DIR"/assets/shaders"

#include "glad/glad.h"
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/rotate_vector.hpp"
#include <glm/gtx/vector_angle.hpp>

#include <GLFW/glfw3.h>
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <fstream>
#include <sstream>
#include <string>
inline static std::string dumpStringFromFile(std::string filename) {
    std::ifstream f(filename); //taking file as inputstream
    std::string str;
    if(f) {
        std::ostringstream ss;
        ss << f.rdbuf(); // reading data
        str = ss.str();
    }
    return str;
}
#include "stb_image.h"
#endif
