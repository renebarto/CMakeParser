//------------------------------------------------------------------------------
// Copyright   : Copyright(c) 2021 René Barto
//
// File        : Tracing.cpp
//
// Namespace   : tracing
//
// Class       : Tracing
//
// Description :
//
//------------------------------------------------------------------------------

#include "tracing/Tracing.h"

#include <filesystem>
#include <iomanip>
#include "utility/Console.h"
#include "tracing/ConsoleTraceLineWriter.h"
#include "tracing/TraceWriter.h"

namespace tracing {

static const char ForwardSlashChar = '/';
static const char BackSlashChar = '\\';

std::string NormalizePath(const std::string& path)
{
    auto normalizedPath = path;
    std::replace(normalizedPath.begin(), normalizedPath.end(), BackSlashChar, ForwardSlashChar);
    return normalizedPath;
}

static std::string ExtractFileName(const std::string& path)
{
    std::string convertedPath = NormalizePath(path);
    return convertedPath.substr(convertedPath.rfind(ForwardSlashChar) + 1u);
}

IsTraceCategoryEnabledFunction Tracing::m_isTraceCategoryEnabledFunc = nullptr;
CategorySet<TraceCategory> Tracing::m_defaultTraceFilter = 
    (TraceCategory::Error | TraceCategory::Warning | TraceCategory::Information);
Tracing::Mutex Tracing::m_traceMutex;
ITraceWriter* Tracing::m_traceWriter = nullptr;

static ConsoleTraceLineWriter s_console;
static TraceWriter s_traceWriter(s_console);

Tracing::~Tracing() noexcept
{
}

void Tracing::SetTracingEnabledFunction(IsTraceCategoryEnabledFunction enabledFunc)
{
    m_isTraceCategoryEnabledFunc = enabledFunc;
}

void Tracing::SetDefaultTraceFilter(const CategorySet<TraceCategory>& defaultFilter)
{
    Tracing::m_defaultTraceFilter = defaultFilter;
}

CategorySet<TraceCategory> Tracing::GetDefaultTraceFilter()
{
    return Tracing::m_defaultTraceFilter;
}

void Tracing::SetTraceWriter(ITraceWriter* traceWriter)
{
    m_traceWriter = traceWriter;
}

bool Tracing::IsTraceCategoryEnabled(TraceCategory category)
{
    if (m_isTraceCategoryEnabledFunc != nullptr)
    {
        return m_isTraceCategoryEnabledFunc(category);
    }
    else
    {
        return m_defaultTraceFilter.is_set(category);
    }
}

void Tracing::Trace(TraceCategory category, const std::string& path, int line, const std::string& functionName, const std::string& msg)
{
    if (!IsTraceCategoryEnabled(category))
        return;
    Lock guard(m_traceMutex);
    std::string fileName = ExtractFileName(path);
    if (m_traceWriter != nullptr)
    {
        m_traceWriter->Trace(category, fileName, line, functionName, msg);
    }
    // Else do nothing
}

} // namespace tracing
