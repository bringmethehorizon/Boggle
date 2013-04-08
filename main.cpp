#include "Dictionary.h"

#define ROWS 4
#define COLS 4

wstring matrix[COLS]=
{
	L"МАЯК",
	L"БСЪЯ",
	L"КБРД",
	L"ЕЗПО"
};
set<wstring> results;

void solve()
{
	//Lower the letters
	for(int i=0; i<ROWS; i++)
		for(int j=0; j<COLS; j++)
			matrix[i][j]=towlower(matrix[i][j]);
	for(int i=0; i<ROWS; i++)
	{
		for(int j=0; j<COLS; j++)
		{
			wcout << matrix[i][j];
		}
		wcout << endl;
	}
}

int main()
{
	setlocale(LC_CTYPE, "");
	_setmode(_fileno(stdout), _O_U8TEXT);
	//// Do something
	//Dictionary dict("bg1.dict");	
	//auto start = chrono::steady_clock::now();
	//if(dict.wordExists(L"ябълка")) wcout << "Found" << endl;
	//else wcout << "Not found" << endl;
	////dict.printWords();	
	//auto end = chrono::steady_clock::now();
	//auto diff = chrono::duration <double, milli> (end-start);
	//wcout << "Exec:" 
	//	<< diff.count() 
	//	<< " ms" << endl;
	solve();
	system("pause");
	return 0;
}