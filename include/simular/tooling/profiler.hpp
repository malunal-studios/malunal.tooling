/// @file   profiler.hpp
/// @brief  Contains the profiler used with the performance tooling.
/// @author John Christman sorakatadzuma@gmail.com
/// @copyright 2024 Simular Technologies, LLC.
#pragma once


namespace simular::tooling {

/// @brief   Responsible for tracking all profiling data necessary for the
///          loading and graphing of that data in another application.
/// @details The profiler is a non-invasive registrar of profiling data that
///          runs on a separate thread and periodically clears an event queue
///          of data that was registered by probes.
struct profiler final {
    /// @brief   Whether this profiler should defer when events get drained from
    ///          the event queue during execution.
    /// @details If you application is extremely peformance critical, you may
    ///          want to enable this as context switches of the profiler may
    ///          have an impact on the performance of your application,
    ///          especially in a multi-threaded environment.
    /// @remarks Be sure to benchmark your application with and without the
    ///          profiler running before making the decision to enable this!
    bool defer_drain{false};

    /// @brief   Gets the singleton instance of this profiler.
    /// @details The profiler is designed to be a single use object. If you need
    ///          another profiler, you should specify one with another name.
    /// @returns The singleton instance of this profiler.
    static profiler&
    instance() noexcept {
        static profiler k_instance;
        return k_instance;
    }

    /// @brief   Starts a profiling session by starting the profiling thread.
    /// @details The profiling thread is responsible for draining the event
    ///          queue periodically when new events are recorded.
    /// @param   name The name of the session that is being started.
    static void
    start_session(std::string_view name) noexcept {
        auto& inst = instance();
        inst.running_ = true;
        inst.session_name_ = name;
        inst.event_thread_ = std::thread(&profiler::profile, &inst);
    }

    /// @brief   Stops a profiling session by notifying the profiling thread
    ///          to join, and producing the timeline.
    /// @details Will block the calling thread until the profiling thread has
    ///          finished any tasks it has left. Mostly, the profiling thread
    ///          will be draining the event queue and updating the timeline.
    /// @returns The timeline that was recorded by this profiler.
    static timeline
    stop_session() noexcept {
        auto& inst = instance();
        inst.running_ = false;
        inst.check_events_.notify_all();
        if (inst.event_thread_.joinable())
            inst.event_thread_.join();
        return std::move(inst.timeline_);
    }

    /// @brief   Provides access to the current session name.
    /// @returns The session name that is currently set.
    /// @remarks The session name changes every time a call to `start_session`
    ///          is made, so make sure to grab it before starting a new one.
    static std::string_view
    session_name() noexcept {
        auto& inst = instance();
        return inst.session_name_;
    }

    /// @brief   Records the provided event by pushing it into an event queue.
    /// @details The recorded event will be drained when the profiler determines
    ///          it is okay to do so. If the `defer_drain` variable was set on
    ///          the profiler, it will wait until the session is over before
    ///          doing so. Otherwise, it will process events as soon and quickly
    ///          as possible and then block itself until it needs to process
    ///          more events.
    /// @param   e The event that should be recorded into the timeline.
    static void
    record_event(const event_variant_t& e) noexcept {
        auto& inst = instance();
        inst.timeline_.push(e);
        inst.check_events_.notify_all();
    }

private:
    bool
    should_drain_events() const noexcept {
        auto deferred = running_ && defer_drain;
        return !event_queue_.empty() || !deferred;
    }

    void
    drain_event_queue() noexcept {
        while (!event_queue_.empty()) {
            auto e = event_queue_.front();
            event_queue_.pop();
            update_timeline(e);
        }
    }    

    void
    profile() noexcept {
        while (running_) {
            std::unique_lock<std::mutex> lock(mutex_);
            auto fn = std::mem_fn(&profiler::should_drain_events);
            check_events_.wait(lock, std::bind(fn, this));
            drain_event_queue();
        }
    }

    void
    update_timeline(const event_variant_t& e) noexcept {
        timeline_.push(e);
    }

private:
    std::queue<event_variant_t> event_queue_;
    std::condition_variable check_events_;
    timeline timeline_;
    std::mutex mutex_;
    std::thread event_thread_;
    std::string_view session_name_;
    std::atomic<bool> running_{false};
};

} // namespace simular::perf
