#include "gph_draw.h"
#include "gph_timer.h"
#include "gph_window.h"

#ifndef FRAMEWORK_H_29837472983
#define FRAMEWORK_H_29837472983

namespace gph
{
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

	struct SApplication {
		const uint32_t							MAX_PARTICLES			= 64;
		const double							GRAVITY					= 9.8;

		SWindow									Window					= {};
		::gph::STimer							Timer					= {};
		::gph::SScene							Scene					= {};
		uint64_t								CountFrames				= 0;
		::gph::container<::gph::SColor>			Pixels					= {};
	};
} // namespace

#endif // FRAMEWORK_H_29837472983
