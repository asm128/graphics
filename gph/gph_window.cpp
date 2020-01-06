#include "gph_window.h"

LRESULT WINAPI							wndProc				(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)		{
	switch(uMsg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int									gph::windowSetup			(::gph::SWindow & window, ::gph::SCoord2<uint32_t> size)		{
	window.Size							= size.Cast<int32_t>();

	WNDCLASSEX								& wndClass			= window.Class;
	wndClass.lpszClassName				= window.ClassName;
	wndClass.lpfnWndProc				= wndProc;
	wndClass.hInstance					= GetModuleHandle(0);
	void									* result			= (void*)RegisterClassEx(&wndClass);
	if(0 == result)
		return -1;
	RECT									windowRect			=
		{ 0
		, 0
		, (int32_t)window.Size.x
		, (int32_t)window.Size.y
		};
	AdjustWindowRectEx(&windowRect, WS_OVERLAPPEDWINDOW, FALSE, 0);

	window.Handle						= CreateWindowEx(0, window.ClassName, "Window 0", WS_OVERLAPPEDWINDOW
		, window.Position.x
		, window.Position.y
		, windowRect.right - windowRect.left
		, windowRect.bottom - windowRect.top
		, 0, 0, wndClass.hInstance, 0
	);
	if(0 == window.Handle)
		return -1;
	ShowWindow(window.Handle, SW_SHOW);
	return 0;
}

int									gph::windowCleanup		(::gph::SWindow & window)		{
	UnregisterClass(window.ClassName, window.Class.hInstance);
	return 0;
}


int									gph::windowUpdate		(::gph::SWindow & window)		{
	MSG										msg					= {};
	bool									running				= true;
	window.Resized						= false;
	while(PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if(msg.message == WM_QUIT)
			running								= false;
	}
	RECT									clientRect			= {};
	GetClientRect(window.Handle, &clientRect);
	if( window.Size.x != clientRect.right
	 || window.Size.y != clientRect.bottom
	) {
		window.Size.x					= clientRect.right	;
		window.Size.y					= clientRect.bottom;
		window.Resized					= true;
	}
	return running ? 0 : 1;
}

int										gph::drawBuffer			(HDC hdc, ::gph::view_grid<::gph::SColor> pixels) {
	BITMAPINFO									bitmapInfo			= {{sizeof(BITMAPINFO)},{}};
	bitmapInfo.bmiHeader.biWidth			= pixels.metrics().x;
	bitmapInfo.bmiHeader.biHeight			= pixels.metrics().y;
	bitmapInfo.bmiHeader.biPlanes			= 1;
	bitmapInfo.bmiHeader.biBitCount			= 32;
	bitmapInfo.bmiHeader.biCompression		= BI_RGB;
	bitmapInfo.bmiHeader.biSizeImage		= pixels.metrics().x * pixels.metrics().y * sizeof(::gph::SColor);
	bitmapInfo.bmiHeader.biXPelsPerMeter	= 0x0ec4;
	bitmapInfo.bmiHeader.biYPelsPerMeter	= 0x0ec4;
	HDC											hdcCompatible		= CreateCompatibleDC(hdc);
	if(hdcCompatible) {
		char										* ppvBits			= 0;
		HBITMAP										backBitmap			= CreateDIBSection(hdcCompatible, &bitmapInfo, DIB_RGB_COLORS, (void**)&ppvBits, 0, 0);
		if(backBitmap) {
			SetDIBits(hdcCompatible, backBitmap, 0, pixels.metrics().y, pixels.begin(), &bitmapInfo, DIB_RGB_COLORS);
			HBITMAP										oldBitmap			= (HBITMAP)SelectObject(hdcCompatible, backBitmap);
			BitBlt(hdc, 0, 0, pixels.metrics().x, pixels.metrics().y, hdcCompatible, 0, 0, SRCCOPY);
			SelectObject(hdcCompatible, oldBitmap);
			DeleteObject(backBitmap);
		}
		DeleteDC(hdcCompatible);
	}
	return 0;
}
