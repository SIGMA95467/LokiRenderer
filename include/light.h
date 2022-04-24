#pragma once
#include "color.h"
#include "matrixUtils.h"



class Light
{
public:
				Light(const glm::vec3& pos, const glm::vec3& target, const glm::vec3& up);
				~Light();

				glm::mat4 viewMatrix;
				glm::mat4 projectionMaxtrix;
				
				float intensity;

				float* depthbuffer;

				glm::vec3 position;
				glm::vec3 direction;

				glm::vec3 color;
				void UpdateLight();
};


