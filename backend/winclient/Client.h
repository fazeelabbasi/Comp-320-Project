﻿#pragma once

#include <winsock2.h>

class Client {
	SOCKET ConnectSocket = INVALID_SOCKET;

public:
	void start(PCSTR host);
	void sendInfo();
	void receiveInfo();
	bool isConnected();
	void stop();
};
