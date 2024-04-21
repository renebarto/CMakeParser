//------------------------------------------------------------------------------
// Copyright   : Copyright(c) 2021 René Barto
//
// File        : MockTraceWriter.cpp
//
// Namespace   : tracing
//
// Class       : MockTraceWriter
//
// Description :
//
//------------------------------------------------------------------------------

#pragma once

#include "tracing/ITraceWriter.h"

#include "test-platform/GoogleMock.h"

namespace tracing {

class MockTraceWriter
    : public ITraceWriter
{
public:
    virtual ~MockTraceWriter() = default;

    MOCK_METHOD(void, Trace, (TraceCategory, const std::string&, int, const std::string&, const std::string&), (override));
};

} // namespace tracing
