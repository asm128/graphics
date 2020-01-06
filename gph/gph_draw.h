#include "gph_math.h"

#ifndef GPH_DRAW_H_2378942983742398
#define GPH_DRAW_H_2378942983742398

namespace gph
{
	struct SColor {
		uint8_t											r;
		uint8_t											g;
		uint8_t											b;
		uint8_t											a;

		SColor											operator/				(double scalar)				const	noexcept				{ return {(uint8_t)clamp(r / scalar, 0.0, 255.0), (uint8_t)clamp(g / scalar, 0.0, 255.0), (uint8_t)clamp(b / scalar, 0.0, 255.0), (uint8_t)clamp(a / scalar, 0.0, 255.0)}; }
		SColor											operator*				(double scalar)				const	noexcept				{ return {(uint8_t)clamp(r * scalar, 0.0, 255.0), (uint8_t)clamp(g * scalar, 0.0, 255.0), (uint8_t)clamp(b * scalar, 0.0, 255.0), (uint8_t)clamp(a * scalar, 0.0, 255.0)}; }
		SColor											operator+				(const SColor& other)		const	noexcept				{ return {(uint8_t)clamp(r + other.r, 0, 255), (uint8_t)clamp(g + other.g, 0, 255), (uint8_t)clamp(b + other.b, 0, 255), (uint8_t)clamp(a + other.a, 0, 255) }; }
		SColor											operator-				(const SColor& other)		const	noexcept				{ return {(uint8_t)clamp(r - other.r, 0, 255), (uint8_t)clamp(g - other.g, 0, 255), (uint8_t)clamp(b - other.b, 0, 255), (uint8_t)clamp(a - other.a, 0, 255)}; }
	};

	template<typename _tElement>
	struct view {
	protected:
		_tElement										* Data				= 0;
		uint32_t										Count				= 0;
	public:
		template<size_t _sCount>
		inline	constexpr								view				(_tElement (&static_array)[_sCount])				: Data(static_array), Count(_sCount)	{}
														view				(_tElement *elements, uint32_t count)				: Data(elements)	, Count(count)		{ if(Count && 0 == elements) { throw(""); } }
		inline	constexpr								view				()													= default;

		_tElement&										operator[]			(uint32_t index)									{ if(index >= Count) { throw(""); } return Data[index]; }
		const _tElement&								operator[]			(uint32_t index)				const				{ if(index >= Count) { throw(""); } return Data[index]; }

		inline constexpr	const uint32_t&				size				()								const	noexcept	{ return Count; }
		inline constexpr	_tElement*					begin				()								const	noexcept	{ return Data; }
		inline constexpr	_tElement*					end					()								const	noexcept	{ return Data + Count; }

	};

	template<typename _tElement>
	struct container : public view<_tElement> {
		using											view<_tElement>			::Data;
		using											view<_tElement>			::Count;
		using											view<_tElement>			::size;
		uint32_t										Size					= 0;

														~container				()													{ free(Data); }

		int												resize					(uint32_t newSize)									{
			if(newSize < Count)
				return Count = newSize;
			if(newSize > Size) {
				uint32_t											newSizeWithExtraSpace	= ::std::max(8U, newSize * (newSize / 2));
				_tElement											* newData				= (_tElement*)malloc(sizeof(_tElement) * newSizeWithExtraSpace);
				if(0 == newData)
					return -1;
				if(Data)
					memcpy(newData, Data, Count * sizeof(_tElement));
				_tElement											* oldData			= Data;
				Data											= newData;
				free(oldData);
				Size											= newSizeWithExtraSpace;
			}
			return Count = newSize;
		}
		int32_t											push_back				(const _tElement & value)							{ uint32_t oldSize = size(); if(-1 == resize(size() + 1)) return -1; Data[oldSize] = value; return oldSize; }
	};

	template<typename _tElement>
	struct view_grid {
	protected:
		_tElement										* Data					= 0;
		SCoord2<uint32_t>								Metrics					= 0;
	public:
														view_grid				(_tElement *elements, SCoord2<uint32_t> metrics)		: Data(elements), Metrics(metrics)	{ if((metrics.x || metrics.y) && 0 == elements) { throw(""); } }

		::gph::view<_tElement>							operator[]				(uint32_t row)										{ if(row >= Metrics.y) { throw(""); } return ::gph::view<_tElement		>{&Data[row * Metrics.x], Metrics.x}; }
		::gph::view<const _tElement>					operator[]				(uint32_t row)					const				{ if(row >= Metrics.y) { throw(""); } return ::gph::view<const _tElement>{&Data[row * Metrics.x], Metrics.x}; }

		inline constexpr	const SCoord2<uint32_t>&		metrics					()													{ return Metrics; }
		inline constexpr	_tElement*					size					()								const	noexcept	{ return Metrics.x * Metrics.y; }
		inline constexpr	_tElement*					begin					()								const	noexcept	{ return Data; }
		inline constexpr	_tElement*					end						()								const	noexcept	{ return Data + size(); }
	};

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
