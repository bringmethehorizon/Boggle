#include "resource.h"
#include "WordsSolver.h"

using namespace std;

#define hS1 123
#define hS2 124
#define hE1 125
#define hE2 126
#define hE3 127
#define hE4 128
#define hS 141
#define hL 142

#define hL1 141
#define hL2 142
#define hL3 143
#define hL4 144
#define hL5 145
#define hL6 146
#define hL7 147
#define hL8 148
#define hL9 149
#define hL10 150
#define hL11 151
#define hL12 152
#define hL13 153
#define hL14 154
#define hL15 155
#define hL16 156

extern map<wstring,wstring> results;

const char g_szClassName[] = "WordsClass";
HWND hStatic1,hStatic2,hEditControls[4],hListBox,hSearch,hLabelControls[16];
HFONT font,fontHighlighted;
thread th,th1;
HDC hDC;
PARAFORMAT pf;
CHARFORMAT cf;
HBRUSH hb;

bool marked[16];

void search()
{
	//marked[5]=1;
	memset(marked,0,sizeof(bool)*16);
	for(int i=0; i<16; i++)
	{
		RECT rect;
		GetClientRect(hLabelControls[i], &rect);
		InvalidateRect(hLabelControls[i], &rect, TRUE);
	}
	//wstring str[4];
	int k=0;bool ready=true;
	for(int i=0; i<4; i++)
	{
		wchar_t temp[5];
		GetWindowTextW(hEditControls[i],temp,5);
		if(wcslen(temp)<4) {ready=false;break;}
		matrix[i]=temp;
		//MessageBoxW(NULL,str[i].c_str(),L"FUN!",MB_OK);
		for(int j=0; j<4; j++)
		{
			wstring tmp;
			tmp+=toupper(matrix[i][j], locale(""));
			//if(tmp!=L"")
			SendMessageW(hLabelControls[k],WM_SETTEXT,0,(LPARAM)tmp.c_str());
			k++;
			//SendMessageW(hLabelControls[k],WM_set
		}
	}
	if(ready)
	{
		calculate();
	}

}

void initializeControls(HWND hwnd)
{
	font=CreateFont(20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "Times New Roman");
	fontHighlighted=CreateFont(20, 0, 0, 0, FW_DEMIBOLD, 0, 0, 0, 0, 0, 0, 0, 0, "Ariel Black");
	hStatic1=CreateWindowEx(NULL,"STATIC","Enter 4 letters in each line and press Search:",
		WS_VISIBLE|WS_CHILD|SS_CENTER,24,20,200,48,
		hwnd,(HMENU)hS1,GetModuleHandle(NULL),NULL);
	hStatic2=CreateWindowEx(NULL,"STATIC","Results:",
		WS_VISIBLE|WS_CHILD|SS_CENTER,270,50,200,48,
		hwnd,(HMENU)hS1,GetModuleHandle(NULL),NULL);
	for(int i=0; i<4; i++)
	{
		hEditControls[i]=CreateWindowExW(WS_EX_CLIENTEDGE,L"EDIT",L"",
			WS_TABSTOP|WS_CHILD|WS_VISIBLE|SS_CENTER,
			24,70+(30*i),100,24,hwnd,(HMENU)hE1,GetModuleHandle(NULL),NULL);
		SendMessage(hEditControls[i],EM_LIMITTEXT,4,0);
		SendMessage(hEditControls[i],WM_SETFONT,(WPARAM)font,0);

	}
	hSearch=CreateWindowExW(NULL,L"BUTTON",L"Search!",
		WS_TABSTOP|WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
		24,70+(30*5),230,30,hwnd,(HMENU)hS,GetModuleHandle(NULL),NULL);
	int k=0;
	for(int i=1; i<=4; i++)
	{
		for(int j=1; j<=4; j++)
		{
			hLabelControls[k]=CreateWindowEx(WS_EX_CLIENTEDGE,"STATIC","",
				WS_TABSTOP|WS_VISIBLE|WS_CHILD|SS_CENTER,
				110+(30*j),40+(30*i),24,24,
				hwnd,(HMENU)hS1,GetModuleHandle(NULL),NULL);
			SendMessage(hLabelControls[k],WM_SETFONT,(WPARAM)font,0);
			k++;
		}
	}

	hListBox = CreateWindowExW(WS_EX_CLIENTEDGE, L"listbox", L"", 
		WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_AUTOVSCROLL| LBS_NOTIFY, 
		270, 70, 200, 300, hwnd, (HMENU)hL, NULL, NULL);
	SendMessage(hListBox,WM_SETFONT,(WPARAM)font,0);

}

