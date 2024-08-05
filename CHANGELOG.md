# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.0.0] - 2024-08-05

### Added

- [Profiler](./include/simular/tooling/profiler.hpp) which handles event recording.
- [Timeline](./include/simular/tooling/timeline.hpp) which stores the recorded timeline, and offers functionality for visitation.
- [Timing Probe](./include/simular/tooling/probes.hpp) which can capture timing data to fill the timeline.
- [Timing Event](./include/simular/tooling/events.hpp) which can store timing data for a specific event captured by a timing probe.
- [YAML Visitor](./include/simular/tooling/visitors.hpp) Able to visit each element of the timeline and produce a YAML string of the timeline, which can be written to a file.
