/// @file   probes.hpp
/// @brief  Contains the different types of probes for the performance tooling.
/// @author John Christman sorakatadzuma@gmail.com
/// @copyright 2024 Simular Technologies, LLC.
#pragma once


namespace simular::tooling {

/// @brief   Defines the type of probe that one should abide by.
/// @details This is used in the specialization of probes as a manner of
///          supporting probes which behave in slightly different ways,
///          depending on the needs to the user.
enum class probe_type {
    /// @brief   The probe is deferred.
    /// @details A deferred probe tracks its data based on when it was created
    ///          and destroyed.
    deferred,

    /// @brief   The probe is classic.
    /// @details A classic probe tracks its data based on when the user needs it
    ///          to, by calling the provided functions.
    classic
};

/// @brief   A timing probe is a profiling tool which records timing information
///          for a given context, and pushes it to the profiler when called to
///          stop, or the scope is closed.
/// @details This specialization, the default one, is a deferring probe. That
///          means it will capture the start and end of the frame that it is
///          measuring upon construction and destruction respectively. This
///          could also be called a `scoped_timing_probe`, because it measures
///          the scope it was created and destroyed in.
/// @tparam  Name The name for this timing probe; the name will be provided to
///          the profiler when the event is pushed.
/// @tparam  Type Used in the specialization of the timing probe; determines
///          whether this probe specializes for deferred event delivery, or
///          or allows the creator to control the start and end of the probe.
template<probe_type Type = probe_type::deferred>
struct timing_probe {
    /// @brief   Creates a new instance of the probe and grabs the start time
    ///          for the probe.
    /// @param   name The name of this timing probe.
    /// @remarks Since this version is the default specialization, and it is
    ///          a deferring probe, we must grab the time now and when this
    ///          instance is destroyed.
    timing_probe(const std::string& name) noexcept
        : name_{ name }
        , start_{ perf_clock_t::now() }
    { }

    /// @brief   Grabs the end time, provides the timing event to the profiler,
    ///          and destroys this instance.
    /// @remarks Since this version is the default specialization, and it is
    ///          a deferring probe, we must grab the time when an instance is
    ///          created, and now when it's destroyed.
    ~timing_probe() noexcept {
        using std::this_thread::get_id;

        // Pull this immediately to correctly represent timing.
        auto end_ = perf_clock_t::now();
        auto& tool = profiler::instance();
        tool.record_event(timing_event {
            .name  = name_,
            .tid   = get_id(),
            .start = start_,
            .end   = end_
        });
    }

private:
    std::string name_;
    time_point_t start_;
};

/// @brief   A timing probe is a profiling tool which records timing information
///          for a given context, and pushes it to the profiler when called to
///          stop, or the scope is closed.
/// @details This specialization is a non-deferring probe. It allows the creator
///          to start and stop the timing at will. Each time the measurement is
///          stopped, a new timing event will be provided to the profiler.
/// @tparam  Name The name for this timing probe; the name will be provided to
///          the profiler when the event is pushed.
template<>
struct timing_probe<probe_type::classic> {
    /// @brief   Sets the start time for the probe to `now`.
    /// @details This is the non-deferring timing probe, so this is provided to
    ///          allow the creator of the probe to measure multiple times.
    void
    start(const std::string& name) const noexcept {
        name_ = name;
        start_ = perf_clock_t::now();
    }

    /// @brief   Obtains the stop time for the probe, which is `now`, then
    ///          pushes a timing event to the profiler.
    /// @details This is the non-deferring timing probe, so this is provided to
    ///          allow the creator of the probe to measure multiple times.
    void
    stop() const noexcept {
        using std::this_thread::get_id;

        // Pull this immediately to correctly represent timing.
        auto end_ = perf_clock_t::now();
        auto& tool = profiler::instance();
        tool.record_event(timing_event {
            .name  = name_,
            .tid   = get_id(),
            .start = start_,
            .end   = end_
        });
    }

private:
    // Don't look at me like that
    mutable std::string name_;
    mutable time_point_t start_;
};


/// @brief   Type definition for a deferred timing probe.
/// @details Users may find it more readable or easier to use this type over
///          specifying the `probe_type` template argument explicitly.
using deferred_timing_probe = timing_probe<probe_type::deferred>;

/// @brief   Type definition for a classic timing probe.
/// @details Users may find it more readable or easier to use this type over
///          specifying the `probe_type` template argument explicitly.
using classic_timing_probe = timing_probe<probe_type::classic>;

} // namespace simular::perf
