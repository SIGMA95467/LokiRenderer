#include "rasterizer.h"

//#define WIRE_FRAME
#define SHADE

Rasterizer::Rasterizer(RenderContext* pcontext)
{
				mRenderContext = pcontext;
}

Rasterizer::~Rasterizer()
{

}

//void Rasterizer::DrawTriangle(Vertex* vertexs, IShader* shader)
//{
//				glm::vec2 bboxmin(mathUtils::Infinity, mathUtils::Infinity);
//				glm::vec2 bboxmax(mathUtils::NegativeInfinity, mathUtils::NegativeInfinity);
//				
//
//				//viewportTransform(vertexs);
//				BoundBox(vertexs, bboxmin, bboxmax);
//
//
//				glm::ivec2 P;
//				Color color;
//				for (P.x = bboxmin.x; P.x <= bboxmax.x; P.x++) {
//								for (P.y = bboxmin.y; P.y <= bboxmax.y; P.y++)
//								{					
//
//
//												glm::vec3 p(P.x, P.y, 0);
//												glm::vec3 barycentricCoord = CalcBarycentric(vertexs[0].position, vertexs[1].position, vertexs[2].position, p);
//												float fInvW = 1.0f / (barycentricCoord.x * vertexs[0].w + barycentricCoord.y * vertexs[1].w + barycentricCoord.z * vertexs[2].w);
//												float depth = barycentricCoord.x * vertexs[0].w + barycentricCoord.y * vertexs[0].w + barycentricCoord.z * vertexs[0].w;
//												//depth *= fInvW;
//
//												if (is_inside_triangle(barycentricCoord)) {
//																//深度测试
//																if (mRenderContext->depthBuffer[(P.x + P.y * mRenderContext->width)] < depth) continue;
//																//if (depth <= mRenderContext->depthBuffer[P.x + P.y * mRenderContext->width]) {
//																				//顶点属性插值
//																				glm::vec2 interpUV(0, 0);
//																				interpUV.x = barycentricCoord.x * vertexs[0].u + barycentricCoord.y * vertexs[1].u + barycentricCoord.z * vertexs[2].u;
//																				interpUV.y = barycentricCoord.x * vertexs[0].v + barycentricCoord.y * vertexs[1].v + barycentricCoord.z * vertexs[2].v;
//																				Color interpCol = fInvW * (barycentricCoord.x * vertexs[0].color + barycentricCoord.y * vertexs[0].color + barycentricCoord.z * vertexs[0].color);
//																				shader->FragmentInColor = &interpCol;
//																				shader->FragmentInUV = &interpUV;
//																				
//
//
//
//#ifdef WIRE_FRAME
//																				glm::vec3 verts[]{ vertexs[0].position, vertexs[1].position, vertexs[2].position };
//																				DrawWireFrame(verts, Color::white);
//#endif // WIRE_FRAME
//
//#ifdef SHADE
//																				shader->fragment(barycentricCoord, color);
//																				DrawPixel(P.x, P.y, color);
//#endif // SHADE
//																				mRenderContext->depthBuffer[P.x + P.y * mRenderContext->width] = depth;
//
//												}
//
//
//								}
//				}
//}


