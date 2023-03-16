#include "mesh.h"

mesh_::mesh_(std::vector<vertex_>& vertices, std::vector<unsigned int>& indices, std::vector<texture_>& textures){
    vertices_ = vertices;
    indices_ = indices;
    textures_ = textures;

    setup_mesh();
}

void mesh_::setup_mesh(){
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glGenBuffers(1, &ebo_);

    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);

    glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(vertex_), &vertices_[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(unsigned int), &indices_[0], GL_STATIC_DRAW);

    // vertices
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_), (void*)0);
    // normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_), (void*)offsetof(vertex_, normal_));
    // texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_), (void*)offsetof(vertex_, tex_coords_));

    glBindVertexArray(0);
}

void mesh_::draw(shader_ shader){
    unsigned int diffuse_nr = 1, specular_nr = 1;
    for(unsigned int i = 0; i < textures_.size(); i++){
        glActiveTexture(GL_TEXTURE0 + i);
        std::string number;
        std::string name = textures_[i].type_;
        if(name == "texture_diffuse")
            number = std::to_string(diffuse_nr++);
        else if(name == "texture_specular")
            number = std::to_string(specular_nr++);
        shader.set_int("material." + name + number, i);
        glBindTexture(GL_TEXTURE_2D, textures_[i].id_);
    }
    glActiveTexture(GL_TEXTURE0);

    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}