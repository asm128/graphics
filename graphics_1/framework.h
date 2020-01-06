#include "gph_draw.h"
#include "gph_framework.h"

#ifndef FRAMEWORK_H_29837472983
#define FRAMEWORK_H_29837472983

namespace gph
{
	struct SScene {
		::gph::container<::gph::SRectangle<int32_t>>	Rectangles				= {};
		::gph::SColor									ShapeColors	[4]			=
			{ {0xff, 0, 0, 0}
			, {0, 0xff, 0, 0}
			, {0, 0, 0xff, 0}
			, {0xff, 0xff, 0, 0}
			};
	};

	struct SApplication {
		::gph::SFramework								Framework				= {};
		::gph::SScene									Scene					= {};
	};
} // namespace

#endif // FRAMEWORK_H_29837472983
