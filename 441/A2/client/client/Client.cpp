/*
 * Client.cpp
 *
 * Created on Oct 17, 2018
 *  
 * Richard Truong
 * 
 * CPSC 441
*/

#include <WinSock2.h>
#include <iostream>
#include <String>

#include "TCP.h"
#include "Twitter.h"

#pragma comment(lib,"ws2_32.lib")	// Winsock Library
#pragma warning(disable:4996)		// inet_addr is a deprecated function, override warnings and compile

using namespace std;

int main() {

	cout << "Loading Client Application..." << endl;

	// TCP Session
	TCP* tcpConnect = new TCP();
	
	// Twitter session
	Twitter* twitterEngine = new Twitter();
	twitterEngine->Options();

	while (twitterEngine->getMode() != 2) {
		
		twitterEngine->Login();
			
		while (twitterEngine->getLogin()) {
			
			tcpConnect->Initialize();
			
			while (tcpConnect->getConnection()) {

				twitterEngine->Tweet(tcpConnect);
			}
			
			twitterEngine->Options();
		};

		// Close TCP Session
		tcpConnect->Close();
	}

	// Done
	return 0;
}

