#include "root.h"

LRESULT CALLBACK CsIDE::Window::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_CREATE:
		CsIDE::Window::SetUI(hwnd);
		return 0;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case NEW_POC:
			CsIDE::File::new_poc();
			break;
		case OPEN_POC:
			CsIDE::File::open_poc();
			break;
		default:
			break;
		}
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	default:
		return DefWindowProcW(hwnd, uMsg, wParam, lParam);
	}
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
	// 保存全局 hInstance，供 RegisterClass/CreateWindow 使用
	CsIDE::Window::hInstance = hInstance;

	// 创建窗口并显示
	CsIDE::Window::SetWindow(nCmdShow);

	// 启动消息循环（阻塞直到 WM_QUIT）
	CsIDE::Window::SetMessage();

	return 0;
}