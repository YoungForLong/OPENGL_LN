#pragma once
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#define DEFAULT_CAMERA_POS (glm::vec3(0.0f, 0.0f, 0.0f))
#define DEFAULT_TARGET_POS (glm::vec3(0.0f, 0.0f, -1.0f))
#define CAMERA_SPEED float(2.5f)
#define CAMERA_SENSITIVITY 0.02f

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

		bool handleKeyboardEvent(void* keyVal);
		bool handleMouseEvent(void* posVal);

		virtual void tick(float dt);
	public:
		static glm::vec3 ORIGIN_Y;
	protected:
		void calFront();
	private:
		glm::vec3 _pos;
		glm::vec3 _up;
		glm::vec3 _front;
		EulerAngle _euler;
		float _curSpeed;
		glm::vec2 _lastMousePos;
		bool _firstGainFocus;
	};
}