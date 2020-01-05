#include "framework.h"
#include "gph_timer.h"

#include <Windows.h>

struct SWindow {
	WNDCLASSEX								Class				= {sizeof(WNDCLASSEX),};
	const char								ClassName	[256]	= "demos_window";
	::gph::SCoord<int32_t>					Position			= {100, 100};
	::gph::SCoord<int32_t>					Size				= {640, 480};
	HWND									Handle				= 0;
};

;


struct SScene {
	::gph::container<::gph::SCoord<float>>	ParticlePositions		= {};
	::gph::container<float>					ParticleSpeeds			= {};

	::gph::SCoord<double>					RectPosition			= {};
	::gph::SCoord<int32_t>					RectSize				= {};
	::gph::SColor							ShapeColors	[4]			=
		{ {0xff, 0, 0, 0}
		, {0, 0xff, 0, 0}
		, {0, 0, 0xff, 0}
		, {0xff, 0xff, 0, 0}
		};
};

struct SApplication {
	SWindow									Window					= {};
	::gph::STimer							Timer					= {};
	::SScene								Scene					= {};
	uint64_t								CountFrames				= 0;
	::gph::container<::gph::SColor>			Pixels					= {};
};

LRESULT WINAPI							wndProc				(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)		{
	switch(uMsg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int										drawBuffer			(HDC hdc, ::gph::view_grid<::gph::SColor> pixels) {
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

int									update					(SApplication & app)		{
	app.Timer.Tick();
	double									secondsLastFrame		= app.Timer.TimeDelta * .000001;
	::gph::view<::gph::SColor>				view_colors				= app.Scene.ShapeColors;

	app.Scene.RectSize					= {app.Window.Size.x / 2, app.Window.Size.y / 2};
	app.Scene.RectPosition.x			+= secondsLastFrame * 100;
	if( app.Scene.RectPosition.x > app.Window.Size.x )
		app.Scene.RectPosition.x					= 0;
	for(int32_t y = 0; y < 2; ++y)
	for(int32_t x = 0; x < 2; ++x) {
		int32_t									colorIndex				= y * 2 + x;
		const ::gph::SColor						colorRectangle			= view_colors[(colorIndex + 0ULL) % ::std::size(app.Scene.ShapeColors)];
		const ::gph::SColor						colorCircle				= view_colors[(colorIndex + 1ULL) % ::std::size(app.Scene.ShapeColors)];
		const ::gph::SColor						colorLineHorizontal		= view_colors[(colorIndex + 2ULL) % ::std::size(app.Scene.ShapeColors)];
		const ::gph::SColor						colorLineVertical		= view_colors[(colorIndex + 3ULL) % ::std::size(app.Scene.ShapeColors)];

		::gph::drawRectangle		({app.Pixels.begin(), app.Window.Size.Cast<uint32_t>()}, ::gph::SCoord<int32_t>{(int32_t)(app.Scene.RectPosition.x * x)		, (int32_t)(app.Scene.RectPosition.y * y)}		, app.Scene.RectSize		, colorRectangle			);
		::gph::drawCircle			({app.Pixels.begin(), app.Window.Size.Cast<uint32_t>()}, ::gph::SCoord<int32_t>{(int32_t)(app.Scene.RectPosition.x * x)		, (int32_t)(app.Scene.RectPosition.y * y)}		+ (app.Window.Size / 4), 16	, colorCircle				);
		::gph::drawLineVertical		({app.Pixels.begin(), app.Window.Size.Cast<uint32_t>()}, ::gph::SCoord<int32_t>{(int32_t)(app.Scene.RectPosition.x * x)		, (int32_t)(app.Scene.RectPosition.y * y) - 8}	+ (app.Window.Size / 4), 16	, colorLineHorizontal		);
		::gph::drawLineHorizontal	({app.Pixels.begin(), app.Window.Size.Cast<uint32_t>()}, ::gph::SCoord<int32_t>{(int32_t)(app.Scene.RectPosition.x * x) - 8	, (int32_t)(app.Scene.RectPosition.y * y)}		+ (app.Window.Size / 4), 16	, colorLineVertical			);
	}

	for(uint32_t iParticle = 0; iParticle < app.Scene.ParticlePositions.size(); ++iParticle) {
		::gph::SCoord<float>						& particlePosition		= app.Scene.ParticlePositions[iParticle];
		float										& particleSpeed			= app.Scene.ParticleSpeeds[iParticle];
		particlePosition.y						+= (float)(particleSpeed * secondsLastFrame);
		if( particlePosition.y >= app.Window.Size.y)
			particlePosition.y						= 0;
		if(0 == (rand() % 16)) ++particlePosition.x;
		if(0 == (rand() % 16)) --particlePosition.x;
		if(particlePosition.x < 0) particlePosition.x = 0;
		if(particlePosition.x >= app.Window.Size.x) particlePosition.x = app.Window.Size.x - 1.0f;
			 if(0 == (rand() % 16)) particleSpeed *= 0.9999999f;
		else if(0 == (rand() % 16)) particleSpeed *= 1.1111111f;
		if(particleSpeed > 330) particleSpeed = 330;
		if(particleSpeed < 1) particleSpeed = 1;
		::gph::setPixel({app.Pixels.begin(), app.Window.Size.Cast<uint32_t>()}, {(int32_t)particlePosition.x, (int32_t)(app.Window.Size.y - particlePosition.y)}, view_colors[iParticle % view_colors.size()]);
	}

	++app.CountFrames;
	return 0;
}

int									windowSetup	(SWindow & window)		{
	WNDCLASSEX								& wndClass			= window.Class;
	wndClass.lpszClassName				= window.ClassName;
	wndClass.lpfnWndProc				= wndProc;
	wndClass.hInstance					= GetModuleHandle(0);
	void									* result			= (void*)RegisterClassEx(&wndClass);
	if(0 == result)
		return -1;
	window.Handle					= CreateWindowEx(0, window.ClassName, "Window 0", WS_OVERLAPPEDWINDOW
		, window.Position.x
		, window.Position.y
		, window.Size.x
		, window.Size.y
		, 0, 0, wndClass.hInstance, 0
	);
	if(0 == window.Handle)
		return -1;
	ShowWindow(window.Handle, SW_SHOW);
	return 0;
}

int									setup				(SApplication & app)		{
	app.Window.Position					= {100, 100};
	app.Window.Size						= {640, 480};
	app.Pixels.resize(app.Window.Size.x * app.Window.Size.y);
	memset(app.Pixels.begin(), 0, app.Pixels.size() * sizeof(::gph::SColor));
	if(-1 == windowSetup(app.Window))
		return -1;

	app.Scene.ParticlePositions	.resize(256);
	app.Scene.ParticleSpeeds	.resize(256);
	memset(app.Scene.ParticlePositions	.begin(), 0, app.Scene.ParticlePositions.size() * sizeof(::gph::SCoord<float>));
	memset(app.Scene.ParticleSpeeds		.begin(), 0, app.Scene.ParticleSpeeds	.size() * sizeof(float));
	for(uint32_t iParticle = 0; iParticle < app.Scene.ParticlePositions.size(); ++iParticle) {
		::gph::SCoord<float>					& particlePosition		= app.Scene.ParticlePositions[iParticle];
		float									& particleSpeed			= app.Scene.ParticleSpeeds[iParticle];
		particlePosition.x					= (float)(rand() % app.Window.Size.x);
		particleSpeed						= 9.8f;
	}
	return 0;
}

int									cleanup				(SApplication & app)		{
	UnregisterClass(app.Window.ClassName, app.Window.Class.hInstance);
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
	GetClientRect(app.Window.Handle, &clientRect);
	if( app.Window.Size.x != clientRect.right
	 || app.Window.Size.y != clientRect.bottom
	) {
		app.Window.Size.x					= clientRect.right	;
		app.Window.Size.y					= clientRect.bottom;
		app.Pixels.resize(app.Window.Size.x * app.Window.Size.y);
		memset(app.Pixels.begin(), 0, app.Pixels.size() * sizeof(::gph::SColor));
	}
	return running ? 0 : 1;
}

int									main				()		{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_DELAY_FREE_MEM_DF);	// Windows only

	SApplication							app;
	if(-1 == setup(app))
		return EXIT_FAILURE;

	bool									running				= true;
	app.Scene.RectPosition				= {app.Window.Size.x / 2.0, app.Window.Size.y / 2.0};
	app.Scene.RectSize					= {app.Window.Size.x / 2, app.Window.Size.y / 2};
	while(running) {
		if(::windowUpdate(app))
			running								= false;

		::update(app);

		HDC										dc					= GetDC(app.Window.Handle);
		::drawBuffer(dc, {app.Pixels.begin(), app.Window.Size.Cast<uint32_t>()});
		ReleaseDC(app.Window.Handle, dc);
	}
	::cleanup(app);
	return EXIT_SUCCESS;
}
