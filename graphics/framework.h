#include <cstdint>
#include <algorithm>

#ifndef FRAMEWORK_H_29837472983
#define FRAMEWORK_H_29837472983

namespace gph
{
	template<typename _tValue>
	struct SCoord {
		_tValue											x;
		_tValue											y;

		SCoord<_tValue>									operator+				(const SCoord<_tValue> & other)		const	noexcept		{ return { x + other.x, y + other.y }; }
		SCoord<_tValue>									operator-				(const SCoord<_tValue> & other)		const	noexcept		{ return { x - other.x, y - other.y }; }
		SCoord<_tValue>									operator/				(double scalar)						const	noexcept		{ return { (int32_t)(x / scalar), (int32_t)(y / scalar) }; }
		SCoord<_tValue>									operator*				(double scalar)						const	noexcept		{ return { (int32_t)(x * scalar), (int32_t)(y * scalar) }; }

		double											SqLength				()									const	noexcept		{ return x * (double)x + y * (double)y; }
		double											Length					()									const	noexcept		{
			const double									squaredLength			= SqLength();
			return squaredLength ? sqrt(squaredLength) : 0;
		}
		template<typename _tOther>
		SCoord<_tOther>									Cast					()															{ return {(_tOther)x, (_tOther)y}; }

	};

	template<typename _tValue>
	double											clamp					(_tValue value, _tValue min, _tValue max)					{ return ::std::min(::std::max(min, value), max); }

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
		using											view<_tElement>		::Data;
		using											view<_tElement>		::Count;

														~container			()													{ free(Data); }

		int												resize				(uint32_t newSize)									{
			if(newSize < Count)
				return Count = newSize;
			_tElement										* newData				= (_tElement*)malloc(sizeof(_tElement) * newSize);
			if(0 == newData)
				return -1;
			if(Data)
				memcpy(newData, Data, Count * sizeof(_tElement));
			_tElement										* oldData				= Data;
			Data										= newData;
			free(oldData);
			return Count = newSize;
		}
	};

	template<typename _tElement>
	struct view_grid {
	protected:
		_tElement										* Data				= 0;
		SCoord<uint32_t>								Metrics				= 0;
	public:
														view_grid			(_tElement *elements, SCoord<uint32_t> metrics)		: Data(elements), Metrics(metrics)	{ if((metrics.x || metrics.y) && 0 == elements) { throw(""); } }

		::gph::view<_tElement>							operator[]			(uint32_t row)										{ if(row >= Metrics.y) { throw(""); } return ::gph::view<_tElement		>{&Data[row * Metrics.x], Metrics.x}; }
		::gph::view<const _tElement>					operator[]			(uint32_t row)					const				{ if(row >= Metrics.y) { throw(""); } return ::gph::view<const _tElement>{&Data[row * Metrics.x], Metrics.x}; }

		inline constexpr	const SCoord<uint32_t>&		metrics				()													{ return Metrics; }
		inline constexpr	_tElement*					size				()								const	noexcept	{ return Metrics.x * Metrics.y; }
		inline constexpr	_tElement*					begin				()								const	noexcept	{ return Data; }
		inline constexpr	_tElement*					end					()								const	noexcept	{ return Data + size(); }
	};

	int												setPixel			(::gph::view_grid<::gph::SColor> pixels, ::gph::SCoord<int32_t>	position										, ::gph::SColor color);
	int												drawRectangle		(::gph::view_grid<::gph::SColor> pixels, ::gph::SCoord<int32_t>	rectPosition, ::gph::SCoord<int32_t> rectSize	, ::gph::SColor color);
	int												drawCircle			(::gph::view_grid<::gph::SColor> pixels, ::gph::SCoord<int32_t>	rectPosition, double radius						, ::gph::SColor color);
	int												drawLineVertical	(::gph::view_grid<::gph::SColor> pixels, ::gph::SCoord<int32_t>	linePosition, uint32_t lineLength				, ::gph::SColor color);
	int												drawLineHorizontal	(::gph::view_grid<::gph::SColor> pixels, ::gph::SCoord<int32_t>	linePosition, uint32_t lineLength				, ::gph::SColor color);
} // namespace

#endif // FRAMEWORK_H_29837472983
