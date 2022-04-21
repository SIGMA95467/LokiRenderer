#include "camera.h"
#include "window.h"
#include "mathUtils.h"
Camera::Camera(const glm::vec3& _pos, const glm::vec3& _target, const glm::vec3& _up)
{
				position = _pos;
				target = _target;
				up = _up;
				viewMatrix = MatrixUtils::calcViewMatrix(_pos, _target, _up);
				viewportMaxtrix = glm::mat4(0);
}

Camera::~Camera()
{

}

void Camera::UpdateViewMatrix()
{
				viewMatrix = MatrixUtils::calcViewMatrix(position, target, up);
}




void Camera::SetPespective(float fovy, float aspect, float _zNear, float _zFar)
{
				zFar = _zFar;
				zNear = _zNear;
				Fov = fovy;
				Aspect = aspect;
				projectionMaxtrix = MatrixUtils::calcPerspProjectMatrix(fovy, aspect, zNear, zFar);
}
void updata_camera_pos(Camera* camera)
{
				glm::vec3 from_target = camera->position - camera->target;			// vector point from target to camera's position
				float radius = glm::length(from_target);

				float phi = (float)atan2(from_target[0], from_target[2]); // azimuth angle(��λ��), angle between from_target and z-axis��[-pi, pi]
				float theta = (float)acos(from_target[1] / radius);		  // zenith angle(�춥��), angle between from_target and y-axis, [0, pi]
				float x_delta = window->mouse_info.orbit_delta[0] / window->width;
				float y_delta = window->mouse_info.orbit_delta[1] / window->height;

				// for mouse wheel
				radius *= (float)pow(0.95, window->mouse_info.wheel_delta);

				float factor = 1.5 * mathUtils::Pi;
				// for mouse left button
				phi += x_delta * factor;
				theta += y_delta * factor;
				if (theta > mathUtils::Pi) theta = mathUtils::Pi - mathUtils::EPSILON * 100;
				if (theta < 0)  theta = mathUtils::EPSILON * 100;

				camera->position[0] = camera->target[0] + radius * sin(phi) * sin(theta);
				camera->position[1] = camera->target[1] + radius * cos(theta);
				camera->position[2] = camera->target[2] + radius * sin(theta) * cos(phi);

				// for mouse right button
				factor = radius * (float)tan(60.0 / 360 * mathUtils::Pi) * 2.2;
				x_delta = window->mouse_info.fv_delta[0] / window->width;
				y_delta = window->mouse_info.fv_delta[1] / window->height;
				glm::vec3 left = x_delta * factor * camera->x;
				glm::vec3 up = y_delta * factor * camera->y;

				camera->position += (left - up);
				camera->target += (left - up);
}

void handle_mouse_events(Camera* camera)
{
				if (window->buttons[0])
				{
								glm::vec2 cur_pos = get_mouse_pos();
								window->mouse_info.orbit_delta = window->mouse_info.orbit_pos - cur_pos;
								window->mouse_info.orbit_pos = cur_pos;
				}

				if (window->buttons[1])
				{
								glm::vec2 cur_pos = get_mouse_pos();
								window->mouse_info.fv_delta = window->mouse_info.fv_pos - cur_pos;
								window->mouse_info.fv_pos = cur_pos;
				}

				updata_camera_pos(camera);
}

void handle_key_events(Camera* camera)
{
				float distance = glm::length(camera->target - camera->position);

				if (window->keys['W'])
				{
								camera->position += -10.0f / window->width * camera->z * distance;
				}
				if (window->keys['S'])
				{
								camera->position += 0.05f * camera->z;
				}
				if (window->keys[VK_UP] || window->keys['Q'])
				{
								camera->position += 0.05f * camera->y;
								camera->target += 0.05f * camera->y;
				}
				if (window->keys[VK_DOWN] || window->keys['E'])
				{
								camera->position += -0.05f * camera->y;
								camera->target += -0.05f * camera->y;
				}
				if (window->keys[VK_LEFT] || window->keys['A'])
				{
								camera->position += -0.05f * camera->x;
								camera->target += -0.05f * camera->x;
				}
				if (window->keys[VK_RIGHT] || window->keys['D'])
				{
								camera->position += 0.05f * camera->x;
								camera->target += 0.05f * camera->x;
				}
				if (window->keys[VK_ESCAPE])
				{
								window->is_close = 1;
				}
}

void handle_events(Camera* camera)
{
				//calculate camera axis
				camera->z = glm::normalize(camera->position.z - camera->target);
				camera->x = glm::normalize(cross(camera->up, camera->z));
				camera->y = glm::normalize(cross(camera->z, camera->x));

				//mouse and keyboard events
				handle_mouse_events(camera);
				handle_key_events(camera);
}