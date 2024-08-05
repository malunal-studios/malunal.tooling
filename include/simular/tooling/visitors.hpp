/// @file   visitors.hpp
/// @brief  Contains some visitors that can be used on a timeline of the
//          performance tooling.
/// @author John Christman sorakatadzuma@gmail.com
/// @copyright 2024 Simular Technologies, LLC.
#pragma once


namespace simular::tooling {

/// @brief   A visitor responsible for visiting each event of a timeline and
///          collecting it into a YAML array that can be dumped to a file.
struct yaml_visitor final {
    /// @brief   Initializes the output stream of this visitor.
    /// @details Since this visitor will write a YAML file for the timeline it
    ///          is visiting, we must start the YAML contents with the
    ///          `timeline:` object.
    yaml_visitor() noexcept {
        oss << "timeline:" << std::endl;
    }

    /// @brief   Visits every node of the timeline and writes it as a YAML array
    ///          element to the output stream.
    /// @param   timeline_event The event from the timeline that we are writing.
    void
    visit(const event_variant_t& timeline_event) noexcept {
        std::visit([this](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, timing_event>)
                write_to_stream(arg);
        }, timeline_event);
    }

    /// @brief   Dumps the contents that was collected for the YAML file.
    /// @returns The string that was built from the events of the timeline.
    std::string
    dump() const noexcept {
        return oss.str();
    }

private:
    void
    write_to_stream(const timing_event& timing) noexcept {
        using std::chrono::time_point_cast;
        using std::chrono::microseconds;
        auto start_conv = time_point_cast<microseconds>(timing.start);
        auto end_conv   = time_point_cast<microseconds>(timing.end);
        auto start_epoch_micro = start_conv.time_since_epoch();
        auto end_epoch_micro   = end_conv.time_since_epoch();

        // Tag this event so we know which one it is later.
        oss << "- !timing_event" << std::endl;
        oss << "  name:  " << timing.name       << std::endl;
        oss << "  tid:   " << timing.tid        << std::endl;
        oss << "  start: " << start_epoch_micro << std::endl;
        oss << "  end:   " << end_epoch_micro   << std::endl;
    }

private:
    std::ostringstream oss;
};

} // namespace simular::tooling