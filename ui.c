#include "ui.h"
#include <windows.h>
#include <wingdi.h>
#include <stdio.h>
#include <windowsx.h>
#include <assert.h>

#define MAP_LEFT 10
#define MAP_TOP 80
#define BLOCK_WIDTH 40

#define IDC_START 1
#define IDC_CONFIGURE 2
#define IDC_QUIT 3

TCHAR szClassName[] = TEXT("MineSweeperClass");
HWND hWnd;

struct RECTWH {
	LONG left;
	LONG top;
	ULONG width;
	ULONG height;
};

static LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

static BOOL g_bInit = FALSE;

static STARTBTN_CLICK	g_lpfnStartButtonClickCallBack;
static MAP_CLICK	g_lpfnMapClickCallBack;
static MAPCONFIG_CHANGE	g_lpfnMapConfigChangeCallBack;

static CHAR	 g_aMap[50][50];

static UINT g_uWidth,g_uHeight;
static RECT rcStartButton;

static BOOL g_bMapEnable = FALSE;

static HMENU hRootMenu;

static const DWORD dwWindowStyle = WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX;

static void __CreateMenu() {
	HMENU hGameMenu;

	hRootMenu = CreateMenu();
	hGameMenu = CreatePopupMenu();

	if(hRootMenu == 0) {
		printf("CreateMenu error %d\n",GetLastError());
		return;
	}

	AppendMenu(hRootMenu,MF_POPUP,(UINT_PTR)hGameMenu,TEXT("Game"));
	AppendMenu(hGameMenu,MF_STRING,IDC_START,TEXT("Start(&S)"));
	AppendMenu(hGameMenu,MF_STRING,IDC_CONFIGURE,TEXT("Configure(&C)"));
	AppendMenu(hGameMenu,MF_SEPARATOR,IDC_QUIT,0);
	AppendMenu(hGameMenu,MF_STRING,2,TEXT("Quit"));

}

int ui_init(void) {
	
	WNDCLASSEX wc;
	HINSTANCE hInstance; 
	
	assert(g_bInit == 0);

	hInstance = GetModuleHandleA(NULL);

	wc.cbWndExtra = 0;
	wc.cbClsExtra = 0;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = szClassName;

	wc.lpszMenuName = NULL;
	wc.style = 0;
	RegisterClassEx(&wc);

	__CreateMenu();

	hWnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, szClassName, TEXT("Mine Sweeper"), dwWindowStyle, 400, 400, 800, 800, NULL, hRootMenu, hInstance, NULL);

	//EnumChildWindows(hWnd, EnumChildProc, 0);
	
	memset(g_aMap,-1,sizeof(g_aMap));

	UpdateWindow(hWnd);

	g_bMapEnable = TRUE;

	g_bInit = TRUE;
	return 0;

}

int ui_show(void) {
	assert(g_bInit == TRUE);
	ShowWindow(hWnd, SW_SHOW);
	return 0;
}

int ui_map_clear(void) {
	memset(g_aMap,-1,sizeof(g_aMap));
	ui_map_refresh();
	return 0;
}

int ui_map_setenable(int enable) {
	assert(g_bInit == TRUE);
	g_bMapEnable = enable;
	return 0;
}

