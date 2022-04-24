#include "model.h"
#include "mesh.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>


Model::Model(std::string filePath)
{
				int index = filePath.find_last_of("/");
				directory = filePath.substr(0, index + 1);
				LoadModel(filePath);
}

Model::~Model()
{
				for (int i = 0; i < mMeshes.size(); i++)
				{
								delete mMeshes[i];
				}

				for (int i = 0; i < mMaterials.size(); i++)
				{
								delete mMaterials[i];
				}
				mMeshes.clear();
				mMaterials.clear();
}



std::vector<Mesh*>& Model::GetMeshes()
{
				return mMeshes;
}

std::vector<Material*>& Model::GetMaterials()
{
				return mMaterials;
}

void Model::LoadModel(std::string path)
{
				tinyobj::attrib_t attrib;


				std::vector<tinyobj::shape_t> shapes;
				std::vector<tinyobj::material_t> materials;

				std::string warn;
				std::string err;
				bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str(), directory.c_str());
				if (!warn.empty()) {
								std::cout << "WARN: " << warn << std::endl;
				}
				if (!err.empty()) {
								std::cerr << err << std::endl;
				}

				if (!ret) {
								std::cerr << "Failed to load " << path << std::endl;
								return;
				}




				printf("normals   = %d\n", (int)(attrib.normals.size()) / 3);
				printf("texcoords = %d\n", (int)(attrib.texcoords.size()) / 2);
				printf("materials = %d\n", (int)materials.size());
				printf("shapes    = %d\n", (int)shapes.size());

				// Append `default` material
				materials.push_back(tinyobj::material_t());

				//for (size_t i = 0; i < materials.size(); i++) {
				for (auto material : materials) {
								//printf("material[%d].diffuse_texname = %s\n", int(i),
								//				materials[i].diffuse_texname.c_str());
								std::cout << material.diffuse_texname << material.normal_texname << material.ambient_texname << material.emissive_texname 
												<< material.specular_texname << std::endl;
				}


				for (int i = 0; i < materials.size() - 1; i++)
				{
								
								tinyobj::material_t *material = &materials[i];
								if (material) {
												Material* pMat = new Material(material->name);
												if (material->diffuse_texname.length() > 0) 
												{
																pMat->DiffuseTex = new Texture(directory + material->diffuse_texname);
												}
												if (material->normal_texname.length() > 0)
												{
																pMat->NormalTex = new Texture(directory + material->normal_texname);
												}
												if (material->ambient_texname.length() > 0)
												{
																pMat->AmbientTex = new Texture(directory + material->ambient_texname);
												}
												if (material->emissive_texname.length() > 0)
												{
																pMat->EmissionTex = new Texture(directory + material->emissive_texname);
												}
												if (material->specular_texname.length() > 0)
												{
																pMat->specularTex = new Texture(directory + material->specular_texname);
												}
																
												//TODO: read mtl
												//std::string OcclusionTex_texname = "helmet_emission.tga";
												//pMat->OcclusionTex = new Texture(directory + OcclusionTex_texname);

												if (pMat) mMaterials.push_back(pMat);
								}
				}

//--------------------------------------------------------------------mesh------------------------------------------------

			//for (size_t i = 0; i < shapes.size(); i++)
				for (auto shape : shapes)
				{
								/*ProcessMesh(shapes[i].mesh, attrib);*/

								//tinyobj::mesh_t& mesh = shape.mesh;
								Mesh* pmesh = new Mesh();
								
								printf("indices size: %d  triangles: %d\n", (int)(shape.mesh.indices.size()), (int)shape.mesh.indices.size() / 3);
								//pmesh->numVertices = shape.mesh.indices.size() / 3;
								//for ( auto idx : shape.mesh.indices)
								for (size_t f = 0; f < shape.mesh.indices.size() / 3; f++)
								{
												
												tinyobj::index_t idx0 = shape.mesh.indices[3 * f + 0];
												tinyobj::index_t idx1 = shape.mesh.indices[3 * f + 1];
												tinyobj::index_t idx2 = shape.mesh.indices[3 * f + 2];

												
												pmesh->Vertices.reserve(attrib.vertices.size() / 3);
												pmesh->Normals.reserve(attrib.normals.size() / 3);
												pmesh->UVs.reserve(attrib.texcoords.size() / 3);
												pmesh->Triangles.reserve(shape.mesh.indices.size() / 3);
												//memcpy(positions.data(), mesh->positions.data(), sizeof(Vector3) * positions.size());

												bool hasUV = attrib.texcoords.size() > 0 ? true : false;

												// Walk through each of the mesh's vertices
												{
																if (attrib.vertices.size() > 0) {

																				int f0 = idx0.vertex_index;
																				int f1 = idx1.vertex_index;
																				int f2 = idx2.vertex_index;
																				assert(f0 >= 0);
																				assert(f1 >= 0);
																				assert(f2 >= 0);
																
																				for (auto f : { f0, f1, f2 })
																				{
																								pmesh->Vertices.push_back(glm::vec3(attrib.vertices[3 * f], attrib.vertices[3 * f + 1], attrib.vertices[3 * f + 2]));
																				}
																}

																if (attrib.normals.size() > 0) {
																				int nf0 = idx0.normal_index;
																				int nf1 = idx1.normal_index;
																				int nf2 = idx2.normal_index;
																				assert(nf0 >= 0);
																				assert(nf1 >= 0);
																				assert(nf2 >= 0);
																				for (auto nf : { nf0, nf1, nf2 }) {
																								pmesh->Normals.push_back(glm::vec3(attrib.normals[3 * nf + 0], attrib.normals[3 * nf + 1], attrib.normals[3 * nf + 2]));
																				}

																}

																if (attrib.texcoords.size() > 0) {
																				int tf0 = idx0.texcoord_index;
																				int tf1 = idx1.texcoord_index;
																				int tf2 = idx2.texcoord_index;
																				assert(tf0 >= 0);
																				assert(tf0 >= 0);
																				assert(tf0 >= 0);
																				for (auto tf : { tf0, tf1, tf2 }) {
																								glm::vec2 tmpTexcoord(0, 0);
																								pmesh->UVs.push_back(glm::vec2(attrib.texcoords[2 * tf], 1 - attrib.texcoords[2 * tf + 1]));
																				}

																}														
																
												}
								}
								// Copying vector by copy function
								//for (auto i : shape.mesh.indices) pmesh->Triangles.emplace_back(i.vertex_index);
								//copy(mesh.indices.begin(), mesh.indices.end(), back_inserter(pmesh->Triangles));
								pmesh->buildFacetNormals();
								mMeshes.push_back(pmesh);
								//std::cout << "ProcessMesh:" << shape.name;

								//mMeshMatMap.insert(std::make_pair(pmesh, mMaterials[shape.mesh.material_ids[0]]));
				}


}

std::map<Mesh*, Material*>& Model::GetMeshMatMap()
{
				return mMeshMatMap;
				
}
