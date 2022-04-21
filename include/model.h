#pragma once
#include "RenderState.h"
#include "material.h"
#include "glm/glm.hpp"

#include <vector>
#include <string>
#include <map>




class Mesh;


class Model
{
public:
				Model(std::string filePath);
				~Model();
				std::vector<Mesh*>& GetMeshes();
				std::vector<Material*>& GetMaterials();
				std::map<Mesh*, Material*>& GetMeshMatMap();

private:
				std::vector<Mesh*> mMeshes;
				std::vector<Material*> mMaterials;
				std::string directory;

				void LoadModel(std::string path);
				//void ProcessMesh(const tinyobj::mesh_t& mesh, const tinyobj::attrib_t &attrib);
				//void ProcessMaterial(tinyobj::material_t* material);

				std::map<Mesh*, Material*> mMeshMatMap;

};