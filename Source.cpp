#include <windows.h>

TCHAR szClassName[] = TEXT("Window");

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			const HDC hdc = BeginPaint(hWnd, &ps);
			SetTextColor(hdc, RGB(255, 255, 255));
			SetBkMode(hdc, TRANSPARENT);
			TextOut(hdc, 10, 10, TEXT("Hello, world!"), 13);
			EndPaint(hWnd, &ps);
		}
		break;
	case WM_NCHITTEST:
		wParam = DefWindowProc(hWnd, msg, wParam, lParam);
		if (wParam == HTCLIENT)
			return HTCAPTION;
		else
			return wParam;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

void SetWindowBlur(HWND hWnd)
{
	const HINSTANCE hModule = LoadLibrary(TEXT("user32.dll"));
	if (hModule)
	{
		struct ACCENTPOLICY
		{
			int nAccentState;
			int nFlags;
			int nColor;
			int nAnimationId;
		};
		struct WINCOMPATTRDATA
		{
			int nAttribute;
			PVOID pData;
			ULONG ulDataSize;
		};
		typedef BOOL(WINAPI*pSetWindowCompositionAttribute)(HWND, WINCOMPATTRDATA*);
		const pSetWindowCompositionAttribute SetWindowCompositionAttribute = (pSetWindowCompositionAttribute)GetProcAddress(hModule, "SetWindowCompositionAttribute");
		if (SetWindowCompositionAttribute)
		{
			ACCENTPOLICY policy = { 3, 0, 0, 0 };
			WINCOMPATTRDATA data = { 19, &policy, sizeof(ACCENTPOLICY) };
			SetWindowCompositionAttribute(hWnd, &data);
		}
		FreeLibrary(hModule);
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInst, LPSTR pCmdLine, int nCmdShow)
{
	MSG msg;
	WNDCLASS wndclass = {
		0,
		WndProc,
		0,
		0,
		hInstance,
		0,
		LoadCursor(0, IDC_ARROW),
		(HBRUSH)GetStockObject(BLACK_BRUSH),
		0,
		szClassName
	};
	RegisterClass(&wndclass);
	HWND hWnd = CreateWindow(
		szClassName,
		TEXT("Window"),
		WS_POPUPWINDOW,
		0,
		0,
		500,
		500,
		0,
		0,
		hInstance,
		0
		);
	SetWindowBlur(hWnd);
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);
	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}