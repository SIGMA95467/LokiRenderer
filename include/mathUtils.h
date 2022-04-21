#pragma once
#include <cmath>
#include <cfloat>
#include <xmmintrin.h>
#include <glm/glm.hpp>
class mathUtils
{
public:
				mathUtils() {}
				~mathUtils() {}
				static float Pi;
				static float Rad2Deg;
				static float Deg2Rad;
				static float Infinity;
				static float NegativeInfinity;
				static float EPSILON;

				static float Clamp(float x, float min = 0, float max = 1)
				{
								return fmin(max, fmax(x, min));
				}
				static float Min(float a, float b)
				{
								return fmin(a, b);
				}

				static float Max(float a, float b)
				{
								return fmax(a, b);
				}

				static float Acos(float theta)
				{
								return acos(theta);
				}

				static float Asin(float theta)
				{
								return asin(theta);
				}

				static float Sin(float theta)
				{
								return sin(theta);
				}

				static float Cos(float theta)
				{
								return cos(theta);
				}

				static float Tan(float theta)
				{
								return tan(theta);
				}

				static int Round(float f)
				{
								return round(f);
				}

				static inline float Lerp(float a, float b, float t)
				{
								return a + (b - a) * t;
				}

				static inline float Abs(float a)
				{
								return std::abs(a);
				}
			
				static inline void Lerp(glm::vec3& v0, glm::vec3& v1, glm::vec3& v2, float t) 
				{
								v0 = v1 * (1 - t) + v2 * t;
				}

};