#include "mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class model_{
public:
    model_(char *path){
        load_model(path);
    }
    void draw(shader_ shader);
private:
    std::vector<mesh_> meshes_;
    std::string directory_;
    void load_model(std::string path);
    void process_node(aiNode *node, const aiScene* scene);
    mesh_ process_mesh(aiMesh *mesh, const aiScene* scene);
    std::vector<texture_> load_material_textures(aiMaterial* material, aiTextureType type, std::string type_name);
};