#pragma once

#include "json-parser/JSONValue.h"

namespace json_parser {

class JSONNumber : public JSONValue
{
public:
    JSONNumber();
    JSONNumber(const std::string& value);
    JSONNumber(int8_t value);
    JSONNumber(uint8_t value);
    JSONNumber(int16_t value);
    JSONNumber(uint16_t value);
    JSONNumber(int32_t value);
    JSONNumber(uint32_t value);
    JSONNumber(int64_t value);
    JSONNumber(uint64_t value);
    JSONNumber(float value);
    JSONNumber(double value);
    virtual ~JSONNumber() {};

    virtual void Serialize(std::ostream& stream, int indentDepth = 0, bool indentInitial = true) const override;

    void Clear() { m_value.clear(); }
    virtual JSONValueType Type() const override { return JSONValueType::Number; }
    const std::string& GetValue() const { return m_value; }
    bool GetValue(int8_t& value) const;
    bool GetValue(uint8_t& value) const;
    bool GetValue(int16_t& value) const;
    bool GetValue(uint16_t& value) const;
    bool GetValue(int32_t& value) const;
    bool GetValue(uint32_t& value) const;
    bool GetValue(int64_t& value) const;
    bool GetValue(uint64_t& value) const;
    bool GetValue(float& value) const;
    bool GetValue(double& value) const;
    void SetValue(const std::string& value) { m_value = value; }
    void SetValue(int8_t value);
    void SetValue(uint8_t value);
    void SetValue(int16_t value);
    void SetValue(uint16_t value);
    void SetValue(int32_t value);
    void SetValue(uint32_t value);
    void SetValue(int64_t value);
    void SetValue(uint64_t value);
    void SetValue(float value);
    void SetValue(double value);

private:
    std::string m_value;
};

} // namespace json_parser

namespace serialization {

template<>
inline std::string Serialize(const json_parser::JSONNumber& value)
{
    std::ostringstream stream;
    value.Serialize(stream, 0);
    return stream.str();
}

} // namespace serialization
