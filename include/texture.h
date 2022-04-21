#pragma once
#include "color.h"

#include <iostream>  
#include <vector>  
#include <string>  
#include <glm/glm.hpp>



class Texture {
public:
				Texture(std::string path);

				virtual ~Texture() {
				};


				float SampleIntensity(float inU, float inV) const;

				glm::vec3 SampleColor(float inU, float inV) const;
				//unsigned int getColor(float inU, float inV) const;
				int GetWidth();
				int GetHeight();

private:
				int mWidth;
				int mHeight;
				int mChannels;
				unsigned char* mPixels;
				std::string mPath;

				void GetRGB(glm::vec3& col, int x, int y) const;
				void GetIntensity(float& intensity, int u, int v) const;
				glm::vec3 BilinearSampleColor(float u, float v) const;
				glm::vec3 NearestSampleColor(float u, float v) const;
};
