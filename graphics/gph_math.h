#include <cstdint>
#include <algorithm>

#ifndef GPH_MATH_H_2983749283
#define GPH_MATH_H_2983749283

namespace gph
{
	struct					SPairSinCos				{ double Sin, Cos; };

	static inline			SPairSinCos				getSinCos				(double theta)																noexcept	{ return {sin(theta), cos(theta)};						}

	template<typename _tBase>
	struct SCoord2 {
		typedef									SCoord2<_tBase>			TCoord2;
												_tBase					x, y;
		//
		constexpr								TCoord2					operator+				(const TCoord2& other)												const	noexcept	{ return {(_tBase)(x + other.x), (_tBase)(y + other.y)};				}
		constexpr								TCoord2					operator-				(const TCoord2& other)												const	noexcept	{ return {(_tBase)(x - other.x), (_tBase)(y - other.y)};				}
		constexpr								TCoord2					operator*				(double			scalar)												const	noexcept	{ return {(_tBase)(x * scalar), (_tBase)(y * scalar)};					}
		constexpr								TCoord2					operator/				(double			scalar)												const				{ return {(_tBase)(x / scalar), (_tBase)(y / scalar)};					}
		constexpr								TCoord2					operator*				(int64_t		scalar)												const	noexcept	{ return {(_tBase)(x * scalar), (_tBase)(y * scalar)};					}
		constexpr								TCoord2					operator/				(int64_t		scalar)												const				{ return {(_tBase)(x / scalar), (_tBase)(y / scalar)};					}
		constexpr								TCoord2					operator*				(uint64_t		scalar)												const	noexcept	{ return {(_tBase)(x * scalar), (_tBase)(y * scalar)};					}
		constexpr								TCoord2					operator/				(uint64_t		scalar)												const				{ return {(_tBase)(x / scalar), (_tBase)(y / scalar)};					}
		constexpr								TCoord2					operator*				(int32_t		scalar)												const	noexcept	{ return {(_tBase)(x * scalar), (_tBase)(y * scalar)};					}
		constexpr								TCoord2					operator/				(int32_t		scalar)												const				{ return {(_tBase)(x / scalar), (_tBase)(y / scalar)};					}
		constexpr								TCoord2					operator*				(uint32_t		scalar)												const	noexcept	{ return {(_tBase)(x * scalar), (_tBase)(y * scalar)};					}
		constexpr								TCoord2					operator/				(uint32_t		scalar)												const				{ return {(_tBase)(x / scalar), (_tBase)(y / scalar)};					}
		//
												TCoord2&				operator+=				(const TCoord2& other)														noexcept	{ x += other.x; y += other.y;							return *this;	}
												TCoord2&				operator-=				(const TCoord2& other)														noexcept	{ x -= other.x; y -= other.y;							return *this;	}
												TCoord2&				operator*=				(double			scalar)														noexcept	{ x = (_tBase)(x * scalar); y = (_tBase)(y * scalar);	return *this;	}
												TCoord2&				operator/=				(double			scalar)																	{ x = (_tBase)(x / scalar); y = (_tBase)(y / scalar);	return *this;	}
												TCoord2&				operator*=				(int64_t		scalar)														noexcept	{ x = (_tBase)(x * scalar); y = (_tBase)(y * scalar);	return *this;	}
												TCoord2&				operator/=				(int64_t		scalar)																	{ x = (_tBase)(x / scalar); y = (_tBase)(y / scalar);	return *this;	}
												TCoord2&				operator*=				(int32_t		scalar)														noexcept	{ x = (_tBase)(x * scalar); y = (_tBase)(y * scalar);	return *this;	}
												TCoord2&				operator/=				(int32_t		scalar)																	{ x = (_tBase)(x / scalar); y = (_tBase)(y / scalar);	return *this;	}
												TCoord2&				operator*=				(uint32_t		scalar)														noexcept	{ x = (_tBase)(x * scalar); y = (_tBase)(y * scalar);	return *this;	}
												TCoord2&				operator/=				(uint32_t		scalar)																	{ x = (_tBase)(x / scalar); y = (_tBase)(y / scalar);	return *this;	}
												TCoord2&				operator*=				(uint64_t		scalar)														noexcept	{ x = (_tBase)(x * scalar); y = (_tBase)(y * scalar);	return *this;	}
												TCoord2&				operator/=				(uint64_t		scalar)																	{ x = (_tBase)(x / scalar); y = (_tBase)(y / scalar);	return *this;	}
		//
		constexpr								bool					operator==				(const TCoord2& other)												const	noexcept	{ return x == other.x && y == other.y;									}
		inline constexpr						bool					operator!=				(const TCoord2& other)												const	noexcept	{ return !operator==(other);											}
		constexpr								TCoord2					operator-				()																	const	noexcept	{ return {x*-1, y*-1};													}
		//
		template<typename _t>
		constexpr inline						SCoord2<_t>				Cast					()																	const	noexcept	{ return {(_t)x, (_t)y};																																				}
		inline									TCoord2&				InPlaceScale			(double			scalar)														noexcept	{ return *this *= scalar;																																				}
		inline									TCoord2&				InPlaceScale			(double scalarx, double scalary)											noexcept	{ return *this = {(_tBase)(x * scalarx), (_tBase)(y * scalary)};																										}
		inline									TCoord2&				InPlaceScale			(const SCoord2<double>& other)												noexcept	{ return InPlaceScale(other.x, other.y);																																}
		inline									TCoord2&				InPlaceNormalize		()																						{ const _tBase sqLen = LengthSquared(); return sqLen ? *this /= ::sqrt(sqLen) : *this;																	}
		inline constexpr						TCoord2					GetScaled				(double			scalar)												const	noexcept	{ return {(_tBase)(x * scalar), (_tBase)(y * scalar)};																													}
		inline constexpr						TCoord2					GetScaled				(double scalarx, double scalary)									const	noexcept	{ return {(_tBase)(x * scalarx), (_tBase)(y * scalary)};																												}
		inline									TCoord2					GetNormalized			()																	const				{ const _tBase sqLen = LengthSquared(); if(sqLen) { const double len = ::sqrt(sqLen); return {(_tBase)(x / len), (_tBase)(y / len)}; } else return {x, y};	}
		constexpr								double					Dot						(const TCoord2& other)												const	noexcept	{ return x * other.x + y * other.y;																																		}
		constexpr								_tBase					LengthSquared			()																	const	noexcept	{ return x * x + y * y;																																					}
		constexpr								double					Length					()																	const				{ const _tBase sqLen = LengthSquared(); return sqLen ? ::sqrt(sqLen) : 0;																								}
		constexpr								double					AngleWith				(const TCoord2& other)												const				{ double lengthsProduct = Length() * other.Length(); return lengthsProduct ? ::acos(Dot(other) / lengthsProduct) : 0;													}
												void					AddScaled				(const TCoord2& vectorToScaleAndAdd, float scale)										{
			x																+= vectorToScaleAndAdd.x * scale;
			y																+= vectorToScaleAndAdd.y * scale;
		}
												TCoord2&				Rotate					(double theta)																			{
			const ::gph::SPairSinCos											pairSinCos				= ::gph::getSinCos(theta);
			const double														px						= x * pairSinCos.Cos - y * pairSinCos.Sin;
			y																= (_tBase)(x * pairSinCos.Sin + y * pairSinCos.Cos);
			x																= (_tBase)px;
			return *this;
		}
	};	// struct SCoord2

