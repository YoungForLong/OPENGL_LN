#pragma once

#define lerp(__T, f, t, dt) (OPENGL_LN::Lerp<__T>()(f, t, dt))

namespace OPENGL_LN
{
	template <typename __T>
	class Lerp
	{
	public:
		__T operator()(const __T& from, const __T& to, const float delta)
		{
			__T diff = to - from;
			return from + diff * delta;
		}
	};
}