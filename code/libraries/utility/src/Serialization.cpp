#include "utility/Serialization.h"

#include <bitset>
#include <iomanip>
#include "utility/TypeCast.h"

namespace serialization {

constexpr int BitsToOctalDigits(int bits)
{
    return (bits + 2) / 3;
}

constexpr int BitsToHexaDecimalDigits(int bits)
{
    return (bits + 3) / 4;
}

std::string Serialize(char value, int width)
{
    std::ostringstream stream;

    stream << std::dec << static_cast<int>(value);
    return serialization::Align(stream.str(), width);
}

std::string Serialize(wchar_t value, int width)
{
    std::ostringstream stream;

    stream << std::dec << static_cast<int>(value);
    return serialization::Align(stream.str(), width);
}

std::string Serialize(int8_t value, int width, int base, bool showHexBase)
{
    std::ostringstream stream;

    switch (base)
    {
        case 2:
            {
                std::bitset<8> x(static_cast<uint8_t>(value));
                stream << std::setfill('0') << std::setw(8) << x;
            }
            break;
        case 8:
            {
                stream << std::oct << std::setfill('0') << std::setw((width > 0) ? width : BitsToOctalDigits(8)) << static_cast<int>(value) << std::dec;
            }
            break;
        case 10:
            {
                stream << std::dec << std::setfill('0') << static_cast<int>(value);
            }
            break;
        case 16:
            {
                if (showHexBase) stream << "0x";
                stream << std::hex << std::noshowbase << std::uppercase << std::setfill('0') << std::setw((width > 0) ? width : BitsToHexaDecimalDigits(8)) << static_cast<int>(value) << std::dec;
            }
            break;
        default:
            {
                throw std::logic_error("Invalid base specified: " + std::to_string(base));
            }
    }
    return serialization::Align(stream.str(), width);
}

std::string Serialize(uint8_t value, int width, int base, bool showHexBase)
{
    std::ostringstream stream;

    switch (base)
    {
        case 2:
            {
                std::bitset<8> x(value);
                stream << std::setfill('0') << std::setw(8) << x;
            }
            break;
        case 8:
            {
                stream << std::oct << std::setfill('0') << std::setw((width > 0) ? width : BitsToOctalDigits(8)) << static_cast<int>(value) << std::dec;
            }
            break;
        case 10:
            {
                stream << std::dec << std::setfill('0') << static_cast<int>(value);
            }
            break;
        case 16:
            {
                if (showHexBase) stream << "0x";
                stream << std::hex << std::noshowbase << std::uppercase << std::setfill('0') << std::setw((width > 0) ? width : BitsToHexaDecimalDigits(8)) << static_cast<int>(value) << std::dec;
            }
            break;
        default:
            {
                throw std::logic_error("Invalid base specified: " + std::to_string(base));
            }
    }
    return serialization::Align(stream.str(), width);
}

std::string Serialize(int16_t value, int width, int base, bool showHexBase)
{
    std::ostringstream stream;

    switch (base)
    {
        case 2:
            {
                std::bitset<16> x(static_cast<uint16_t>(value));
                stream << std::setfill('0') << std::setw(16) << x;
            }
            break;
        case 8:
            {
                stream << std::oct << std::setfill('0') << std::setw((width > 0) ? width : BitsToOctalDigits(16)) << value << std::dec;
            }
            break;
        case 10:
            {
                stream << std::dec << std::setfill('0') << value;
            }
            break;
        case 16:
            {
                if (showHexBase) stream << "0x";
                stream << std::hex << std::noshowbase << std::uppercase << std::setfill('0') << std::setw((width > 0) ? width : BitsToHexaDecimalDigits(16)) << value << std::dec;
            }
            break;
        default:
            {
                throw std::logic_error("Invalid base specified: " + std::to_string(base));
            }
    }
    return serialization::Align(stream.str(), width);
}

std::string Serialize(uint16_t value, int width, int base, bool showHexBase)
{
    std::ostringstream stream;

    switch (base)
    {
        case 2:
            {
                std::bitset<16> x(value);
                stream << std::setfill('0') << std::setw(16) << x;
            }
            break;
        case 8:
            {
                stream << std::oct << std::setfill('0') << std::setw((width > 0) ? width : BitsToOctalDigits(16)) << value << std::dec;
            }
            break;
        case 10:
            {
                stream << std::dec << std::setfill('0') << value;
            }
            break;
        case 16:
            {
                if (showHexBase) stream << "0x";
                stream << std::hex << std::noshowbase << std::uppercase << std::setfill('0') << std::setw((width > 0) ? width : BitsToHexaDecimalDigits(16)) << value << std::dec;
            }
            break;
        default:
            {
                throw std::logic_error("Invalid base specified: " + std::to_string(base));
            }
    }
    return serialization::Align(stream.str(), width);
}

std::string Serialize(int32_t value, int width, int base, bool showHexBase)
{
    std::ostringstream stream;

    switch (base)
    {
        case 2:
            {
                std::bitset<32> x(static_cast<uint32_t>(value));
                stream << std::setfill('0') << std::setw(32) << x;
            }
            break;
        case 8:
            {
                stream << std::oct << std::setfill('0') << std::setw((width > 0) ? width : BitsToOctalDigits(32)) << value << std::dec;
            }
            break;
        case 10:
            {
                stream << std::dec << std::setfill('0') << value;
            }
            break;
        case 16:
            {
                if (showHexBase) stream << "0x";
                stream << std::hex << std::noshowbase << std::uppercase << std::setfill('0') << std::setw((width > 0) ? width : BitsToHexaDecimalDigits(32)) << value << std::dec;
            }
            break;
        default:
            {
                throw std::logic_error("Invalid base specified: " + std::to_string(base));
            }
    }
    return serialization::Align(stream.str(), width);
}

std::string Serialize(uint32_t value, int width, int base, bool showHexBase)
{
    std::ostringstream stream;

    switch (base)
    {
        case 2:
            {
                std::bitset<32> x(value);
                stream << std::setfill('0') << std::setw(32) << x;
            }
            break;
        case 8:
            {
                stream << std::oct << std::setfill('0') << std::setw((width > 0) ? width : BitsToOctalDigits(32)) << value << std::dec;
            }
            break;
        case 10:
            {
                stream << std::dec << std::setfill('0') << value;
            }
            break;
        case 16:
            {
                if (showHexBase) stream << "0x";
                stream << std::hex << std::noshowbase << std::uppercase << std::setfill('0') << std::setw((width > 0) ? width : BitsToHexaDecimalDigits(32)) << value << std::dec;
            }
            break;
        default:
            {
                throw std::logic_error("Invalid base specified: " + std::to_string(base));
            }
    }
    return serialization::Align(stream.str(), width);
}

std::string Serialize(int64_t value, int width, int base, bool showHexBase)
{
    std::ostringstream stream;

    switch (base)
    {
        case 2:
            {
                std::bitset<64> x(static_cast<uint64_t>(value));
                stream << std::setfill('0') << std::setw(64) << x;
            }
            break;
        case 8:
            {
                stream << std::oct << std::setfill('0') << std::setw((width > 0) ? width : BitsToOctalDigits(64)) << value << std::dec;
            }
            break;
        case 10:
            {
                stream << std::dec << std::setfill('0') << value;
            }
            break;
        case 16:
            {
                if (showHexBase) stream << "0x";
                stream << std::hex << std::noshowbase << std::uppercase << std::setfill('0') << std::setw((width > 0) ? width : BitsToHexaDecimalDigits(64)) << value << std::dec;
            }
            break;
        default:
            {
                throw std::logic_error("Invalid base specified: " + std::to_string(base));
            }
    }
    return serialization::Align(stream.str(), width);
}

std::string Serialize(uint64_t value, int width, int base, bool showHexBase)
{
    std::ostringstream stream;

    switch (base)
    {
        case 2:
            {
                std::bitset<64> x(value);
                stream << std::setfill('0') << std::setw(64) << x;
            }
            break;
        case 8:
            {
                stream << std::oct << std::setfill('0') << std::setw((width > 0) ? width : BitsToOctalDigits(64)) << value << std::dec;
            }
            break;
        case 10:
            {
                stream << std::dec << std::setfill('0') << value;
            }
            break;
        case 16:
            {
                if (showHexBase) stream << "0x";
                stream << std::hex << std::noshowbase << std::uppercase << std::setfill('0') << std::setw((width > 0) ? width : BitsToHexaDecimalDigits(64)) << value << std::dec;
            }
            break;
        default:
            {
                throw std::logic_error("Invalid base specified: " + std::to_string(base));
            }
    }
    return serialization::Align(stream.str(), width);
}

std::string Serialize(float value, int width, int precision, FloatingPointRepresentation representation)
{
    std::ostringstream stream;

    switch (representation)
    {
        case FloatingPointRepresentation::Fixed:
            stream.setf(std::ios_base::floatfield, std::ios_base::fixed);
            stream << std::setfill('0') << std::setw(width) << std::setprecision(precision) << value;
            break;
        case FloatingPointRepresentation::Exponential:
            stream.setf(std::ios_base::floatfield, std::ios_base::scientific);
            stream << std::setfill('0') << std::setw(width) << std::setprecision(precision) << value;
            break;
        case FloatingPointRepresentation::Mixed:
            stream.unsetf(std::ios_base::floatfield);
            stream << std::setfill('0') << std::setprecision(precision) << value;
            break;
        default:
            {
                throw std::logic_error("Invalid representation specified: " + std::to_string(static_cast<int>(representation)));
            }
    }

    return serialization::Align(stream.str(), width);
}

std::string Serialize(double value, int width, int precision, FloatingPointRepresentation representation)
{
    std::ostringstream stream;

    switch (representation)
    {
        case FloatingPointRepresentation::Fixed:
            stream.setf(std::ios_base::floatfield, std::ios_base::fixed);
            stream << std::setfill('0') << std::setw(width) << std::setprecision(precision) << value;
            break;
        case FloatingPointRepresentation::Exponential:
            stream.setf(std::ios_base::floatfield, std::ios_base::scientific);
            stream << std::setfill('0') << std::setw(width) << std::setprecision(precision) << value;
            break;
        case FloatingPointRepresentation::Mixed:
            stream.unsetf(std::ios_base::floatfield);
            stream << std::setfill('0') << std::setprecision(precision) << value;
            break;
        default:
            {
                throw std::logic_error("Invalid representation specified: " + std::to_string(static_cast<int>(representation)));
            }
    }

    return serialization::Align(stream.str(), width);
}

std::string Serialize(long double value, int width, int precision, FloatingPointRepresentation representation)
{
    std::ostringstream stream;

    switch (representation)
    {
        case FloatingPointRepresentation::Fixed:
            stream.setf(std::ios_base::floatfield, std::ios_base::fixed);
            stream << std::setfill('0') << std::setw(width) << std::setprecision(precision) << value;
            break;
        case FloatingPointRepresentation::Exponential:
            stream.setf(std::ios_base::floatfield, std::ios_base::scientific);
            stream << std::setfill('0') << std::setw(width) << std::setprecision(precision) << value;
            break;
        case FloatingPointRepresentation::Mixed:
            stream.unsetf(std::ios_base::floatfield);
            stream << std::setfill('0') << std::setprecision(precision) << value;
            break;
        default:
            {
                throw std::logic_error("Invalid representation specified: " + std::to_string(static_cast<int>(representation)));
            }
    }

    return serialization::Align(stream.str(), width);
}

std::string Serialize(const std::string &value, int width, bool quote)
{
    std::ostringstream stream;

    if (quote) stream << "\"";
    stream << value;
    if (quote) stream << "\"";

    return serialization::Align(stream.str(), width);
}

std::string Serialize(const char* value, int width, bool quote)
{
    std::ostringstream stream;

    stream << Serialize(std::string(value), width, quote);

    return serialization::Align(stream.str(), width);
}

std::string Serialize(const void* value, int width)
{
    std::ostringstream stream;

    if (value != nullptr)
    {
        stream << "0x" + Serialize(CastPointerToInteger(value), 0, 16);
    }
    else
    {
        stream << "null";
    }

    return serialization::Align(stream.str(), width);
}

std::string Serialize(void* value, int width)
{
    return Serialize(const_cast<const void *>(value), width); //TICS !PRE-CON#002 Purposely done to avoid code duplication
}

std::string Serialize(const std::chrono::nanoseconds& value, int width)
{
    std::ostringstream stream;
    stream << value.count() << " ns";
    return serialization::Align(stream.str(), width);
}

std::string Serialize(const std::chrono::milliseconds& value, int width)
{
    std::ostringstream stream;
    stream << value.count() << " ms";
    return serialization::Align(stream.str(), width);
}

std::string Serialize(const std::chrono::seconds& value, int width)
{
    std::ostringstream stream;
    stream << value.count() << " s";
    return serialization::Align(stream.str(), width);
}

template<class Rep, class Period>
std::string Serialize(std::chrono::duration<Rep, Period>& value, int width)
{
    return Serialize(std::chrono::duration_cast<std::chrono::nanoseconds>(value), width);
}

std::string SerializeData(const std::uint8_t* value, std::size_t size)
{
    std::ostringstream stream;

    if (value != nullptr)
    {
        stream << std::endl;
        const std::size_t LineDisplayBytes = 16;
        const std::size_t MaxDisplayBytes = 4096;
        std::size_t displayBytes = (size < MaxDisplayBytes) ? size : MaxDisplayBytes;
        for (std::size_t i = 0; i < displayBytes; i += LineDisplayBytes)
        {
            stream << Serialize(i, 0, 16) << "  ";
            for (std::size_t j = 0; j < LineDisplayBytes; ++j)
            {
                if (j != 0) stream << " ";
                if ((i + j) < displayBytes) stream << Serialize(value[i + j], 2, 16);
                else stream << "  ";
            }
            stream << "  ";
            for (std::size_t j = 0; j < LineDisplayBytes; ++j)
            {
                if (j != 0) stream << " ";
                if ((i + j) < displayBytes)
                {
                    auto ch = value[i + j];
                    stream << ((std::isprint(ch) ? static_cast<char>(ch) : '.'));
                }
                else
                {
                    stream << " ";
                }
            }
            stream << std::endl;
        }
    }
    else
    {
        stream << "null";
    }
    return stream.str();
}

std::string SerializeData(const std::vector<std::uint8_t>& value)
{
    if (value.size() > 0) // TICS !PRE-CFL#013
        return SerializeData(value.data(), value.size());
    return "-";
}

std::string SerializeMACAddress(const std::vector<std::uint8_t>& address)
{
    std::ostringstream stream;
    if (address.size() != 6) // TICS !PRE-CFL#013
        return "Invalid format for MAC address";
    stream << std::hex << std::setw(2) << std::setfill('0') << std::noshowbase 
        << static_cast<int>(address[0]) << ":";
    stream << std::hex << std::setw(2) << std::setfill('0') << std::noshowbase 
        << static_cast<int>(address[1]) << ":";
    stream << std::hex << std::setw(2) << std::setfill('0') << std::noshowbase 
        << static_cast<int>(address[2]) << ":";
    stream << std::hex << std::setw(2) << std::setfill('0') << std::noshowbase 
        << static_cast<int>(address[3]) << ":";
    stream << std::hex << std::setw(2) << std::setfill('0') << std::noshowbase 
        << static_cast<int>(address[4]) << ":";
    stream << std::hex << std::setw(2) << std::setfill('0') << std::noshowbase 
        << static_cast<int>(address[5]);
    return stream.str();
}

} // namespace serialization
