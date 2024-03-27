#pragma once
#include <chrono>
#include <ctime>

enum class TimeUnit { SECOND = 0, MILLISECOND, MICORSECOND, NANOSECOND };

uint64_t GetCurrentTime(TimeUnit unit);

#define WAIT_UNTIL_INTERVAL_LARGE_THAN_FPS_TIME(current_time, last_frame_time, \
                                                fps_microsecond_time)          \
  uint64_t interval_time = current_time - last_frame_time;                     \
  while (interval_time < fps_microsecond_time) {                               \
    std::this_thread::sleep_for(                                               \
        std::chrono_literals::operator""us(static_cast<unsigned long long>(    \
            fps_microsecond_time - interval_time)));                           \
    current_time = GetCurrentTime(TimeUnit::MICORSECOND);                      \
    interval_time = current_time - last_frame_time;                            \
  }
