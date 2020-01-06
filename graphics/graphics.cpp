#include "framework.h"
#include "gph_timer.h"

#include <Windows.h>

struct SScene {
	::gph::container<::gph::SCoord2<float>>	ParticlePositions		= {};
	::gph::container<float>					ParticleSpeeds			= {};

	::gph::SRectangle<double>				Rectangle				= {};
	::gph::SColor							ShapeColors	[4]			=
		{ {0xff, 0, 0, 0}
		, {0, 0xff, 0, 0}
		, {0, 0, 0xff, 0}
		, {0xff, 0xff, 0, 0}
		};
};

struct SWindow {
	WNDCLASSEX								Class					= {sizeof(WNDCLASSEX),};
	const char								ClassName	[256]		= "demos_window";
	::gph::SCoord2<int32_t>					Position				= {100, 100};
	::gph::SCoord2<int32_t>					Size					= {640, 480};
	HWND									Handle					= 0;
};

struct SApplication {
	const uint32_t							MAX_PARTICLES			= 64;
	const double							GRAVITY					= 9.8;

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

	app.Scene.Rectangle.Size			= {app.Window.Size.x / 2.0, app.Window.Size.y / 2.0};
	app.Scene.Rectangle.Offset.x		+= secondsLastFrame * 100;

	::gph::view_grid<::gph::SColor>			pixels					= {app.Pixels.begin(), app.Window.Size.Cast<uint32_t>()};
	if( app.Scene.Rectangle.Offset.x > app.Window.Size.x )
		app.Scene.Rectangle.Offset.x		= 0;

	::gph::STriangle<int32_t>						triangle				= {};
	::gph::SRectangle<int32_t>						rectangle				= {{}, app.Scene.Rectangle.Size.Cast<int32_t>()};
	::gph::SCircle<int32_t>							circle					= {{}, 16};
	for(int32_t y = 0; y < 2; ++y)
	for(int32_t x = 0; x < 2; ++x) {
		::gph::container<::gph::SCoord2<int32_t>>		pixelCoords;
		int32_t									colorIndex				= y * 2 + x;
		const ::gph::SColor						colorRectangle			= view_colors[(colorIndex + 0ULL) % ::std::size(app.Scene.ShapeColors)];
		const ::gph::SColor						colorCircle				= view_colors[(colorIndex + 1ULL) % ::std::size(app.Scene.ShapeColors)];
		const ::gph::SColor						colorLineHorizontal		= view_colors[(colorIndex + 2ULL) % ::std::size(app.Scene.ShapeColors)];
		const ::gph::SColor						colorLineVertical		= view_colors[(colorIndex + 3ULL) % ::std::size(app.Scene.ShapeColors)];
		triangle.A							= {10 * y + (int32_t)(app.Scene.Rectangle.Offset.x * x) / 6	, y * 50 + (int32_t)(app.Scene.Rectangle.Offset.x * x) / 2};
		triangle.B							= {20 * y + (int32_t)(app.Scene.Rectangle.Offset.x * x) / 2	, y * 60 + (int32_t)(app.Scene.Rectangle.Offset.x * x) / 8};
		triangle.C							= {30 * y + (int32_t)(app.Scene.Rectangle.Offset.x * x) / 4	, y * 70 + (int32_t)(app.Scene.Rectangle.Offset.x * x)};
		triangle.A							-= app.Scene.Rectangle.Offset.Cast<int32_t>() * x;
		triangle.B							-= app.Scene.Rectangle.Offset.Cast<int32_t>() * x;
		triangle.C							-= app.Scene.Rectangle.Offset.Cast<int32_t>() * x;
		triangle.A.Rotate(app.CountFrames / 10.0);
		triangle.B.Rotate(app.CountFrames / 10.0);
		triangle.C.Rotate(app.CountFrames / 10.0);
		triangle.A							+= app.Scene.Rectangle.Offset.Cast<int32_t>() * x;
		triangle.B							+= app.Scene.Rectangle.Offset.Cast<int32_t>() * x;
		triangle.C							+= app.Scene.Rectangle.Offset.Cast<int32_t>() * x;
		rectangle	.Offset					= {(int32_t)(app.Scene.Rectangle.Offset.x * x)			, (int32_t)(app.Scene.Rectangle.Offset.y * y)};
		circle		.Offset					= ::gph::SCoord2<int32_t>{(int32_t)(app.Scene.Rectangle.Offset.x * x), (int32_t)(app.Scene.Rectangle.Offset.y * y)} + (app.Window.Size / 4);
		::gph::SCoord2<int32_t>					offsetLineVertical		= ::gph::SCoord2<int32_t>{(int32_t)(app.Scene.Rectangle.Offset.x * x)		, (int32_t)(app.Scene.Rectangle.Offset.y * y) - 8}	+ (app.Window.Size / 4);
		::gph::SCoord2<int32_t>					offsetLineHorizontal	= ::gph::SCoord2<int32_t>{(int32_t)(app.Scene.Rectangle.Offset.x * x) - 8	, (int32_t)(app.Scene.Rectangle.Offset.y * y)}		+ (app.Window.Size / 4);

		//::gph::drawRectangle		(pixels, rectangle	, colorRectangle);
		//::gph::drawCircle			(pixels, circle		, colorCircle);
		//::gph::drawTriangle			(pixels, triangle	, colorLineHorizontal);
		::gph::drawTriangle				(pixels.metrics(), triangle, pixelCoords);
		//::gph::drawLineVertical		(pixels, offsetLineVertical		, 16	, colorLineVertical		);
		//::gph::drawLineHorizontal	(pixels, offsetLineHorizontal	, 16	, colorLineHorizontal	);

		::gph::SLine<int32_t>			line0	= {::gph::SCoord2<int32_t>{+8, }	, ::gph::SCoord2<int32_t>{-8, 16}};
		::gph::SLine<int32_t>			line1	= {::gph::SCoord2<int32_t>{0, -8}	, ::gph::SCoord2<int32_t>{16, +8}};
		line0.A.Rotate((double)app.CountFrames);
		line0.B.Rotate((double)app.CountFrames);
		line1.B.Rotate((double)app.CountFrames);
		line1.A.Rotate((double)app.CountFrames);
		line0.A += offsetLineVertical;
		line0.B += offsetLineVertical;
		line1.B += offsetLineHorizontal;
		line1.A += offsetLineHorizontal;
		::gph::drawLine				(pixels, line0, colorLineVertical	);
		::gph::drawLine				(pixels, line1, colorLineHorizontal	);

		::gph::drawLine				(pixels, {triangle.A, triangle.B}, colorCircle);
		::gph::drawLine				(pixels, {triangle.B, triangle.C}, colorCircle);
		::gph::drawLine				(pixels, {triangle.C, triangle.A}, colorCircle);
		for(uint32_t iPixelCoord = 0; iPixelCoord < pixelCoords.size(); ++iPixelCoord) {
			const ::gph::SCoord2<int32_t>			& pixelCoord	=  pixelCoords[iPixelCoord];
			setPixel(pixels, pixelCoord, {(uint8_t)(pixelCoord.x + triangle.A.x), (uint8_t)(pixelCoord.y + triangle.A.y), (uint8_t)(pixelCoord.x + pixelCoord.y)});
		}
	}

