#pragma once
#include <glm/glm.hpp>

#include "color.h"
#include "mesh.h"
#include "texture.h"
#include <cmath>
#include "mathUtils.h"
#include "vertex.h"
#include "light.h"

class IShader
{
public:
    //light
    glm::vec3 LightColor;
    glm::vec3 ka, kd ,ks, ke;



    //glm::vec3* DirectionLightDir;
    //float* DirectionLightIntensity;

    //Light* light;
    

    glm::vec3* WorldSpaceViewPos;

    glm::mat4* ViewMatrix;
    glm::mat4* ModelMatrix;
    glm::mat4* ProjectionMatrix;
    glm::mat4* PVMMatrix;
    glm::mat4* ViewportMatrix;
    glm::mat4* ModelViewMatrix;
    glm::mat4* NormalMatrix;
    //glm::mat4* World2ObjectMatrix;
    

    Mesh* RenderMesh;
    Texture* diffuseT, * ambientT, * normalT, *emissionT, *occlusionT;

    Color* FragmentInColor;
    glm::vec2* FragmentInUV;

    //Vector3** FragmentVaringIns;
    //glm::vec3* Fragment_payload;

    //glm::vec3* lightDirVal, * lightCol, * lightPos;

    glm::vec3 WorldPos;
    glm::vec3 WorldNorlmal;
    glm::vec3 WorldTexcoord;
    glm::vec3 WorldTangent;
    glm::vec3 WorldbiTangent;
    glm::vec3 ScreenPos;
    glm::mat3 TBN;

    //Interpolated variables
    glm::vec3 interpLightDir, interpNormal,
        interpViewDir, interpCol;
    glm::vec2 interpUV;
    float interpAO;
    
    //Per vertex data
    glm::vec3 normals[3], viewDir[3];
    glm::vec3 varying_diffuse, varying_specular, ReflectDir, LightDir;


    glm::vec3 ambient, diffuse, specular;
    glm::vec3 halfwayDir;

    
    //IShader() 
    //{


    //}
    //virtual ~IShader() {
    //    delete[] Fragment_payload;
    //};
    virtual glm::vec4 vertex(int bufferIndex, int triangleIndex, VSOutput* vsOutput) = 0;
    virtual void fragment(const glm::vec3& barycentric, Color& color) = 0;
};

class FlatShader : public IShader
{
    float varIntensity;
    virtual glm::vec4 vertex(int bufferIndex, int triangleIndex, VSOutput* vsOutput)
    {
        //RenderMesh->w
        
        glm::vec3 meshVertex = RenderMesh->Vertices[bufferIndex];
        glm::vec4 tmp(meshVertex, 1.0f);

        //glm::vec3(0, 4, 7)        
        glm::vec4 tmplight = *ViewMatrix * glm::vec4(1, 1, 1, 1);
        LightDir = glm::vec3(tmplight.x, tmplight.y, tmplight.y);
        //glm::vec3 meshFaceNormal = RenderMesh->FaceNormals[triangleIndex];
        //glm::vec4 tmpFaceNormal = *ModelViewMatrix* glm::vec4(meshFaceNormal, 0.0f);
        //glm::vec3 WorldFaceNormal(tmpFaceNormal.x, tmpFaceNormal.y, tmpFaceNormal.z);
        //varIntensity = std::max(0.0f, glm::dot(WorldFaceNormal, -LightDir));

        varIntensity = std::max(0.0f, glm::dot(RenderMesh->FaceNormals[triangleIndex], LightDir));

        return *PVMMatrix * tmp; // transform it to screen coordinates
    }

    virtual void fragment(const glm::vec3& barycentric, Color& color)
    {
        color = Color::white * varIntensity;
    }
};



class PhongShader : public IShader
{
    virtual glm::vec4 vertex(int bufferIndex, int triangleIndex, VSOutput* vsOutput)
    {
        glm::vec3 meshVertex = RenderMesh->Vertices[bufferIndex];
        
        //法线
        glm::vec3 meshNormal = glm::normalize(RenderMesh->Normals[bufferIndex]);       
        glm::vec4 worldNormal = *NormalMatrix * glm::vec4(meshNormal, 0.0f);
        //顶点坐标*VP矩阵 局部空间 -> 观察空间
        glm::vec4 worldPos = *ModelViewMatrix * glm::vec4(meshVertex, 0.0f);
        glm::vec3 worldView = *WorldSpaceViewPos - glm::vec3(worldPos.x, worldPos.y, worldPos.z);
        
        vsOutput->worldView = worldView;
        vsOutput->worldNormal = glm::vec3(worldNormal.x, worldNormal.y, worldNormal.z);

        //LightDir = glm::normalize(glm::vec3(0, 1, -1));
        

        glm::vec4 tmp(meshVertex, 1.0f);

        return *PVMMatrix * tmp; // MVP
    }