	template<typename _tBase>
	struct SCoord3 {
		typedef									SCoord3<_tBase>			TCoord3;
												_tBase					x, y, z;
		inline									const _tBase&			operator[]				(uint32_t index)													const				{ if(index >= 3) throw(""); return ((&x)[index]); }
		inline									_tBase&					operator[]				(uint32_t index)																		{ if(index >= 3) throw(""); return ((&x)[index]); }
		//
		constexpr								TCoord3					operator+				(const TCoord3& other)												const	noexcept	{ return {x + other.x, y + other.y, z + other.z};												}
		constexpr								TCoord3					operator-				(const TCoord3& other)												const	noexcept	{ return {x - other.x, y - other.y, z - other.z};												}
		constexpr								TCoord3					operator*				(double scalar)														const	noexcept	{ return {(_tBase)(x * scalar), (_tBase)(y * scalar), (_tBase)(z * scalar)};					}
		constexpr								TCoord3					operator/				(double scalar)														const				{ return {(_tBase)(x / scalar), (_tBase)(y / scalar), (_tBase)(z / scalar)};					}
		constexpr								TCoord3					operator*				(int64_t scalar)													const	noexcept	{ return {(_tBase)(x * scalar), (_tBase)(y * scalar), (_tBase)(z * scalar)};					}
		constexpr								TCoord3					operator/				(int64_t scalar)													const				{ return {(_tBase)(x / scalar), (_tBase)(y / scalar), (_tBase)(z / scalar)};					}
		constexpr								TCoord3					operator*				(uint64_t scalar)													const	noexcept	{ return {(_tBase)(x * scalar), (_tBase)(y * scalar), (_tBase)(z * scalar)};					}
		constexpr								TCoord3					operator/				(uint64_t scalar)													const				{ return {(_tBase)(x / scalar), (_tBase)(y / scalar), (_tBase)(z / scalar)};					}
		constexpr								TCoord3					operator*				(int32_t scalar)													const	noexcept	{ return {(_tBase)(x * scalar), (_tBase)(y * scalar), (_tBase)(z * scalar)};					}
		constexpr								TCoord3					operator/				(int32_t scalar)													const				{ return {(_tBase)(x / scalar), (_tBase)(y / scalar), (_tBase)(z / scalar)};					}
		constexpr								TCoord3					operator*				(uint32_t scalar)													const	noexcept	{ return {(_tBase)(x * scalar), (_tBase)(y * scalar), (_tBase)(z * scalar)};					}
		constexpr								TCoord3					operator/				(uint32_t scalar)													const				{ return {(_tBase)(x / scalar), (_tBase)(y / scalar), (_tBase)(z / scalar)};					}
		//
												TCoord3&				operator+=				(const TCoord3& other)														noexcept	{ x += other.x; y += other.y; z += other.z;										return *this;	}
												TCoord3&				operator-=				(const TCoord3& other)														noexcept	{ x -= other.x; y -= other.y; z -= other.z;										return *this;	}
												TCoord3&				operator*=				(double scalar)																noexcept	{ x = (_tBase)(x * scalar); y = (_tBase)(y * scalar); z = (_tBase)(z * scalar);	return *this;	}
												TCoord3&				operator/=				(double scalar)																			{ x = (_tBase)(x / scalar); y = (_tBase)(y / scalar); z = (_tBase)(z / scalar);	return *this;	}
												TCoord3&				operator*=				(int64_t scalar)															noexcept	{ x = (_tBase)(x * scalar); y = (_tBase)(y * scalar); z = (_tBase)(z * scalar);	return *this;	}
												TCoord3&				operator/=				(int64_t scalar)																		{ x = (_tBase)(x / scalar); y = (_tBase)(y / scalar); z = (_tBase)(z / scalar);	return *this;	}
												TCoord3&				operator*=				(int32_t scalar)															noexcept	{ x = (_tBase)(x * scalar); y = (_tBase)(y * scalar); z = (_tBase)(z * scalar);	return *this;	}
												TCoord3&				operator/=				(int32_t scalar)																		{ x = (_tBase)(x / scalar); y = (_tBase)(y / scalar); z = (_tBase)(z / scalar);	return *this;	}
												TCoord3&				operator*=				(uint32_t scalar)															noexcept	{ x = (_tBase)(x * scalar); y = (_tBase)(y * scalar); z = (_tBase)(z * scalar);	return *this;	}
												TCoord3&				operator/=				(uint32_t scalar)																		{ x = (_tBase)(x / scalar); y = (_tBase)(y / scalar); z = (_tBase)(z / scalar);	return *this;	}
												TCoord3&				operator*=				(uint64_t scalar)															noexcept	{ x = (_tBase)(x * scalar); y = (_tBase)(y * scalar); z = (_tBase)(z * scalar);	return *this;	}
												TCoord3&				operator/=				(uint64_t scalar)																		{ x = (_tBase)(x / scalar); y = (_tBase)(y / scalar); z = (_tBase)(z / scalar);	return *this;	}
		//
		constexpr								bool					operator==				(const TCoord3& other)												const	noexcept	{ return x == other.x && y == other.y && z == other.z;											}
		inline constexpr						bool					operator!=				(const TCoord3& other)												const	noexcept	{ return !operator==(other);																	}
		constexpr								TCoord3					operator-				()																	const	noexcept	{ return {x*-1, y*-1, z*-1};																	}
		//
		template<typename _t>
		inline constexpr 						SCoord3<_t>				Cast					()																	const	noexcept	{ return {(_t)x, (_t)y, (_t)z};																							}
		inline									TCoord3&				Scale					(double scalar)																noexcept	{ return *this *= scalar;																								}
		inline									TCoord3&				Scale					(const TCoord3& other)														noexcept	{ x *= other.x; y *= other.y; z *= other.z; return *this;																}
		inline									TCoord3&				Normalize				()																						{ const _tBase sqLen = LengthSquared(); return sqLen ? *this /= ::sqrt(sqLen) : *this;									}
		constexpr								double					Dot						(const TCoord3& other)												const	noexcept	{ return x * other.x + y * other.y + z * other.z;																		}
		constexpr								_tBase					LengthSquared			()																	const	noexcept	{ return x * x + y * y + z * z;																							}
		constexpr								double					Length					()																	const				{ const _tBase sqLen = LengthSquared(); return sqLen ? ::sqrt(sqLen) : 0;												}
		constexpr								double					AngleWith				(const TCoord3& other)												const				{ const double lengthsProduct = Length() * other.Length(); return lengthsProduct ? ::acos(Dot(other) / lengthsProduct) : 0;	}
												void					AddScaled				(const TCoord3& vectorToScaleAndAdd, float scale)										{
			x																+= vectorToScaleAndAdd.x * scale;
			y																+= vectorToScaleAndAdd.y * scale;
			z																+= vectorToScaleAndAdd.z * scale;
		}

