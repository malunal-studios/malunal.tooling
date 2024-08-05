# Simular.Tooling

A header-only, instrumentation profiling, and general performance tooling, toolkit for projects under Simular Technologies. This is not meant to be a massively comprehensive toolkit, rather, it is meant to get the job done and not be invasive to the code that will be tooled.

### Requirements

To use this library, you'll need a compiler that supports `__cpp_concepts` which is generally supported in C++20 or higher compatible compilers. You'll also need CMake `>=3.16`, because we intend to support precompiled headers and that is the first version of CMake that supports it. In the future, CMake support may be dropped or deprecated depending on the progress of our other projects; namely `simular.cherry` and `simular.cpak`.

### Usage

As mentioned, this project is meant to be as uninvasive as possible (relatively speaking for instrumentation). There is a [profiler](./include/simular/tooling/profiler.hpp) that handles the collection of [timeline](./include/simular/tooling/timeline.hpp) [events](./include/simular/tooling/events.hpp) and the processing of those events on a thread separate from the main thread. There are [probes](./include/simular/tooling/probes.hpp) which are designed to inform the profiler of the events that take place. Here is the practical example of how to use all of these tools (the following code comes from [here](./example/testing.cpp)):
```cpp
#include <iostream>
#include <simular/tooling.hpp>

static void
fun2() noexcept {
    using namespace std::chrono_literals;
    using namespace simular::tooling;
    deferred_timing_probe tp("fun2");

    // Simulate more processing.
    std::this_thread::sleep_for(1000ms);
    std::cout << "Finished!" << std::endl;
}

static void
fun1() noexcept {
    using namespace std::chrono_literals;
    using namespace simular::tooling;
    deferred_timing_probe tp("fun1");
    
    // Simulate processing.
    std::this_thread::sleep_for(100ms);
    fun2();
}

int
main() {
    using namespace simular::tooling;
    profiler::start_session("Testing");
    fun1();

    auto timeline = profiler::stop_session();
    auto visitor  = yaml_visitor();
    timeline.accept(visitor);
    std::cout << visitor.dump() << std::endl;
}
```

### Extending the toolkit

If you need your own events, visitors, probes, etc. it's relatively easy to do this yourself, but you'll need to touch a couple of the headers to do so. Below describes what you'll need to do as well as an example of an extension.

- [events.hpp](./include/simular/tooling/events.hpp) To add your event type or modify existing ones as well as update the definition of `event_variant_t` to accept any new events that you add.
- [probes.hpp](./include/simular/tooling/probes.hpp) To modify or add a probe that sends your custom event to the profiler.
- [visitors.hpp](./include/simular/tooling/visitors.hpp) To modify existing visitors to visitor your event type, or add a new visitor to perform custom logic upon the resulting timeline.

#### Example

##### *events.hpp*

```diff
- using event_variant_t = std::variant<timing_event>;
+ using stored_variant_t = std::variant<int, float>;
+ struct watch_event final {
+     std::string_view name;
+     std::thread::id tid;
+     time_point_t when;
+     stored_variant_t stored;
+ };
+ 
+ using event_varint_t = std::variant<
+     timing_event,
+     watch_event
+ >;
```

##### *probes.hpp*

```cpp
template<probe_type Type = probe_type::deferred>
struct watch_probe final {
    watch_probe(
        std::string_view name,
        stored_variant_t stored
    ) noexcept
        : stored_{ stored }
        , name_{ name }
        , when_{ perf_clock_t::now() }
    { }

    ~watch_probe() noexcept {
        using std::this_thread::get_id;
        auto& tool = profiler::instance();
        tool.record_event(watch_event {
            .name   = name_,
            .tid    = get_id(),
            .when   = when_,
            .stored = stored_
        });
    }

private:
    stored_variant_t stored_;
    std::string_view name_;
    time_point_t     when_;
};

template<>
struct watch_probe<probe_type::classic> final {
    void
    capture(
        std::string_view name,
        stored_variant_t stored
    ) noexcept {
        using std::this_thread::get_id;
        auto& tool = profiler::instance();
        tool.record_event(watch_event {
            .name   = name,
            .tid    = get_id(),
            .when   = perf_clock_t::now(),
            .stored = stored
        });
    }
};
```

##### *visitors.hpp*

```diff
struct yaml_visitor final {
    /// ...
    
    void
    visit(const event_variant_t& timeline_event) noexcept {
        std::visit([this](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, timing_event>)
                write_to_stream(arg);
            
+           // Use this form incase the function name isn't the same.
+           if constexpr (std::is_same_v<T, watch_event>)
+               write_to_stream(arg);
        }, timeline_event);
    }

    /// ...

private:
+   void
+   write_to_stream(const watch_event& watched) noexcept {
+       using std::chrono::time_point_cast;
+       using std::chrono::microseconds;
+       auto when_conv = time_point_cast<microseconds>(watched.when);
+       auto when_epoch_micro = when_conv.time_since_epoch();
+
+       // Tag the event so we know which one it is later.
+       oss << "- !watch_event" << std::endl;
+       oss << "  name:   " << watched.name << std::endl;
+       oss << "  tid:    " << watched.tid  << std::endl;
+       oss << "  when:   " << watched.when << std::endl;
+       std::visit([&oss](auto&& arg) {
+           oss << "stored: " << arg << std::endl;
+       }, watched.stored);
+   }
};

```