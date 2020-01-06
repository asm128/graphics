#include "gph_draw.h"

int										gph::setPixel
	(	::gph::view_grid<::gph::SColor>		pixels
	,	::gph::SCoord2<int32_t>				position
	,	::gph::SColor						color
	) {
	if( position.x >= 0 && position.x < (int32_t)pixels.metrics().x
	 &&	position.y >= 0 && position.y < (int32_t)pixels.metrics().y
	)
		pixels[position.y][position.x]		= color;
	return 0;
}

int										gph::drawRectangle
	(	::gph::view_grid<::gph::SColor> pixels
	,	::gph::SRectangle2D<int32_t>	rectangle
	,	::gph::SColor					color
	) {
	for(int32_t y = 0; y < rectangle.Size.y; ++y)
	for(int32_t x = 0; x < rectangle.Size.x; ++x)
		::gph::setPixel(pixels, {rectangle.Offset.x + x, rectangle.Offset.y + y}, color);
	return 0;
}

int										gph::drawCircle
	(	::gph::view_grid<::gph::SColor> pixels
	,	::gph::SCircle2D<int32_t>		circle
	,	::gph::SColor					color
	) {
	for(int32_t y = -(int32_t)circle.Radius; y < (int32_t)circle.Radius; ++y)
	for(int32_t x = -(int32_t)circle.Radius; x < (int32_t)circle.Radius; ++x) {
		const ::gph::SCoord2<int32_t>				position				= {x, y};
		if(position.Length() <= circle.Radius)
			::gph::setPixel(pixels, {circle.Center.x + x, circle.Center.y + y}, color);
	}
	return 0;
}

int										gph::drawLineVertical
	(	::gph::view_grid<::gph::SColor> pixels
	,	::gph::SCoord2<int32_t>	linePosition
	,	uint32_t				lineLength
	,	::gph::SColor			color
	) {
	for(int32_t y = 0; y < (int32_t)lineLength; ++y)
		::gph::setPixel(pixels, {linePosition.x, linePosition.y + y}, color);
	return 0;
}

int										gph::drawLineHorizontal
	(	::gph::view_grid<::gph::SColor> pixels
	,	::gph::SCoord2<int32_t>	linePosition
	,	uint32_t				lineLength
	,	::gph::SColor			color
	) {
	for(int32_t x = 0; x < (int32_t)lineLength; ++x)
		::gph::setPixel(pixels, {linePosition.x + x, linePosition.y}, color);
	return 0;
}

int										gph::drawLine			(::gph::view_grid<::gph::SColor> pixels, ::gph::SLine2D<int32_t> line, ::gph::SColor color) {
	int32_t										dx						= abs(line.B.x - line.A.x);
	int32_t										dy						= -abs(line.B.y - line.A.y);
	int32_t										sx						= line.A.x < line.B.x ? 1 : -1;
	int32_t										sy						= line.A.y < line.B.y ? 1 : -1;
	int32_t										err						= dx + dy;  // error value e_xy
	::gph::setPixel(pixels, line.A, color);
	while (true) {
		if (line.A.x == line.B.x && line.A.y == line.B.y)
			break;
		int32_t										e2						= 2 * err;
		if (e2 >= dy || e2 <= dx) { // e_xy + e_y < 0
			if (e2 >= dy) {
				err										+= dy; // e_xy + e_x > 0
				line.A.x								+= sx;
			}
			if (e2 <= dx) { // e_xy + e_y < 0
				err										+= dx;
				line.A.y								+= sy;
			}
			::gph::setPixel(pixels, line.A, color);
		}
	}
	return 0;
}