		constexpr								TCoord3					Cross					(const TCoord3& right)												const	noexcept	{ return {y * right.z - z * right.y, z * right.x - x * right.z, x * right.y - y * right.x };	}
												TCoord3&				Cross					(const TCoord3& vector1, const TCoord3& vector2)							noexcept	{
			x																= vector1.y * vector2.z - vector1.z * vector2.y;
			y																= vector1.z * vector2.x - vector1.x * vector2.z;
			z																= vector1.x * vector2.y - vector1.y * vector2.x;
			return *this;
		}
												TCoord3&				CrossAndNormalize		(const TCoord3& in_vLeft, const TCoord3& in_vRight)										{
			x																= in_vLeft->y * in_vRight->z - in_vLeft->z * in_vRight->y;
			y																= in_vLeft->z * in_vRight->x - in_vLeft->x * in_vRight->z;
			z																= in_vLeft->x * in_vRight->y - in_vLeft->y * in_vRight->x;
			Normalize();
			return *this;
		};
												TCoord3&				RotateX					(double theta)																			{
			const ::gph::SPairSinCos											pairSinCos				= ::gph::getSinCos(theta);
			const double														pz						= y * pairSinCos.Cos - z * pairSinCos.Sin;
			y																= (_tBase)(y * pairSinCos.Sin + z * pairSinCos.Cos);
			z																= (_tBase)pz;
			return *this;
		}

												TCoord3&				RotateY					(double theta)																			{
			const ::gph::SPairSinCos											pairSinCos				= ::gph::getSinCos(theta);
			const double														px						= x * pairSinCos.Cos - z * pairSinCos.Sin;
			z																= (_tBase)(x * pairSinCos.Sin + z * pairSinCos.Cos);
			x																= (_tBase)px;
			return *this;
		}

												TCoord3&				RotateZ					(double theta)																			{
			const ::gph::SPairSinCos											pairSinCos				= ::gph::getSinCos(theta);
			const double														px						= x * pairSinCos.Cos - y * pairSinCos.Sin;
			y																= (_tBase)(x * pairSinCos.Sin + y * pairSinCos.Cos);
			x																= (_tBase)px;
			return *this;
		}
	};	// struct SCoord3

	template<typename _tValue>	struct SRectangle		{ SCoord2<_tValue> Offset, Size;			};
	template<typename _tValue>	struct SCircle			{ SCoord2<_tValue> Offset; double Radius;	};
	template<typename _tValue>	struct STriangle		{ SCoord2<_tValue> A, B, C;					};
	template<typename _tValue>	struct STriangle3		{ SCoord3<_tValue> A, B, C;					};
	template<typename _tValue>	struct SLine			{ SCoord2<_tValue> A, B;					};

	template<typename _tValue>
	double											clamp					(_tValue value, _tValue min, _tValue max)											{ return ::std::min(::std::max(min, value), max); }

