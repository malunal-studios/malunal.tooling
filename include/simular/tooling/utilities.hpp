/// @file   utilities.hpp
/// @brief  Contains some utilities for users of the performance tooling.
/// @author John Christman sorakatadzuma@gmail.com
/// @copyright 2024 Simular Technologies, LLC.
#pragma once


namespace simular::tooling {

/// @brief   Obtains the current source location and creates a string out of
///          that location.
/// @details This function can be used to name a timing event, simply by
///          providing it to a timing probe when that timing probe starts to
///          measure. Having this information may be useful later when the event
///          data is being loaded into a graphical application for analysis.
/// @param   location The current source location.
/// @returns The constructed string of the source location file, line, and
///          function.
inline std::string
current_source_location(
    std::source_location location =
        std::source_location::current()
) noexcept {
    static std::ostringstream oss;
    oss.str(""); // Clear the output stream.
    oss << location.file_name() << ":"
        << location.line()      << " "
        << location.function_name();
    return oss.str();
}

} // namespace simular::tooling