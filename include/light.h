#pragma once
#include "color.h"




class Light
{
public:
				Light();
				~Light();


				glm::vec3 position;
				glm::vec3 direction;

				Color	color;
				float	intensity;				


};


