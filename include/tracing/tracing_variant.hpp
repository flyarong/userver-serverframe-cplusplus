#pragma once

#include <opentracing/value.h>
#include <logging/log_extra.hpp>

namespace tracing {

opentracing::Value ToOpentracingValue(logging::LogExtra::Value);

}  // namespace tracing
