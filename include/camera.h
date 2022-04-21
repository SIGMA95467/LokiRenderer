#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "MatrixUtils.h"

class Camera
{
public:
				Camera(const glm::vec3& pos, const glm::vec3& target, const glm::vec3& up);
				~Camera();



				void SetPespective(float fovy, float aspect, float zNear, float zFar);

				//Default set right hand coord
				void SetViewPort(int startX, int startY, int width, int height, bool isLeftHand = false);

				void UpdateViewMatrix();



				glm::vec3 target;
				glm::vec3 up;
				glm::vec3 position;
				glm::vec3 x, y, z;

				glm::mat4 viewMatrix;
				glm::mat4 projectionMaxtrix;
				glm::mat4 viewportMaxtrix;

				float zNear;
				float zFar;

				float DepthRangeNear;
				float DepthRangeFar;

				float Fov;
				float Aspect;
};



void updata_camera_pos(Camera* camera);
void handle_events(Camera* camera);

