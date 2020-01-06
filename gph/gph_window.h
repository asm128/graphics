#include "gph_math.h"
#include "gph_container.h"
#include "gph_color.h"

#include <Windows.h>

#ifndef GPH_WINDOW_H_9283467982374
#define GPH_WINDOW_H_9283467982374

namespace gph
{
	struct SWindow {
		WNDCLASSEX										Class					= {sizeof(WNDCLASSEX),};
		const char										ClassName	[256]		= "demos_window";
		::gph::SCoord2<int32_t>							Position				= {100, 100};
		::gph::SCoord2<int32_t>							Size					= {640, 480};
		HWND											Handle					= 0;
		bool											Resized					= true;
	};

	int												windowSetup				(::gph::SWindow & window, ::gph::SCoord2<uint32_t> size = {1360, 768});
	int												windowCleanup			(::gph::SWindow & window);
	int												windowUpdate			(::gph::SWindow & window);
	int												drawBuffer				(HDC hdc, ::gph::view_grid<::gph::SColor> pixels);
} // namespace

#endif // GPH_WINDOW_H_9283467982374
