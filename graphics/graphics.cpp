#define NOMINMAX
#include <Windows.h>
#include <cstdint>
#include <algorithm>

LRESULT WINAPI				wndProc				(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)		{
	switch(uMsg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

struct SCoord {
	int32_t						x;
	int32_t						y;
};


double						clamp				(double value, double min, double max)			{ return ::std::min(::std::max(min, value), max); }

struct SColor {
	uint8_t						r;
	uint8_t						g;
	uint8_t						b;
	uint8_t						a;

	SColor						operator/			(double scalar)	const	noexcept				{
		return
			{ (uint8_t)clamp(r / scalar, 0, 255)
			, (uint8_t)clamp(g / scalar, 0, 255)
			, (uint8_t)clamp(b / scalar, 0, 255)
			, (uint8_t)clamp(a / scalar, 0, 255)
			};
	}
	SColor						operator*			(double scalar)	const	noexcept				{
		return
			{ (uint8_t)clamp(r * scalar, 0, 255)
			, (uint8_t)clamp(g * scalar, 0, 255)
			, (uint8_t)clamp(b * scalar, 0, 255)
			, (uint8_t)clamp(a * scalar, 0, 255)
			};
	}
	operator					COLORREF			()			const noexcept		{
		return	((COLORREF)r)
			|	((COLORREF)g) << 8
			|	((COLORREF)b) << 16
			;
	}
};

int							drawRectangle
	(	HDC		dc
	,	SCoord	rectPosition
	,	SCoord	rectSize
	,	SColor	color
	) {
	for(int32_t y = 0; y < rectSize.y; ++y)
	for(int32_t x = 0; x < rectSize.x; ++x)
		SetPixel(dc, rectPosition.x + x, rectPosition.y + y, color * ((double)y * .1) );
	return 0;
}
int							setPixel
	(	SColor	* pixels
	,	SCoord	targetSize
	,	SCoord	position
	,	SColor	color
	) {
	if( position.x >= 0 && position.x < targetSize.x
	 &&	position.y >= 0 && position.y < targetSize.y
	)
		pixels[position.y * targetSize.x + position.x] = color;
	return 0;
}
int							drawRectangle
	(	SColor	* pixels
	,	SCoord	targetSize
	,	SCoord	rectPosition
	,	SCoord	rectSize
	,	SColor	color
	) {
	for(int32_t y = 0; y < rectSize.y; ++y)
	for(int32_t x = 0; x < rectSize.x; ++x)
		setPixel(pixels, targetSize, {rectPosition.x + x, rectPosition.y + y}, color * ((double)y * .1) );
	return 0;
}

int										drawBuffer			(HDC hdc, SColor* pixels, SCoord targetSize) {
	BITMAPINFO									bitmapInfo			= {{sizeof(BITMAPINFO)},{}};
	bitmapInfo.bmiHeader.biWidth			= targetSize.x;
	bitmapInfo.bmiHeader.biHeight			= targetSize.y;
	bitmapInfo.bmiHeader.biPlanes			= 1;
	bitmapInfo.bmiHeader.biBitCount			= 32;
	bitmapInfo.bmiHeader.biCompression		= BI_RGB;
	bitmapInfo.bmiHeader.biSizeImage		= targetSize.x * targetSize.y * sizeof(SColor);
	bitmapInfo.bmiHeader.biXPelsPerMeter	= 0x0ec4;
	bitmapInfo.bmiHeader.biYPelsPerMeter	= 0x0ec4;
	HDC											hdcCompatible		= CreateCompatibleDC(hdc);
	if(hdcCompatible) {
		char										* ppvBits			= 0;
		HBITMAP										backBitmap			= CreateDIBSection(hdcCompatible, &bitmapInfo, DIB_RGB_COLORS, (void**)&ppvBits, 0, 0);
		if(backBitmap) {
			SetDIBits(hdcCompatible, backBitmap, 0, targetSize.y, pixels, &bitmapInfo, DIB_RGB_COLORS);
			HBITMAP										oldBitmap			= (HBITMAP)SelectObject(hdcCompatible, backBitmap);
			BitBlt(hdc, 0, 0, targetSize.x, targetSize.y, hdcCompatible, 0, 0, SRCCOPY);
			SelectObject(hdcCompatible, oldBitmap);
			DeleteObject(backBitmap);
		}
		DeleteDC(hdcCompatible);
	}
	return 0;
}

int							main				()		{
	WNDCLASSEX						wndClass			= {sizeof(WNDCLASSEX),};
	const char						wndClassName	[]	= "demos_ndow";
	wndClass.lpszClassName		= wndClassName;
	wndClass.lpfnWndProc		= wndProc;
	wndClass.hInstance			= GetModuleHandle(0);
	void							* result			= (void*)RegisterClassEx(&wndClass);
	if(0 == result)
		return EXIT_FAILURE;

	SCoord							windowPosition		= {100, 100};
	SCoord							windowSize			= {640, 480};

	HWND							hWnd				= CreateWindowEx(0, wndClassName, "Window 0", WS_OVERLAPPEDWINDOW
		, windowPosition.x
		, windowPosition.y
		, windowSize.x
		, windowSize.y
		, 0, 0, wndClass.hInstance, 0
	);
	ShowWindow(hWnd, SW_SHOW);
	bool							running				= true;
	SColor							colors	[4]			=
	{ {0xff, 0, 0, 0}
	, {0, 0xff, 0, 0}
	, {0, 0, 0xff, 0}
	, {0xff, 0xff, 0, 0}
	};

	SColor							* pixels			= (SColor*)malloc(sizeof(SColor) * windowSize.x * windowSize.y);
	while(running) {
		MSG								msg					= {};
		while(PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if(msg.message == WM_QUIT)
				running						= false;
		}
		RECT							clientRect			= {};
		GetClientRect(hWnd, &clientRect);
		if( windowSize.x != clientRect.right
		 || windowSize.y != clientRect.bottom
		) {
			windowSize.x				= clientRect.right	;
			windowSize.y				= clientRect.bottom;
			free(pixels);
			pixels						= (SColor*)malloc(sizeof(SColor) * windowSize.x * windowSize.y);
		}

		SCoord							rectPosition		= {windowSize.x / 2, windowSize.y / 2};
		SCoord							rectSize			= {windowSize.x / 2, windowSize.y / 2};
		for(int32_t y = 0; y < 2; ++y)
		for(int32_t x = 0; x < 2; ++x)
			drawRectangle(pixels, windowSize, {rectPosition.x * x, rectPosition.y * y}, rectSize, colors[y * 2 + x]);

		HDC								dc					= GetDC(hWnd);
		drawBuffer(dc, pixels, windowSize);
		ReleaseDC(hWnd, dc);
	}
	UnregisterClass(wndClassName, wndClass.hInstance);
	free(pixels);
	return EXIT_SUCCESS;
}
