#include "renderer.h"
#include "window.h"



Renderer::Renderer(int width, int height, const char* title)
{

				/*window_init(width, height, title);*/

				mRenderContext = new RenderContext();
				mRenderContext->width = width;
				mRenderContext->height = height;
				mRenderContext->bpp = sizeof(unsigned int);

				mRenderContext->frameBuffer = new unsigned int[width * height];
				mRenderContext->depthBuffer = new float[width * height];
				memset(mRenderContext->frameBuffer, 0x0, mRenderContext->width * mRenderContext->height * mRenderContext->bpp);
				memset(mRenderContext->depthBuffer, 0x0, mRenderContext->width * mRenderContext->height * mRenderContext->bpp);

				mRasterizer = new Rasterizer(mRenderContext);
}

Renderer::~Renderer()
{
				delete mRenderContext;
				delete mRasterizer;
				delete[] mRenderContext->frameBuffer;
				delete[] mRenderContext->depthBuffer;
}

void Renderer::SetCurCamera(Camera* cam)
{
				mCamera = cam;
}


float* Renderer::GetDepthBuffer()
{
				return mRenderContext->depthBuffer;
}


void Renderer::RenderMesh(Mesh* mesh, glm::mat4* modelMatrix, IShader* shader, Material* material, Light* light)
{
				if (light) {
								shader->light = light;
				}
				else {
								//set shader matrix
								glm::mat4 MVP = mCamera->projectionMaxtrix * mCamera->viewMatrix * (*modelMatrix);
								shader->PVMMatrix = &MVP;

								shader->ViewMatrix = &(mCamera->viewMatrix);
								shader->ProjectionMatrix = &(mCamera->projectionMaxtrix);
								shader->ViewportMatrix = &(mCamera->viewportMaxtrix);

								shader->ModelMatrix = modelMatrix;
								//glm::mat4 World2Object = glm::inverse(*modelMatrix);
								glm::mat4 normal_matrix = glm::transpose(glm::inverse(*modelMatrix));
								shader->NormalMatrix = &normal_matrix;

								glm::mat4 MV = mCamera->viewMatrix * (*modelMatrix);
								shader->ModelViewMatrix = &MV;

								shader->WorldSpaceViewPos = &(mCamera->position);

								//set shader material
								if (material) {
												shader->specularT = material->specularTex;
												shader->ambientT = material->AmbientTex;
												shader->diffuseT = material->DiffuseTex;
												shader->emissionT = material->EmissionTex;
												shader->normalT = material->NormalTex;
												shader->occlusionT = material->OcclusionTex;
								}

								shader->RenderMesh = mesh;

								mShader = shader;
				}
	


				

				glm::vec4 clipSpaceCoord[3];
				glm::vec4 ndcCoord[3];
				glm::vec4 screenCoord[3];
				//Vertex screenCoord[3];
				//Vertex clipedTriangleVerts[9];

				int w = GetWindowWidth();
				int h = GetWindowHeight();

				numBackfaceCull = 0;
				//for (int i = 0; i < mesh->Triangles.size(); i++)
				for (int i = 0; i < mesh->Vertices.size() / 3; i++)
				{
								VSOutput vsOutputs[3];
								//int vertexInZCount = 0;
								
								for (int j = 0; j < 3; j++)
								{
												int vertexIndex = i * 3 + j;

												//tianglePrimitive.clipSpacePoses[j] = mat->Shader->vertex(mesh->Triangles[i*3 +j], j);
												//vsOutputs[j].position = shader->vertex(mesh->Triangles[i * 3 + j], j, vsOutputs + j);
												
												//顶点着色器 MVP变换
												glm::vec4 a = shader->vertex(vertexIndex, i, vsOutputs + j);
												vsOutputs[j].position = a;
												//vsOutputs[j].position = shader->vertex(vertexIndex, j, vsOutputs + j);
												//vsOutputs[j].color.r = mesh->Colors[vertexIndex].x;
												//vsOutputs[j].color.g = mesh->Colors[vertexIndex].y;
												//vsOutputs[j].color.b = mesh->Colors[vertexIndex].z;
												//vsOutputs[j].color.a = mesh->Colors[vertexIndex].w;
												if(mesh->UVs.size())
												vsOutputs[j].uv = mesh->UVs[vertexIndex];

												clipSpaceCoord[j] = vsOutputs[j].position;
												// clip space -> NDC 透视除法
												ndcCoord[j] = clipSpaceCoord[j] / clipSpaceCoord[j].w;

												
												//Adding half a pixel to avoid gaps on small vertex values
												screenCoord[j].x = 0.5f * w * (1 + ndcCoord[j].x) + 0.5;
												screenCoord[j].y = 0.5f * h * (1 + ndcCoord[j].y) + 0.5;
												//裁剪空间的w值
												screenCoord[j].w = clipSpaceCoord[j].w;

												
								}
								
								
								//clip							
								if (SimpleClip(clipSpaceCoord)) continue;


								//背面剔除
								//glm::vec4 u = ndcCoord[1] - ndcCoord[0];
								//glm::vec4 v = ndcCoord[2] - ndcCoord[0];
								////float z = (u.x * v.y) - (u.y * v.x);
								//float z = (v.x * u.y) - (v.y * u.x);
								//if (z > 0) {
								//				numBackfaceCull++;
								//				continue;
								//}
								for(int k = 0; k < 3; ++k) 
												vsOutputs[k].position = screenCoord[k];

								mRasterizer->DrawTriangle(&vsOutputs[0], &vsOutputs[1], &vsOutputs[2], shader);


//#ifdef WIRE_FRAME
//								glm::vec3 verts[]{ vertexs[0].position, vertexs[1].position, vertexs[2].position };
//								Rasterizer::DrawWireFrame(verts, Color::white);
//#endif // WIRE_FRAME
								
								
				}
}









