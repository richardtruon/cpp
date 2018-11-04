#include <WinSock2.h>
#include <String>

#pragma once

using namespace std;

class TCP {

public:
	void Initialize();
	bool getConnection();
	void Send(string tweet);
	string Receive();
	void Close();
	TCP();
	~TCP();

private:
	bool connectedToServer = false;
	SOCKET newConnection;
	SOCKET* Connection;
};
