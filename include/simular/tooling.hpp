/// @file   tooling.hpp
/// @brief  Contains all the headers for tooling.
/// @author John Christman sorakatadzuma@gmail.com
/// @copyright 2024 Simular Technologies, LLC.
#pragma once
#include "tooling/common.hpp"
#include "tooling/events.hpp"
#include "tooling/timeline.hpp"
#include "tooling/visitors.hpp"
#include "tooling/profiler.hpp"
#include "tooling/probes.hpp"
#include "tooling/utilities.hpp"

/// @def     SIMULAR_TOOLING_MEASURE_SCOPE(name)
/// @brief   Measures the timing of an arbitrary scope.
/// @details Calls into the tooling library to capture the timing for the given
///          scope and provides it to the tooling library profiler when the
///          scope closes.
/// @param   name The string name provided for the scope.
/// @remarks SIMULAR_TOOLING_ENABLE_MACROS must be defined in order for this to
///          work, otherwise nothing will happen as the definition will expand
///          to nothing.

#ifdef SIMULAR_TOOLING_ENABLE_MACROS
#define SIMULAR_TOOLING_MEASURE_SCOPE(name) \
    simular::tooling::deferred_timing_probe dtp(name)

#define SIMULAR_TOOLING_MEASURE_FUNCTION    \
    simular::tooling::deferred_timing_probe \
        dtp(simular::tooling::current_source_location())
#else
#define SIMULAR_TOOLING_MEASURE_SCOPE(name)
#define SIMULAR_TOOLING_MEASURE_FUNCTION
#endif /* SIMULAR_TOOLING_ENABLE_MACROS */