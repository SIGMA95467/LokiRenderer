#pragma once
#include "color.h"
#include "RenderState.h"
#include <glm/glm.hpp>
#include "shader.h"


class Vertex;
class IShader;
class VSOutput;


class Rasterizer
{
public:
				Rasterizer(RenderContext* pcontext);
				~Rasterizer();



				void DrawPixel(int x, int y, Color& c);
				//void DrawPixel(int x, int y, unsigned int& c);
				void viewportTransform(Vertex* vertexs);
				void BoundBox(Vertex* vertexs, glm::vec2& bboxmin, glm::vec2& bboxmax);

	
				bool is_inside_triangle(glm::vec3 barycentricCoord);
				void DrawWireFrame(glm::vec4* vertexs, Color& color);
				//void DrawWireFrame(const std::vector<glm::vec3>& vertexs, Color& color);

				void DrawTriangle(VSOutput* vert0, VSOutput* vert1, VSOutput* vert2, IShader* shader);
				void DrawLine(int x1, int y1, int x2, int y2, Color &color);

	

				//glm::vec3 CalcBarycentric(glm::vec3& a, glm::vec3& b, glm::vec3& c, glm::vec3& p);
				glm::vec3 CalcBarycentric(glm::vec4& a, glm::vec4& b, glm::vec4& c, glm::vec4& p);

private:
				RenderContext* mRenderContext;
};

