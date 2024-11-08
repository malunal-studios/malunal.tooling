/// @file   common.hpp
/// @brief  Contains common objects for the performance tooling.
/// @author John Christman sorakatadzuma@gmail.com
/// @copyright 2024 Malunal Studios, LLC.
#pragma once
#include <atomic>
#include <chrono>
#include <concepts>
#include <condition_variable>
#include <cstdint>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <source_location>
#include <sstream>
#include <string>
#include <variant>


namespace malunal::tooling {

/// @brief   Ease of use type definition for absurdly long qualified type name
///          of `std::chrono::steady_clock`.
/// @details We choose `steady_clock` over `high_resolution_clock` because in
///          standard, the HRC is type defined to be `system_clock` which will
///          account for changes in the system clock. We don't particularly
///          care if the system clock changed. We only care how much time
///          elapsed. Ideally, we hope that the system supports microsecond
///          precision. Most modern systems support nanosecond precision so
///          this should not be a worry.
using perf_clock_t = std::chrono::steady_clock;

/// @brief   Ease of use type definition for absurdly long qualified type name
///          of `std::chrono::steady_clock::time_point`.
/// @details The time point recorded by the timing related probes will remain as
///          a time point when it is sent to the profiler. The profiler will
///          track the start of the entire profiling session, and will use the
///          calculated elapsed time, in microseconds, to create a timeline. The
///          elapsed time can also be used to get an average time spent on any
///          given timing event.
using time_point_t = perf_clock_t::time_point;

} // namespace malunal::tooling
