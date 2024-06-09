#ifndef FD_DEFS
#define FD_DEFS

#define FD_ASSET_DIR FD_PROJECT_DIR"/assets"
#define FD_MODEL_DIR FD_PROJECT_DIR"/assets/models"
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
static inline glm::vec3 findClosestPointOnRay(glm::vec3 ray_origin, glm::vec3 ray_dir, glm::vec3 point) {
    float ray_dir_len = length(ray_dir);
    glm::vec3 ray_dir_normal = ray_dir / ray_dir_len;
    float proj = dot(point - ray_origin, ray_dir_normal);
    if (proj <= 0)
        return ray_origin;
    // if (proj >= ray_dir_len)
    //     return ray_origin + ray_dir;
    return ray_dir_normal * proj + ray_origin;
}
#include "stb_image.h"
#endif
