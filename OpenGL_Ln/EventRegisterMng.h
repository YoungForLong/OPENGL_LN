#pragma once
#include <functional>
#include <unordered_map>
#include <vector>
#include <list>

#include "EventTypes.h"

typedef std::function<bool(void*)> CallBack;

// a part of mvc
class EventRegisterMng
{
public:
	static EventRegisterMng* instance();

	void registerEventPair(const int eventType, const CallBack& callBack);

	void dispatchEvent(const int eventType, void* extra);
protected:
	EventRegisterMng() {}
	~EventRegisterMng() {}

private:
	static EventRegisterMng* _instance;
	
	std::unordered_map<int, std::list<CallBack>> _eventMap;
};