	for(uint32_t iParticle = 0; iParticle < app.Scene.ParticlePositions.size(); ++iParticle) {
		::gph::SCoord2<float>					& particlePosition		= app.Scene.ParticlePositions[iParticle];
		float									& particleSpeed			= app.Scene.ParticleSpeeds[iParticle];
		particleSpeed						+= float(app.GRAVITY * secondsLastFrame);
		particlePosition.y					+= (float)(particleSpeed * secondsLastFrame);
		if( particlePosition.y >= app.Window.Size.y)
			particlePosition.y					= 0;
		if(0 == (rand() % 16)) ++particlePosition.x;
		if(0 == (rand() % 16)) --particlePosition.x;
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
	window.Handle						= CreateWindowEx(0, window.ClassName, "Window 0", WS_OVERLAPPEDWINDOW
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
	app.Window.Size						= {1280, 720};
	app.Pixels.resize(app.Window.Size.x * app.Window.Size.y);
	memset(app.Pixels.begin(), 0, app.Pixels.size() * sizeof(::gph::SColor));
	if(-1 == windowSetup(app.Window))
		return -1;

	app.Scene.ParticlePositions	.resize(app.MAX_PARTICLES);
	app.Scene.ParticleSpeeds	.resize(app.MAX_PARTICLES);
	memset(app.Scene.ParticlePositions	.begin(), 0, app.Scene.ParticlePositions.size() * sizeof(::gph::SCoord2<float>));
	memset(app.Scene.ParticleSpeeds		.begin(), 0, app.Scene.ParticleSpeeds	.size() * sizeof(float));
	for(uint32_t iParticle = 0; iParticle < app.Scene.ParticlePositions.size(); ++iParticle) {
		::gph::SCoord2<float>					& particlePosition		= app.Scene.ParticlePositions[iParticle];
		float									& particleSpeed			= app.Scene.ParticleSpeeds[iParticle];
		particlePosition.x					= (float)(rand() % app.Window.Size.x);
		particleSpeed						= float(rand() % 9);
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
	app.Scene.Rectangle.Offset			= {app.Window.Size.x / 2.0, app.Window.Size.y / 2.0};
	app.Scene.Rectangle.Size			= {app.Window.Size.x / 2.0, app.Window.Size.y / 2.0};
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