void Rasterizer::DrawTriangle(VSOutput *vert0, VSOutput *vert1, VSOutput *vert2, IShader* shader)
{

				glm::vec2 bboxmin(mathUtils::Infinity, mathUtils::Infinity);
				glm::vec2 bboxmax(mathUtils::NegativeInfinity, mathUtils::NegativeInfinity);
				glm::vec2 clamp(mRenderContext->width - 1, mRenderContext->height - 1);

				//Screen space clip by bounding box
				bboxmin.x = mathUtils::Max(0.f, mathUtils::Min(bboxmin.x, vert0->position.x));
				bboxmin.y = mathUtils::Max(0.f, mathUtils::Min(bboxmin.y, vert0->position.y));

				bboxmax.x = mathUtils::Min(clamp.x, mathUtils::Max(bboxmax.x, vert0->position.x));
				bboxmax.y = mathUtils::Min(clamp.y, mathUtils::Max(bboxmax.y, vert0->position.y));

				bboxmin.x = mathUtils::Max(0.f, mathUtils::Min(bboxmin.x, vert1->position.x));
				bboxmin.y = mathUtils::Max(0.f, mathUtils::Min(bboxmin.y, vert1->position.y));

				bboxmax.x = mathUtils::Min(clamp.x, mathUtils::Max(bboxmax.x, vert1->position.x));
				bboxmax.y = mathUtils::Min(clamp.y, mathUtils::Max(bboxmax.y, vert1->position.y));

				bboxmin.x = mathUtils::Max(0.f, mathUtils::Min(bboxmin.x, vert2->position.x));
				bboxmin.y = mathUtils::Max(0.f, mathUtils::Min(bboxmin.y, vert2->position.y));

				bboxmax.x = mathUtils::Min(clamp.x, mathUtils::Max(bboxmax.x, vert2->position.x));
				bboxmax.y = mathUtils::Min(clamp.y, mathUtils::Max(bboxmax.y, vert2->position.y));

				glm::ivec2 P;
				Color color;


				//float area = edgeFunction(vertexs[0].position, vertexs[1].position, vertexs[2].position); 
				bool isInline = false;
				for (P.x = bboxmin.x; P.x <= bboxmax.x; P.x++) {
								for (P.y = bboxmin.y; P.y <= bboxmax.y; P.y++)
								{
												glm::vec4 currentPoint(P.x, P.y, 0, 0);

												glm::vec3 barycentricCoord = CalcBarycentric(vert0->position, vert1->position, vert2->position, currentPoint);
												
												float Z = 1.0f / (barycentricCoord.x / vert0->position.w + barycentricCoord.y / vert1->position.w + barycentricCoord.z / vert2->position.w);
												
												//float depth = barycentricCoord.x * vert0->position.w + barycentricCoord.y * vert1->position.w + barycentricCoord.z * vert2->position.w;
												//float depth = Z * (barycentricCoord.x * vert0->position.w / vert0->position.w + barycentricCoord.y * vert1->position.w / vert1->position.w + barycentricCoord.z * vert2->position.w / vert2->position.w);
												float depth = Z * (barycentricCoord.x + barycentricCoord.y + barycentricCoord.z);


												if (!is_inside_triangle(barycentricCoord)) continue;


												

												//float depth = (barycentricCoord.x * vert0->position.z, barycentricCoord.y*vert1->position.z, barycentricCoord.z * vert2->position.z);

												//深度测试
												if (mRenderContext->depthBuffer[P.x + P.y * mRenderContext->width] < depth) continue;

												//透视正确顶点属性插值
												glm::vec2 interpUV(0, 0);
												interpUV =  Z * (barycentricCoord.x * vert0->uv / vert0->position.w + barycentricCoord.y * vert1->uv / vert1->position.w + barycentricCoord.z * vert2->uv / vert2->position.w);
												
												//Color interpCol = Z * (barycentricCoord.x * vert0->color + barycentricCoord.y * vert1->color + barycentricCoord.z * vert2->color);
												//shader->interpCol = &interpCol;
												shader->interpUV = interpUV;

												shader->interpViewDir = glm::normalize(Z * (barycentricCoord.x * vert0->worldView / vert0->position.w
																+ barycentricCoord.y * vert1->worldView / vert1->position.w
																+ barycentricCoord.z * vert2->worldView / vert2->position.w));

												shader->interpNormal = glm::normalize(Z * (barycentricCoord.x * vert0->worldNormal / vert0->position.w
																+ barycentricCoord.y * vert1->worldNormal / vert1->position.w
																+ barycentricCoord.z * vert2->worldNormal / vert2->position.w));






#ifdef SHADE
												shader->fragment(barycentricCoord, color);
												DrawPixel(P.x, P.y, color);
#endif // SHADE
#ifdef WIRE_FRAME
												glm::vec4 verts[]{ vert0->position, vert1->position, vert2->position };
												DrawWireFrame(verts, Color::white);
#endif // WIRE_FRAME

												mRenderContext->depthBuffer[P.x + P.y * mRenderContext->width] = depth;

								}
				}



}


void Rasterizer::DrawWireFrame(glm::vec4* vertexs, Color& color) {
				DrawLine(vertexs->x, vertexs->y, (vertexs + 1)->x, (vertexs + 1)->y, color);
				DrawLine((vertexs + 1)->x, (vertexs + 1)->y, (vertexs + 2)->x, (vertexs + 2)->y, color);
				DrawLine(vertexs->x, vertexs->y, (vertexs + 2)->x, (vertexs + 2)->y, color);

}




