/*
 * Server.cpp
 *
 * Created on Oct 17, 2018
 *
 * Richard Truong
 *
 * CPSC 441
*/

#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <iostream>
#include <string>

#include "TCP.h"
#include "Twitter.h"

int main() {

	// New session to store tweet data	
	Twitter* dataEngine = new Twitter();
	
	// TCP Session
	TCP* tcpConnect = new TCP(dataEngine);
	
	tcpConnect->Initialize();
	while (tcpConnect->getSocket()) {

		// change the encoding mode in between client sessions
		dataEngine->setMode();
		while (dataEngine->getMode() != 4) {

			tcpConnect->Connect();
			while (tcpConnect->getClient()) {

				// server blocking at the receive stage
				tcpConnect->Receive();
			}

			dataEngine->setMode();
		}

		tcpConnect->resetListen();
	}

	// close TCP connection
	tcpConnect->Close();

	// Done
	return 0;

}