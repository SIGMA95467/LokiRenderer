#include "mesh.h"
Mesh::Mesh()
{
    Vertices.clear();
    UVs.clear();
    Normals.clear();
    Triangles.clear();
    Colors.clear();
    Tangents.clear();
    Tangents.clear();
    biTangents.clear();

    
}

Mesh::~Mesh()
{

}

void Mesh::describeMesh() {
    for (int i = 0; i < Vertices.size() / 3; ++i) {
        printf("Vertex  %2.1d: %f, %f, %f \n", i, Vertices[i].x, Vertices[i].y, Vertices[i].z);
    }
    printf("Meshsize is: %d \n", Vertices.size() / 3);
}

void Mesh::buildFacetNormals() {
    for (int i = 0; i < Vertices.size() / 3; ++i) {    
        glm::vec3 N1 = Vertices[3 * i + 1] - Vertices[3 * i + 0];
        glm::vec3 N2 = Vertices[3 * i + 2] - Vertices[3 * i + 0];
        FaceNormals.push_back(glm::normalize(glm::cross(N2,N1)));
    }
}
