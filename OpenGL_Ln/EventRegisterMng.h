#pragma once
#include <functional>
#include <unordered_map>

typedef std::function<void()> CallBack;

// a part of mvc
class EventRegisterMng
{
public:
	enum EventFrquency
	{

	};
public:
	static EventRegisterMng* instance();

	void registerEventPair(const int eventType, const CallBack& callBack);

	void dispatchEvent(const int eventType);
protected:
	EventRegisterMng() {}
	~EventRegisterMng() {}

private:
	static EventRegisterMng* _instance;
	
	std::unordered_map<int, CallBack> _eventMap;
};