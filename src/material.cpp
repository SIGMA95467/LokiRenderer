#include "material.h"
#include "shader.h"

//“∞÷∏’Î?
Material::Material(std::string name)
{
    Name = name;
    DiffuseTex = nullptr;
    NormalTex = nullptr;
    OcclusionTex = nullptr;
    EmissionTex = nullptr;
    AmbientTex = nullptr;
    RoughnessTex = nullptr;
    MetalnessTex = nullptr;
    OcclusionTex = nullptr;
};

//TODO: samart pointer?
Material:: ~Material() {
    if (DiffuseTex)
    {
        delete DiffuseTex;
    }
    if (NormalTex)
    {
        delete NormalTex;
    }
    if (OcclusionTex)
    {
        delete OcclusionTex;
    }
    if (EmissionTex)
    {
        delete EmissionTex;
    }
    if (AmbientTex)
    {
        delete AmbientTex;
    }
    if (RoughnessTex)
    {
        delete RoughnessTex;
    }
    if (MetalnessTex)
    {
        delete MetalnessTex;
    }
    //if (Shader)
    //{
    //    delete Shader;
    //}
};