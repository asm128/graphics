#include "framework.h"

int										gph::setPixel
	(	::gph::SColor			* pixels
	,	::gph::SCoord<int32_t>	targetSize
	,	::gph::SCoord<int32_t>	position
	,	::gph::SColor			color
	) {
	if( position.x >= 0 && position.x < targetSize.x
	 &&	position.y >= 0 && position.y < targetSize.y
	)
		pixels[position.y * targetSize.x + position.x] = color;
	return 0;
}

int										gph::drawRectangle
	(	::gph::SColor			* pixels
	,	::gph::SCoord<int32_t>	targetSize
	,	::gph::SCoord<int32_t>	rectPosition
	,	::gph::SCoord<int32_t>	rectSize
	,	::gph::SColor			color
	) {
	for(int32_t y = 0; y < rectSize.y; ++y)
	for(int32_t x = 0; x < rectSize.x; ++x)
		::gph::setPixel(pixels, targetSize, {rectPosition.x + x, rectPosition.y + y}, color);
	return 0;
}

int										gph::drawCircle
	(	::gph::SColor			* pixels
	,	::gph::SCoord<int32_t>	targetSize
	,	::gph::SCoord<int32_t>	rectPosition
	,	double					radius
	,	::gph::SColor			color
	) {
	for(int32_t y = -(int32_t)radius; y < (int32_t)radius; ++y)
	for(int32_t x = -(int32_t)radius; x < (int32_t)radius; ++x) {
		const ::gph::SCoord<int32_t>				position				= {x, y};
		if(position.Length() <= radius)
			::gph::setPixel(pixels, targetSize, {rectPosition.x + x, rectPosition.y + y}, color);
	}
	return 0;
}

int										gph::drawLineVertical
	(	::gph::SColor			* pixels
	,	::gph::SCoord<int32_t>	targetSize
	,	::gph::SCoord<int32_t>	linePosition
	,	uint32_t				lineLength
	,	::gph::SColor			color
	) {
	for(int32_t y = 0; y < (int32_t)lineLength; ++y)
		::gph::setPixel(pixels, targetSize, {linePosition.x, linePosition.y + y}, color);
	return 0;
}
int										gph::drawLineHorizontal
	(	::gph::SColor			* pixels
	,	::gph::SCoord<int32_t>	targetSize
	,	::gph::SCoord<int32_t>	linePosition
	,	uint32_t				lineLength
	,	::gph::SColor			color
	) {
	for(int32_t x = 0; x < (int32_t)lineLength; ++x)
		::gph::setPixel(pixels, targetSize, {linePosition.x + x, linePosition.y}, color);
	return 0;
}
