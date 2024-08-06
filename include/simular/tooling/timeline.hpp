/// @file   timeline.hpp
/// @brief  Contains the defintion of a timeline for the performance tooling.
/// @author John Christman sorakatadzuma@gmail.com
/// @copyright 2024 Simular Technologies, LLC.
#pragma once


namespace simular::tooling {
namespace detail {

/// @brief   Defines a function pointer using the provided type, that returns
///          nothing and takes in shared pointer to the timeline node that is
///          being visited.
/// @tparam  Vistor The type of the visitor.
template<typename Visitor>
using visitor_visit_node_fn =
    void(Visitor::*)(const event_variant_t&)
        noexcept;

/// @brief   Defines a type constraint that assures the given type has a
///          definition for a function to set the thread ID of the timeline
///          being processed, and a visit function to timeline node.
/// @tparam  Visitor The type of the visitor.
template<typename Visitor>
concept TimelineVisitor = requires {
    { &Visitor::visit } -> std::same_as<visitor_visit_node_fn<Visitor>>;
};

} // namespace simular::tooling::detail


/// @brief   A structure for storing information about a timeline of events.
/// @details More specifically, this timeline, which is for performance tooling
///          stores information about the measurement of events that took place
///          within
struct timeline final {
    using iterator = std::vector<event_variant_t>::iterator;
    using const_iterator = std::vector<event_variant_t>::const_iterator;
    using reverse_iterator = std::vector<event_variant_t>::reverse_iterator;
    using const_reverse_iterator = std::vector<event_variant_t>::const_reverse_iterator;

    timeline() noexcept = default;
    ~timeline() noexcept = default;

    /// @brief Move constructor for transferring data efficiently.
    /// @param other The timeline that we will be moving data from.
    timeline(timeline&& other) noexcept
        : mutex_{ }
        , events_{ std::move(other.events_) }
    { }

    /// @brief   Move assignment operator for transferring data efficiently.
    /// @param   other the timeline that we will be moving data from.
    /// @returns This timeline with the moved data.
    timeline&
    operator=(timeline&& other) noexcept {
        events_ = std::move(other.events_);
        return *this;
    }


    /// @brief   Pushes the given event into this timeline.
    /// @details Since this timeline method will modify the timeline, it
    ///          needs to be locked so other objects cannot modify it at
    ///          the same time and potentially corrupt it or bring it out
    ///          of synch with observers.
    /// @param   event The event that should be inserted.
    void
    push(const event_variant_t& e) noexcept {
        std::lock_guard<std::mutex> lock(mutex_);
        events_.push_back(e);
    }

    /// @brief   Pops the last event in this timeline.
    /// @details Since this timeline method will modify the timeline, it
    ///          needs to be locked so other objects cannot modify it at
    ///          the same time and potentially corrupt it or bring it out
    ///          of synch with observers.
    void
    pop() noexcept {
        std::lock_guard<std::mutex> lock(mutex_);
        events_.pop_back();
    }

    /// @brief   Clears the timeline of all events.
    /// @details Since this timeline method will modify the timeline, it
    ///          needs to be locked so other objects cannot modify it at
    ///          the same time and potentially corrupt it or bring it out
    ///          of synch with observers.
    void
    clear() noexcept {
        std::lock_guard<std::mutex> lock(mutex_);
        events_.clear();
    }

    /// @brief   Gets the first element in the timeline.
    /// @returns A reference to the first element of the timeline.
    event_variant_t&
    front() noexcept {
        return events_.front();
    }

    /// @brief   Immutably gets the first element in the timeline.
    /// @returns An immutable reference to the first element of the timeline.
    const event_variant_t&
    front() const noexcept {
        return events_.front();
    }

    /// @brief   Gets the last element in the timeline.
    /// @returns A reference to the last element of the timeline.
    event_variant_t&
    back() noexcept {
        return events_.back();
    }

    /// @brief   Immutably gets the last element in the timeline.
    /// @returns An immutable reference to the last element of the timeline.
    const event_variant_t&
    back() const noexcept {
        return events_.back();
    }

    /// @brief   Checks if the timeline has events.
    /// @returns True if the timeline has events; false otherwise.
    bool
    empty() const noexcept {
        return events_.empty();
    }