	template<typename _tValue>
	int												orient2d				(const SCoord2<_tValue>& a, const SCoord2<_tValue>& b, const SCoord2<_tValue>& c)		{ return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x); }

	template<typename _tValue>	inline	_tValue&	min3					(_tValue& a, _tValue& b, _tValue& c)												{ return ::std::min(a, ::std::min(b, c)); }
	template<typename _tValue>	inline	_tValue&	max3					(_tValue& a, _tValue& b, _tValue& c)												{ return ::std::max(a, ::std::max(b, c)); }

	template<typename _tBase>
	struct SMatrix4 {
		typedef				SMatrix4<_tBase>	_tMat4, TMatrix4;
		typedef				SCoord3<_tBase>		TCoord3;
							_tBase				_11, _12, _13, _14
								,				_21, _22, _23, _24
								,				_31, _32, _33, _34
								,				_41, _42, _43, _44
								;

		inline				TCoord3&			operator[]					(uint32_t index)																					{ if(index >= 4) throw(""); return *(TCoord3*)((&_11) + 4 * index); }
		constexpr			bool				operator ==					(const _tMat4& other)															const	noexcept	{ return _11 == other._11 && _12 == other._12 && _13 == other._13 && _14 == other._14 && _21 == other._21 && _22 == other._22 && _23 == other._23 && _24 == other._24 && _31 == other._31 && _32 == other._32 && _33 == other._33 && _34 == other._34 && _41 == other._41 && _42 == other._42 && _43 == other._43 && _44 == other._44; }
		constexpr inline	bool				operator !=					(const _tMat4& other)															const	noexcept	{ return !operator==(other); }

		constexpr			_tMat4				operator +					(const _tMat4& other)															const	noexcept	{ return {_11 + other._11	, _12 + other._12	, _13 + other._13	, _14 + other._14	, _21 + other._21	, _22 + other._22	, _23 + other._23	, _24 + other._24	, _31 + other._31	, _32 + other._32	, _33 + other._33	, _34 + other._34	, _41 + other._41	, _42 + other._42	, _43 + other._43	, _44 + other._44	}; }
		constexpr			_tMat4				operator -					(const _tMat4& other)															const	noexcept	{ return {_11 - other._11	, _12 - other._12	, _13 - other._13	, _14 - other._14	, _21 - other._21	, _22 - other._22	, _23 - other._23	, _24 - other._24	, _31 - other._31	, _32 - other._32	, _33 - other._33	, _34 - other._34	, _41 - other._41	, _42 - other._42	, _43 - other._43	, _44 - other._44	}; }
		constexpr			_tMat4				operator *					(double scalar)																	const	noexcept	{								return {(_tBase)(_11 * scalar), (_tBase)(_12 * scalar), (_tBase)(_13 * scalar), (_tBase)(_14 * scalar), (_tBase)(_21 * scalar), (_tBase)(_22 * scalar), (_tBase)(_23 * scalar), (_tBase)(_24 * scalar), (_tBase)(_31 * scalar), (_tBase)(_32 * scalar), (_tBase)(_33 * scalar), (_tBase)(_34 * scalar), (_tBase)(_41 * scalar), (_tBase)(_42 * scalar), (_tBase)(_43 * scalar), (_tBase)(_44 * scalar) }; }
							_tMat4				operator /					(double scalar)																	const				{ if(0 == scalar) throw("");	return {(_tBase)(_11 / scalar), (_tBase)(_12 / scalar), (_tBase)(_13 / scalar), (_tBase)(_14 / scalar), (_tBase)(_21 / scalar), (_tBase)(_22 / scalar), (_tBase)(_23 / scalar), (_tBase)(_24 / scalar), (_tBase)(_31 / scalar), (_tBase)(_32 / scalar), (_tBase)(_33 / scalar), (_tBase)(_34 / scalar), (_tBase)(_41 / scalar), (_tBase)(_42 / scalar), (_tBase)(_43 / scalar), (_tBase)(_44 / scalar) }; }
		constexpr			_tMat4				operator *					(const _tMat4& right)															const	noexcept	{
			return
				{ _11*right._11 + _12*right._21 + _13*right._31 + _14*right._41, _11*right._12 + _12*right._22 + _13*right._32 + _14*right._42, _11*right._13 + _12*right._23 + _13*right._33 + _14*right._43, _11*right._14 + _12*right._24 + _13*right._34 + _14*right._44
				, _21*right._11 + _22*right._21 + _23*right._31 + _24*right._41, _21*right._12 + _22*right._22 + _23*right._32 + _24*right._42, _21*right._13 + _22*right._23 + _23*right._33 + _24*right._43, _21*right._14 + _22*right._24 + _23*right._34 + _24*right._44
				, _31*right._11 + _32*right._21 + _33*right._31 + _34*right._41, _31*right._12 + _32*right._22 + _33*right._32 + _34*right._42, _31*right._13 + _32*right._23 + _33*right._33 + _34*right._43, _31*right._14 + _32*right._24 + _33*right._34 + _34*right._44
				, _41*right._11 + _42*right._21 + _43*right._31 + _44*right._41, _41*right._12 + _42*right._22 + _43*right._32 + _44*right._42, _41*right._13 + _42*right._23 + _43*right._33 + _44*right._43, _41*right._14 + _42*right._24 + _43*right._34 + _44*right._44
				};
		}
		inline				_tMat4&				operator +=					(const _tMat4& other)																	noexcept	{ return *this = operator+(other	); }
		inline				_tMat4&				operator -=					(const _tMat4& other)																	noexcept	{ return *this = operator-(other	); }
		inline				_tMat4&				operator *=					(double scalar)																			noexcept	{ return *this = operator*(scalar	); }
		inline				_tMat4&				operator /=					(double scalar)																						{ return *this = operator/(scalar	); }
		inline				_tMat4&				operator *=					(const _tMat4& right)																	noexcept	{ return *this = operator*(right	); }

		inline				void				SetTranspose				(const _tMat4& m)																					{ *this = m.GetTranspose(); }
		inline				void				SetInverse					(const _tMat4& m)																					{ *this = m.GetInverse();	}
		inline				void				Transpose					(const _tMat4& m)																					{ *this = GetTranspose();	}
		inline				void				Invert						()																									{ *this = GetInverse();		}

		inline				_tMat4&				LinearInterpolate			(const _tMat4&p, const _tMat4&q, double fTime)											noexcept	{ return *this = ((q-p)*fTime)+p; }
		constexpr			TCoord3				InverseTranslate			(const TCoord3& vec)															const	noexcept	{ return { vec.x - _41, vec.y - _42, vec.z - _43 }; }
							void				InverseTranslateInPlace		(TCoord3& vec)																	const	noexcept	{ vec.x -= _41; vec.y -= _42; vec.z -= _43; }
		constexpr			TCoord3				Transform					(const TCoord3& v)																const				{
			return
			TCoord3
				{	(v.x*_11 + v.y*_21 + v.z*_31 + _41)	// x
				,	(v.x*_12 + v.y*_22 + v.z*_32 + _42)	// y
				,	(v.x*_13 + v.y*_23 + v.z*_33 + _43)	// z
				}	// GVECTOR3
				/	(v.x*_14 + v.y*_24 + v.z*_34 + _44)	// w
			;
		}
		constexpr			TCoord3				TransformDirection			(const TCoord3& vector)														const	noexcept	{
			return
				{	vector.x * _11 + vector.y * _21 + vector.z * _31
				,	vector.x * _12 + vector.y * _22 + vector.z * _32
				,	vector.x * _13 + vector.y * _23 + vector.z * _33
				};
		}
		constexpr			TCoord3				TransformInverseDirection	(const TCoord3& _v)															const	noexcept	{
			return
				{	_v.x * _11 + _v.y * _12 + _v.z * _13
				,	_v.x * _21 + _v.y * _22 + _v.z * _23
				,	_v.x * _31 + _v.y * _32 + _v.z * _33
				};
		}
							void				Viewport					(const ::gph::SCoord2<uint32_t> & offscreenMetrics, double fFar, double fNear)			noexcept	{
			_11 = (_tBase)(2.0 / offscreenMetrics.x);	_12 =										_13 =												_14 =
			_21 = (_tBase)0;							_22 = (_tBase)(2.0 / offscreenMetrics.y);	_23 =												_24 =
			_31 =										_32 = (_tBase)0;							_33 = (_tBase)(1.0f / (fFar - fNear));				_34 =
			_41 =										_42 = (_tBase)0;							_43 = (_tBase)(-fNear * (1.0f / (fFar - fNear)));	_44 = (_tBase)1;
			_33 = 1;
			_43 = 0;
		}
							void				Identity					()																						noexcept	{
			*this									=
				{ (_tBase)1,  (_tBase)0,  (_tBase)0,  (_tBase)0
				, (_tBase)0,  (_tBase)1,  (_tBase)0,  (_tBase)0
				, (_tBase)0,  (_tBase)0,  (_tBase)1,  (_tBase)0
				, (_tBase)0,  (_tBase)0,  (_tBase)0,  (_tBase)1
				};
		}
							void				RotationX					(double angle)																			noexcept	{
			::gph::SPairSinCos							angleSinCos					= ::gph::getSinCos(angle);
			_11 = (_tBase)1;	_12 =							_13 =
			_31 = (_tBase)0;	_32 = -(_tBase)angleSinCos.Sin;	_33 = (_tBase)angleSinCos.Cos;
			_21 = (_tBase)0;	_22 =  (_tBase)angleSinCos.Cos;	_23 = (_tBase)angleSinCos.Sin;
			_41 = _42 = _43 = _14 = _24 = _34 = (_tBase)0; _44 = (_tBase)1;
		}
							void				RotationY					(double angle)																			noexcept	{
			::gph::SPairSinCos							angleSinCos					= ::gph::getSinCos(angle);
			_11 = (_tBase)angleSinCos.Cos;	_12 = (_tBase)0;	_13 = -(_tBase)angleSinCos.Sin;
			_21 = (_tBase)0;				_22 = (_tBase)1;	_23 =  (_tBase)0;
			_31 = (_tBase)angleSinCos.Sin;	_32 = (_tBase)0;	_33 =  (_tBase)angleSinCos.Cos;
			//
			_41 = _42 = _43 = _14 = _24 = _34 = (_tBase)0; _44 = (_tBase)1;
		}
							void				RotationZ					(double angle)																			noexcept	{
			::gph::SPairSinCos							angleSinCos					= ::gph::getSinCos(angle);
			_11 =  (_tBase)angleSinCos.Cos;	_12 = (_tBase)angleSinCos.Sin;	_13 = (_tBase)0;
			_21 = -(_tBase)angleSinCos.Sin;	_22 = (_tBase)angleSinCos.Cos;	_23 =
			_31 =							_32 = (_tBase)0;				_33 = (_tBase)1;
			//
			_41 = _42 = _43 = _14 = _24 = _34 = (_tBase)0; _44 = (_tBase)1;
		}
							void				Scale						(_tBase x, _tBase y, _tBase z, bool bEraseContent)										noexcept	{ Scale({x, y, z}, bEraseContent); }
		inline				void				Scale						(const TCoord3& ypr, bool bEraseContent)												noexcept
		{
			if( bEraseContent ) {
				_11 = (_tBase)ypr.x;	_12 =					_13 =					_14 =
				_21 = (_tBase)0;		_22 = (_tBase)ypr.y;	_23 =					_24 =
				_31 =					_32 = (_tBase)0;		_33 = (_tBase)ypr.z;	_34 =
				_41 =					_42 =					_43 = (_tBase)0;		_44 = (_tBase)1;
			}
			else {
				_11 = (_tBase)(_11*ypr.x); _22 = (_tBase)(_22*ypr.y); _33 = (_tBase)(_33*ypr.z);
			}
		}
							void				SetTranslation				(const TCoord3& vTranslation, bool bEraseContent)										noexcept	{
			if( bEraseContent ) {
				_11 = (_tBase)1;	_12 =				_13 =				_14 =
				_21 = (_tBase)0;	_22 = (_tBase)1;	_23 =				_24 =
				_31 =				_32 = (_tBase)0;	_33 = (_tBase)1;	_34 = (_tBase)0;
				/*_41 =				_42 =				_43 */				_44 = (_tBase)1;
			}
			_41	= vTranslation.x; _42 = vTranslation.y; _43 = vTranslation.z;
		}
							void				FieldOfView					(double fAngle, double fAspect, double zn, double zf)												{
			double										fTan						= tan( fAngle / 2.0 );
			_11 = (_tBase)(1.0 / ( fAspect * fTan )			);
			_22 = (_tBase)(1.0 / fTan						);
			_33 = (_tBase)(zf / ( zf - zn )					);
			_34 = (_tBase)(1								);
			_43 = (_tBase)(( -( zf * zn ) ) / ( zf - zn )	);

			_12 = _13 = _14 = _21 = _23 = _24 = _31 = _32 = _41 = _42 = _44 = (_tBase)0;
		//	return *this;
		} // FoV
							void				LookAt						(const TCoord3& vPosition, const TCoord3& vTarget, const TCoord3& vUp)							{
			TCoord3										F							= TCoord3{vTarget - vPosition}.Normalize();
			TCoord3										R							= vUp	.Cross(F)		.Normalize();
			TCoord3										U							= F		.Cross(R)		.Normalize();

			_11 = R.x;	_12 = U.x;	_13 = F.x;	_14 = (_tBase)0;
			_21 = R.y;	_22 = U.y;	_23 = F.y;	_24 = (_tBase)0;
			_31 = R.z;	_32 = U.z;	_33 = F.z;	_34 = (_tBase)0;
			_41 = (_tBase)-vPosition.Dot(R); 	// x
			_42 = (_tBase)-vPosition.Dot(U); 	// y
			_43 = (_tBase)-vPosition.Dot(F);	// z
			_44 = (_tBase)1;
		}

							void				View3D						(const TCoord3& vPosition, const TCoord3& vRight, const TCoord3& vUp, const TCoord3& vFront)	{
			_11 = vRight.x;	_12 = vUp.x; _13 = vFront.x; _14 = (_tBase)0;
			_21 = vRight.y;	_22 = vUp.y; _23 = vFront.y; _24 = (_tBase)0;
			_31 = vRight.z;	_32 = vUp.z; _33 = vFront.z; _34 = (_tBase)0;
			_41 = (_tBase)-vPosition.Dot(vRight	);
			_42 = (_tBase)-vPosition.Dot(vUp	);
			_43 = (_tBase)-vPosition.Dot(vFront	);
			_44 = (_tBase)1;
		}
							void				Billboard					(const TCoord3& vPos, const TCoord3& vDir, const TCoord3& vWorldUp)								{
			TCoord3										vUp
				,										vRight
				;
			double										fAngle						= vWorldUp.Dot(vDir);
			vUp										= vWorldUp - vDir * fAngle;
			vUp.Normalize();
			vRight									= vUp.Cross( vDir );
			_11 = vRight.x;	_12 = vRight.y;	_13 = vRight.z;	_14=(_tBase)0;
			_21 = vUp.x; 	_22 = vUp.y; 	_23 = vUp.z; 	_24=(_tBase)0;
			_31 = vDir.x;	_32 = vDir.y;	_33 = vDir.z;	_34=(_tBase)0;
			_41 = vPos.x;	_42 = vPos.y;	_43 = vPos.z;	_44=(_tBase)1;
		} // Billboard
		inline				void				Rotation					(_tBase x, _tBase y, _tBase z)																		{ return Rotation({x, y, z}); }
							void				Rotation					(const TCoord3 &vc)																					{
			::gph::SPairSinCos							yaw							= ::gph::getSinCos(vc.z);
			::gph::SPairSinCos							pitch						= ::gph::getSinCos(vc.y);
			::gph::SPairSinCos							roll						= ::gph::getSinCos(vc.x);

			_14 = _24 = _34 = _41 = _42 = _43		= (_tBase)0;
			_44										= (_tBase)1;

			_11										= (_tBase)(pitch.Cos*yaw.Cos								);
			_12										= (_tBase)(pitch.Cos*yaw.Sin								);
			_13										= (_tBase)(-pitch.Sin										);
			_21										= (_tBase)(roll.Sin*pitch.Sin*yaw.Cos+roll.Cos*-yaw.Sin		);
			_22										= (_tBase)(roll.Sin*pitch.Sin*yaw.Sin+roll.Cos* yaw.Cos		);
			_23										= (_tBase)(roll.Sin*pitch.Cos								);
			_31										= (_tBase)(roll.Cos*pitch.Sin*yaw.Cos+-roll.Sin*-yaw.Sin	);
			_32										= (_tBase)(roll.Cos*pitch.Sin*yaw.Sin+-roll.Sin* yaw.Cos	);
			_33										= (_tBase)(roll.Cos*pitch.Cos								);

		//	return *this;
		} // Rota

							void				RotationArbitraryAxis		(const TCoord3& _vcAxis, _tBase a)																		{
			::gph::SPairSinCos							pairSinCos					= ::gph::getSinCos(a);
			TCoord3										vcAxis						= _vcAxis;
			double										fSum						= 1.0 - pairSinCos.Cos;

			if( vcAxis.LengthSquared() != 1.0 )
				vcAxis.Normalize();

			_11										= (_tBase)(	(vcAxis.x * vcAxis.x) * fSum + pairSinCos.Cos				);
			_12										= (_tBase)(	(vcAxis.x * vcAxis.y) * fSum - (vcAxis.z * pairSinCos.Sin)	);
			_13										= (_tBase)(	(vcAxis.x * vcAxis.z) * fSum + (vcAxis.y * pairSinCos.Sin)	);

			_21										= (_tBase)(	(vcAxis.y * vcAxis.x) * fSum + (vcAxis.z * pairSinCos.Sin)	);
			_22										= (_tBase)(	(vcAxis.y * vcAxis.y) * fSum + pairSinCos.Cos 				);
			_23										= (_tBase)(	(vcAxis.y * vcAxis.z) * fSum - (vcAxis.x * pairSinCos.Sin)	);

			_31										= (_tBase)(	(vcAxis.z * vcAxis.x) * fSum - (vcAxis.y * pairSinCos.Sin)	);
			_32										= (_tBase)(	(vcAxis.z * vcAxis.y) * fSum + (vcAxis.x * pairSinCos.Sin)	);
			_33										= (_tBase)(	(vcAxis.z * vcAxis.z) * fSum + pairSinCos.Cos				);

			_14 = _24 = _34 = _41 = _42 = _43		= (_tBase)0;
			_44										= (_tBase)1;
		}

		constexpr			_tMat4				GetTranspose				()																				const	noexcept	{ return {_11, _21, _31, _41,  _12, _22, _32, _42,  _13, _23, _33, _43,  _14, _24, _34, _44};	}
		inline constexpr	TCoord3				GetTranslation				()																				const	noexcept	{ return { _41, _42, _43 }; }
							_tMat4				GetInverse					()																				const				{
			_tMat4										mTranspose					= GetTranspose()
				,										mResult						= *this
				;
			_tBase										fTemp[12];

			fTemp[ 0]								= mTranspose._33 * mTranspose._44;
			fTemp[ 1]								= mTranspose._34 * mTranspose._43;
			fTemp[ 2]								= mTranspose._32 * mTranspose._44;
			fTemp[ 3]								= mTranspose._34 * mTranspose._42;
			fTemp[ 4]								= mTranspose._32 * mTranspose._43;
			fTemp[ 5]								= mTranspose._33 * mTranspose._42;
			fTemp[ 6]								= mTranspose._31 * mTranspose._44;
			fTemp[ 7]								= mTranspose._34 * mTranspose._41;
			fTemp[ 8]								= mTranspose._31 * mTranspose._43;
			fTemp[ 9]								= mTranspose._33 * mTranspose._41;
			fTemp[10]								= mTranspose._31 * mTranspose._42;
			fTemp[11]								= mTranspose._32 * mTranspose._41;

			mResult._11								 = fTemp[0]*mTranspose._22 + fTemp[3]*mTranspose._23 + fTemp[4] *mTranspose._24;
			mResult._11								-= fTemp[1]*mTranspose._22 + fTemp[2]*mTranspose._23 + fTemp[5] *mTranspose._24;
			mResult._12								 = fTemp[1]*mTranspose._21 + fTemp[6]*mTranspose._23 + fTemp[9] *mTranspose._24;
			mResult._12								-= fTemp[0]*mTranspose._21 + fTemp[7]*mTranspose._23 + fTemp[8] *mTranspose._24;
			mResult._13								 = fTemp[2]*mTranspose._21 + fTemp[7]*mTranspose._22 + fTemp[10]*mTranspose._24;
			mResult._13								-= fTemp[3]*mTranspose._21 + fTemp[6]*mTranspose._22 + fTemp[11]*mTranspose._24;
			mResult._14								 = fTemp[5]*mTranspose._21 + fTemp[8]*mTranspose._22 + fTemp[11]*mTranspose._23;
			mResult._14								-= fTemp[4]*mTranspose._21 + fTemp[9]*mTranspose._22 + fTemp[10]*mTranspose._23;
			mResult._21								 = fTemp[1]*mTranspose._12 + fTemp[2]*mTranspose._13 + fTemp[5] *mTranspose._14;
			mResult._21								-= fTemp[0]*mTranspose._12 + fTemp[3]*mTranspose._13 + fTemp[4] *mTranspose._14;
			mResult._22								 = fTemp[0]*mTranspose._11 + fTemp[7]*mTranspose._13 + fTemp[8] *mTranspose._14;
			mResult._22								-= fTemp[1]*mTranspose._11 + fTemp[6]*mTranspose._13 + fTemp[9] *mTranspose._14;
			mResult._23								 = fTemp[3]*mTranspose._11 + fTemp[6]*mTranspose._12 + fTemp[11]*mTranspose._14;
			mResult._23								-= fTemp[2]*mTranspose._11 + fTemp[7]*mTranspose._12 + fTemp[10]*mTranspose._14;
			mResult._24								 = fTemp[4]*mTranspose._11 + fTemp[9]*mTranspose._12 + fTemp[10]*mTranspose._13;
			mResult._24								-= fTemp[5]*mTranspose._11 + fTemp[8]*mTranspose._12 + fTemp[11]*mTranspose._13;

			fTemp[ 0]								= mTranspose._13 * mTranspose._24;
			fTemp[ 1]								= mTranspose._14 * mTranspose._23;
			fTemp[ 2]								= mTranspose._12 * mTranspose._24;
			fTemp[ 3]								= mTranspose._14 * mTranspose._22;
			fTemp[ 4]								= mTranspose._12 * mTranspose._23;
			fTemp[ 5]								= mTranspose._13 * mTranspose._22;
			fTemp[ 6]								= mTranspose._11 * mTranspose._24;
			fTemp[ 7]								= mTranspose._14 * mTranspose._21;
			fTemp[ 8]								= mTranspose._11 * mTranspose._23;
			fTemp[ 9]								= mTranspose._13 * mTranspose._21;
			fTemp[10]								= mTranspose._11 * mTranspose._22;
			fTemp[11]								= mTranspose._12 * mTranspose._21;

			mResult._31								 = fTemp[0] *mTranspose._42 + fTemp[3] *mTranspose._43 + fTemp[4] *mTranspose._44;
			mResult._31								-= fTemp[1] *mTranspose._42 + fTemp[2] *mTranspose._43 + fTemp[5] *mTranspose._44;
			mResult._32								 = fTemp[1] *mTranspose._41 + fTemp[6] *mTranspose._43 + fTemp[9] *mTranspose._44;
			mResult._32								-= fTemp[0] *mTranspose._41 + fTemp[7] *mTranspose._43 + fTemp[8] *mTranspose._44;
			mResult._33								 = fTemp[2] *mTranspose._41 + fTemp[7] *mTranspose._42 + fTemp[10]*mTranspose._44;
			mResult._33								-= fTemp[3] *mTranspose._41 + fTemp[6] *mTranspose._42 + fTemp[11]*mTranspose._44;
			mResult._34								 = fTemp[5] *mTranspose._41 + fTemp[8] *mTranspose._42 + fTemp[11]*mTranspose._43;
			mResult._34								-= fTemp[4] *mTranspose._41 + fTemp[9] *mTranspose._42 + fTemp[10]*mTranspose._43;
			mResult._41								 = fTemp[2] *mTranspose._33 + fTemp[5] *mTranspose._34 + fTemp[1] *mTranspose._32;
			mResult._41								-= fTemp[4] *mTranspose._34 + fTemp[0] *mTranspose._32 + fTemp[3] *mTranspose._33;
			mResult._42								 = fTemp[8] *mTranspose._34 + fTemp[0] *mTranspose._31 + fTemp[7] *mTranspose._33;
			mResult._42								-= fTemp[6] *mTranspose._33 + fTemp[9] *mTranspose._34 + fTemp[1] *mTranspose._31;
			mResult._43								 = fTemp[6] *mTranspose._32 + fTemp[11]*mTranspose._34 + fTemp[3] *mTranspose._31;
			mResult._43								-= fTemp[10]*mTranspose._34 + fTemp[2] *mTranspose._31 + fTemp[7] *mTranspose._32;
			mResult._44								 = fTemp[10]*mTranspose._33 + fTemp[4] *mTranspose._31 + fTemp[9] *mTranspose._32;
			mResult._44								-= fTemp[8] *mTranspose._32 + fTemp[11]*mTranspose._33 + fTemp[5] *mTranspose._31;

			double										fDet							=	mTranspose._11*mResult._11
				+																			mTranspose._12*mResult._12
				+																			mTranspose._13*mResult._13
				+																			mTranspose._14*mResult._14
				;

			fDet									= 1.0/fDet;

			mResult._11								= (_tBase)(mResult._11 * fDet);
			mResult._12								= (_tBase)(mResult._12 * fDet);
			mResult._13								= (_tBase)(mResult._13 * fDet);
			mResult._14								= (_tBase)(mResult._14 * fDet);

			mResult._21								= (_tBase)(mResult._21 * fDet);
			mResult._22								= (_tBase)(mResult._22 * fDet);
			mResult._23								= (_tBase)(mResult._23 * fDet);
			mResult._24								= (_tBase)(mResult._24 * fDet);

			mResult._31								= (_tBase)(mResult._31 * fDet);
			mResult._32								= (_tBase)(mResult._32 * fDet);
			mResult._33								= (_tBase)(mResult._33 * fDet);
			mResult._34								= (_tBase)(mResult._34 * fDet);

			mResult._41								= (_tBase)(mResult._41 * fDet);
			mResult._42								= (_tBase)(mResult._42 * fDet);
			mResult._43								= (_tBase)(mResult._43 * fDet);
			mResult._44								= (_tBase)(mResult._44 * fDet);

			return mResult;
		}

							double				GetDeterminant				()																				const				{
			_tMat4										mTranspose					= GetTranspose()
				,										mThis						= *this
				;
			_tBase										fTemp[12];

			fTemp[ 0]								 = mTranspose._33 * mTranspose._44;
			fTemp[ 1]								 = mTranspose._34 * mTranspose._43;
			fTemp[ 2]								 = mTranspose._32 * mTranspose._44;
			fTemp[ 3]								 = mTranspose._34 * mTranspose._42;
			fTemp[ 4]								 = mTranspose._32 * mTranspose._43;
			fTemp[ 5]								 = mTranspose._33 * mTranspose._42;
			fTemp[ 6]								 = mTranspose._31 * mTranspose._44;
			fTemp[ 7]								 = mTranspose._34 * mTranspose._41;
			fTemp[ 8]								 = mTranspose._31 * mTranspose._43;
			fTemp[ 9]								 = mTranspose._33 * mTranspose._41;
			fTemp[10]								 = mTranspose._31 * mTranspose._42;
			fTemp[11]								 = mTranspose._32 * mTranspose._41;

			mThis._11								 = fTemp[0]*mTranspose._22 + fTemp[3]*mTranspose._23 + fTemp[4] *mTranspose._24;
			mThis._11								-= fTemp[1]*mTranspose._22 + fTemp[2]*mTranspose._23 + fTemp[5] *mTranspose._24;
			mThis._12								 = fTemp[1]*mTranspose._21 + fTemp[6]*mTranspose._23 + fTemp[9] *mTranspose._24;
			mThis._12								-= fTemp[0]*mTranspose._21 + fTemp[7]*mTranspose._23 + fTemp[8] *mTranspose._24;
			mThis._13								 = fTemp[2]*mTranspose._21 + fTemp[7]*mTranspose._22 + fTemp[10]*mTranspose._24;
			mThis._13								-= fTemp[3]*mTranspose._21 + fTemp[6]*mTranspose._22 + fTemp[11]*mTranspose._24;
			mThis._14								 = fTemp[5]*mTranspose._21 + fTemp[8]*mTranspose._22 + fTemp[11]*mTranspose._23;
			mThis._14								-= fTemp[4]*mTranspose._21 + fTemp[9]*mTranspose._22 + fTemp[10]*mTranspose._23;
			mThis._21								 = fTemp[1]*mTranspose._12 + fTemp[2]*mTranspose._13 + fTemp[5] *mTranspose._14;
			mThis._21								-= fTemp[0]*mTranspose._12 + fTemp[3]*mTranspose._13 + fTemp[4] *mTranspose._14;
			mThis._22								 = fTemp[0]*mTranspose._11 + fTemp[7]*mTranspose._13 + fTemp[8] *mTranspose._14;
			mThis._22								-= fTemp[1]*mTranspose._11 + fTemp[6]*mTranspose._13 + fTemp[9] *mTranspose._14;
			mThis._23								 = fTemp[3]*mTranspose._11 + fTemp[6]*mTranspose._12 + fTemp[11]*mTranspose._14;
			mThis._23								-= fTemp[2]*mTranspose._11 + fTemp[7]*mTranspose._12 + fTemp[10]*mTranspose._14;
			mThis._24								 = fTemp[4]*mTranspose._11 + fTemp[9]*mTranspose._12 + fTemp[10]*mTranspose._13;
			mThis._24								-= fTemp[5]*mTranspose._11 + fTemp[8]*mTranspose._12 + fTemp[11]*mTranspose._13;

			fTemp[ 0]								 = mTranspose._13 * mTranspose._24;
			fTemp[ 1]								 = mTranspose._14 * mTranspose._23;
			fTemp[ 2]								 = mTranspose._12 * mTranspose._24;
			fTemp[ 3]								 = mTranspose._14 * mTranspose._22;
			fTemp[ 4]								 = mTranspose._12 * mTranspose._23;
			fTemp[ 5]								 = mTranspose._13 * mTranspose._22;
			fTemp[ 6]								 = mTranspose._11 * mTranspose._24;
			fTemp[ 7]								 = mTranspose._14 * mTranspose._21;
			fTemp[ 8]								 = mTranspose._11 * mTranspose._23;
			fTemp[ 9]								 = mTranspose._13 * mTranspose._21;
			fTemp[10]								 = mTranspose._11 * mTranspose._22;
			fTemp[11]								 = mTranspose._12 * mTranspose._21;

			mThis._31								 = fTemp[0] *mTranspose._42 + fTemp[3] *mTranspose._43 + fTemp[4] *mTranspose._44;
			mThis._31								-= fTemp[1] *mTranspose._42 + fTemp[2] *mTranspose._43 + fTemp[5] *mTranspose._44;
			mThis._32								 = fTemp[1] *mTranspose._41 + fTemp[6] *mTranspose._43 + fTemp[9] *mTranspose._44;
			mThis._32								-= fTemp[0] *mTranspose._41 + fTemp[7] *mTranspose._43 + fTemp[8] *mTranspose._44;
			mThis._33								 = fTemp[2] *mTranspose._41 + fTemp[7] *mTranspose._42 + fTemp[10]*mTranspose._44;
			mThis._33								-= fTemp[3] *mTranspose._41 + fTemp[6] *mTranspose._42 + fTemp[11]*mTranspose._44;
			mThis._34								 = fTemp[5] *mTranspose._41 + fTemp[8] *mTranspose._42 + fTemp[11]*mTranspose._43;
			mThis._34								-= fTemp[4] *mTranspose._41 + fTemp[9] *mTranspose._42 + fTemp[10]*mTranspose._43;
			mThis._41								 = fTemp[2] *mTranspose._33 + fTemp[5] *mTranspose._34 + fTemp[1] *mTranspose._32;
			mThis._41								-= fTemp[4] *mTranspose._34 + fTemp[0] *mTranspose._32 + fTemp[3] *mTranspose._33;
			mThis._42								 = fTemp[8] *mTranspose._34 + fTemp[0] *mTranspose._31 + fTemp[7] *mTranspose._33;
			mThis._42								-= fTemp[6] *mTranspose._33 + fTemp[9] *mTranspose._34 + fTemp[1] *mTranspose._31;
			mThis._43								 = fTemp[6] *mTranspose._32 + fTemp[11]*mTranspose._34 + fTemp[3] *mTranspose._31;
			mThis._43								-= fTemp[10]*mTranspose._34 + fTemp[2] *mTranspose._31 + fTemp[7] *mTranspose._32;
			mThis._44								 = fTemp[10]*mTranspose._33 + fTemp[4] *mTranspose._31 + fTemp[9] *mTranspose._32;
			mThis._44								-= fTemp[8] *mTranspose._32 + fTemp[11]*mTranspose._33 + fTemp[5] *mTranspose._31;

			double										fDet						=	mTranspose._11*mThis._11
				+																		mTranspose._12*mThis._12
				+																		mTranspose._13*mThis._13
				+																		mTranspose._14*mThis._14
				;
			return fDet;
		}

	};	// struct

	template<typename _tElement>
							STriangle3<_tElement>&						transform								(::gph::STriangle3<_tElement>& triangle, const ::gph::SMatrix4<_tElement>& transform)									{
		triangle.A															= transform.Transform(triangle.A);
		triangle.B															= transform.Transform(triangle.B);
		triangle.C															= transform.Transform(triangle.C);
		return triangle;
	}
} // namespace

#endif // GPH_MATH_H_2983749283
