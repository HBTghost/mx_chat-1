#pragma once
#include <functional>
#include <iostream>
#include "PackageHelper.h"
#include "MxObject.h"
class EventOnMessageHandler {
public:
	std::function<void(MessageModel)> eventOnMessageFnc;
	void addHandler(std::function<void(MessageModel)> _callback) {
		eventOnMessageFnc = _callback;
	}
	void OnMessageEventHandler(MessageModel m) {
		if (eventOnMessageFnc != nullptr) {
			eventOnMessageFnc(m);
		}
	}
};