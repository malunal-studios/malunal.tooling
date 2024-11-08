/// @file   events.hpp
/// @brief  Contains events for the performance tooling.
/// @author John Christman sorakatadzuma@gmail.com
/// @copyright 2024 Malunal Studios, LLC.
#pragma once


namespace malunal::tooling {

/// @brief   Represents the event of a timing measurement taking place.
/// @details Timing events are primarily performed by a timing probe. The probe
///          will capture the thread ID, start, and end time for the event and
///          provide it to the profiler. The profiler will then, when possible,
///          send it to a timeline that it keeps updated. The timeline will
///          break the event into two nodes and interleave all events with each
///          other based on their start and end times.
struct timing_event final {
    /// @brief   The name of the event that took place.
    /// @details This will likely be provided by a probe but can be set manually
    ///          when providing events manually to the profiler.
    std::string name;

    /// @brief   The ID of the thread that the event took place on.
    /// @details It's important for the timeline to know what thread the event
    ///          took place on, at least for the display of the timeline.
    std::thread::id tid;

    /// @brief   When the event started.
    /// @details This will be one node in the timeline. All events that start
    ///          after it will be children of that node.
    time_point_t start;

    /// @brief   When the event ended.
    /// @details This will be another node in the timeline. It may have children
    ///          if it is not the inner most event, otherwise it will be a leaf
    ///          in the structure of the timeline.
    time_point_t end;

    /// @brief   Checks if this event contains the other event.
    /// @details This works by checking if the start time of the event is
    ///          after the start time of this event, and the end time of the
    ///          event is before the end time of this event.
    /// @param   event The event were checking containment of.
    /// @return  True if this event would contain the event based on its start
    ///          and end time points.
    bool
    contains(const timing_event& event) const noexcept {
        return event.start > start &&
               event.end   < end;
    }
};


/// @brief   Compares two timing events to each other.
/// @param   lhs The left hand timing event to compare.
/// @param   rhs The right hand timing even to compare.
/// @returns True if all of the name, thread id, start, and end fields of the
///          timing events being compared, are equal; false otherwise.
inline bool
operator==(const timing_event& lhs, const timing_event& rhs) noexcept {
    return lhs.name  == rhs.name  &&
           lhs.tid   == rhs.tid   &&
           lhs.start == rhs.start &&
           lhs.end   == rhs.end;
}

/// @brief   Compares two timing events to each other.
/// @param   lhs The left hand timing event to compare.
/// @param   rhs The right hand timing even to compare.
/// @returns True if one of the name, thread id, start, and end fields of the
///          timing events being compared, are *not* equal; false otherwise.
inline bool
operator!=(const timing_event& lhs, const timing_event& rhs) noexcept {
    return !operator==(lhs, rhs);
}


/// @brief   A type definition around a variant which can store any type of
///          event that this profiler can receive.
/// @remarks At the time of writing this, it only supports one, but it's
///          here in case more are added or you decide to extend it
///          yourself.
using event_variant_t = std::variant<timing_event>;

} // namespace malunal::tooling
