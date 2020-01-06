#include "gph_timer.h"
#include "gph_window.h"

#ifndef GPH_FRAMEWORK_H_23847298374
#define GPH_FRAMEWORK_H_23847298374

namespace gph
{
	struct SFramework {
		SWindow											Window					= {};
		::gph::STimer									Timer					= {};
		uint64_t										CountFrames				= 0;
		::gph::container<::gph::SColor>					Pixels					= {};
	};
} // namespace

#endif // GPH_FRAMEWORK_H_23847298374
