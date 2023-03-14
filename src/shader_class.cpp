#include "shader_class.h"

shader_::shader_(const char* vertex_shader_path, const char* fragment_shader_path){
    // read shader source code
    std::string vertex_shader_code, fragment_shader_code;
    std::ifstream vertex_shader_file, fragment_shader_file;
    std::stringstream vertex_shader_stream, fragment_shader_stream;
    vertex_shader_file.open(vertex_shader_path);
    fragment_shader_file.open(fragment_shader_path);
    vertex_shader_stream << vertex_shader_file.rdbuf();
    fragment_shader_stream << fragment_shader_file.rdbuf();
    vertex_shader_file.close();
    fragment_shader_file.close();
    vertex_shader_code = vertex_shader_stream.str();
    fragment_shader_code = fragment_shader_stream.str();
    const char* vertex_shader_source, *fragment_shader_source;
    vertex_shader_source = vertex_shader_code.c_str();
    fragment_shader_source = fragment_shader_code.c_str();

    // shaders compilation
    unsigned int vertex_shader, fragment_shader;
    int success;
    char info_log[512];

    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
    glCompileShader(vertex_shader);
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info_log << std::endl;
    }

    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
    glCompileShader(fragment_shader);
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << info_log << std::endl;
    }

    // shaders link
    shader_id_ = glCreateProgram();
    glAttachShader(shader_id_, vertex_shader);
    glAttachShader(shader_id_, fragment_shader);
    glLinkProgram(shader_id_);
    glGetProgramiv(shader_id_, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(shader_id_, 512, NULL, info_log);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << info_log << std::endl;
    }

    // delete shaders
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

void shader_::use(){
    glUseProgram(shader_id_);
}

void shader_::set_bool(const std::string& name, bool value) const{
    auto location = glGetUniformLocation(shader_id_, name.c_str());
    glUniform1i(location, value);
}

void shader_::set_int(const std::string& name, int value) const{
    auto location = glGetUniformLocation(shader_id_, name.c_str());
    glUniform1i(location, value);
}

void shader_::set_float(const std::string& name, float value) const{
    auto location = glGetUniformLocation(shader_id_, name.c_str());
    glUniform1f(location, value);
}

void shader_::delete_shader() const{
    glDeleteProgram(shader_id_);
}

void shader_::set_mat4(const std::string& name, glm::mat4 value) const{
    auto location = glGetUniformLocation(shader_id_, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void shader_::set_vec3(const std::string& name, float value_x, float value_y, float value_z) const{
    auto location = glGetUniformLocation(shader_id_, name.c_str());
    glUniform3f(location, value_x, value_y, value_z);
}

void shader_::set_vec3(const std::string& name, glm::vec3 value) const{
    auto location = glGetUniformLocation(shader_id_, name.c_str());
    glUniform3f(location, value.x, value.y, value.z);
}