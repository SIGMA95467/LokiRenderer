#pragma once
#include <glm/glm.hpp>
#include <vector>

//Struct containing information relevant to the renderer about the vertices, normals and
//texture coordinates of a model. Also keeps track of useful stuff for iterating.
class Mesh {
public:
    Mesh();
    //Mesh(const Mesh &mesh);
    ~Mesh();
    int numVertices;
    std::vector<glm::vec3> Vertices;
    std::vector<glm::vec3> Normals;
    std::vector<glm::vec2> UVs;
    std::vector<glm::vec4> Colors;
    std::vector<glm::vec3> Tangents;
    std::vector<glm::vec3> biTangents;
    std::vector<int> Triangles;

    //Per face values
    int numFaces;
    std::vector<glm::vec3> FaceNormals; //Normals for the whole face
    std::vector<glm::ivec3> VertexIndices;
    std::vector<glm::ivec3> TextureIndices;
    std::vector<glm::ivec3> NormalsIndices;

    //Simple mesh description for debugging.
    void describeMesh();

    //Builds facet normals used in early back face culling
    void buildFacetNormals();

    //Builds tangent and bitangent vectors for normal mapping
    void buildTangentSpace();
};
