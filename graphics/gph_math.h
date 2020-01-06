#include <cstdint>
#include <algorithm>

#ifndef GPH_MATH_H_2983749283
#define GPH_MATH_H_2983749283

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
	struct SRectangle		{ SCoord<_tValue> Offset, Size; };
	template<typename _tValue>
	struct SCircle			{ SCoord<_tValue> Offset; double Radius; };
	template<typename _tValue>
	struct STriangle		{ SCoord<_tValue> A, B, C; };
	template<typename _tValue>
	struct SLine			{ SCoord<_tValue> A, B; };

	template<typename _tValue>
	double											clamp					(_tValue value, _tValue min, _tValue max)											{ return ::std::min(::std::max(min, value), max); }

	template<typename _tValue>
	int												orient2d				(const SCoord<_tValue>& a, const SCoord<_tValue>& b, const SCoord<_tValue>& c)		{ return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x); }

	template<typename _tValue>	inline	_tValue&	min3					(_tValue& a, _tValue& b, _tValue& c)												{ return ::std::min(a, ::std::min(b, c)); }
	template<typename _tValue>	inline	_tValue&	max3					(_tValue& a, _tValue& b, _tValue& c)												{ return ::std::max(a, ::std::max(b, c)); }

	template<typename _tValue>
	struct SMatrix4 {
		_tValue											_11, _12, _13, _14;
		_tValue											_21, _22, _23, _24;
		_tValue											_31, _32, _33, _34;
		_tValue											_41, _42, _43, _44;

		SCoord<_tValue>									operator+				(const SCoord<_tValue> & other)		const	noexcept		{ return
			{ _11 + other._11 , _12 + other._12 , _13 + other._13 , _14 + other._14
			, _21 + other._21 , _22 + other._22 , _23 + other._23 , _24 + other._24
			, _31 + other._31 , _32 + other._32 , _33 + other._33 , _34 + other._34
			, _41 + other._41 , _42 + other._42 , _43 + other._43 , _44 + other._44
			};
		}
		SCoord<_tValue>									operator-				(const SCoord<_tValue> & other)		const	noexcept		{ return
			{ _11 - other._11 , _12 - other._12 , _13 - other._13 , _14 - other._14
			, _21 - other._21 , _22 - other._22 , _23 - other._23 , _24 - other._24
			, _31 - other._31 , _32 - other._32 , _33 - other._33 , _34 - other._34
			, _41 - other._41 , _42 - other._42 , _43 - other._43 , _44 - other._44
			};
		}
		SCoord<_tValue>									operator/				(double scalar)						const	noexcept		{ return
			{ _11 / scalar, _12 / scalar, _13 / scalar, _14 / scalar
			, _21 / scalar, _22 / scalar, _23 / scalar, _24 / scalar
			, _31 / scalar, _32 / scalar, _33 / scalar, _34 / scalar
			, _41 / scalar, _42 / scalar, _43 / scalar, _44 / scalar
			};
		}
		SCoord<_tValue>									operator*				(double scalar)						const	noexcept		{ return
			{ _11 * scalar, _12 * scalar, _13 * scalar, _14 * scalar
			, _21 * scalar, _22 * scalar, _23 * scalar, _24 * scalar
			, _31 * scalar, _32 * scalar, _33 * scalar, _34 * scalar
			, _41 * scalar, _42 * scalar, _43 * scalar, _44 * scalar
			};
		}
	};
} // namespace

#endif // GPH_MATH_H_2983749283
