#include "gph_math.h"
#include "gph_color.h"
#include "gph_container.h"

#ifndef GPH_DRAW_H_2378942983742398
#define GPH_DRAW_H_2378942983742398

namespace gph
{
	int												setPixel				(::gph::view_grid<::gph::SColor> pixels, ::gph::SCoord2<int32_t>	position							, ::gph::SColor color);
	int												drawRectangle			(::gph::view_grid<::gph::SColor> pixels, ::gph::SRectangle<int32_t>	rectangle							, ::gph::SColor color);
	int												drawCircle				(::gph::view_grid<::gph::SColor> pixels, ::gph::SCircle<int32_t>	circle								, ::gph::SColor color);
	int												drawLineVertical		(::gph::view_grid<::gph::SColor> pixels, ::gph::SCoord2<int32_t>	linePosition, uint32_t lineLength	, ::gph::SColor color);
	int												drawLineHorizontal		(::gph::view_grid<::gph::SColor> pixels, ::gph::SCoord2<int32_t>	linePosition, uint32_t lineLength	, ::gph::SColor color);
	int												drawLine				(::gph::view_grid<::gph::SColor> pixels, ::gph::SLine<int32_t> line										, ::gph::SColor color);

	template<typename _tValue>
	int32_t											drawTriangle			(view_grid<SColor> pixels, const STriangle<_tValue>& triangle, const SColor& color)		{
		// Compute triangle bounding box
		int32_t												minX					= (int32_t)min3(triangle.A.x, triangle.B.x, triangle.C.x);
		int32_t												minY					= (int32_t)min3(triangle.A.y, triangle.B.y, triangle.C.y);
		int32_t												maxX					= (int32_t)max3(triangle.A.x, triangle.B.x, triangle.C.x);
		int32_t												maxY					= (int32_t)max3(triangle.A.y, triangle.B.y, triangle.C.y);

		// Clip against screen bounds
		minX											= ::std::max(minX, 0);
		minY											= ::std::max(minY, 0);
		maxX											= ::std::min(maxX, (int32_t)pixels.metrics().x - 1);
		maxY											= ::std::min(maxY, (int32_t)pixels.metrics().y - 1);

		// Rasterize
		SCoord2<int32_t>										p;
		for (p.y = minY; p.y <= maxY; ++p.y)
		for (p.x = minX; p.x <= maxX; ++p.x) {
			// Determine barycentric coordinates
			int													w0						= orient2d(triangle.B, triangle.C, p);
			int													w1						= orient2d(triangle.C, triangle.A, p);
			int													w2						= orient2d(triangle.A, triangle.B, p);

			// If p is on or inside all edges, render pixel.
			(void)color;
			if (w0 >= 0 && w1 >= 0 && w2 >= 0)
				setPixel(pixels, p, color); //, w0, w1, w2);
		}
		return 0;
	}
	template<typename _tValue>
	int32_t											drawTriangle			(::gph::SCoord2<uint32_t> targetSize, const STriangle<_tValue>& triangle, ::gph::container<::gph::SCoord2<int32_t>>& pixelCoords)		{
		// Compute triangle bounding box
		int32_t												minX					= (int32_t)min3(triangle.A.x, triangle.B.x, triangle.C.x);
		int32_t												minY					= (int32_t)min3(triangle.A.y, triangle.B.y, triangle.C.y);
		int32_t												maxX					= (int32_t)max3(triangle.A.x, triangle.B.x, triangle.C.x);
		int32_t												maxY					= (int32_t)max3(triangle.A.y, triangle.B.y, triangle.C.y);

		// Clip against screen bounds
		minX											= ::std::max(minX, 0);
		minY											= ::std::max(minY, 0);
		maxX											= ::std::min(maxX, (int32_t)targetSize.x - 1);
		maxY											= ::std::min(maxY, (int32_t)targetSize.y - 1);

		// Rasterize
		SCoord2<int32_t>										p;
		for (p.y = minY; p.y <= maxY; ++p.y)
		for (p.x = minX; p.x <= maxX; ++p.x) {
			// Determine barycentric coordinates
			int													w0						= orient2d(triangle.B, triangle.C, p);
			int													w1						= orient2d(triangle.C, triangle.A, p);
			int													w2						= orient2d(triangle.A, triangle.B, p);
			// If p is on or inside all edges, render pixel.
			if (w0 >= 0 && w1 >= 0 && w2 >= 0)
				pixelCoords.push_back(p);
		}
		return 0;
	}
} // namespace

#endif // GPH_DRAW_H_2378942983742398
