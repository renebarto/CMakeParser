#include "cmake-parser/DirectoryStack.h"

#include "tracing/Tracing.h"

namespace cmake_parser {

DirectoryStack::DirectoryStack()
    : m_stack{}
    , m_size{}
{
}
DirectoryStack::~DirectoryStack()
{
    m_stack.clear();
    m_size = {};
}

void DirectoryStack::Push(DirectoryPtr item)
{
    m_stack.push_back(item);
    m_size++;
    TRACE_DEBUG("Stack push {}, size={}", item, m_size);
}

DirectoryPtr DirectoryStack::Top() const
{
    if (m_size > 0)
        return m_stack[m_size - 1];
    return nullptr;
}

DirectoryPtr DirectoryStack::At(size_t indexFromTop) const
{
    if (m_size > indexFromTop)
    {
        return m_stack[m_size - indexFromTop - 1];
    }
    return nullptr;
}

DirectoryPtr DirectoryStack::Pop()
{
    auto result = Top();
    if (m_size > 0)
    {
        m_size--;
        m_stack.erase(m_stack.end() - 1);
    }
    TRACE_DEBUG("Stack pop {}, size={}", result, m_size);
    return result;
}

size_t DirectoryStack::Size() const
{
    return m_size;
}

} // namespace cmake_parser