void Rasterizer::DrawLine(int x1, int y1, int x2, int y2, Color& color) {
				//transpose line if it is too steep
				bool steep = false;
				if (std::abs(x1 - x2) < std::abs(y1 - y2)) {
								std::swap(x1, y1);
								std::swap(x2, y2);
								steep = true;
				}

				//Redefine line so that it is left to right
				if (x1 > x2) {
								std::swap(x1, x2);
								std::swap(y1, y2);
				}

				//Redefined to use only int arithmetic
				int dx = x2 - x1;
				int dy = y2 - y1;
				int derror2 = std::abs(dy) * 2;
				int error2 = 0;
				int y = y1;

				for (int x = x1; x <= x2; x++) {
								if (steep) {
												DrawPixel(y, x, color); //Swap back because of steep transpose
								}
								else {
												DrawPixel(x, y, color);
								}
								error2 += derror2;
								if (error2 > dx) {
												y += (y2 > y1 ? 1 : -1);
												error2 -= dx * 2;
								}
				}
}


void Rasterizer::DrawPixel(int x, int y, Color& c)
{
				//左下角开始
				int coord = x + (mRenderContext->height - y - 1) * mRenderContext->width;
				if(coord < mRenderContext->width * mRenderContext->height && coord >= 0)
								mRenderContext->frameBuffer[coord] = (c.GetUintA() << 24) | (c.GetUintR() << 16) | (c.GetUintG() << 8) | (c.GetUintB() << 0);
}



glm::vec3 Rasterizer::CalcBarycentric(glm::vec4& a, glm::vec4& b, glm::vec4& c, glm::vec4& p)
{
				glm::vec3 v0 = b - a, v1 = c - a, v2 = p - a;

				float d00 = v0.x * v0.x + v0.y * v0.y;
				float d01 = v0.x * v1.x + v0.y * v1.y;
				float d11 = v1.x * v1.x + v1.y * v1.y;
				float d20 = v2.x * v0.x + v2.y * v0.y;
				float d21 = v2.x * v1.x + v2.y * v1.y;

				float denom = d00 * d11 - d01 * d01;



				float v = (d11 * d20 - d01 * d21) / denom;
				float w = (d00 * d21 - d01 * d20) / denom;
				float u = 1.0f - v - w;

				return glm::vec3(u, v, w);

				//float c1 = (p.x * (b.y - c.y) + (c.x - b.x) * p.y + b.x * c.y - c.x * b.y) / (a.x * (b.y - c.y) + (c.x - b.x) * a.y + b.x * c.y - c.x * b.y);
				//float c2 = (p.y * (c.y - a.y) + (a.x - c.x) * p.y + c.x * a.y - a.x * c.y) / (b.x * (c.y - a.y) + (a.x - c.x) * b.y + c.x * a.y - a.x * c.y);
				//return glm::vec3(c1, c2, 1 - c1 - c2);
}

bool Rasterizer::is_inside_triangle(glm::vec3 barycentricCoord)
{
				bool flag = false;
				// here epsilon is to alleviate precision bug
				if (barycentricCoord.x > -0.00001f && barycentricCoord.y > -0.00001f && barycentricCoord.z > -0.00001f)
								flag = true;

				return flag;
}

void Rasterizer::viewportTransform(Vertex* vertexs) {
				for (int i = 0; i < 3; i++) {
								//Adding half a pixel to avoid gaps on small vertex values
								//NDC -> screen space
								(vertexs + i)->position.x = (((vertexs + i)->position.x + 1) * mRenderContext->width * 0.5) + 0.5;
								(vertexs + i)->position.y = (((vertexs + i)->position.y + 1) * mRenderContext->height * 0.5) + 0.5;
				}
}

void Rasterizer::BoundBox(Vertex* vertexs, glm::vec2& bboxmin, glm::vec2& bboxmax) {
				glm::vec2 clamp(mRenderContext->width - 1, mRenderContext->height - 1);

				for (int i = 0; i < 3; i++)
				{
								bboxmin.x = mathUtils::Max(0.f, mathUtils::Min(bboxmin.x, (vertexs + i)->position.x));
								bboxmin.y = mathUtils::Max(0.f, mathUtils::Min(bboxmin.y, (vertexs + i)->position.y));

								bboxmax.x = mathUtils::Min(clamp.x, mathUtils::Max(bboxmax.x, (vertexs + i)->position.x));
								bboxmax.y = mathUtils::Min(clamp.y, mathUtils::Max(bboxmax.y, (vertexs + i)->position.y));
				}
				
}