#include "root.h"

namespace CsIDE {
	// 在单一 .cpp 中定义全局实例
	dat data;

	namespace Window {
		HINSTANCE hInstance = nullptr;
		HWND hwnd = nullptr;
	}
}