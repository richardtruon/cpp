/*
 * Twitter.cpp
 *
 * Created on Oct 17, 2018
 *
 * Richard Truong
 *
 * CPSC 441
*/

#include "Twitter.h"
#include "TCP.h"
#include <iostream> 
#include <cstdlib> 
#include <String>
#include <sstream>

using namespace std;

// Client options
void Twitter::Options() {
	loggedIn = false;
	string selectOption;
	do {
		std::cout << "\n  - - -  MAIN MENU  - - -  " << std::endl;
		std::cout << "Select one of the following:" << std::endl;
		std::cout << "1) Login to Twitter" << std::endl;
		std::cout << "2) Close program\n" << std::endl;
		std::cout << "Your selection: ";
		getline(cin, selectOption);
	} while (selectOption.compare("1") != 0 && selectOption.compare("2") != 0);

	appMode = stoi(selectOption);
}

int Twitter::getMode() {
	return appMode;
}

// Only followers who are sophisticated intelligentia can login (myleaderrocks)
// T-Rump's login (iamsoawesome)
void Twitter::Login() {
	loggedIn = false;
	int loginAttempts = 0;
	string passwd;
	while (loginAttempts <= 3) {
		// Correct password entered
		if (loggedIn) {
			// Identify who has logged in
			isLeader = (passwd == "imtrump") ? true : false;
			loggedIn =  true;
			break;
		} else {
			loginAttempts++;
			// hide password input
			/*HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
			DWORD mode = 0;
			GetConsoleMode(hStdin, &mode);
			SetConsoleMode(hStdin, mode & (~ENABLE_ECHO_INPUT));*/

			if (loginAttempts == 1) {
				std::cout << "Enter password: ";
			} else {
				std::cout << "\nTry Again. Enter correct password: ";
			}
			getline(cin, passwd);
			if (passwd == "yaytrump" || passwd == "imtrump") {
				loggedIn = true;
			}
			// show console input again
			/*SetConsoleMode(hStdin, mode);
			tcsetattr(STDIN_FILENO, TCSANOW, &oldt);*/
		}
	}
	if (!loggedIn) {
		appMode = 2;
	}
}

bool Twitter::getLogin() {
	return loggedIn;
}

void Twitter::Tweet(TCP* tcpConnect) {
	this->tcpConnect = tcpConnect;
	loggedIn = false;
	// Request strings or encodings
	string message;
	string hex = "0x";
	string returnedMessage;
	std::cout << "\nEnter tweet or \'--logout\'" << std::endl;
	// get user message
	getline(cin, message);
	// check length of tweet and ensure it does not exceed max character length of 280
	if (message.length() <= tweetLength) {
		if (message != "--logout") {
			// Sophisticated intelligentia can decode tweets
			if (message.string::find(hex) != std::string::npos) {
				if (message.string::length() < 2) {
					std::cout << "\nYour message is empty..." << std::endl;
				}
				else {
					std::cout << "\nDecoding tweet: \'" << message << "\'" << std::endl;
					tcpConnect->Send(message);
					returnedMessage = tcpConnect->Receive();
					if (empty(returnedMessage)) {
						std::cout << "\nDecode was unsuccessful, double check the tweet" \
							<< std::endl;
					}
					else {
						std::cout << "\nT-Rump says: \'" << returnedMessage << "\'" \
							<< std::endl;
					}
				}
			}
			else if (empty(message)) {
				std::cout << "\nYour message is empty..." << std::endl;
				// Only T-Rump can encode tweets
			}
			else {
				std::cout << "\nEncoding tweet: \'" << message << "\'" << std::endl;
				tcpConnect->Send(message);
				returnedMessage = tcpConnect->Receive();
				if (isLeader) {
					if (empty(returnedMessage)) {
						std::cout << "\nEncode was unsuccessful, double check your tweet" \
							<< std::endl;
					}
					else {
						std::cout << "\nEncoded tweet: \'" << returnedMessage << "\'" \
							<< std::endl;
					}
				}
				else {
					std::cout << "Request Denited. Enter ONLY ENCODED messages" << std::endl;
				}
			}
		}
		else {
			loggedIn = false;
			tcpConnect->Close();
		}
	} else {
		std::cout << "\nRequest denied. Your tweet exceeds 280 words";
	}
}

Twitter::Twitter() {}
Twitter::~Twitter() {}
