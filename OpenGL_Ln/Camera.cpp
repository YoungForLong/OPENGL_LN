#include "Camera.h"
#include "EventRegisterMng.h"
#include <GLFW\glfw3.h>
#include <iostream>

glm::vec3 OPENGL_LN::Camera::ORIGIN_Y = glm::vec3(0.0f, 1.0f, 0.0f);

OPENGL_LN::Camera::Camera(const glm::vec3 & pos) :
	_pos(pos),
	_up(ORIGIN_Y),
	_euler(0, -90.0f, 0),
	_lastMousePos(360, 360),
	_firstGainFocus(false)
{
	EventRegisterMng::instance()->registerEventPair(EventTypes::EVENT_KEYBOARD,
		std::bind(&Camera::handleKeyboardEvent, this, std::placeholders::_1));
	EventRegisterMng::instance()->registerEventPair(EventTypes::EVENT_MOUSE,
		std::bind(&Camera::handleMouseEvent, this, std::placeholders::_1));
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


bool OPENGL_LN::Camera::handleKeyboardEvent(void * keyVal)
{
	calFront();

	int* intVal = static_cast<int*>(keyVal);
	switch (*intVal)
	{
		case GLFW_KEY_W:
		{
			_pos += _curSpeed * _front;
			break;
		}
		case GLFW_KEY_S:
		{
			_pos -= _curSpeed * _front;
			break;
		}
		case GLFW_KEY_A:
		{
			_pos -= glm::normalize(glm::cross(_front, _up)) * _curSpeed;
			break;
		}
		case GLFW_KEY_D:
		{
			_pos += glm::normalize(glm::cross(_front, _up)) * _curSpeed;
			break;
		}
		default:
			break;
	}

	return true;
}

bool OPENGL_LN::Camera::handleMouseEvent(void * posVal)
{
	double* posX = static_cast<double*>(posVal);
	double* posY = posX + 1;
	std::cout << "x, y: " << *posX << ", " << *posY << std::endl;
	if (!_firstGainFocus)
	{
		_lastMousePos = glm::vec2(*posX, *posY);
		_firstGainFocus = true;
		return true;
	}
	double xOffset = *posX - _lastMousePos.x;
	double yOffset = _lastMousePos.y - *posY;
	_lastMousePos = glm::vec2(*posX, *posY);
	
	_euler._yaw += (CAMERA_SENSITIVITY * xOffset);
	/*rotation y is anti-wise*/
	_euler._pitch += (CAMERA_SENSITIVITY * yOffset);
	
	return true;
}

glm::vec3 OPENGL_LN::Camera::getCameraLookAt()
{
	calFront();
	return _front;
}

void OPENGL_LN::Camera::tick(float dt)
{
	_curSpeed = CAMERA_SPEED * dt;
}

glm::vec3 OPENGL_LN::Camera::getPos() const
{
	return _pos;
}

void OPENGL_LN::Camera::calFront()
{
	_front.x = cos(glm::radians(_euler._pitch)) * cos(glm::radians(_euler._yaw));
	_front.y = sin(glm::radians(_euler._pitch));
	_front.z = sin(glm::radians(_euler._yaw)) * cos(glm::radians(_euler._pitch));
	_front = glm::normalize(_front);
}
