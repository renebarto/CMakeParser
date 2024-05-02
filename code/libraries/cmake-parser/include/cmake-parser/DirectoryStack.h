#pragma once

#include <filesystem>
#include <vector>
#include "cmake-parser/Directory.h"

namespace cmake_parser {

class DirectoryStack
{
private:
    std::vector<DirectoryPtr> m_stack;
    size_t m_size;

public:
    DirectoryStack();
    ~DirectoryStack();

    void Push(DirectoryPtr item);
    DirectoryPtr Top() const;
    DirectoryPtr At(size_t indexFromTop) const;
    DirectoryPtr Pop();
    size_t Size() const;
};

} // namespace cmake_parser