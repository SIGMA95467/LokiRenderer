#pragma once
#include <string>  
#include "Texture.h"
class IShader;

class Material {
public:
    Material(std::string name);

    virtual ~Material();

    //private:
    std::string Name;
    int RenderQueue;
    Texture* DiffuseTex;
    Texture* NormalTex;
    //IShader* Shader;
    
    Texture* OcclusionTex;
    Texture* EmissionTex;
    Texture* AmbientTex;
    Texture* RoughnessTex;
    Texture* MetalnessTex;
    Texture* specularTex;
};


