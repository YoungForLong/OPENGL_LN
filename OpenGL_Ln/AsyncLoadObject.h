#pragma once

namespace OPENGL_LN
{
	class AsyncLoadObject
	{
	public:
		AsyncLoadObject();
		virtual ~AsyncLoadObject();
		virtual bool hasLoadFinished() const { return _loadFinished; }
		virtual void asyncLoad(const char* filename, const unsigned int id) = 0;
		virtual void callBackHandleLoad(const void* any, const unsigned int id) = 0;
	protected:
		bool _loadFinished;
	};
}