#include "icv_time.hpp"
#include <thread>
#include <typeinfo>

template <typename T1, typename T2>
static T1 ConvertTimeToType(T2 duration, TimeUnit unit) {
  switch (unit) {
  case TimeUnit::SECOND: {
    return std::chrono::duration_cast<std::chrono::seconds>(duration).count();
  }
  case TimeUnit::MILLISECOND: {
    return std::chrono::duration_cast<std::chrono::milliseconds>(duration)
        .count();
  }
  case TimeUnit::MICORSECOND: {
    return std::chrono::duration_cast<std::chrono::microseconds>(duration)
        .count();
  }
  case TimeUnit::NANOSECOND: {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(duration)
        .count();
  }
  }
  return (T1)0;
}

uint64_t GetCurrentTime(TimeUnit unit) {
  return ConvertTimeToType<uint64_t, std::chrono::system_clock::duration>(
      std::chrono::system_clock::now().time_since_epoch(), unit);
}
