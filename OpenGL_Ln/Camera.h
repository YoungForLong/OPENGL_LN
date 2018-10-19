#pragma once
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#define DEFAULT_CAMERA_POS (glm::vec3(0.0f, 0.0f, 0.0f))
#define DEFAULT_TARGET_POS (glm::vec3(0.0f, 0.0f, -1.0f))

namespace OPENGL_LN
{
	struct EulerAngle
	{
		EulerAngle(float pitch, float yaw, float roll)
		{
			_pitch = pitch;
			_yaw = yaw;
			_roll = roll;
		}
		float _pitch;
		float _yaw;
		float _roll;
	};

	class Camera
	{
	public:
		Camera(const glm::vec3& pos);
		~Camera();

		glm::mat4 lookAt(const glm::vec3& target);

		glm::mat4 getCurTrans();
	public:
		static glm::vec3 ORIGIN_Y;
	protected:
		void calFront();
	private:
		glm::vec3 _pos;
		glm::vec3 _up;
		glm::vec3 _front;
		EulerAngle _euler;
	};
}