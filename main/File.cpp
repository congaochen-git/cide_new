#include "root.h"
#include "resource.h"
#include <ShlObj.h>
#include <string>
#include <commdlg.h>

#pragma comment(lib,"Shell32.lib")

INT_PTR CALLBACK CsIDE::File::WDpRoc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_BUTTON1: {
			wchar_t folderPath[MAX_PATH] = { 0 };
			BROWSEINFOW bi = { 0 };
			LPITEMIDLIST pidl;

			bi.hwndOwner = hwnd;
			bi.lpszTitle = L"选择项目文件夹";
			bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
			pidl = SHBrowseForFolderW(&bi);

			if (pidl != NULL) {
				SHGetPathFromIDListW(pidl, folderPath);
				CsIDE::data.XmForder = _wcsdup(folderPath);

				SetDlgItemTextW(hwnd, IDC_EDIT2, folderPath);
			}
			else {
				MessageBoxW(hwnd, L"未选择文件夹", L"错误", MB_OK | MB_ICONERROR);
				return FALSE;
			}
		}
		case IDCANCEL:
			EndDialog(hwnd, IDCANCEL);
			return TRUE;
		case IDOK: {
			wchar_t nameBuffer[256] = { 0 };

			GetDlgItemTextW(hwnd, IDC_EDIT1, nameBuffer, 256);

			if (wcslen(nameBuffer) == 0) {
				MessageBoxW(hwnd, L"项目名称不能为空", L"错误", MB_OK | MB_ICONERROR);
				return FALSE;
			}

			CsIDE::data.XmFileName = _wcsdup(nameBuffer);

			EndDialog(hwnd, IDOK);
			return TRUE;
		}
		default:
			break;
		}
		break;
	}

	return FALSE;
}
void CsIDE::File::new_poc(void) {
	if (!DialogBoxW(CsIDE::Window::hInstance, MAKEINTRESOURCEW(IDD_DIALOG1), CsIDE::Window::hwnd, WDpRoc)) {
		MessageBoxW(CsIDE::Window::hwnd, L"窗口错误", L"错误", MB_OK | MB_ICONERROR);
	}
}
void CsIDE::File::open_poc(void) {
	OPENFILENAMEW ofn = { 0 };
	wchar_t fd[MAX_PATH] = { 0 };

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = CsIDE::Window::hwnd;
	ofn.lpstrFilter = L"cide文件\0*.poc\0所有文件\0*.*\0";
	ofn.lpstrTitle = L"选择文件";
	ofn.lpstrFile = fd;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;

	if (!GetOpenFileNameW(&ofn)) {
		DWORD err = CommDlgExtendedError();

		if (err != 0) {
			wchar_t msg[128] = { 0 };

			swprintf_s(msg, _countof(msg), L"对话框错误: 0x%08X", err);

			MessageBoxW(CsIDE::Window::hwnd, msg, L"错误", MB_OK | MB_ICONERROR);
		}
		else {
			// 用户取消或没有错误码
			MessageBoxW(CsIDE::Window::hwnd, L"已取消或未选择文件。", L"信息", MB_OK | MB_ICONINFORMATION);
		}

		return;
	}

	// 成功，复制路径到全局数据（与其他代码风格一致，使用 _wcsdup）
	CsIDE::data.XmName_Path = _wcsdup(fd);
}