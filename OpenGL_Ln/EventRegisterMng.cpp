#include "EventRegisterMng.h"

EventRegisterMng* EventRegisterMng::_instance = NULL;

EventRegisterMng * EventRegisterMng::instance()
{
	if (!_instance)
		_instance = new EventRegisterMng;
	return _instance;
}

void EventRegisterMng::registerEventPair(const int eventType, const CallBack & callBack)
{
	_eventMap.insert(std::pair<int, CallBack>(eventType, callBack));
}

