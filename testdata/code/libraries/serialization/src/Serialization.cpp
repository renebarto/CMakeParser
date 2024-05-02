#include "serialization/Serialization.h"

namespace serialization {

std::string Align(const std::string& text, int width)
{
    if (static_cast<size_t>(std::abs(width)) > text.length())
    {
        if (width < 0)
        {
            return text + std::string(static_cast<unsigned int>(-width) - text.length(), ' ');
        }
        else
        {
            return std::string(static_cast<unsigned int>(width) - text.length(), ' ') + text;
        }
    }
    return text;
}
} // namespace serialization
