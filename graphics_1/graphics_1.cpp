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

// Vertex coordinates for cube faces
static constexpr const ::gph::SCoord3<int8_t>						normalsCube	[6]						=
	{ { 0,  0,  1}	// Right	-
	, {-1,  0,  0}	// Back		-
	, { 0, -1,  0}	// Bottom	-
	, { 0,  0, -1}	// Left		-
	, { 1,  0,  0}	// Front	-
	, { 0,  1,  0}	// Top		-
	};

int									cleanup					(::gph::SApplication & app)		{ (void)app; return 0; }
int									setup					(::gph::SApplication & app)		{
	::gph::SFramework						& framework				= app.Framework;
	::gph::SWindow							& window				= framework.Window;
	framework.Pixels.resize(window.Size.x * window.Size.y);
	memset(framework.Pixels.begin(), 0, framework.Pixels.size() * sizeof(::gph::SColor));
	app.Scene.Models.resize(6);
	for(uint32_t iModel = 0; iModel < app.Scene.Models.size(); ++iModel) {
		::gph::SModel							& model	= app.Scene.Models[iModel]	= {};
		model.Center						= {-0.5, 0, -.5};
		model.Scale							= {1, 1, 1};
		model.Position						= {5, 0, 0};
		model.Position.RotateY(iModel * (::MATH_2PI / app.Scene.Models.size()));
	}
	return 0;
}

int									update					(::gph::SApplication & app)		{
	::gph::SFramework						& framework				= app.Framework;
	::gph::SWindow							& window				= framework.Window;
	double									secondsLastFrame		= framework.Timer.TimeDelta * .000001;
	(void)secondsLastFrame;
	memset(framework.Pixels.begin(), 0, framework.Pixels.size() * sizeof(::gph::SColor));
	if(window.Resized) {
		framework.Pixels.resize(window.Size.x * window.Size.y);
		memset(framework.Pixels.begin(), 0, framework.Pixels.size() * sizeof(::gph::SColor));
	}
	::gph::SCoord3<float>					cameraPosition			= {8, 8, 8};
	::gph::SCoord3<float>					cameraTarget			= {0, 0, 0};
	::gph::SCoord3<float>					cameraUp				= {0, -1, 0};
	::gph::SCoord3<float>					lightAngle				= {8, -8, 8};

	lightAngle.Normalize();

	::gph::SMatrix4<float>					matrixView				= {};
	::gph::SMatrix4<float>					matrixProjection		= {};
	matrixView.LookAt(cameraPosition, cameraTarget, cameraUp);


	app.Scene.Triangles	.resize((uint32_t)::std::size(geometryCube));
	app.Scene.Normals	.resize((uint32_t)::std::size(normalsCube));
	for(uint32_t iModel = 0; iModel < app.Scene.Models.size(); ++iModel) {
		::gph::SModel							& model	= app.Scene.Models[iModel];
		for(uint32_t iTriangle = 0; iTriangle < app.Scene.Triangles.size(); ++iTriangle) {
			::gph::STriangle3D<float>				triangle				= geometryCube[iTriangle].Cast<float>();
			double									& rotation				= model.Rotation;
			rotation							+= secondsLastFrame / 10;
			if(iTriangle % 2) {
				::gph::SCoord3<float>					normal					= normalsCube[iTriangle / 2].Cast<float>();
				normal.RotateY(rotation);
				app.Scene.Normals[iTriangle / 2]	= normal;
			}
			model.Position.RotateY(secondsLastFrame / 50);
			triangle.A							+= model.Center;
			triangle.B							+= model.Center;
			triangle.C							+= model.Center;
			triangle.A.RotateY(rotation);
			triangle.B.RotateY(rotation);
			triangle.C.RotateY(rotation);
			triangle.A							-= model.Center;
			triangle.B							-= model.Center;
			triangle.C							-= model.Center;
			triangle.A.Scale(model.Scale);
			triangle.B.Scale(model.Scale);
			triangle.C.Scale(model.Scale);
			triangle.A							+= model.Position;
			triangle.B							+= model.Position;
			triangle.C							+= model.Position;
			triangle.A.Scale(100);
			triangle.B.Scale(100);
			triangle.C.Scale(100);
			::gph::transform(triangle, matrixView);
			app.Scene.Triangles[iTriangle]		=
				{ {(int32_t)triangle.A.x, (int32_t)triangle.A.y}
				, {(int32_t)triangle.B.x, (int32_t)triangle.B.y}
				, {(int32_t)triangle.C.x, (int32_t)triangle.C.y}
				};
		}

		::gph::view<::gph::SColor>				view_colors				= app.Scene.ShapeColors;
		::gph::view_grid<::gph::SColor>			pixels					= {framework.Pixels.begin(), window.Size.Cast<uint32_t>()};
		::gph::container<::gph::SCoord2<int32_t>>trianglePixels			= {};
		for(uint32_t iTriangle = 0; iTriangle < app.Scene.Triangles.size(); ++iTriangle) {
			::gph::STriangle2D<int32_t>				triangle				= app.Scene.Triangles[iTriangle];
			::gph::SCoord2<int32_t>					position				= window.Size / 2.0;
			triangle.A							+= position;
			triangle.B							+= position;
			triangle.C							+= position;

			::gph::SColor							colorTriangle			= ::gph::SColor{0xFF, 0, 0};
			::gph::SCoord3<float>					normal					= app.Scene.Normals[iTriangle / 2];
			colorTriangle						= (colorTriangle / 10.0) + (colorTriangle * normal.Dot(lightAngle));

			trianglePixels.resize(0);
			::gph::drawTriangle(pixels.metrics(), triangle, trianglePixels);
			for(uint32_t iPixel = 0; iPixel < trianglePixels.size(); ++iPixel) {
				const ::gph::SCoord2<int32_t>			pixelCoord					= trianglePixels[iPixel];
				pixels[pixelCoord.y][pixelCoord.x]	= colorTriangle;
			}
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
