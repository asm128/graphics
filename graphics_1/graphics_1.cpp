#include "framework.h"

int									cleanup					(::gph::SApplication & app)		{ (void)app; return 0; }
int									setup					(::gph::SApplication & app)		{
	::gph::SFramework						& framework				= app.Framework;
	::gph::SWindow							& window				= framework.Window;
	framework.Pixels.resize(window.Size.x * window.Size.y);
	memset(framework.Pixels.begin(), 0, framework.Pixels.size() * sizeof(::gph::SColor));

	app.Scene.Rectangles.resize(4);
	for(int32_t y = 0; y < 2; ++y)
	for(int32_t x = 0; x < 2; ++x) {
		::gph::SRectangle<int32_t>				& rectangle					= app.Scene.Rectangles[y * 2 + x];
		rectangle.Size						= {window.Size.x / 2, window.Size.y / 2};
		rectangle.Offset					= {rectangle.Size.x * x, rectangle.Size.y * y};
	}
	return 0;
}

int									update					(::gph::SApplication & app)		{
	::gph::SFramework						& framework				= app.Framework;
	::gph::SWindow							& window				= framework.Window;
	double									secondsLastFrame		= framework.Timer.TimeDelta * .000001;
	(void)secondsLastFrame;
	if(window.Resized) {
		framework.Pixels.resize(window.Size.x * window.Size.y);
		memset(framework.Pixels.begin(), 0, framework.Pixels.size() * sizeof(::gph::SColor));
		for(int32_t y = 0; y < 2; ++y)
		for(int32_t x = 0; x < 2; ++x) {
			::gph::SRectangle<int32_t>				& rectangle					= app.Scene.Rectangles[y * 2 + x];
			rectangle.Size						= {window.Size.x / 2, window.Size.y / 2};
			rectangle.Offset					= {rectangle.Size.x * x, rectangle.Size.y * y};
		}
	}
	::gph::view<::gph::SColor>				view_colors				= app.Scene.ShapeColors;
	::gph::view_grid<::gph::SColor>			pixels					= {framework.Pixels.begin(), window.Size.Cast<uint32_t>()};
	for(uint32_t iRectangle = 0; iRectangle < app.Scene.Rectangles.size(); ++iRectangle) {
		const ::gph::SColor						colorRectangle			= view_colors[iRectangle % ::std::size(app.Scene.ShapeColors)] / 2;
		::gph::drawRectangle(pixels, {app.Scene.Rectangles[iRectangle].Offset.Cast<int32_t>(), app.Scene.Rectangles[iRectangle].Size.Cast<int32_t>()}, colorRectangle);
	}
	framework.Timer.Tick();
	++framework.CountFrames;
	return 0;
}

int									main				()		{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_DELAY_FREE_MEM_DF);	// Windows only

	::gph::SApplication						app;
	::gph::SWindow							& window				= app.Framework.Window;
	window.Position						= {100, 100};
	if(-1 == ::gph::windowSetup(window, {1360, 768}))
		return -1;
	if(-1 == setup(app))
		return EXIT_FAILURE;

	bool									running				= true;
	while(running) {
		if(::gph::windowUpdate(window))
			running								= false;

		::update(app);

		HDC										dc					= GetDC(window.Handle);
		::gph::drawBuffer(dc, {app.Framework.Pixels.begin(), window.Size.Cast<uint32_t>()});
		ReleaseDC(window.Handle, dc);
	}
	::cleanup(app);

	::gph::windowCleanup(window);
	return EXIT_SUCCESS;
}
