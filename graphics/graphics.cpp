#include "framework.h"

int									cleanup				(::gph::SApplication & app)		{ (void)app; return 0; }
int									setup				(::gph::SApplication & app)		{
	::gph::SFramework						& framework			= app.Framework;
	::gph::SWindow							& window			= framework.Window;
	framework.Pixels.resize(window.Size.x * window.Size.y);
	memset(framework.Pixels.begin(), 0, framework.Pixels.size() * sizeof(::gph::SColor));

	app.Scene.ParticlePositions	.resize(app.MAX_PARTICLES);
	app.Scene.ParticleSpeeds	.resize(app.MAX_PARTICLES);
	memset(app.Scene.ParticlePositions	.begin(), 0, app.Scene.ParticlePositions.size() * sizeof(::gph::SCoord2<float>));
	memset(app.Scene.ParticleSpeeds		.begin(), 0, app.Scene.ParticleSpeeds	.size() * sizeof(float));
	for(uint32_t iParticle = 0; iParticle < app.Scene.ParticlePositions.size(); ++iParticle) {
		::gph::SCoord2<float>					& particlePosition		= app.Scene.ParticlePositions[iParticle];
		float									& particleSpeed			= app.Scene.ParticleSpeeds[iParticle];
		particlePosition.x					= (float)(rand() % window.Size.x);
		particleSpeed						= float(rand() % 9);
	}
	return 0;
}

