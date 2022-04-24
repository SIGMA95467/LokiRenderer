
#include "window.h"
//#include "renderer.h"
#include <iostream>
#include <cmath>
#include "renderer.h"
#include "shader.h"
#include "model.h"
#include "color.h"
#include <vector>
#include "light.h"


static const int windowWidth = 960;
static const int windowHeight = 540;

//TODO: encapsulation 
int main(int argc, char** argv)
{
				window_init(windowWidth, windowHeight, "LokiRenderer");
				unsigned char* framebuffer = nullptr;

				const std::string base_dir = "../../../../assets/";
				std::string obj_dir;
				obj_dir = "helmet/helmet.obj";		
				//obj_dir = "cube/cube.obj";
				//obj_dir = "floor/floor.obj";
				//obj_dir = "sphere/sphere.obj";
				//obj_dir = "teapot/teapot.obj";
				//obj_dir = "Miku/Miku.obj";
				//obj_dir = "diablo3_pose/diablo3_pose.obj";
				//obj_dir = "african_head/african_head.obj";

				std::string path = base_dir + obj_dir;
				if (argc > 1) {
								path = std::string(argv[1]);
				}
			 Model* model = new Model(path);

				//IShader* shader = new FlatShader();		
				IShader* shader = new PhongShader();
				//IShader* shader = new TextureMapShader();
				//IShader* shadowshader = new ShadowShader();
				

				Camera* camera = new Camera(glm::vec3(1, 1, 2), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
				//Camera* camera = new Camera(glm::vec3(1, 1, 2), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
				camera->SetPespective(60, 16.0 / 9, 0.3, 2000);


				glm::mat4 modelMatrix;

				modelMatrix[0][0] = 1.0f; modelMatrix[0][1] = 0.0f;     modelMatrix[0][2] = 0.0f; modelMatrix[0][3] = 0.0f;
				modelMatrix[1][0] = 0.0f;	  modelMatrix[1][1] = 1.0f; modelMatrix[1][2] = 0.0f; modelMatrix[1][3] = 0.0f;
				modelMatrix[2][0] = 0.0f;   modelMatrix[2][1] = 0.0f;     modelMatrix[2][2] = 1.0f; modelMatrix[2][3] = 0.0f;
				modelMatrix[3][0] = 0.0f; modelMatrix[3][1] = 0.0f;  modelMatrix[3][2] = 0.0f; modelMatrix[3][3] = 1.0f;

				Renderer* renderer = new Renderer(windowWidth, windowHeight, "LokiRenderer");
				renderer->SetCurCamera(camera);

				//Light *light = new Light(glm::vec3(1, 1, 2), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
				
				Color ClearColor(0.212, 0.212, 0.212);

				int num_frames = 0;
				float print_time = platform_get_time();
				while (!window->is_close)
				{
								
								float curr_time = platform_get_time();

								handle_events(camera);
								camera->UpdateViewMatrix();
								renderer->ClearBuffer(&ClearColor);

								//std::map<Mesh*, Material*>::iterator meshIter = model->GetMeshMatMap().begin();
								//renderer->RenderMesh(meshIter->first, &modelMatrix, shader, meshIter->second);
								std::vector<Mesh*>& meshes = model->GetMeshes();
								std::vector<Material*>& materials = model->GetMaterials();

								//TODO: shadowmap
							
								//renderer->RenderMesh(meshes[0], &modelMatrix, shadowshader, materials[0], light);
								//light->depthbuffer = renderer->GetDepthBuffer();

								
								if (materials.size()) renderer->RenderMesh(meshes[0], &modelMatrix, shader, materials[0]);
								else renderer->RenderMesh(meshes[0], &modelMatrix, shader);

								// calculate and display FPS
								num_frames += 1;
								if (curr_time - print_time >= 1) {
												int sum_millis = (int)((curr_time - print_time) * 1000);
												int avg_millis = sum_millis / num_frames;
												printf("fps: %3d, avg: %3d ms,			backface_cull: %d\n", num_frames, avg_millis, renderer->numBackfaceCull);
												//printf("fps: %3d, avg: %3d ms\n", num_frames, avg_millis);
												num_frames = 0;
												print_time = curr_time;

								}
								// reset mouse information
								window->mouse_info.wheel_delta = 0;
								window->mouse_info.orbit_delta = glm::vec2(0, 0);
								window->mouse_info.fv_delta = glm::vec2(0, 0);

								// send framebuffer to window 
								renderer->Draw();
								msg_dispatch();
				}
				delete renderer;
				delete model;
				delete camera;
				delete shader;
		
				window_destroy();
				
				return 0;
}




