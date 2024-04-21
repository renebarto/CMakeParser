#include <functional>
#include <mutex>
#include <string>
#include "utility/Format.h"
#include "tracing/CategorySet.h"
#include "tracing/TraceCategory.h"

namespace tracing {

class ITraceWriter;

using IsTraceCategoryEnabledFunction = std::function<bool(TraceCategory category)>;

class Tracing
{
private:
    // We might want to remove this over time, and use only the TraceFilter
    static IsTraceCategoryEnabledFunction m_isTraceCategoryEnabledFunc;

    static ITraceWriter* m_traceWriter;
    static CategorySet<TraceCategory> m_defaultTraceFilter;

    typedef std::recursive_mutex Mutex;
    typedef std::lock_guard<Mutex> Lock;
    static Mutex m_traceMutex;

public:
    Tracing() = default;
    virtual ~Tracing() noexcept;
    
    static void SetDefaultTraceFilter(const CategorySet<TraceCategory>& defaultFilter);
    static CategorySet<TraceCategory> GetDefaultTraceFilter();

    static void SetTracingEnabledFunction(IsTraceCategoryEnabledFunction enabledFunc);
    
    static void SetTraceWriter(ITraceWriter* traceWriter);

    static bool IsTraceCategoryEnabled(TraceCategory category);
    static void Trace(TraceCategory category, const std::string& path, int line, const std::string& functionName, const std::string& message);
    template <typename ... Args>
    static void Trace(TraceCategory category, const std::string& path, int line, const std::string& functionName, const std::string& format, Args const& ... args) noexcept
    {
        if (IsTraceCategoryEnabled(category))
        {
            std::string str;
            utility::Format(str, format, args ...);
            Trace(category, path, line, functionName, str);
        }
    }
};

} // namespace tracing

inline void TraceError(const std::string& path, int line, const std::string& functionName, const std::string& message)
{
    tracing::Tracing::Trace(tracing::TraceCategory::Error, path, line, functionName, message);
}
template <typename ... Args>
inline void TraceError(const std::string& path, int line, const std::string& functionName, const std::string& format, Args const& ... args) noexcept
{
    tracing::Tracing::Trace(tracing::TraceCategory::Error, path, line, functionName, format, args ...);
}

inline void TraceWarning(const std::string& path, int line, const std::string& functionName, const std::string& message)
{
    tracing::Tracing::Trace(tracing::TraceCategory::Warning, path, line, functionName, message);
}
template <typename ... Args>
inline void TraceWarning(const std::string& path, int line, const std::string& functionName, const std::string& format, Args const& ... args) noexcept
{
    tracing::Tracing::Trace(tracing::TraceCategory::Warning, path, line, functionName, format, args ...);
}

inline void TraceInfo(const std::string& path, int line, const std::string& functionName, const std::string& message)
{
    tracing::Tracing::Trace(tracing::TraceCategory::Information, path, line, functionName, message);
}
template <typename ... Args>
inline void TraceInfo(const std::string& path, int line, const std::string& functionName, const std::string& format, Args const& ... args) noexcept
{
    tracing::Tracing::Trace(tracing::TraceCategory::Information, path, line, functionName, format, args ...);
}

inline void TraceData(const std::string& path, int line, const std::string& functionName, const std::string& message)
{
    tracing::Tracing::Trace(tracing::TraceCategory::Data, path, line, functionName, message);
}
template <typename ... Args>
inline void TraceData(const std::string& path, int line, const std::string& functionName, const std::string& format, Args const& ... args) noexcept
{
    tracing::Tracing::Trace(tracing::TraceCategory::Data, path, line, functionName, format, args ...);
}

inline void TraceDebug(const std::string& path, int line, const std::string& functionName, const std::string& message)
{
    tracing::Tracing::Trace(tracing::TraceCategory::Debug, path, line, functionName, message);
}
template <typename ... Args>
inline void TraceDebug(const std::string& path, int line, const std::string& functionName, const std::string& format, Args const& ... args) noexcept
{
    tracing::Tracing::Trace(tracing::TraceCategory::Debug, path, line, functionName, format, args ...);
}

#define TRACE_INFRA(category, message, ...) tracing::Tracing::Trace(category, __FILE__, __LINE__, __func__, std::string(message), ## __VA_ARGS__)

#define TRACE_ERROR(message, ...) tracing::Tracing::Trace(tracing::TraceCategory::Error, __FILE__, __LINE__, __func__, std::string(message), ## __VA_ARGS__)
#define TRACE_WARNING(message, ...) tracing::Tracing::Trace(tracing::TraceCategory::Warning, __FILE__, __LINE__, __func__, std::string(message), ## __VA_ARGS__)
#define TRACE_INFO(message, ...) tracing::Tracing::Trace(tracing::TraceCategory::Information, __FILE__, __LINE__, __func__, std::string(message), ## __VA_ARGS__)
#define TRACE_DEBUG(message, ...) tracing::Tracing::Trace(tracing::TraceCategory::Debug, __FILE__, __LINE__, __func__, std::string(message), ## __VA_ARGS__)
#define TRACE_DATA(message, ...) tracing::Tracing::Trace(tracing::TraceCategory::Data, __FILE__, __LINE__, __func__, std::string(message), ## __VA_ARGS__)
