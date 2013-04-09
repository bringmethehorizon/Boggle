#include "Dictionary.h"

#define ROWS 4
#define COLS 4

Dictionary dict("bg1.dict");	

wstring matrix[COLS]=
{
	L"МАЯК",
	L"БСЪЯ",
	L"КБРД",
	L"ЕЗПО"
};

//set<wstring> results;
vector<wstring> results;
mutex results_mutex;
void solver(int x, int y, wstring so_far, char depth)
{
	if(so_far.length()==2)
	{
		if(!dict.pairExists(so_far)) return;
	}
	if(so_far.length()==3)
	{
		if(!dict.tripleExists(so_far)) return;
	}
	//if(so_far.empty()) return;
	if(depth==MAX_DEPTH) return;
	so_far+=matrix[x][y];
	depth++;
	if(dict.wordExists(so_far)) 
	{
		//results_mutex.lock();
		results.push_back(so_far);
		//results_mutex.unlock();
	}//results.insert(so_far);
	if(x-1>=0) solver(x-1,y,so_far,depth);
	if(x+1<ROWS) solver(x+1,y,so_far,depth);
	if(y-1>=0) solver(x,y-1,so_far,depth);
	if(y+1<COLS) solver(x,y+1,so_far,depth);
}

void solve()
{
	thread* t[ROWS];
	//Lower the letters
	for(int i=0; i<ROWS; i++)
		for(int j=0; j<COLS; j++)
			matrix[i][j]=towlower(matrix[i][j]);
	//for(int i=0; i<ROWS; i++)
	//{
	//	for(int j=0; j<COLS; j++)
	//	{
	//		wcout << matrix[i][j];
	//	}
	//	wcout << endl;
	//}
	for(int i=0; i<ROWS; i++)
	{
		t[i]=new thread([=]
		{		
			for(int j=0; j<COLS; j++)
			{
				solver(i,j,L"",0);
			}
		});
	}
	for(auto i: t)
	{
		i->join();
	}
}

struct comparator
{
	bool operator()(wstring s1, wstring s2)
	{
		return (s1.length()>s2.length());
	}
} comp;

int main()
{
	setlocale(LC_CTYPE, "");
	_setmode(_fileno(stdout), _O_U8TEXT);
	//// Do something
	//Dictionary dict("bg1.dict");	
	auto start = chrono::steady_clock::now();
	//if(dict.wordExists(L"ябълка")) wcout << "Found" << endl;
	//else wcout << "Not found" << endl;
	////dict.printWords();	

	solve();	
	auto end = chrono::steady_clock::now();
	auto diff = chrono::duration <double, milli> (end-start);
	wcout << "Exec:" 
		<< diff.count() 
		<< " ms" << endl;
	sort(results.begin(),results.end(),comp);
	for(auto i: results)
	{
		wcout << i << endl;
	}
	system("pause");
	return 0;
}