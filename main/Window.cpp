#include <Windows.h>
#include <string>
#include <commctrl.h>
#include "root.h"

void CsIDE::Window::SetWindow(int nCmdShow) {
	WNDCLASSEXW wcx;

	ZeroMemory(&wcx, sizeof(wcx));

	wcx.cbSize = sizeof(wcx);
	wcx.style = CS_HREDRAW | CS_VREDRAW;
	wcx.lpfnWndProc = WndProc;
	wcx.cbClsExtra = 0;
	wcx.cbWndExtra = 0;
	wcx.hInstance = hInstance;
	wcx.hIcon = LoadIconW(NULL, IDI_APPLICATION);
	wcx.hCursor = LoadCursorW(NULL, IDC_ARROW);
	wcx.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcx.lpszMenuName = nullptr;
	wcx.lpszClassName = L"ciclass";
	wcx.hIconSm = LoadIconW(NULL, IDI_APPLICATION);

	if (!RegisterClassExW(&wcx)) {
		std::wstring err = L"注册窗口类错误，错误代码： " + std::to_wstring(GetLastError());
		
		MessageBoxW(NULL, err.c_str(), L"错误", MB_OK | MB_ICONERROR);
		return;
	}

	hwnd = CreateWindowExW(0, wcx.lpszClassName, L"ChenIDE", WS_OVERLAPPEDWINDOW,
		0, 0, 800, 600, nullptr, nullptr, hInstance, nullptr);

	if (!hwnd) {
		std::wstring err = L"创建主窗口错误，错误代码： " + std::to_wstring(GetLastError());

		MessageBoxW(NULL, err.c_str(), L"错误", MB_OK | MB_ICONERROR);

		return;
	}

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);
}

void CsIDE::Window::SetMessage(void) {
	MSG msg;

	// 正确的消息循环：GetMessage 返回 >0 表示收到消息，0 表示 WM_QUIT，-1 错误
	while (GetMessageW(&msg, nullptr, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
}

void CsIDE::Window::SetUI(HWND hwndp) {
	HMENU hmenu = CreateMenu();
	HMENU hFileMenu = CreatePopupMenu();

	AppendMenuW(hFileMenu, MF_STRING, NEW_POC, L"新建项目");
	AppendMenuW(hFileMenu, MF_STRING, OPEN_POC, L"打开项目");

	// 将文件子菜单作为弹出菜单添加到主菜单
	AppendMenuW(hmenu, MF_POPUP, (UINT_PTR)hFileMenu, L"文件");

	// 将菜单绑定到主窗口
	SetMenu(hwndp, hmenu);

	RECT rcClient;
	GetClientRect(hwndp, &rcClient);

	INITCOMMONCONTROLSEX icex = { 0 };

	icex.dwSize = sizeof(icex);
	icex.dwICC = ICC_BAR_CLASSES;

	if (!InitCommonControlsEx(&icex)) {
		MessageBoxW(hwndp, L"初始化 Common Controls 失败", L"错误", MB_OK | MB_ICONERROR);
		return;
	}

	HWND htoos = CreateWindowExW(0,TOOLBARCLASSNAME, nullptr,
		WS_CHILD | WS_VISIBLE | TBSTYLE_WRAPABLE,
		0, 0, 50, 30,
		hwndp, (HMENU)TOOLBAR_ID, hInstance, nullptr);

	if (!htoos) {
		std::wstring err = L"创建工具栏错误，错误代码： " + std::to_wstring(GetLastError());

		MessageBoxW(NULL, err.c_str(), L"错误", MB_OK | MB_ICONERROR);
	}

	TBBUTTON tbs[2] = { 0 };
	TBADDBITMAP tbBitmap = { 0 };

	tbBitmap.hInst = HINST_COMMCTRL;
	tbBitmap.nID = IDB_STD_SMALL_COLOR;

	SendMessageW(htoos, TB_ADDBITMAP, 0, (LPARAM)&tbBitmap);

	ZeroMemory(&tbs, sizeof(tbs));

	tbs[0].iBitmap = STD_FILENEW;
	tbs[0].idCommand = NEW_POC;
	tbs[0].fsState = TBSTATE_ENABLED;
	tbs[0].fsStyle = TBSTYLE_BUTTON;

	tbs[1].iBitmap = STD_FILEOPEN;
	tbs[1].idCommand = OPEN_POC;
	tbs[1].fsState = TBSTATE_ENABLED;
	tbs[1].fsStyle = TBSTYLE_BUTTON;

	SendMessageW(htoos, TB_ADDBUTTONS, (WPARAM)2, (LPARAM)&tbs);
}