int ui_map_setsize(unsigned int width,unsigned int height) {
	RECT  rcMainWindow,rcClient;
	printf("ui_map_setsize()\n");
	assert(g_bInit == TRUE);
	g_uWidth = width;
	g_uHeight = height;
	SetRect(&rcClient, 0, 0, MAP_LEFT*2 + g_uWidth*BLOCK_WIDTH + 5, MAP_TOP + MAP_LEFT + g_uHeight*BLOCK_WIDTH + 5);
	GetWindowRect(hWnd, &rcMainWindow);   //获得程序窗口位于屏幕坐标
	//GetClientRect(hWnd, &rcClient);
	AdjustWindowRect(&rcClient, dwWindowStyle/*|WS_SYSMENU|WS_SYSMENU*//*&(~WS_SIZEBOX)*/, TRUE);
	//MoveWindow(hWnd,rcMainWindow.left,rcMainWindow.top,,0,TRUE);
	MoveWindow(hWnd,rcMainWindow.left,rcMainWindow.top,rcClient.right-rcClient.left, rcClient.bottom - rcClient.top, TRUE);
	//SetWindowPos(hWnd,NULL,(BUF_LEFT + rc,0,0,0,SWP_NOZORDER|SWP_NOSIZE);

	ui_map_refresh();
	return 0;
}
/*
int ui_mines_count_show(int n);
int ui_mines_rest_show(int n);
*/
int ui_reg_startbtn_click(STARTBTN_CLICK callback) {
	assert(callback != NULL);
	g_lpfnStartButtonClickCallBack = callback;
	return 0;
}

int ui_reg_map_click(MAP_CLICK callback) {
	assert(callback != NULL);
	g_lpfnMapClickCallBack = callback;
	return 0;
}

int ui_reg_mapconfig_change(MAPCONFIG_CHANGE callback) {
	assert(callback != NULL);
	g_lpfnMapConfigChangeCallBack = callback;
	return 0;
}

int ui_map_setblock(int x,int y,char n) {
	assert(g_bInit == TRUE);
	assert(n>=-1 && n<=9);
	g_aMap[x][y] = n;
	
}

int ui_map_refresh(void) {
	RECT rcClient={0};
	assert(g_bInit == TRUE);
	//GetClientRect(hWnd,&rcClient);
	//UpdateWindow(hWnd);
	InvalidateRect(hWnd,NULL,FALSE);
	return 0;
}

int ui_show_message(char *msg) {
	MessageBoxA(hWnd, msg, "Mine Sweeper", MB_OK);
}

int ui_loop() {
	MSG msg;
	assert(g_bInit == TRUE);
	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}

void ui_quit() {
	PostQuitMessage(0);
}

static void GetBlockRect(INT x, INT y, LPRECT pRect) {
	pRect->left = MAP_LEFT + x * BLOCK_WIDTH;
	pRect->top = MAP_TOP + y * BLOCK_WIDTH;
	pRect->right = pRect->left + BLOCK_WIDTH;
	pRect->bottom = pRect->top + BLOCK_WIDTH;
}

static void DrawStartButton(HDC hDC) {
	TCHAR		szStart[] = TEXT("Start");
	HBRUSH		hBrush = CreateSolidBrush(GetSysColor(COLOR_BTNHILIGHT));
	rcStartButton.right = g_uWidth * BLOCK_WIDTH + MAP_LEFT;
	rcStartButton.left = rcStartButton.right - 120;
	rcStartButton.top = 10;
	rcStartButton.bottom = 70;
	
	

	FillRect(hDC, &rcStartButton, hBrush);
	FrameRect(hDC, &rcStartButton, hBrush);
	DrawText(hDC, szStart, -1, &rcStartButton, DT_SINGLELINE|DT_CENTER|DT_VCENTER);
}