    /// @brief   Gets the current size of the timeline.
    /// @returns The number of events contained within the timeline.
    size_t
    size() const noexcept {
        return events_.size();
    }

    /// @brief   Gets the current max size of the timeline.
    /// @returns The number of events the timeline could contain.
    size_t
    capacity() const noexcept {
        return events_.capacity();
    }

    /// @brief   Resizes the maximum capacity of the timeline.
    /// @details Since this timeline method will modify the timeline, it
    ///          needs to be locked so other objects cannot modify it at
    ///          the same time and potentially corrupt it or bring it out
    ///          of synch with observers.
    /// @param   size The new maximum timeline size.
    void
    reserve(size_t size) noexcept {
        std::lock_guard<std::mutex> lock(mutex_);
        events_.reserve(size);
    }

    /// @brief   Resizes the current capacity of the timeline.
    /// @details Since this timeline method will modify the timeline, it
    ///          needs to be locked so other objects cannot modify it at
    ///          the same time and potentially corrupt it or bring it out
    ///          of synch with observers.
    /// @param   size The new current timeline size.
    void
    resize(size_t size) noexcept {
        std::lock_guard<std::mutex> lock(mutex_);
        events_.resize(size);
    }

    /// @brief   Obtains a mutable iterator to the beginning of the underlying
    ///          structure of this timeline.
    /// @returns The beginning iterator for the events vector stored by this
    ///          timeline.
    iterator
    begin() noexcept {
        return events_.begin();
    }

    /// @brief   Obtains a mutable iterator to the ending of the underlying
    ///          structure of this timeline.
    /// @returns The ending iterator for the events vector stored by this
    ///          timeline.
    iterator
    end() noexcept {
        return events_.end();
    }

    /// @brief   Obtains an immutable iterator to the beginning of the underlying
    ///          structure of this timeline.
    /// @returns The beginning iterator for the events vector stored by this
    ///          timeline.
    const_iterator
    cbegin() const noexcept {
        return events_.cbegin();
    }

    /// @brief   Obtains an immutable iterator to the ending of the underlying
    ///          structure of this timeline.
    /// @returns The ending iterator for the events vector stored by this
    ///          timeline.
    const_iterator
    cend() const noexcept {
        return events_.cend();
    }

    /// @brief   Obtains a mutable reverse iterator to the beginning of the
    ///          underlying structure of this timeline.
    /// @returns The reverse beginning iterator for the events vector stored by
    ///          this timeline.
    reverse_iterator
    rbegin() noexcept {
        return events_.rbegin();
    }

    /// @brief   Obtains a mutable reverse iterator to the ending of the
    ///          underlying structure of this timeline.
    /// @returns The reverse ending iterator for the events vector stored by
    ///          this timeline.
    reverse_iterator
    rend() noexcept {
        return events_.rend();
    }

    /// @brief   Obtains an immutable reverse iterator to the beginning of the
    ///          underlying structure of this timeline.
    /// @returns The reverse beginning iterator for the events vector stored by
    ///          this timeline.
    const_reverse_iterator
    crbegin() const noexcept {
        return events_.rbegin();
    }

    /// @brief   Obtains an immutable reverse iterator to the ending of the
    ///          underlying structure of this timeline.
    /// @returns The reverse ending iterator for the events vector stored by
    ///          this timeline.
    const_reverse_iterator
    crend() const noexcept {
        return events_.rend();
    }

    /// @brief   Iterates each thread timeline.
    /// @details This is where a visitor will start when iterating a timeline.
    ///          The visitor will have it's thread variable set by the timeline
    ///          and then it will be provided the root node for that thread.
    ///          All of the child nodes will be provided to the visitor by the
    ///          root.
    /// @tparam  Visitor The type of the visitor.
    /// @param   visitor The visitor requesting to visit the nodes of this
    //           timeline and its children.
    template<detail::TimelineVisitor Visitor>
    void accept(Visitor& visitor) const noexcept {
        for (const auto& evar : events_)
            visitor.visit(evar);
    }

private:
    std::mutex mutex_;
    std::vector<event_variant_t> events_;
};

} // namespace simular::tooling
