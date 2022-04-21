#pragma once
#include <cmath>
#include <assert.h>
#include <iostream>
#include <cstdint>
#include "mathUtils.h"

#define PI 3.14159265


class Color
{
private:


public:
				float r;
				float g;
				float b;
				float a;
				uint8_t uintR;
				uint8_t uintG;
				uint8_t uintB;
				uint8_t uintA;
				static Color red;
				static Color white;
				static Color green;
				static Color blue;
				static Color black;
				Color();
				Color(float r, float g, float b, float a = 1);
				Color(const Color& color);
				Color(unsigned int c);
				int GetUintR();
				int GetUintG();
				int GetUintB();
				int GetUintA();


				Color& operator/=(float num);
				Color& operator+=(const Color& v);
				Color& operator-=(const Color& v);
				Color& operator*=(float num);
				Color& operator*=(const Color& v);
				friend Color operator * (const Color& lhs, double rhs);
				friend Color operator * (double lhs, const Color& rhs);
				friend Color operator + (const Color& lhs, const Color& rhs);

				friend std::ostream& operator<<(std::ostream& output, const Color& color)
				{
								output << "(" << color.r << "," << color.g << "," << color.b << ")" << "\n";
								return output;
				}

				static inline Color Lerp(const Color& start, const Color& end, float t)
				{
								float r = mathUtils::Lerp(start.r, end.r, t);
								float g = mathUtils::Lerp(start.g, end.g, t);
								float b = mathUtils::Lerp(start.b, end.b, t);
								float a = mathUtils::Lerp(start.a, end.a, t);
								return Color(r, g, b, a);
				}

};