int									update					(::gph::SApplication & app)		{
	::gph::SFramework						& framework			= app.Framework;
	::gph::SWindow							& window			= framework.Window;
	if(window.Resized) {
		framework.Pixels.resize(window.Size.x * window.Size.y);
		memset(framework.Pixels.begin(), 0, framework.Pixels.size() * sizeof(::gph::SColor));
		for(int32_t y = 0; y < 2; ++y)
		for(int32_t x = 0; x < 2; ++x) {
			::gph::SRectangle2D<double>				& rectangle					= app.Scene.Rectangle;
			rectangle.Size						= {window.Size.x / 2.0, window.Size.y / 2.0};
			rectangle.Offset					= {rectangle.Size.x * x, rectangle.Size.y * y};
		}
	}
	double									secondsLastFrame		= framework.Timer.TimeDelta * .000001;
	::gph::view<::gph::SColor>				view_colors				= app.Scene.ShapeColors;

	app.Scene.Rectangle.Size			= {window.Size.x / 2.0, window.Size.y / 2.0};
	app.Scene.Rectangle.Offset.x		+= secondsLastFrame * 100;

	::gph::view_grid<::gph::SColor>			pixels					= {framework.Pixels.begin(), window.Size.Cast<uint32_t>()};
	if( app.Scene.Rectangle.Offset.x > window.Size.x )
		app.Scene.Rectangle.Offset.x		= 0;

	::gph::STriangle2D<int32_t>						triangle				= {};
	::gph::SRectangle2D<int32_t>					rectangle				= {{}, app.Scene.Rectangle.Size.Cast<int32_t>()};
	::gph::SCircle2D<int32_t>						circle					= {{}, 16};
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
		triangle.A.Rotate(framework.CountFrames / 10.0);
		triangle.B.Rotate(framework.CountFrames / 10.0);
		triangle.C.Rotate(framework.CountFrames / 10.0);
		triangle.A							+= app.Scene.Rectangle.Offset.Cast<int32_t>() * x;
		triangle.B							+= app.Scene.Rectangle.Offset.Cast<int32_t>() * x;
		triangle.C							+= app.Scene.Rectangle.Offset.Cast<int32_t>() * x;
		rectangle	.Offset					= {(int32_t)(app.Scene.Rectangle.Offset.x * x)			, (int32_t)(app.Scene.Rectangle.Offset.y * y)};
		circle		.Center					= ::gph::SCoord2<int32_t>{(int32_t)(app.Scene.Rectangle.Offset.x * x), (int32_t)(app.Scene.Rectangle.Offset.y * y)} + (window.Size / 4);
		::gph::SCoord2<int32_t>					offsetLineVertical		= ::gph::SCoord2<int32_t>{(int32_t)(app.Scene.Rectangle.Offset.x * x)		, (int32_t)(app.Scene.Rectangle.Offset.y * y) - 8}	+ (window.Size / 4);
		::gph::SCoord2<int32_t>					offsetLineHorizontal	= ::gph::SCoord2<int32_t>{(int32_t)(app.Scene.Rectangle.Offset.x * x) - 8	, (int32_t)(app.Scene.Rectangle.Offset.y * y)}		+ (window.Size / 4);

		::gph::drawRectangle		(pixels, rectangle	, colorRectangle);
		//::gph::drawCircle			(pixels, circle		, colorCircle);
		//::gph::drawTriangle			(pixels, triangle	, colorLineHorizontal);
		::gph::drawTriangle				(pixels.metrics(), triangle, pixelCoords);
		//::gph::drawLineVertical		(pixels, offsetLineVertical		, 16	, colorLineVertical		);
		//::gph::drawLineHorizontal	(pixels, offsetLineHorizontal	, 16	, colorLineHorizontal	);
		for(uint32_t iPixelCoord = 0; iPixelCoord < pixelCoords.size(); ++iPixelCoord) {
			const ::gph::SCoord2<int32_t>			& pixelCoord	=  pixelCoords[iPixelCoord];
			setPixel(pixels, pixelCoord, {(uint8_t)(pixelCoord.x + triangle.A.x), (uint8_t)(pixelCoord.y + triangle.A.y), (uint8_t)(pixelCoord.x + pixelCoord.y)});
		}

		::gph::SLine2D<int32_t>					line0					= {::gph::SCoord2<int32_t>{+8, }	, ::gph::SCoord2<int32_t>{-8, 16}};
		::gph::SLine2D<int32_t>					line1					= {::gph::SCoord2<int32_t>{0, -8}	, ::gph::SCoord2<int32_t>{16, +8}};
		line0.A.Rotate((double)framework.CountFrames);
		line0.B.Rotate((double)framework.CountFrames);
		line1.B.Rotate((double)framework.CountFrames);
		line1.A.Rotate((double)framework.CountFrames);
		line0.A						+= offsetLineVertical;
		line0.B						+= offsetLineVertical;
		line1.B						+= offsetLineHorizontal;
		line1.A						+= offsetLineHorizontal;
		::gph::drawLine(pixels, line0, colorLineVertical	);
		::gph::drawLine(pixels, line1, colorLineHorizontal	);

		::gph::drawLine(pixels, {triangle.A, triangle.B}, colorCircle);
		::gph::drawLine(pixels, {triangle.B, triangle.C}, colorCircle);
		::gph::drawLine(pixels, {triangle.C, triangle.A}, colorCircle);
	}

	for(uint32_t iParticle = 0; iParticle < app.Scene.ParticlePositions.size(); ++iParticle) {
		::gph::SCoord2<float>					& particlePosition		= app.Scene.ParticlePositions[iParticle];
		float									& particleSpeed			= app.Scene.ParticleSpeeds[iParticle];
		particleSpeed						+= float(app.GRAVITY * secondsLastFrame);
		particlePosition.y					+= (float)(particleSpeed * secondsLastFrame);
		if( particlePosition.y >= window.Size.y)
			particlePosition.y					= 0;
		if(0 == (rand() % 16)) ++particlePosition.x;
		if(0 == (rand() % 16)) --particlePosition.x;
		::gph::setPixel({framework.Pixels.begin(), window.Size.Cast<uint32_t>()}, {(int32_t)particlePosition.x, (int32_t)(window.Size.y - particlePosition.y)}, view_colors[iParticle % view_colors.size()]);
	}

	++framework.CountFrames;
	framework.Timer.Tick();
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
