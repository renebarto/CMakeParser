#pragma once

#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>
#include "utility/Deserialization.h"
#include "utility/Serialization.h"

namespace utility {
    
// Printing with C# like formatting
// Format:
// {<index><,width<:format>>}
// If index is left out, the logical ordering applies (0, 1, 2, 3...)
// If index is specified, it refers to the parameter with specified index, counting from 0
// If width is left out, the width needed to print is used
// If width is specified, this width will be used (right aligned for positive width, left aligned for negative width), unless the width needed is larger
// If format is left out, the standard printing format is used
// If format is specified, the type of argument defines how the format is used:
// For integral values:
//      D<n> for decimal values with n digits (leading zeros if needed, actual width if n is too small)
//      O<n> for octal values with n digits (leading zeros if needed, actual width if n is too small)
//      B<n> for binary values with n digits (leading zeros if needed, actual width if n is too small)
//      X<n> for hexadecimal values with n digits (leading zeros if needed, actual width if n is too small)
// For floating point values:
//      F<n> for fixed representation using n fractional digits
//      E<n> for exponential representation using n fractional digits
//      G<n> for fixed or exponential representation (whichever fits best) (n is ignored)

const std::string& GetFormatError();
bool HaveFormatError();
void ResetFormatError();
void SetFormatError(const std::string& message);

template<typename V>
typename std::enable_if<!std::is_integral<V>::value && !std::is_floating_point<V>::value, std::string>::type
BasicFormat(V value, int width, const std::string& /*format*/)
{
    ResetFormatError();
    return serialization::Serialize(value, width);
};

// Formatting for integral values
template<typename V>
typename std::enable_if<std::is_integral<V>::value, std::string>::type
BasicFormat(V value, int width, const std::string& format)
{
    ResetFormatError();
    if (!format.empty())
    {
        switch (format[0])
        {
            case 'D':
                {
                    int digits {};
                    if (format.length() > 1)
                        serialization::Deserialize(format.substr(1), digits);
                    if (digits == 0)
                        digits = width;
                    return serialization::Align(serialization::Serialize(value, digits, 10), width);
                }
                break;
            case 'O':
                {
                    int digits {};
                    if (format.length() > 1)
                        serialization::Deserialize(format.substr(1), digits);
                    if (digits == 0)
                        digits = width;
                    return serialization::Align(serialization::Serialize(value, digits, 8), width);
                }
                break;
            case 'B':
                {
                    // Binary representation always uses the bit count for the type represented as length
                    return serialization::Align(serialization::Serialize(value, 0, 2), width);
                }
                break;
            case 'X':
                {
                    int digits {};
                    if (format.length() > 1)
                        serialization::Deserialize(format.substr(1), digits);
                    if (digits == 0)
                        digits = width;
                    return serialization::Align(serialization::Serialize(value, digits, 16), width);
                }
                break;
            default:
                return serialization::Serialize(value, width);
        }
    }
    return serialization::Serialize(value, width);
};

// Formatting for floating point values
template<typename V>
typename std::enable_if<std::is_floating_point<V>::value, std::string>::type
BasicFormat(V value, int width, const std::string& format)
{
    ResetFormatError();
    if (!format.empty())
    {
        switch (format[0]) {
            case 'F':
                {
                    std::string subFormat = format.substr(1);
                    auto decimalPointIndex = subFormat.find('.');
                    std::string integralDecimalsSpecifier = subFormat;
                    std::string fractionalDecimalsSpecifier;
                    if (decimalPointIndex != std::string::npos)
                    {
                        integralDecimalsSpecifier = subFormat.substr(0, decimalPointIndex);
                        fractionalDecimalsSpecifier = subFormat.substr(decimalPointIndex + 1);
                    }
                    int integralDecimals {};
                    int fractionalDecimals {};
                    serialization::Deserialize(integralDecimalsSpecifier, integralDecimals);
                    serialization::Deserialize(fractionalDecimalsSpecifier, fractionalDecimals);
                    return serialization::Align(serialization::Serialize(value, integralDecimals + fractionalDecimals + 1, fractionalDecimals,
                                           serialization::FloatingPointRepresentation::Fixed), width);
                }
                break;
            case 'E':
                {
                    std::string subFormat = format.substr(1);
                    auto decimalPointIndex = subFormat.find('.');
                    std::string integralDecimalsSpecifier = subFormat;
                    std::string fractionalDecimalsSpecifier;
                    if (decimalPointIndex != std::string::npos)
                    {
                        integralDecimalsSpecifier = subFormat.substr(0, decimalPointIndex);
                        fractionalDecimalsSpecifier = subFormat.substr(decimalPointIndex + 1);
                    }
                    int integralDecimals {};
                    int fractionalDecimals {};
                    serialization::Deserialize(integralDecimalsSpecifier, integralDecimals);
                    serialization::Deserialize(fractionalDecimalsSpecifier, fractionalDecimals);
                    return serialization::Align(serialization::Serialize(value, integralDecimals + fractionalDecimals + 1, fractionalDecimals,
                                           serialization::FloatingPointRepresentation::Exponential), width);
                }
                break;
            case 'G':
                {
                    std::string subFormat = format.substr(1);
                    auto decimalPointIndex = subFormat.find('.');
                    std::string integralDecimalsSpecifier = subFormat;
                    std::string fractionalDecimalsSpecifier;
                    if (decimalPointIndex != std::string::npos)
                    {
                        integralDecimalsSpecifier = subFormat.substr(0, decimalPointIndex);
                        fractionalDecimalsSpecifier = subFormat.substr(decimalPointIndex + 1);
                    }
                    int integralDecimals {};
                    int fractionalDecimals {};
                    serialization::Deserialize(integralDecimalsSpecifier, integralDecimals);
                    serialization::Deserialize(fractionalDecimalsSpecifier, fractionalDecimals);
                    // We need different behaviour here due to how default output of floating point numbers works
                    return serialization::Align(serialization::Serialize(value, 0, integralDecimals + fractionalDecimals + 1,
                                           serialization::FloatingPointRepresentation::Mixed), width);
                }
                break;
            default:
                return serialization::Serialize(value, width);
        }
    }
    return serialization::Serialize(value, width);
};

inline
void Format(std::string& /*buffer*/,
            size_t /*index*/, size_t /*parameterIndex*/,
            int /*parameterWidth*/,
            const std::string& /*parameterFormat*/)
{
    ResetFormatError();
}

template<typename V, typename ... Args>
void Format(std::string& buffer,
            size_t index, size_t parameterIndex,
            int parameterWidth,
            const std::string& parameterFormat,
            V value, Args ... FArgs)
{
    ResetFormatError();
    if (index == parameterIndex)
    {
        buffer += BasicFormat(value, parameterWidth, parameterFormat);
    }
    else
        Format(buffer, index, parameterIndex + 1, parameterWidth, parameterFormat, FArgs ...);
}

inline
bool FormatParameter(
    size_t& parameterIndex,
    size_t numParameters,
    const char* parameterStart,
    const char* parameterEnd,
    int& width,
    std::string& formatSpecifier)
{
    std::string parameterID;
    std::string widthSpecifier;
    const char* parameterIDStart = parameterStart + 1;
    const char* parameterIDEnd = parameterEnd;
    const char* parameterWidthStart {};
    const char* parameterWidthEnd {};
    const char* parameterFormatStart {};
    const char* parameterFormatEnd {};
    const char* currentPosition = parameterIDStart;
    
    const char* parameterWidthDelimiter = strchr(currentPosition, ',');
    if (parameterWidthDelimiter >= parameterEnd)
        parameterWidthDelimiter = nullptr;
    if (parameterWidthDelimiter != nullptr)
    {
        parameterWidthStart = parameterWidthDelimiter + 1;
        parameterWidthEnd = parameterEnd;
        parameterIDEnd = parameterWidthDelimiter;
        currentPosition = parameterWidthStart;
    }
    const char* parameterFormatDelimiter = strchr(currentPosition, ':');
    if (parameterFormatDelimiter >= parameterEnd)
        parameterFormatDelimiter = nullptr;
    if (parameterFormatDelimiter != nullptr)
    {
        parameterFormatStart = parameterFormatDelimiter + 1;
        parameterFormatEnd = parameterEnd;
        if (parameterIDEnd > parameterFormatDelimiter)
            parameterIDEnd  =  parameterFormatDelimiter;
        if (parameterWidthStart == nullptr)
        {
            parameterWidthStart = parameterFormatDelimiter;
            parameterWidthEnd  =  parameterFormatDelimiter;
        }
        if (parameterWidthEnd > parameterFormatDelimiter)
            parameterWidthEnd  =  parameterFormatDelimiter;
    }
    // Coverity is making very complex and strange assumptions here
    // coverity[dead_error_line]
    // coverity[check_after_deref]
    if ((parameterIDStart != nullptr) && (parameterIDEnd != nullptr))
    {
        parameterID = std::string(parameterIDStart, static_cast<size_t>(parameterIDEnd - parameterIDStart));
    }
    // Coverity is making very complex and strange assumptions here
    // coverity[dead_error_line]
    if ((parameterWidthStart != nullptr) && (parameterWidthEnd != nullptr))
    {
        widthSpecifier = std::string(parameterWidthStart, static_cast<size_t>(parameterWidthEnd - parameterWidthStart));
    }
    // Coverity is making very complex and strange assumptions here
    // coverity[dead_error_line]
    if ((parameterFormatStart != nullptr) && (parameterFormatEnd != nullptr))
    {
        formatSpecifier = std::string(parameterFormatStart, static_cast<size_t>(parameterFormatEnd - parameterFormatStart));
    }
    if (!parameterID.empty() && !serialization::Deserialize(parameterID, parameterIndex))
    {
        std::ostringstream stream;
        stream << "Invalid index specified: " << parameterID;
        SetFormatError(stream.str());
        return false;
    }
    if (parameterIndex >= numParameters)
    {
        std::ostringstream stream;
        stream << "Invalid index specified: " << parameterID << " should be between 0 and " << numParameters - 1;
        SetFormatError(stream.str());
        return false;
    }
    if (!widthSpecifier.empty() && !serialization::Deserialize(widthSpecifier, width))
    {
        std::ostringstream stream;
        stream << "Invalid width specified: " << widthSpecifier;
        SetFormatError(stream.str());
        return false;
    }
    return true;
}

template <typename ... Args>
void Format(std::string& buffer,
            const char* format,
            Args const& ... args)
{
    ResetFormatError();
    size_t numParameters = sizeof...(args);
    std::vector<bool> parameterUsed(numParameters);
    const char* currentPosition = format;
    size_t parameterOrdinal {};
    while (*currentPosition != '\0')
    {
        const char* parameterStartIndication = strchr(currentPosition, '{');
        if (parameterStartIndication != nullptr)
        {
            buffer += std::string(currentPosition, static_cast<size_t>(parameterStartIndication - currentPosition));
            const char* parameterEndIndication = strchr(parameterStartIndication + 1, '}');
            if (parameterEndIndication != nullptr)
            {
                size_t parameterIndex = parameterOrdinal;
                int width {};
                std::string formatSpecifier;
                if (!FormatParameter(parameterIndex, numParameters, parameterStartIndication, parameterEndIndication, width, formatSpecifier))
                    return;
                
                parameterUsed[parameterIndex] = true;
                try
                {
                    Format(buffer, parameterIndex, 0, width, formatSpecifier, args ...);
                }
                catch (std::exception& e)
                {
                    SetFormatError(e.what());
                }

                currentPosition = parameterEndIndication + 1;
            } else
            {
                break;
            }
            ++parameterOrdinal;
        } 
        else
        {
            buffer += std::string(currentPosition);
            break;
        }
    }
    bool haveUnusedElements = false;
    std::ostringstream stream;
    for (size_t i = 0; i < numParameters; ++i)
    {
        if (!parameterUsed[i])
        {
            if (!haveUnusedElements)
            {
                stream << "Not all parameters have been used. Parameters not used:";
                haveUnusedElements = true;
            }
            stream << " " << i;
        }
    }
    if (haveUnusedElements)
    {
        SetFormatError(stream.str());
    }
}

template <typename ... Args>
void Format(std::string& buffer,
            const std::string& format,
            Args const& ... args)
{
    Format(buffer, format.c_str(), args ...);
}

inline void Format(std::string& buffer, const char * value) noexcept
{
    Format(buffer, "{}", value);
}

inline void Format(std::string& buffer, const wchar_t * value) noexcept
{
    Format(buffer, "{}", value);
}

inline void Format(std::string& buffer, std::string const& value) noexcept
{
    Format(buffer, "{}", value);
}

template <typename ... Args>
std::string FormatString(
            const std::string& format,
            Args const& ... args)
{
    std::string buffer;
    Format(buffer, format.c_str(), args ...);
    return buffer;
}

template <typename ... Args>
std::string FormatString(
            const char* format,
            Args const& ... args)
{
    std::string buffer;
    Format(buffer, format, args ...);
    return buffer;
}

} // namespace utility
