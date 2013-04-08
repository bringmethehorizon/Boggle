#pragma once
#include <iostream>
#include <set>
#include <string>
#include <fstream>
#include <locale>
#include <codecvt>
#include <io.h>
#include <fcntl.h>

using namespace std;

class Dictionary
{
public:
	Dictionary(string);
	~Dictionary(void);
	void printWords();
private:
	set<wstring> data;
};

