#include <chrono>

#ifndef GPH_TIMER_H_9921873439823
#define GPH_TIMER_H_9921873439823

namespace gph
{
	struct STimer {
		uint64_t												TimeDelta			= 0;
		::std::chrono::time_point<::std::chrono::system_clock>	TimePrevious		= ::std::chrono::system_clock::now();
		::std::chrono::time_point<::std::chrono::system_clock>	TimeCurrent			= ::std::chrono::system_clock::now();

		uint64_t												Tick				()	{
			TimeCurrent												= ::std::chrono::system_clock::now();
			TimeDelta												= ::std::chrono::duration_cast<::std::chrono::microseconds>(TimeCurrent - TimePrevious).count();
			TimePrevious											= TimeCurrent;
			return TimeDelta;
		}
	};
} // namespace

#endif // GPH_TIMER_H_9921873439823