void clear()
{
	for(int i=0; i<4; i++)
	{
		SendMessage(hEditControls[i],WM_SETTEXT,0,(LPARAM)L"");
	}
	for(int i=0; i<16; i++)
	{
		SendMessage(hLabelControls[i],WM_SETTEXT,0,(LPARAM)L"");
	}
	SendMessage(hListBox,LB_RESETCONTENT,0,0);
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_CREATE:
		setlocale(LC_CTYPE, "");
		_setmode(_fileno(stdout), _O_U8TEXT);
		_setmode(_fileno(stdin), _O_U16TEXT);
		memset(marked,0,sizeof(bool)*16);
		dict.loadFile("bg1.dict");
		//th.join();
		initializeControls(hwnd);
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case hS:
			{
				search();
			}
			break;
		case ID_FILE_CLEAR:
			clear();
			break;
		case ID_FILE_EXIT:
			PostQuitMessage(0);
			break;
		case ID_HELP_ABOUT:
			MessageBoxW(hwnd,L"Words Solver v1.0\nAuthor: Teodor Merodiyski\nCopyright 2013",L"About",MB_OK);
			break;
		case hL:
			switch(HIWORD(wParam))
			{
			case CBN_SELCHANGE:
				int index = SendMessage(hListBox, LB_GETCURSEL, 0, 0);
				wchar_t buff[100];
				RECT rect;
				SendMessageW(hListBox,LB_GETTEXT,index,(LPARAM)buff);
				wstring str=results[buff];
				memset(marked,0,sizeof(bool)*16);
				wchar_t* p;
				p=wcstok(&str[0],L":");
				while(p!=NULL)
				{
					int t1=_wtoi(p);
					p=wcstok(NULL,L":");
					int t2=_wtoi(p);
					p=wcstok(NULL,L":");
					marked[4*t1+t2]=1;
				}
				for(int i=0; i<16; i++)
				{
					GetClientRect(hLabelControls[i], &rect);
					InvalidateRect(hLabelControls[i], &rect, TRUE);
				}
				
				
				

				//hDC=GetDC(hLabelControls[0]);
				//SetTextColor(hDC,RGB(255,0,0));
				//SendMessage(hLabelControls[3],WM_SETFONT,(WPARAM)fontHighlighted,0);
				//SendMessage( hwnd, EM_REPLACESEL, FALSE, (LPARAM) "red ");
				break;
			}
			break;
		}
		break;

	case WM_CTLCOLORSTATIC:
		{
			SetBkMode((HDC)wParam,TRANSPARENT);
			
			for(int i=0; i<16; i++)
			{
				if((HWND)lParam==hLabelControls[i] && marked[i])
				{
					//hDC = reinterpret_cast<HDC>(wParam);
					SetTextColor((HDC)wParam, RGB(0xFF, 0x00, 0x00));
				}
			}
		}
		return (BOOL)CreateSolidBrush (GetSysColor(COLOR_MENU));
		//break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wc;
	HWND hwnd;
	MSG Msg;


	wc.cbSize        = sizeof(WNDCLASSEX);
	wc.style         = 0;
	wc.lpfnWndProc   = WndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInstance;
	wc.hIcon         = LoadIcon(NULL, MAKEINTRESOURCE(IDI_ICON1));
	wc.hIconSm		 = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wc.lpszMenuName  = MAKEINTRESOURCE(IDR_MENU1);
	wc.lpszClassName = g_szClassName;

	if(!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Window Registration Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		g_szClassName,
		"Words Solver v1.0",
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, 500, 450,
		NULL, NULL, hInstance, NULL);

	if(hwnd == NULL)
	{
		MessageBox(NULL, "Window Creation Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while(GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}
