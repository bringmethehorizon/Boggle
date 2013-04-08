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

using namespace std;

class Dictionary
{
public:
	Dictionary(string);
	~Dictionary(void);
	void printWords();
	bool wordExists(wstring);
private:
	set<wstring> data;
	//vector<wstring> data;
};

