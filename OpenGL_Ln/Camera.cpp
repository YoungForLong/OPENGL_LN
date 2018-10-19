#include "Camera.h"

glm::vec3 OPENGL_LN::Camera::ORIGIN_Y = glm::vec3(0.0f, 1.0f, 0.0f);

OPENGL_LN::Camera::Camera(const glm::vec3 & pos) :
	_pos(pos),
	_up(ORIGIN_Y),
	_euler(0, 0, 0)
{
}

OPENGL_LN::Camera::~Camera()
{
}

glm::mat4 OPENGL_LN::Camera::lookAt(const glm::vec3 & target)
{
	return glm::lookAt(_pos, target, _up);
}

glm::mat4 OPENGL_LN::Camera::getCurTrans()
{
	calFront();
	return glm::lookAt(_pos, _pos + _front, _up);
}

void OPENGL_LN::Camera::calFront()
{
	_front.x = -sin(_euler._yaw) * cos(_euler._pitch);
	_front.y = sin(_euler._pitch);
	_front.z = -cos(_euler._pitch)* cos(_euler._yaw);
	_front = glm::normalize(_front);
}
