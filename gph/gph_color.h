#ifndef GPH_COLOR_H_2379849823
#define GPH_COLOR_H_2379849823

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

} // namespace

#endif // GPH_COLOR_H_2379849823