//inline void Renderer::ViewportTransformVertex(VSOutput* pVSOutput)
//{
//				if (pVSOutput->position.w < FLT_EPSILON)
//								return;
//
//				const float fInvW = 1.0f / pVSOutput->position.w;
//				pVSOutput->position.x *= fInvW;
//				pVSOutput->position.y *= fInvW;
//				pVSOutput->position.z *= fInvW;
//				pVSOutput->position.w = 1.0f;
//
//				pVSOutput->position = mCamera->viewportMaxtrix * pVSOutput->position;
//
//				// divide shader output registers by w; this way we can interpolate them linearly while rasterizing ...
//				pVSOutput->position.w = fInvW;
//
//				pVSOutput->color *= fInvW;
//				pVSOutput->uv *= fInvW;
//				pVSOutput->normal *= fInvW;
//
//				if (mShader->VaryingsCountBitMask & FirstBitMask)
//				{
//								pVSOutput->varying[0] *= fInvW;
//				}
//
//				if (mShader->VaryingsCountBitMask & SecondBitMask)
//				{
//								pVSOutput->varying[1] *= fInvW;
//				}
//}



//void Renderer::DrawPoint(int x, int y, Color& c)
//{
//				mRasterizer->DrawPixel(x, y, c);
//}
//
////Left-Bottom as start corner
//void Renderer::DrawPointLB(int x, int y, Color& c)
//{
//				mRasterizer->DrawPixel(x, mRenderContext->height - y - 1, c);
//}

int Renderer::GetWindowWidth()
{
				return mRenderContext->width;
}

int Renderer::GetWindowHeight()
{
				return mRenderContext->height;
}

void Renderer::ClearBuffer(Color* color)
{
				unsigned int col = (color->GetUintA() << 24) | (color->GetUintR() << 16) | (color->GetUintG() << 8) | (color->GetUintB() << 0);
				memset(mRenderContext->frameBuffer, col, mRenderContext->width * mRenderContext->height * mRenderContext->bpp);
				memset(mRenderContext->depthBuffer, -1, mRenderContext->width * mRenderContext->height * mRenderContext->bpp);
}

void Renderer::Draw()
{
				window_draw((unsigned char*)mRenderContext->frameBuffer);
}


bool Renderer::SimpleClip(glm::vec4* clipSpaceCoord) {
				int count = 0;
				for (int i = 0; i < 3; ++i) {
								glm::vec4 vertex = clipSpaceCoord[i];
								bool inside = (-vertex.w <= vertex.x <= vertex.w)
												&& (-vertex.w <= vertex.y <= vertex.w)
												&& (0 <= vertex.z <= vertex.w);
								if (!inside) ++count;
				}
				//If count equals three it means every vertex was out so we skip it
				return count == 3;
}