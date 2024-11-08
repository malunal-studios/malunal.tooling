#include <iostream>
#include <malunal/tooling.hpp>

static void
fun2() noexcept {
    MALUNAL_TOOLING_MEASURE_FUNCTION;

    // Simulate more processing.
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(1000ms);
    std::cout << "Finished!" << std::endl;
}

static void
fun1() noexcept {
    MALUNAL_TOOLING_MEASURE_FUNCTION;
    
    // Simulate processing.
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(100ms);
    fun2();
}

int
main() {
    using namespace malunal::tooling;
    profiler::start_session("Testing");
    fun1();

    auto timeline = profiler::stop_session();
    auto visitor  = yaml_visitor();
    timeline.accept(visitor);
    std::cout << visitor.dump() << std::endl;
}
