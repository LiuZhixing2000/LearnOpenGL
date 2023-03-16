#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>
#include "shader_class.h"

struct vertex_{
    glm::vec3 position_;
    glm::vec3 normal_;
    glm::vec2 tex_coords_;
};

struct texture_{
    unsigned int id_;
    std::string type_;
};

class mesh_{
public:
    std::vector<vertex_> vertices_;
    std::vector<unsigned int> indices_;
    std::vector<texture_> textures_;

    mesh_(std::vector<vertex_>& vertices, std::vector<unsigned int>& indices, std::vector<texture_>& textures);

    void draw(shader_ shader);
private:
    void setup_mesh();
    unsigned int vao_, vbo_, ebo_;
};