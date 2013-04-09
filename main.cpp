#include "Dictionary.h"

#define ROWS 4
#define COLS 4

Dictionary dict;	
wstring matrix[COLS];
//wstring matrix[COLS]=
//{
//	L"МАЯК",
//	L"БСЪЯ",
//	L"КБРД",
//	L"ЕЗПО"
//};

//set<wstring> results;
vector<wstring> results;
mutex results_mutex;

bool used[ROWS][ROWS][ROWS];

void solver(int x, int y, wstring so_far, char depth, int thread_number)
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
	used[thread_number][x][y]=true;
	depth++;
	if(dict.wordExists(so_far)) 
	{
		results_mutex.lock();
		results.push_back(so_far);
		results_mutex.unlock();
	}//results.insert(so_far);
	if(x-1>=0 && !used[thread_number][x-1][y]) solver(x-1,y,so_far,depth,thread_number);
	if(x+1<ROWS && !used[thread_number][x+1][y]) solver(x+1,y,so_far,depth,thread_number);
	if(y-1>=0 && !used[thread_number][x][y-1]) solver(x,y-1,so_far,depth,thread_number);
	if(y+1<COLS && !used[thread_number][x][y+1]) solver(x,y+1,so_far,depth,thread_number);
	used[thread_number][x][y]=false;
}

void solve()
{
	thread t[ROWS];
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
		t[i]=thread([=]
		{		
			for(int j=0; j<COLS; j++)
			{
				solver(i,j,L"",0,i);
			}
		});
	}
	for(int i=0; i<ROWS; i++)
	{

		t[i].join();
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
	//SetConsoleOutputCP(1251);
	//SetConsoleCP(1251);
	setlocale(LC_CTYPE, "");
	_setmode(_fileno(stdout), _O_U8TEXT);
	_setmode(_fileno(stdin), _O_U16TEXT);
	thread th([]{dict.loadFile("bg1.dict");});	
	//wchar_t c1;
	memset(&used,0,ROWS*ROWS*ROWS);

	wcout << L"Enter the matrix (4 lines with 4 letters NO SPACES):" << endl;
	//wcin >> c1;
	//wcout << c1 << endl;
	//system("pause");
	//return 0;
	for(int i=0; i<ROWS; i++)
	{
		wcin >> matrix[i];
		if(matrix[i].length()!=4)
		{
			wcout << L"\aYou made an error.\nEnter the matrix again from the beginning:" << endl;
			//i=0;
		}
	}
	wcout << L"\nSearching..." << endl;
	th.join();
	auto start = chrono::steady_clock::now();
	solve();	
	auto end = chrono::steady_clock::now();
	auto diff = chrono::duration <double, milli> (end-start);
	wcout << L"Search time:" 
		<< diff.count() 
		<< L" ms" << endl;
	sort(results.begin(),results.end(),comp);
	for(auto i: results)
	{
		wcout << i << endl;
	}
	system("pause");
	wcout << "Exiting...";
	return 0;
}