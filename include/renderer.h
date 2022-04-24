#pragma once
#include "rasterizer.h"
//#include "TypeDef.h"
#include "vertex.h"
#include <glm/glm.hpp>
#include "camera.h"
#include "shader.h"
#include "material.h"
#include "mesh.h"
#include <assert.h>

struct TrianglePrimitive
{
    glm::vec4 clipSpacePoses[3];
    glm::vec4 ndcSpacePoses[3];
    Vertex screenTriangleVerts[3];
};

enum clippingplanes
{
    cp_left = 0,			///< Left frustum clipping plane.
    cp_right,			///< Right frustum clipping plane.
    cp_top,				///< Top frustum clipping plane.
    cp_bottom,			///< Bottom frustum clipping plane.
    cp_near,				///< Near frustum clipping plane.
    cp_far,				///< Far frustum clipping plane.

    cp_numplanes
};


class Renderer
{
public:
    Renderer(int width, int height, const char* title);
    ~Renderer();

    void RenderMesh(Mesh* mesh, glm::mat4* modelMatrix, IShader* shader, Material* material = nullptr, Light* light = nullptr);
    //void ClipTriangle(Mesh* mesh, Matrix4x4* modelMatrix, Material* mat, int triangleIndex, float zNear, float zFar);



    int GetWindowWidth();
    int GetWindowHeight();
    void ClearBuffer(Color *ClearColor);
    void Draw();
    void SetCurCamera(Camera* cam);

    // void SetRenderState();
    bool SimpleClip(glm::vec4* clipSpaceVertices);

    float* GetDepthBuffer();

    //void ViewportTransformVertex(VSOutput* io_pVSOutput);

    int numBackfaceCull;
private:
    RenderContext* mRenderContext;
    IShader* mShader;
    Rasterizer* mRasterizer;
    Camera* mCamera;
    Light* light;

};

