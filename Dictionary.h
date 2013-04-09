#pragma once
#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <fstream>
#include <locale>
#include <codecvt>
#include <io.h>
#include <fcntl.h>
#include <chrono>
#include <algorithm>
#include <thread>
#include <mutex>
#include <Windows.h>

#define MAX_DEPTH 10

using namespace std;

class Dictionary
{
public:
	Dictionary(void);
	~Dictionary(void);
	void loadFile(string);
	void printWords();
	bool wordExists(wstring);
	bool tripleExists(wstring);
	bool pairExists(wstring);
private:
	set<wstring> data;
	set<wstring> triples;
	set<wstring> pairs;
	//vector<wstring> data;
};

