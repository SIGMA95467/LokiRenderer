#pragma once
#include "color.h"
#include <glm/glm.hpp>


struct VSOutput
{
				glm::vec4 position;
				glm::vec3 normal;
				glm::vec2 uv;
				Color color;

				glm::vec3 worldView;
				glm::vec3 worldNormal;
				//glm::vec3 LightSpacePos;


};



class Vertex {
public:
				glm::vec3 position;
				glm::vec3 normal;
				float u;
				float v;
				Color color;
				float w;
				Vertex()
				{

				}
				Vertex(const glm::vec3& position, const glm::vec3 normal, float u, float v, const Color& color = Color()) {
								this->position = position;
								this->normal = normal;
								this->u = u;
								this->v = v;
								this->color = color;
				};

				//Vertex interpolate(const Vertex& vertex, float t) const {
				//				//Vector3 p = position.interpolate(vertex.position, factor);
				//				glm::vec3 p = glm::vec3::Lerp(position, vertex.position, t);
				//				glm::vec3 n = glm::vec3::Lerp(normal, vertex.normal, t);
				//				float invZ1 = 1.0f / position.z;
				//				float invZ2 = 1.0f / vertex.position.z;
				//				float Z = 1.0f / mathUtils::Lerp(invZ1, invZ2, t);

				//				float tu = mathUtils::Lerp(u * invZ1, vertex.u * invZ2, t);
				//				float tv = mathUtils::Lerp(v * invZ1, vertex.v * invZ2, t);
				//				Color c = Color::Lerp(color, vertex.color, t);
				//				return Vertex(p, n, tu * Z, tv * Z, c);
				//};
};