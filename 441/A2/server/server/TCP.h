#include <WinSock2.h>
#include <string>

#pragma once

using namespace std;

// Forward declaration of classes
class Twitter;

class TCP {

public:
	void Initialize();
	void resetListen();
	bool getSocket();
	void Connect();
	bool getClient();
	void Receive();
	void Send(string newTweet);
	void Close();
	TCP(Twitter* dataEngine);
	~TCP();

private: 
	// sockets
	SOCKET listenConnection, newConnection;
	SOCKET* Connection;
	SOCKADDR_IN target;
	int addrlen;
	// socket creation status
	bool sockStatus = false, clientStatus = false;
	// pointer to instance
	Twitter* dataEngine;

};