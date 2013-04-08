#include "Dictionary.h"

int main()
{
	_setmode(_fileno(stdout), _O_U16TEXT);
	Dictionary dict("bg1.dict");
	dict.printWords();
	system("pause");
	return 0;
}