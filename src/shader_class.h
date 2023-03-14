#pragma once
#include <glad/glad.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class shader_{
public:
    unsigned int shader_id_;

    shader_(const char* vertex_shader_path, const char* fragment_shader_path);

    void use();

    void set_bool(const std::string& name, bool value) const;
    void set_int(const std::string& name, int value) const;
    void set_float(const std::string& name, float value) const;
    void set_mat4(const std::string& name, glm::mat4 value) const;
    void set_vec3(const std::string& name, float value_x, float value_y, float value_z) const;
    void set_vec3(const std::string& name, glm::vec3 value) const;
    void delete_shader() const;
};