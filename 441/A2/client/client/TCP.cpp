/*
 * Connect.cpp
 *
 * Created on Oct 17, 2018
 *
 * Richard Truong
 *
 * CPSC 441
*/

#include "TCP.h"
#include <WinSock2.h>
#include <iostream>
#include <String>

using namespace std;

#pragma comment(lib,"ws2_32.lib")	// Winsock Library
#pragma warning(disable:4996)		// inet_addr is a deprecated function, override warnings and compile

void TCP::Initialize() {
	// configure loopback or network client
	string selectOption;
	do {
		std::cout << "Is this a loopback or network client?" << std::endl;
		std::cout << "1) Loopback client" << std::endl;
		std::cout << "2) Network client" << std::endl;
		//std::cout << "Defaulting to option 1 after five seconds" << std::endl;
		std::cout << "Your selection: ";

		/*while (!iskeypressed(10000))
		{
			cout << "." << flush;
		}*/

		getline(cin, selectOption);

	} while (selectOption.compare("1") != 0 \
		&& selectOption.compare("2") != 0);

	std::cout << "Initializing client..." << std::endl;

	// Create listening socket
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &wsaData) != 0)		// Check for errors in the WinSock Startup
	{
		MessageBoxA(NULL, "Socket creation failed", "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}

	// Address initialization
	SOCKADDR_IN addr;
	addr.sin_port = htons(8888);
	int sizeofaddr = sizeof(addr);
	addr.sin_family = AF_INET;

	// Loopback server
	if (selectOption == "1") {
		addr.sin_addr.s_addr = inet_addr("127.0.0.1");
		// Network server
	}
	else {
		addr.sin_addr.s_addr = inet_addr("192.168.1.115");
	}

	std::cout << "\nSearching for server..." << std::endl;
	
	// Connect to TCP server
	newConnection = socket(AF_INET, SOCK_STREAM, NULL);	// Set Connection socket
	Connection = &newConnection;
	if (connect(newConnection, (SOCKADDR*)&addr, sizeofaddr) != 0)		// If we are unable to connect...
	{
		MessageBoxA(NULL, "Failed to Connect", "Error", MB_OK | MB_ICONERROR);
	}
	else {
		// Server connection established
		std::cout << "Connection established" << std::endl;
		connectedToServer = true;
	}
}

bool TCP::getConnection() {
	return connectedToServer;
}

// Send data
void TCP::Send(string tweet) {
	// Copy string to char array
	char cTweet[280];
	int sizeTweet = tweet.size();
	for (int i = 0; i < sizeTweet; i++) {
		cTweet[i] = tweet.at(i);
	}

	// null terminate message
	cTweet[sizeTweet] = '\0';
	const char* tweetPtr = cTweet;
	
	// cout << "Debugging send message: \'" << cTweet << "\' of size " << sizeTweet;
	send(*Connection, tweetPtr, sizeTweet, NULL);
}

string TCP::Receive() {
	string newTweet;
	char rcv_message[10000];

	// Receive data
	int queue;
	do {
		queue = recv(*Connection, rcv_message, sizeof(rcv_message), NULL);
		// null terminate the char arry at the client specified message length 
		rcv_message[queue] = '\0';
		newTweet = rcv_message;
		break;
	} while (queue > 0);
	
	return newTweet;
}

void TCP::Close() {
	// Tell server about disconnecting
	Send("--close");
	// Close the socket
	closesocket(*Connection);
	WSACleanup();
	connectedToServer = false;
	std::cout << "Connection closed" << std::endl;
}

TCP::TCP() {}

TCP::~TCP() {}
