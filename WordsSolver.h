#include "Dictionary.h"

#define ROWS 4
#define COLS 4

Dictionary dict;	
wstring matrix[COLS];
extern HWND hListBox;
extern HWND hSearch;

//vector<wstring> results;

mutex results_mutex;

bool used[ROWS][ROWS][ROWS];


class comparator1
{
public:
	bool operator()(const wstring& s1, const wstring& s2)
	{
		return (s1.length()>s2.length());
	}
};

map<wstring,wstring> results;

void solver(int x, int y, wstring so_far, char depth, int thread_number,wstring positions)
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
	positions+=to_wstring(x)+L":"+to_wstring(y)+L":";
	if(dict.wordExists(so_far)) 
	{
		results_mutex.lock();
		//results.push_back(so_far);
		results.insert(pair<wstring,wstring>(so_far,positions));
		results_mutex.unlock();
	}
	if(x-1>=0 && !used[thread_number][x-1][y]) solver(x-1,y,so_far,depth,thread_number,positions);
	if(x+1<ROWS && !used[thread_number][x+1][y]) solver(x+1,y,so_far,depth,thread_number,positions);
	if(y-1>=0 && !used[thread_number][x][y-1]) solver(x,y-1,so_far,depth,thread_number,positions);
	if(y+1<COLS && !used[thread_number][x][y+1]) solver(x,y+1,so_far,depth,thread_number,positions);

	if(x-1>=0 && y-1>=0 && !used[thread_number][x-1][y-1]) solver(x-1,y-1,so_far,depth,thread_number,positions);
	if(x+1<ROWS && y-1>=0 && !used[thread_number][x+1][y-1]) solver(x+1,y-1,so_far,depth,thread_number,positions);
	if(x-1>=0 && y+1<COLS && !used[thread_number][x-1][y+1]) solver(x-1,y+1,so_far,depth,thread_number,positions);
	if(x+1<ROWS && y+1<COLS && !used[thread_number][x+1][y+1]) solver(x+1,y+1,so_far,depth,thread_number,positions);
	used[thread_number][x][y]=false;
}

void solve()
{
	thread t[ROWS];
	//Lower the letters
	for(int i=0; i<ROWS; i++)
		for(int j=0; j<COLS; j++)
			matrix[i][j]=towlower(matrix[i][j]);

	for(int i=0; i<ROWS; i++)
	{
		t[i]=thread([=]
		{		
			for(int j=0; j<COLS; j++)
			{
				solver(i,j,L"",0,i,L"");
			}
		});
	}
	for(int i=0; i<ROWS; i++)
	{

		t[i].join();
	}
}

void calculate()
{
	vector<wstring> v;
	EnableWindow(hSearch,FALSE);
	SendMessageW(hSearch,WM_SETTEXT,0,(LPARAM)L"Searching...");
	results.clear();
	SendMessageW(hListBox,LB_RESETCONTENT,0,0);
	memset(&used,0,ROWS*ROWS*ROWS);
	solve();
	//sort(results.begin(),results.end(),comp);
	//results.erase( unique( results.begin(), results.end() ), results.end() );
	for(auto i: results)
	{
		v.push_back(i.first);
	}
	sort(v.begin(),v.end(),comparator1());
		
	for(auto i: v)
	{
		SendMessageW(hListBox, LB_ADDSTRING, 0, (LPARAM)i.c_str());
		//wcout << i << endl;
	}
	SendMessageW(hSearch,WM_SETTEXT,0,(LPARAM)L"Search!");
	EnableWindow(hSearch,TRUE);
	//MessageBoxW(NULL,L"FUN",L"Кеф",MB_OK);
}