    virtual void fragment(const glm::vec3& barycentric, Color& color)
    {
        LightDir = interpViewDir;
        ka = glm::vec3(0.01f, 0.01f, 0.01f);
        kd = glm::vec3(0.8f, 0.8f, 0.8f);
        ks = glm::vec3(0.5f, 0.5f, 0.5f);

  

        glm::vec3 rgb(1, 1, 1);
        glm::vec3 light_ambient_intensity = kd;
        glm::vec3 light_diffuse_intensity(0.9, 0.9, 0.9);
        glm::vec3 light_specular_intensity(0.15, 0.15, 0.15);
        //glm::vec3 light_specular_intensity(0.8, 0.8, 0.8);

        //Ambient 
        ambient = light_ambient_intensity * ka;

        //Diffuse
        diffuse = light_diffuse_intensity * kd * std::max(glm::dot(interpNormal, LightDir), 0.0f);

        //Specular
        ReflectDir = glm::reflect(-LightDir, interpNormal);
        float spec = std::pow(std::max((glm::dot(interpViewDir, ReflectDir)), 0.0f), 150.0f);
        specular = light_specular_intensity * ks * spec;

        glm::vec3 col = (ambient + diffuse + specular) * rgb;
        col.x = mathUtils::Clamp(col.x, 0, 1);
        col.y = mathUtils::Clamp(col.y, 0, 1);
        col.z = mathUtils::Clamp(col.z, 0, 1);

        color.r = col.x;
        color.g = col.y;
        color.b = col.z;
        color.a = 1;
    }
};


class TextureMapShader : public IShader {
    virtual glm::vec4 vertex(int bufferIndex, int triangleIndex, VSOutput* vsOutput) {
        glm::vec3 meshVertex = RenderMesh->Vertices[bufferIndex];


        //法线变换
        glm::vec3 meshNormal = glm::normalize(RenderMesh->Normals[bufferIndex]);
        glm::vec4 worldNormal = *NormalMatrix * glm::vec4(meshNormal, 0.0f);
        //顶点坐标*VP矩阵 局部空间 -> 观察空间
        glm::vec4 worldPos = *ModelViewMatrix * glm::vec4(meshVertex, 0.0f);
        glm::vec3 worldView = *WorldSpaceViewPos - glm::vec3(worldPos.x, worldPos.y, worldPos.z);

        vsOutput->worldView = worldView;
        vsOutput->worldNormal = glm::vec3(worldNormal.x, worldNormal.y, worldNormal.z);

        //LightDir = glm::normalize(glm::vec3(0, 1, -1));


        glm::vec4 tmp(meshVertex, 1.0f);

        return *PVMMatrix * tmp; // MVP
    }

    virtual void fragment(const glm::vec3& barycentric, Color& color)
    {
        
        LightDir = interpViewDir;

        interpNormal = normalT ? normalT->SampleColor(interpUV.x, interpUV.y) : interpNormal;
        interpAO = occlusionT ? occlusionT->SampleIntensity(interpUV.x, interpUV.y) : 1.0f;
        //ka = ambientT ? ambientT->SampleColor(interpUV.x, interpUV.y) : glm::vec3(0.01f);
        kd = diffuseT ? diffuseT->SampleColor(interpUV.x, interpUV.y) : glm::vec3(0.8f);
        ka = glm::vec3(0.8f);
        ks = glm::vec3(0.5f, 0.5f, 0.5f);
        ke = emissionT ? emissionT->SampleColor(interpUV.x, interpUV.y) : glm::vec3(0.0f);
        

        glm::vec3 white(1, 1, 1);
        glm::vec3 light_ambient_intensity = kd;
        glm::vec3 light_diffuse_intensity(0.9, 0.9, 0.9);
        glm::vec3 light_specular_intensity(0.15, 0.15, 0.15);
        //glm::vec3 light_specular_intensity(0.8, 0.8, 0.8);

        //Ambient 
        ambient = light_ambient_intensity * ka * interpAO;

        //Diffuse
        diffuse = light_diffuse_intensity * kd * std::max(glm::dot(interpNormal, LightDir), 0.0f);
        //diffuse =  kd * std::max(glm::dot(interpNormal, LightDir), 0.0f);

        //Specular
        ReflectDir = glm::reflect(-LightDir, interpNormal);
        float spec = std::pow(std::max((glm::dot(interpViewDir, ReflectDir)), 0.0f), 5.0f);
        specular = light_specular_intensity * ks * spec ;

        //glm::vec3 col = (ke + ambient + diffuse) + specular * white;

       glm::vec3 col = kd;
   
 
        col.x = mathUtils::Clamp(col.x, 0, 1);
        col.y = mathUtils::Clamp(col.y, 0, 1);
        col.z = mathUtils::Clamp(col.z, 0, 1);

        color.r = col.x;
        color.g = col.y;
        color.b = col.z;
        color.a = 1;
    }
};