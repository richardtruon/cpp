#include "TCP.h"

#include <vector>
#include <String>
#include <map>
#include <unordered_map>

#pragma once

using namespace std;

class Twitter {

public:
	void setMode();
	int getMode();
	string getNewTweet();
	void parseTweet(string tweet);
	void checkWords(int tweetSize);
	void eSequential();
	void eSum();
	void eHash();
	void dSequential();
	void dSum();
	void dHash();
	Twitter();
	~Twitter();

private:
	// Encode/Decode mode 1->Sequence, 2->Sum, 3->Hash
	int appMode = 1;

	// Encode/Decode string type 1->string, 2->hex
	int textMode = 1;

	// Tweet
	string tweetToCheck;
	string wordToCheck;
	string hexString = "0x";
	vector<std::string> indTweet;

	// Processed Tweet
	string newTweet;

	// vector data structure for sequence encode/decode
	vector<string> sequence;
	// array data structure for 16-bit sum encode/decode (0xFFFF = 65535)
	std::unordered_map<string, int> wordSum;
	// hash map for double hash encode/decode
	// 3000 words are used in 95 % of everyday English
	// (assume only a fraction will be used in all our tweets)
	std::map<int, string> hashMap;
	int primeTableSize = 499;	// closest prime to 3000 divided by 6
	
};

