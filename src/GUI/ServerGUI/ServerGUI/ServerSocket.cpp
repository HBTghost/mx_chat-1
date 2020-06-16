#include "pch.h"
#include "ServerSocket.h"
#include "ServerBackgroundService.h"

void ServerSocket::BeginListenClient() {

	sockaddr_in from;
	int addr_from_len = sizeof(from);
	_socketClient = accept(_socketListenClient, (sockaddr*)&from, &addr_from_len);
	SClientPacket* recvPackage = new SClientPacket();
	recvPackage->client = _socketClient;

	if (_socketClient != INVALID_SOCKET) {
		cout << "Has + 1 client connected" << endl;
		LOG_INFO("Has + 1 client connected");
		_listClient.push_back(recvPackage);
	}

	//regis thread server thread 
	AfxBeginThread(recServerThread, (void*)_socketClient);
}
