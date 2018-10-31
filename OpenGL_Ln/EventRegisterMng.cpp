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
	if (_eventMap.find(eventType) != _eventMap.end())
	{
		_eventMap[eventType].push_back(callBack);
	}
	else
	{
		std::list<CallBack> cbList;
		cbList.push_back(callBack);
		auto pr = std::make_pair(eventType, cbList);
		_eventMap.insert(pr);
	}
}

void EventRegisterMng::dispatchEvent(const int eventType, void* extra)
{
	if (_eventMap.find(eventType) != _eventMap.end())
	{
		std::list<CallBack> tmp;
		auto cbList = _eventMap[eventType];
		for (CallBack cb : cbList)
		{
			if (cb(extra))
			{
				tmp.push_back(cb);
			}
		}
		cbList.swap(tmp);
	}
}
