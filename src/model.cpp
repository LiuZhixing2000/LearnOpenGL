#include "model.h"
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