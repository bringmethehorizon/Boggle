#include "Dictionary.h"

int wmain(int argc, wchar_t* argv[])
{
    _setmode(_fileno(stdout), _O_U16TEXT);
	Dictionary dict("bg.dict");
	for(auto i: dict.data)
	{
		wcout << i << endl;
	}
	system("pause");
}