/*
 * TCP.cpp
 *
 * Created on Oct 17, 2018
 *
 * Richard Truong
 *
 * CPSC 441
*/

#include "TCP.h"
#include "Twitter.h"
#include <WinSock2.h>
#include <iostream>
#include <String>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

#pragma comment(lib,"ws2_32.lib")	// Winsock Library
#pragma warning(disable:4996)		// inet_addr is a deprecated function, override warnings and compile

#include <windows.h>

// keypress detect and timer
bool iskeypressed(unsigned timeout_ms = 0) {
	return WaitForSingleObject(
		GetStdHandle(STD_INPUT_HANDLE),
		timeout_ms
	) == WAIT_OBJECT_0;
}


TCP::TCP(Twitter* dataEngine) {
	this->dataEngine = dataEngine;
}

void TCP::resetListen() {
	sockStatus = false;
}

bool TCP::getSocket() {
	return sockStatus;
}

bool TCP::getClient() {
	return clientStatus;
}

void TCP::Initialize() {
	// configure loopback or network server
	string selectOption;
	do {
		std::cout << "Is this a loopback or network server?" << std::endl;
		std::cout << "1) Loopback server" << std::endl;
		std::cout << "2) Network server" << std::endl;
		//std::cout << "Defaulting to option 1 after five seconds" << std::endl;
		std::cout << "Your selection: ";

		//while (!iskeypressed(10000))
		//{
		//	cout << "." << flush;
		//}
		
		getline(cin, selectOption);

	} while (selectOption.compare("1") != 0 \
		&& selectOption.compare("2") != 0);

	std::cout << "Initializing server..." << std::endl;

	// Create listening socket
	WSAData wsaData;

	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &wsaData) != 0)		// Check for errors in the WinSock Startup
	{
		MessageBoxA(NULL, "WinSock startup failed", "Error", MB_OK | MB_ICONERROR);
	}

	// Address initialization
	target.sin_port = htons(8888);
	target.sin_family = AF_INET;
	addrlen = sizeof(target);		// required length of the accepted address)

	// Loopback server
	if (selectOption == "1") {
		target.sin_addr.s_addr = inet_addr("127.0.0.1");
	// Network server
	} else {
		target.sin_addr.s_addr = inet_addr("192.168.1.115");
	}

	// Bind address and port to socket
	listenConnection = socket(AF_INET, SOCK_STREAM, NULL);		// Socket to listen for connections
	if (listenConnection == INVALID_SOCKET) {
		std::cout << "Socket not created successfully" << std::endl;
	} else {
		std::cout << "Socket created successfully" << std::endl;
		sockStatus = true;
	}

	bind(listenConnection, (SOCKADDR*)&target, sizeof(target));		// Bind the address to the socket
}

void TCP::Connect() {
	// Ready to listen for connections
	std::cout << "\nWaiting for client connection..." << std::endl;
	listen(listenConnection, SOMAXCONN);		// listen for incoming connection

	// Connect to TCP client
	newConnection = accept(listenConnection, (SOCKADDR*)&target, &addrlen);	// Accept new connection
	Connection = &newConnection;
	if (newConnection == 0) {		// If we are unable to connect...
		std::cout << "Failed to accept client connection" << std::endl;
	} else {
		// Client connection established
		std::cout << "Client Connected" << std::endl;
	}
	clientStatus = true;
}

void TCP::Receive() {
	// Main Loop for listening
	string tweet;
	char rcv_message[10000];

	// Receive data
	int queue;
	do { 
		queue = recv(*Connection, rcv_message, sizeof(rcv_message), NULL);
		// null terminate the char arry at the client specified message length 
		rcv_message[queue] = '\0';
		tweet = rcv_message;
		break;
	} while (queue > 0);
	 
	// check contents of client message for "--close"
	if (tweet == "--close") {
		clientStatus = false;
		Close();
	} else {
		std::cout << "\nReceived a new tweet... \'" << tweet << "\'" << std::endl;

		dataEngine->parseTweet(tweet);
		Send(dataEngine->getNewTweet());
	}
}

// Send data
void TCP::Send(string newTweet) {
	// Copy string to char array
	char cTweet[280];
	int sizeTweet = newTweet.size();
	for (int i = 0; i < sizeTweet; i++) {
		cTweet[i] = newTweet.at(i);
	}

	// null terminate message
	cTweet[sizeTweet] = '\0';
	const char* tweetPtr = cTweet;

	std::cout << "Sending processed tweet... \'" << cTweet << std::endl;
	send(*Connection, tweetPtr, sizeTweet, NULL);	// Send newTweet back
}

void TCP::Close() {
	clientStatus = false;
	if (*Connection) {
		closesocket(*Connection);
		std::cout << "\nClient has been disconnected" << endl;
	}
	if (dataEngine->getMode() == 4) {
		sockStatus = false;
		closesocket(listenConnection);
		WSACleanup();
		std::cout << "Server has been shutdown" << std::endl;
	}
}

TCP::~TCP() {}