static void DrawMap(HDC hDC) {
	int 		i,j;
	POINT 		pt;
	RECT		rcClient,rcBlock;
	HBRUSH 		hMineBrush, hDefaultBrush, hNumBrush, hZeroBrush;
	TCHAR		szNumber[2];
	
	//DWORD		dwBackColor;
	GetClientRect(hWnd,&rcClient);
	printf("w h = %d %d\n",g_uWidth,g_uHeight);
	hDefaultBrush = GetStockObject(WHITE_BRUSH);
	hMineBrush = GetStockObject(BLACK_BRUSH);
	hNumBrush = GetStockObject(DKGRAY_BRUSH);
	hZeroBrush = GetStockObject(GRAY_BRUSH);
	for(i = 0; i < g_uHeight; i++) {
		for(j = 0; j < g_uWidth; j++) {
			GetBlockRect(j, i, &rcBlock);
			switch(g_aMap[j][i]) {
			case -1:
				SelectObject(hDC, hDefaultBrush);
				FillRect(hDC, &rcBlock, hDefaultBrush);
				break;
			case 9:
				SelectObject(hDC, hMineBrush);
				FillRect(hDC, &rcBlock, hMineBrush);
				break;
			case 0:
				SelectObject(hDC, hZeroBrush);
				FillRect(hDC, &rcBlock, hZeroBrush);
				break;
			default:
				assert(g_aMap[j][i] >= 1 && g_aMap[j][i] <= 8);
				SelectObject(hDC, hNumBrush);
				szNumber[1] = TEXT('\0');
				szNumber[0] = TEXT('0') + g_aMap[j][i];
				FillRect(hDC, &rcBlock, hNumBrush);
				DrawText(hDC,szNumber,-1,&rcBlock,DT_SINGLELINE|DT_CENTER|DT_VCENTER);
				break;
			}

		}
	}

	for(i = 0; i <= g_uHeight; i++) {
		MoveToEx(hDC, MAP_LEFT, MAP_TOP+BLOCK_WIDTH*i, &pt);
		LineTo(hDC, MAP_LEFT+BLOCK_WIDTH*(g_uHeight), MAP_TOP+BLOCK_WIDTH*i);
	}
	for(i=0; i <= g_uWidth; i++) {
		MoveToEx(hDC, MAP_LEFT + BLOCK_WIDTH * i, MAP_TOP, &pt);
		LineTo(hDC , MAP_LEFT + BLOCK_WIDTH * i,MAP_TOP+BLOCK_WIDTH * (g_uWidth ));
	}

}



static LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	TCHAR wszInfo[1024];
	HINSTANCE thisInstance = GetModuleHandle(NULL);
	PAINTSTRUCT  ps;
	switch (Msg)
	{
	case (WM_USER + 1):
		/*
		KillTimer(hWnd, IDT_TIMER1);
		Sleep(10);
		CloseAVI(hAvi);
		*/
		break;
	case WM_CREATE:
		//__CreateMenu();
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam)) {
		case IDC_START:
			printf("0\n");
			break;
		case IDC_CONFIGURE:
			break;
		case IDC_QUIT:
			break;
		}
		/*
		if ((HWND)lParam == hChooseWnd)
		*/
		break;
	case WM_PAINT:
		{
			//RECT rcInfo = {10,10,80,20};
			HDC hDC=BeginPaint(hWnd,&ps);
			SetBkMode(hDC,TRANSPARENT);
			//DrawText(hDC,TEXT("0 / 0"),-1,&rcInfo,DT_SINGLELINE|DT_CENTER|DT_VCENTER);
			DrawStartButton(hDC);
			DrawMap(hDC);
			EndPaint(hWnd,&ps);
		}
		break;
	case WM_LBUTTONDOWN:
		{
			LONG x,y;
			x = GET_X_LPARAM(lParam); 
			y = GET_Y_LPARAM(lParam); 
			printf("%d , %d\n",x,y);
			if(g_bMapEnable == TRUE
				&&g_lpfnMapClickCallBack != NULL
				&&x > MAP_LEFT && x < MAP_LEFT + BLOCK_WIDTH * g_uWidth 
				&& y > MAP_TOP && y < MAP_TOP + BLOCK_WIDTH * g_uHeight) {
				g_lpfnMapClickCallBack((x - MAP_LEFT) / BLOCK_WIDTH , (y - MAP_TOP) / BLOCK_WIDTH);
			} else if(g_lpfnStartButtonClickCallBack != NULL
				&& x > rcStartButton.left && x < rcStartButton.right 
				&& y > rcStartButton.top && y < rcStartButton.bottom) {
				g_lpfnStartButtonClickCallBack();

			}



			//ui_map_refresh();

		}
		break;
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, Msg, wParam, lParam);
	}
	return 0;
}
