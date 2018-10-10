#pragma once
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

namespace OPENGL_LN
{
	class Camera
	{
	public:
		Camera();
		~Camera();
	private:
		glm::vec3 _antiHeader;
		glm::vec3 _right;
		glm::vec3 _up;
	};
}