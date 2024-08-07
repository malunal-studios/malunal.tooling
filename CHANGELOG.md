# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.0.2] - 2024-08-07

### Changed

- Replaced `build.sh` with `build.py` for cross platform purposes and better argument support.
- Adjusted example file to use new `SIMULAR_TOOLING_MEASURE_FUNCTION` macro.
- Added `SIMULAR_TOOLING_ENABLE_MACROS` to compile definitions for example file.
- Updated readme to mention that example in README is based on example file.

### Added

- Macro specifically for testing a function. Will not call anything when `SIMULAR_TOOLING_ENABLE_MACROS` is not defined.

## [1.0.1] - 2024-08-05

### Changed

- Profiler, events, and probes now use `std::string` instead of `std::string_view` because dynamic strings may be provided and may not exist the lifetime of these objects.
- Example uses newly added utility function for getting source code location.

### Added

- Utility function for getting the current source code location.
- Utility macros which can be expanded by defining `SIMULAR_TOOLING_ENABLE_MACROS`. If left undefined, the macros will not expand and produce no code.

### Removed

- Erroneous `std::string_view` field of `simular::tooling::timeline`.

## [1.0.0] - 2024-08-05

### Added

- [Profiler](./include/simular/tooling/profiler.hpp) which handles event recording.
- [Timeline](./include/simular/tooling/timeline.hpp) which stores the recorded timeline, and offers functionality for visitation.
- [Timing Probe](./include/simular/tooling/probes.hpp) which can capture timing data to fill the timeline.
- [Timing Event](./include/simular/tooling/events.hpp) which can store timing data for a specific event captured by a timing probe.
- [YAML Visitor](./include/simular/tooling/visitors.hpp) Able to visit each element of the timeline and produce a YAML string of the timeline, which can be written to a file.
