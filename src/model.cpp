#include "model.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>
unsigned int texture_from_file(const char* path, const std::string& directory, bool gamma = false);

void model_::draw(shader_ shader){
    for(unsigned int i = 0; i < meshes_.size(); i++)
        meshes_[i].draw(shader);
}

void model_::load_model(std::string path){
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
    }

    directory_ = path.substr(0, path.find_last_of('/'));

    process_node(scene->mRootNode, scene);
}

void model_::process_node(aiNode *node, const aiScene* scene){
    for(unsigned int i = 0; i < node->mNumMeshes; i++){
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes_.push_back(process_mesh(mesh, scene));
    }
    for(unsigned int i = 0; i < node->mNumChildren; i++)
        process_node(node->mChildren[i], scene);
}

mesh_ model_::process_mesh(aiMesh *mesh, const aiScene* scene){
    std::vector<vertex_> vertices;
    std::vector<unsigned int> indices;
    std::vector<texture_> textures;

    // handling vertices
    for(unsigned int i = 0; i < mesh->mNumVertices; i++){
        vertex_ vertex;
        glm::vec3 vector;
        // vertex position
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.position_ = vector;
        // normal
        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.normal_ = vector;
        // texture coord
        if(mesh->mTextureCoords[0]){
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.tex_coords_ = vec;
        }
        else
            vertex.tex_coords_ = glm::vec2(0.0f, 0.0f);
        vertices.push_back(vertex);
    }
    // handling indices
    for(unsigned int i = 0; i < mesh->mNumFaces; i++){
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    // handling materials
    if(mesh->mMaterialIndex > 0){
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<texture_> diffuse_maps = load_material_textures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());
        std::vector<texture_> specular_maps = load_material_textures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());
    }

    return mesh_(vertices, indices, textures);
}

std::vector<texture_> model_::load_material_textures(aiMaterial* material, aiTextureType type, std::string type_name){
    std::vector<texture_> textures;
    for(unsigned int i = 0; i < material->GetTextureCount(type); i++){
        aiString str;
        material->GetTexture(type, i, &str);
        bool skip = false;
        for(unsigned int j = 0; j < textures_loaded_.size(); j++){
            if(std::strcmp(textures_loaded_[j].path_.data(), str.C_Str()) == 0){
                textures.push_back(textures_loaded_[j]);
                skip = true;
                break;
            }
        }
        if(!skip){
            texture_ texture;
            texture.id_ = texture_from_file(str.C_Str(), directory_);
            texture.type_ = type_name;
            texture.path_ = str.C_Str();
            textures.push_back(texture);
            textures_loaded_.push_back(texture);
        }
    }
    return textures;
}

unsigned int texture_from_file(const char* path, const std::string& directory, bool gamma){
    std::string filename = std::string(path);
    filename = directory + '/' + filename;

    unsigned int texture_id;
    glGenTextures(1, &texture_id);

    int width, height, nr_channels;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nr_channels, 0);
    if(data){
        GLenum format;
        if(nr_channels == 1)
            format = GL_RED;
        else if(nr_channels == 3)
            format = GL_RGB;
        else if(nr_channels == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, texture_id);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
        std::cout << "Texture failed to load at path: " << path << std::endl;
    stbi_image_free(data);
    return texture_id;
}