#ifndef GPH_CONTAINER_H_2938472983
#define GPH_CONTAINER_H_2938472983


namespace gph
{
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

} // namespace

#endif // GPH_CONTAINER_H_2938472983
