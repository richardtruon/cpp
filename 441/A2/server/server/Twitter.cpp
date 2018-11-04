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
#include <iostream> 
#include <cstdlib> 
#include <sstream>
#include <cassert>
#include <string>

using namespace std;

#include <windows.h>

//// keypress detect and timer
//bool iskeypressed(unsigned timeout_ms = 0) {
//	return WaitForSingleObject(
//		GetStdHandle(STD_INPUT_HANDLE),
//		timeout_ms
//	) == WAIT_OBJECT_0;
//}

Twitter::Twitter() {}

void Twitter::setMode() {
	// default to option 1
	string selectOption = "1";
	do {
		std::cout << "\n  - - -  MAIN MENU  - - -  " << std::endl;
		std::cout << "Select encoder for next client connection:" << std::endl;
		std::cout << "1) Sequential Index" << std::endl;
		std::cout << "2) Word Sum" << std::endl;
		std::cout << "3) Hashmap" << std::endl;
		std::cout << "4) Close server\n" << std::endl;
		//std::cout << "Defaulting to option 1 after five seconds" << std::endl;
		std::cout << "Your selection: ";

		//while (!iskeypressed(10000))
		//{
		//	cout << "." << flush;
		//}
		getline(cin, selectOption);

	} while (selectOption.compare("1") != 0 \
		&& selectOption.compare("2") != 0\
		&& selectOption.compare("3") != 0\
		&& selectOption.compare("4") != 0);

	appMode = stoi(selectOption);
	this->appMode = appMode;
};

int Twitter::getMode() {
	return appMode;
}

string Twitter::getNewTweet() {
	return newTweet;
}

void Twitter::parseTweet(string tweet) {
	// clear cache
	newTweet = "";
	indTweet.clear();

	// hex means we are decoding, string means we are encoding
	textMode = (tweet.string::find(hexString) != std::string::npos) ? 2 : 1;
	// split words by spaces into array
	size_t pos = 0;
	if ((pos = tweet.find(" ")) != std::string::npos) {
		while ((pos = tweet.find(" ")) != std::string::npos) {
			indTweet.push_back(tweet.substr(0, pos));
			tweet.erase(0, pos + 1);
		} 
	}
	indTweet.push_back(tweet);

	// check/process all the words 
	checkWords(indTweet.size());
}

void Twitter::checkWords(int tweetSize) {
	// check for puntuation		
	for (int i = 0; i < tweetSize; i++) {
		char punc = '\0';

		wordToCheck = indTweet[i];
		
		//		before the word: #ruletheworld
		if (ispunct(wordToCheck[0])) {
			newTweet += wordToCheck[0];	// append to new tweet
			wordToCheck.erase(0, 1);
		};
		//		after the word: what?
		if (ispunct(wordToCheck[wordToCheck.length() - 1])) {
			punc = wordToCheck[wordToCheck.length() - 1];
			wordToCheck.erase(wordToCheck.length() - 1, wordToCheck.length() - 2);
		};

		// decode word in one mode
		if (textMode == 2) {
			switch (appMode) {
				case 1:
					dSequential();
					break;
				case 2:
					dSum();
					break;
				case 3:
					dHash();
					break;
				default:
					break;
			}

		// encode word in all modes
		} else {
			eSequential();
			eSum();
			eHash();
		}
		
		// append punctuation if it exists
		if (punc != '\0') {
			newTweet = newTweet.substr(0, newTweet.size() - 1);
			newTweet.append(1, punc);	// append punc to new tweet
			newTweet += " ";
		}
	}

	// trim space at the end of tweet
	if ((newTweet.at(newTweet.length() - 1)) == ' ') {
		newTweet = newTweet.substr(0, newTweet.size() - 1);
	}

	// debugger print out all the words stored in dictionary
	cout << std::endl;
	for (size_t i = 0; i < sequence.size(); i++) {
		cout << sequence.at(i) << " ";
	}
	cout << std::endl << std::endl;

};

