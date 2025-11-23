#pragma once

#include <Windows.h>

#define NEW_POC 101
#define OPEN_POC 102
#define TOOLBAR_ID 103

namespace CsIDE {
	struct dat {
		PWSTR XmName_Path = nullptr;
		PWSTR XmFileName = nullptr;
		PWSTR XmForder = nullptr;
		bool isSave = false;
	};

	// 声明（不在头文件中定义实例）
	extern dat data;

	namespace Window {
		void SetWindow(int nCmdShow);
		void SetUI(HWND hwndp);
		void SetMessage(void);

		// 声明窗口相关全局句柄
		extern HINSTANCE hInstance;
		extern HWND hwnd;

		LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	};

	namespace File {
		void open_poc(void);
		void new_poc(void);
		INT_PTR CALLBACK WDpRoc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	};
};