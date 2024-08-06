#include <iostream>
#include <simular/tooling.hpp>

static void
fun2() noexcept {
    using namespace std::chrono_literals;
    using namespace simular::tooling;
    deferred_timing_probe tp(current_source_location());

    // Simulate more processing.
    std::this_thread::sleep_for(1000ms);
    std::cout << "Finished!" << std::endl;
}

static void
fun1() noexcept {
    using namespace std::chrono_literals;
    using namespace simular::tooling;
    deferred_timing_probe tp(current_source_location());
    
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