// encoding series of functions
void Twitter::eSequential () {
	int wordIndex = -1;

	// Find the word
	if (sequence.size() < 1) {
		wordIndex = 0;
		sequence.push_back(wordToCheck);
	} else {
		for (size_t i = 0; i < sequence.size(); i++) {
			if (sequence.at(i).compare(wordToCheck) == 0) {
				wordIndex = i;
			}
		}
	}
	
	// assign new number if word is not found
	if (wordIndex < 0) {
		wordIndex = sequence.size();
		sequence.push_back(wordToCheck);
	}

	// Append encoding to newTweet if server is in Sequential mode
	if (appMode == 1) {
		std::stringstream sstream;
		sstream << std::hex << wordIndex;
		std::string result = sstream.str();
		newTweet += (hexString + result + " ");
	}
}

void Twitter::eSum () {
	// sum up all the characters in the word
	int wSum = 0;
	for (int i = 0; i < wordToCheck.length(); i++) {
		wSum += (unsigned char)wordToCheck.at(i);
	}

	// make sure wSum is less than 65535
	wSum = wSum % 65536;

	// Store word in sum umap
	wordSum[wordToCheck] = wSum;

	// Append encoding to newTweet if server is in Sum mode
	if (appMode == 2) {
		std::stringstream sstream;
		sstream << std::hex << wSum;
		std::string result = sstream.str();
		newTweet += (hexString + result + " ");
	}
}

// hash string with h1(x) = k and then h2(k) = m, return m
void Twitter::eHash () {
	// break up characters in the word and do +/*/- to get x
	int wHash = 0;
	int hashMode = 1;
	for (int i = 0; i < wordToCheck.length(); i++) {
		switch (hashMode) {
		case 1:
			wHash += (unsigned char)wordToCheck.at(i);
			hashMode = 2;
			break;
		case 2:
			wHash *= (unsigned char)wordToCheck.at(i);
			hashMode = 3;
			break;
		case 3:
			wHash -= (unsigned char)wordToCheck.at(i);
			hashMode = 1;
			break;
		}
	}
	// h(x) = x % table_size = k
	wHash %= primeTableSize;
	// h(k) = PRIME - (k % PRIME) = m
	wHash = primeTableSize - (wHash % primeTableSize);
	// store word at key m
	hashMap.insert(pair<int, string>(wHash, wordToCheck));

	// Append encoding to newTweet if server is in Hash mode
	if (appMode == 3) {
		std::stringstream sstream;
		sstream << std::hex << wHash;
		std::string result = sstream.str();
		newTweet += (hexString + result + " ");
	}
}

// decoding series of functions
void Twitter::dSequential () {
	// convert hex to int
	std::stringstream ss;
	ss << std::hex << wordToCheck.substr(2, wordToCheck.length() - 1);
	int wordIndex;
	ss >> wordIndex;

	// Check if the index of this word is out of bounds
	if (wordIndex >= sequence.size()) {
		// keep the hex value as is in the string
		newTweet += (wordToCheck + " ");
	} else {
		newTweet += (sequence[wordIndex] + " ");
	}
}

void Twitter::dSum () {
	// convert hex to int
	std::stringstream ss;
	ss << std::hex << wordToCheck.substr(2, wordToCheck.length() - 1);
	int wordIndex;
	ss >> wordIndex;

	// traversing an unordered map to retrieve the word
	string retrievedWord;
	for (auto x : wordSum) {
		if ((x.second) == wordIndex) {
			wordToCheck = x.first;
			break;
		}
	}
	newTweet += (wordToCheck + " ");
}

void Twitter::dHash () {
	// conver hex to int
	std::stringstream ss;
	ss << std::hex << wordToCheck.substr(2, wordToCheck.length() - 1);
	int wordIndex;
	ss >> wordIndex;

	// retrieve the word
	map <int, string> ::iterator itr;
	for (itr = hashMap.begin(); itr != hashMap.end(); ++itr) {
		if (itr->first == wordIndex) {
			wordToCheck = itr->second;
		}
	}
	newTweet += (wordToCheck + " ");
}

Twitter::~Twitter() {}
