#include "framework.h"

const double						MATH_PI					= 3.1415;
const double						MATH_2PI				= 2 * MATH_PI;

// Vertex coordinates for cube faces
static constexpr const ::gph::STriangle3D<int8_t>						geometryCube	[12]						=
	{ {{1, 0, 0}, {0, 0, 0}, {0, 1, 0}}	// Right	- first
	, {{1, 0, 0}, {0, 1, 0}, {1, 1, 0}}	// Right	- second

	, {{0, 0, 1}, {0, 1, 0}, {0, 0, 0}}	// Back		- first
	, {{0, 0, 1}, {0, 1, 1}, {0, 1, 0}}	// Back		- second

	, {{1, 0, 0}, {0, 0, 1}, {0, 0, 0}}	// Bottom	- first
	, {{1, 0, 0}, {1, 0, 1}, {0, 0, 1}}	// Bottom	- second

	, {{1, 0, 1}, {0, 1, 1}, {0, 0, 1}}	// Left		- first
	, {{1, 0, 1}, {1, 1, 1}, {0, 1, 1}}	// Left		- second

	, {{1, 0, 1}, {1, 1, 0}, {1, 1, 1}}	// Front	- first
	, {{1, 0, 1}, {1, 0, 0}, {1, 1, 0}}	// Front	- second

	, {{1, 1, 0}, {0, 1, 1}, {1, 1, 1}}	// Top		- first
	, {{1, 1, 0}, {0, 1, 0}, {0, 1, 1}}	// Top		- second
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
		::gph::SRectangle2D<int32_t>			& rectangle					= app.Scene.Rectangles[y * 2 + x];
		rectangle.Size						= {window.Size.x / 2, window.Size.y / 2};
		rectangle.Offset					= {rectangle.Size.x * x, rectangle.Size.y * y};
	}
	app.Scene.Triangles.resize(6);
	for(uint32_t iTriangle = 0; iTriangle < app.Scene.Triangles.size(); ++iTriangle) {
		::gph::STriangle2D<int32_t>					& triangle					= app.Scene.Triangles[iTriangle] = {};
		triangle.A								= {-1, 0};
		triangle.B								= {1, 0};
		triangle.C								= {0, 2};
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
		const ::gph::SColor						colorRectangle			= view_colors[iRectangle % ::std::size(app.Scene.ShapeColors)] * ((framework.CountFrames % 255) / 255.0);
		::gph::drawRectangle(pixels, {app.Scene.Rectangles[iRectangle].Offset.Cast<int32_t>(), app.Scene.Rectangles[iRectangle].Size.Cast<int32_t>()}, colorRectangle);
	}
	::gph::container<::gph::SCoord2<int32_t>>trianglePixels			= {};
	for(uint32_t iTriangle = 0; iTriangle < app.Scene.Triangles.size(); ++iTriangle) {
		::gph::STriangle2D<int32_t>				triangle					= app.Scene.Triangles[iTriangle];
		double									rotation					= iTriangle * (MATH_2PI / app.Scene.Triangles.size()) + framework.CountFrames / 100.0f;
		triangle.A.Rotate(rotation);
		triangle.B.Rotate(rotation);
		triangle.C.Rotate(rotation);
		::gph::SCoord2<int32_t>					position					= {0, 175};
		position.Rotate(rotation);
		position							+= window.Size / 2.0;
		triangle.A							+= position;
		triangle.B							+= position;
		triangle.C							+= position;
		const ::gph::SColor						colorTriangle				= view_colors[(iTriangle + (uint32_t)framework.CountFrames / 100) % ::std::size(app.Scene.ShapeColors)] * (((framework.CountFrames + 16 * iTriangle) % 255) / 255.0);;// / 2;
		trianglePixels.resize(0);
		::gph::drawTriangle(pixels.metrics(), triangle, trianglePixels);
		for(uint32_t iPixel = 0; iPixel < trianglePixels.size(); ++iPixel) {
			const ::gph::SCoord2<int32_t>			pixelCoord					= trianglePixels[iPixel];
			pixels[pixelCoord.y][pixelCoord.x]	= colorTriangle;
		}
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
