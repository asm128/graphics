#include "framework.h"

#include <chrono>

#include <Windows.h>

struct SApplication {
	WNDCLASSEX						WindowClass				= {sizeof(WNDCLASSEX),};
	const char						WindowClassName	[256]	= "demos_window";
	::gph::SCoord<int32_t>			WindowPosition			= {100, 100};
	::gph::SCoord<int32_t>			WindowSize				= {640, 480};
	HWND							WindowHandle			= 0;
	::gph::SColor					* Pixels				= 0;
	uint64_t						CountFrames				= 0;
	uint64_t						TimeDelta				= 0;
	::gph::SCoord<double>			RectPosition			= {};
	::gph::SCoord<int32_t>			RectSize				= {};
	::gph::SColor					ShapeColors	[4]			=
		{ {0xff, 0, 0, 0}
		, {0, 0xff, 0, 0}
		, {0, 0, 0xff, 0}
		, {0xff, 0xff, 0, 0}
		};
};

LRESULT WINAPI							wndProc				(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)		{
	switch(uMsg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int										drawBuffer			(HDC hdc, ::gph::SColor* pixels, ::gph::SCoord<int32_t>	targetSize) {
	BITMAPINFO									bitmapInfo			= {{sizeof(BITMAPINFO)},{}};
	bitmapInfo.bmiHeader.biWidth			= targetSize.x;
	bitmapInfo.bmiHeader.biHeight			= targetSize.y;
	bitmapInfo.bmiHeader.biPlanes			= 1;
	bitmapInfo.bmiHeader.biBitCount			= 32;
	bitmapInfo.bmiHeader.biCompression		= BI_RGB;
	bitmapInfo.bmiHeader.biSizeImage		= targetSize.x * targetSize.y * sizeof(::gph::SColor);
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

int									update					(SApplication & app)		{
	double									secondsLastFrame		= app.TimeDelta * .000001;
	::gph::view<::gph::SColor>				view_colors				= app.ShapeColors;

	app.RectSize						= {app.WindowSize.x / 2, app.WindowSize.y / 2};
	app.RectPosition.x					+= secondsLastFrame * 100;
	if(app.RectPosition.x > app.WindowSize.x)
		app.RectPosition.x					= 0;
	for(int32_t y = 0; y < 2; ++y)
	for(int32_t x = 0; x < 2; ++x) {
		int32_t									colorIndex				= y * 2 + x;
		const ::gph::SColor						colorRectangle			= view_colors[(colorIndex + 0) % ::std::size(app.ShapeColors)];
		const ::gph::SColor						colorCircle				= view_colors[(colorIndex + 1) % ::std::size(app.ShapeColors)];
		const ::gph::SColor						colorLineHorizontal		= view_colors[(colorIndex + 2) % ::std::size(app.ShapeColors)];
		const ::gph::SColor						colorLineVertical		= view_colors[(colorIndex + 3) % ::std::size(app.ShapeColors)];

		::gph::drawRectangle		(app.Pixels, app.WindowSize, ::gph::SCoord<int32_t>{(int32_t)(app.RectPosition.x * x)		, (int32_t)(app.RectPosition.y * y)}		, app.RectSize				, colorRectangle			);
		::gph::drawCircle			(app.Pixels, app.WindowSize, ::gph::SCoord<int32_t>{(int32_t)(app.RectPosition.x * x)		, (int32_t)(app.RectPosition.y * y)}		+ (app.WindowSize / 4), 16	, colorCircle				);
		::gph::drawLineVertical		(app.Pixels, app.WindowSize, ::gph::SCoord<int32_t>{(int32_t)(app.RectPosition.x * x)		, (int32_t)(app.RectPosition.y * y) - 8}	+ (app.WindowSize / 4), 16	, colorLineHorizontal		);
		::gph::drawLineHorizontal	(app.Pixels, app.WindowSize, ::gph::SCoord<int32_t>{(int32_t)(app.RectPosition.x * x) - 8	, (int32_t)(app.RectPosition.y * y)}		+ (app.WindowSize / 4), 16	, colorLineVertical			);
	}
	return 0;
}

int									setup				(SApplication & app)		{
	app.Pixels							= (::gph::SColor*)malloc(sizeof(::gph::SColor) * app.WindowSize.x * app.WindowSize.y);

	WNDCLASSEX								& wndClass			= app.WindowClass;
	wndClass.lpszClassName				= app.WindowClassName;
	wndClass.lpfnWndProc				= wndProc;
	wndClass.hInstance					= GetModuleHandle(0);
	void									* result			= (void*)RegisterClassEx(&wndClass);
	if(0 == result)
		return -1;
	app.WindowHandle					= CreateWindowEx(0, app.WindowClassName, "Window 0", WS_OVERLAPPEDWINDOW
		, app.WindowPosition.x
		, app.WindowPosition.y
		, app.WindowSize.x
		, app.WindowSize.y
		, 0, 0, wndClass.hInstance, 0
	);
	if(0 == app.WindowHandle)
		return -1;
	ShowWindow(app.WindowHandle, SW_SHOW);
	return 0;
}

int									cleanup				(SApplication & app)		{
	free(app.Pixels);
	UnregisterClass(app.WindowClassName, app.WindowClass.hInstance);
	return 0;
}

int									windowUpdate		(SApplication & app)		{
	MSG										msg					= {};
	bool									running				= true;
	while(PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if(msg.message == WM_QUIT)
			running								= false;
	}
	RECT									clientRect			= {};
	GetClientRect(app.WindowHandle, &clientRect);
	if( app.WindowSize.x != clientRect.right
	 || app.WindowSize.y != clientRect.bottom
	) {
		app.WindowSize.x					= clientRect.right	;
		app.WindowSize.y					= clientRect.bottom;
		free(app.Pixels);
		app.Pixels							= (::gph::SColor*)malloc(sizeof(::gph::SColor) * app.WindowSize.x * app.WindowSize.y);
	}
	return running ? 0 : 1;
}

int									main				()		{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_DELAY_FREE_MEM_DF);	// Windows only

	SApplication							app;
	if(-1 == setup(app))
		return EXIT_FAILURE;

	bool									running				= true;
	app.RectPosition					= {app.WindowSize.x / 2.0, app.WindowSize.y / 2.0};
	app.RectSize						= {app.WindowSize.x / 2, app.WindowSize.y / 2};
	::std::chrono::time_point<::std::chrono::system_clock>	timerPrevious		= ::std::chrono::system_clock::now();
	::std::chrono::time_point<::std::chrono::system_clock>	timerCurrent		= ::std::chrono::system_clock::now();
	while(running) {
		if(::windowUpdate(app))
			running								= false;
		timerCurrent						= ::std::chrono::system_clock::now();
		app.TimeDelta						= ::std::chrono::duration_cast<::std::chrono::microseconds>(timerCurrent - timerPrevious).count();
		timerPrevious						= timerCurrent;

		::update(app);

		HDC										dc					= GetDC(app.WindowHandle);
		::drawBuffer(dc, app.Pixels, app.WindowSize);
		ReleaseDC(app.WindowHandle, dc);

		++app.CountFrames;
	}
	::cleanup(app);
	return EXIT_SUCCESS;
}
