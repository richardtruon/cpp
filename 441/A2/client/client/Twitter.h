#include "TCP.h"
#include <String>

#pragma once

using namespace std;

// forward declaration of classes
class TCP;

class Twitter {

public:
	void Options();
	int getMode();
	void Login();
	bool getLogin();
	void Tweet(TCP* tcpConnect);
	Twitter();
	~Twitter();

private:
	// Max tweet character length in 2018
	int tweetLength = 280;
	// Twitter client mode
	bool isLeader = false;
	// Application function
	int appMode = 0;
	// Login status
	bool loggedIn = false;
	// pointer to instance
	TCP* tcpConnect;

};

