#include <cstdint>
#include <algorithm>

#ifndef FRAMEWORK_H_29837472983
#define FRAMEWORK_H_29837472983

namespace gph
{
	template<typename _tValue>
	struct SCoord {
		_tValue								x;
		_tValue								y;

		SCoord								operator+				(const SCoord<_tValue> & other)		const	noexcept				{ return { x + other.x, y + other.y }; }
		SCoord								operator-				(const SCoord<_tValue> & other)		const	noexcept				{ return { x - other.x, y - other.y }; }
		SCoord								operator/				(double scalar)						const	noexcept				{ return { (int32_t)(x / scalar), (int32_t)(y / scalar) }; }
		SCoord								operator*				(double scalar)						const	noexcept				{ return { (int32_t)(x * scalar), (int32_t)(y * scalar) }; }

		double								SqLength				()									const	noexcept				{ return x * (double)x + y * (double)y; }
		double								Length					()									const	noexcept				{
			const double							squaredLength			= SqLength();
			return squaredLength ? sqrt(squaredLength) : 0;
		}
	};

	template<typename _tValue>
	double								clamp					(_tValue value, _tValue min, _tValue max)		{ return ::std::min(::std::max(min, value), max); }

	struct SColor {
		uint8_t								r;
		uint8_t								g;
		uint8_t								b;
		uint8_t								a;

		SColor								operator/				(double scalar)				const	noexcept				{
			return
				{ (uint8_t)clamp(r / scalar, 0.0, 255.0)
				, (uint8_t)clamp(g / scalar, 0.0, 255.0)
				, (uint8_t)clamp(b / scalar, 0.0, 255.0)
				, (uint8_t)clamp(a / scalar, 0.0, 255.0)
				};
		}
		SColor								operator*				(double scalar)				const	noexcept				{
			return
				{ (uint8_t)clamp(r * scalar, 0.0, 255.0)
				, (uint8_t)clamp(g * scalar, 0.0, 255.0)
				, (uint8_t)clamp(b * scalar, 0.0, 255.0)
				, (uint8_t)clamp(a * scalar, 0.0, 255.0)
				};
		}
		SColor								operator+				(const SColor& other)		const	noexcept				{
			return
				{ (uint8_t)clamp(r + other.r, 0, 255)
				, (uint8_t)clamp(g + other.g, 0, 255)
				, (uint8_t)clamp(b + other.b, 0, 255)
				, (uint8_t)clamp(a + other.a, 0, 255)
				};
		}
		SColor								operator-				(const SColor& other)		const	noexcept				{
			return
				{ (uint8_t)clamp(r - other.r, 0, 255)
				, (uint8_t)clamp(g - other.g, 0, 255)
				, (uint8_t)clamp(b - other.b, 0, 255)
				, (uint8_t)clamp(a - other.a, 0, 255)
				};
		}
	};

	template<typename _tElement>
	struct view {
		_tElement							* Data;
		uint32_t							Count;


		template<size_t _sCount>
		inline	constexpr					view				(_tElement (&static_array)[_sCount])				: Data(static_array), Count(_sCount)	{}
											view				(_tElement *elements, uint32_t count)				: Data(elements)	, Count(count)		{ if(Count && 0 == elements) { throw(""); } }

		_tElement&							operator[]			(uint32_t index)									{ if(index >= Count) { throw(""); } return Data[index]; }
		const _tElement&					operator[]			(uint32_t index)				const				{ if(index >= Count) { throw(""); } return Data[index]; }

		inline constexpr	uint32_t		size				()								const	noexcept	{ return Count; }

	};

	int									setPixel
		(	::gph::SColor			* pixels
		,	::gph::SCoord<int32_t>	targetSize
		,	::gph::SCoord<int32_t>	position
		,	::gph::SColor			color
		);
	int									drawRectangle
		(	::gph::SColor			* pixels
		,	::gph::SCoord<int32_t>	targetSize
		,	::gph::SCoord<int32_t>	rectPosition
		,	::gph::SCoord<int32_t>	rectSize
		,	::gph::SColor			color
		);
	int									drawCircle
		(	::gph::SColor			* pixels
		,	::gph::SCoord<int32_t>	targetSize
		,	::gph::SCoord<int32_t>	rectPosition
		,	double					radius
		,	::gph::SColor			color
		);
	int									drawLineVertical
		(	::gph::SColor			* pixels
		,	::gph::SCoord<int32_t>	targetSize
		,	::gph::SCoord<int32_t>	linePosition
		,	uint32_t				lineLength
		,	::gph::SColor			color
		);
	int									drawLineHorizontal
		(	::gph::SColor			* pixels
		,	::gph::SCoord<int32_t>	targetSize
		,	::gph::SCoord<int32_t>	linePosition
		,	uint32_t				lineLength
		,	::gph::SColor			color
		);
} // namespace

#endif // FRAMEWORK_H_29837472983
