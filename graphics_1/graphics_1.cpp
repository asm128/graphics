#include "framework.h"

const double						MATH_PI					= 3.1415;
const double						MATH_2PI				= 2 * MATH_PI;
// Vertex coordinates for cube faces
static constexpr const ::gph::STriangle3D<float>						geometryCube	[12]						=
	{ {{1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}}	// Right	- first			?? I have no idea if this is correct lol
	, {{1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f}}	// Right	- second		?? I have no idea if this is correct lol

	, {{0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}}	// Back		- first			?? I have no idea if this is correct lol
	, {{0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}}	// Back		- second		?? I have no idea if this is correct lol

	, {{1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}}	// Bottom	- first			?? I have no idea if this is correct lol
	, {{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}}	// Bottom	- second		?? I have no idea if this is correct lol

	, {{1.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}}	// Left		- first
	, {{1.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0f}}	// Left		- second

	, {{1.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}}	// Front	- first
	, {{1.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 0.0f}}	// Front	- second

	, {{1.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}}	// Top		- first
	, {{1.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 1.0f}}	// Top		- second
	};
int									cleanup					(::gph::SApplication & app)		{ (void)app; return 0; }
int									setup					(::gph::SApplication & app)		{
	::gph::SFramework						& framework				= app.Framework;
	::gph::SWindow							& window				= framework.Window;
	framework.Pixels.resize(window.Size.x * window.Size.y);
	memset(framework.Pixels.begin(), 0, framework.Pixels.size() * sizeof(::gph::SColor));

	app.Scene.Rectangles.resize(4);
	for(int32_t y = 0; y < 2; ++y)
	for(int32_t x = 0; x < 2; ++x) {
		::gph::SRectangle2D<int32_t>				& rectangle					= app.Scene.Rectangles[y * 2 + x];
		rectangle.Size						= {window.Size.x / 2, window.Size.y / 2};
		rectangle.Offset					= {rectangle.Size.x * x, rectangle.Size.y * y};
	}
	app.Scene.Triangles.resize(12);
	for(uint32_t iTriangle = 0; iTriangle < app.Scene.Triangles.size(); ++iTriangle) {
		::gph::STriangle2D<int32_t>					& triangle					= app.Scene.Triangles[iTriangle] = {};
		triangle.A								= {0, 0};
		triangle.B								= {2, 0};
		triangle.C								= {1, 2};
		triangle.A								*= 100;
		triangle.B								*= 100;
		triangle.C								*= 100;
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
			::gph::SRectangle2D<int32_t>				& rectangle					= app.Scene.Rectangles[y * 2 + x];
			rectangle.Size						= {window.Size.x / 2, window.Size.y / 2};
			rectangle.Offset					= {rectangle.Size.x * x, rectangle.Size.y * y};
		}
	}
	::gph::view<::gph::SColor>				view_colors				= app.Scene.ShapeColors;
	::gph::view_grid<::gph::SColor>			pixels					= {framework.Pixels.begin(), window.Size.Cast<uint32_t>()};
	for(uint32_t iRectangle = 0; iRectangle < app.Scene.Rectangles.size(); ++iRectangle) {
		const ::gph::SColor						colorRectangle			= view_colors[iRectangle % ::std::size(app.Scene.ShapeColors)] / (1 + double(framework.CountFrames % 2));
		::gph::drawRectangle(pixels, {app.Scene.Rectangles[iRectangle].Offset.Cast<int32_t>(), app.Scene.Rectangles[iRectangle].Size.Cast<int32_t>()}, colorRectangle);
	}
	for(uint32_t iTriangle = 0; iTriangle < app.Scene.Triangles.size(); ++iTriangle) {
		::gph::STriangle2D<int32_t>					triangle					= app.Scene.Triangles[iTriangle];
		double										rotation					= iTriangle * (MATH_2PI / 12) + framework.CountFrames / 100.0f;
		triangle.A.Rotate(rotation);
		triangle.B.Rotate(rotation);
		triangle.C.Rotate(rotation);
		::gph::SCoord2<int32_t>						position					= {0, 200};
		position.Rotate(rotation);
		position								+= window.Size / 2.0;
		triangle.A								+= position;
		triangle.B								+= position;
		triangle.C								+= position;
		const ::gph::SColor						colorTriangle			= view_colors[iTriangle % ::std::size(app.Scene.ShapeColors)] / 2;
		::gph::drawTriangle(pixels, triangle, colorTriangle);
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
