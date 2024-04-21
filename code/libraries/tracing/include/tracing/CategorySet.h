#pragma once

#include <set>
#include <ostream>
#include "utility/Serialization.h"

namespace tracing
{

template<class Category>
class CategorySet
{
private:
    std::set<Category> m_set;
    using MaskType = typename std::underlying_type<Category>::type;

public:
    CategorySet()
        : m_set()
    {}

    CategorySet(const CategorySet& other)
        : m_set()
    {
        set(static_cast<Category>(other.GetMask()));
    }

    CategorySet(MaskType mask)
        : m_set()
    {
        set(static_cast<Category>(mask));
    }

    CategorySet(Category traceCategory)
        : m_set()
    {
        set(traceCategory);
    }

    CategorySet& operator = (const CategorySet& other)
    {
        if (this != &other)
        {
            m_set.clear();
            set(static_cast<Category>(other.GetMask()));
        }
        return *this;
    }

    void clear()
    {
        m_set.clear();
    }

    void set(Category traceCategory)
    {
        MaskType tmpMask = static_cast<MaskType>(1) << (sizeof(MaskType) * 8 - 1);
        while (tmpMask != 0)
        {
            if ((static_cast<MaskType>(traceCategory)& tmpMask) != 0)
                m_set.emplace(static_cast<Category>(tmpMask));
            tmpMask >>= 1;
        }
    }

    void reset(Category traceCategory)
    {
        m_set.erase(traceCategory);
    }

    bool is_set(Category traceCategory) const
    {
        return m_set.find(traceCategory) != m_set.end();
    }

    MaskType GetMask() const
    {
        MaskType result {};
        MaskType tmpMask = static_cast<MaskType>(1) << (sizeof(MaskType) * 8 - 1);
        while (tmpMask != 0)
        {
            if (is_set(static_cast<Category>(tmpMask)))
                result |= tmpMask;
            tmpMask >>= 1;
        }
        return result;
    }

    CategorySet& operator |= (Category traceCategory)
    {
        set(traceCategory);
        return *this;
    }
};

template<class Category>
inline CategorySet<Category> operator | (CategorySet<Category> lhs, Category rhs)
{
    CategorySet<Category> newSet = lhs;
    newSet |= rhs;
    return newSet;
}

template<class Category>
inline CategorySet<Category> operator | (Category lhs, Category rhs)
{
    CategorySet<Category> newSet = lhs;
    newSet |= rhs;
    return newSet;
}

template<class Category>
inline bool operator == (const CategorySet<Category>& lhs, const CategorySet<Category>& rhs)
{
    return lhs.GetMask() == rhs.GetMask();
}

template<class Category>
inline bool operator == (const typename std::underlying_type<Category>::type& lhs, const CategorySet<Category>& rhs)
{
    return lhs == rhs.GetMask();
}

template<class Category>
inline bool operator == (const CategorySet<Category>& lhs, const typename std::underlying_type<Category>::type& rhs)
{
    return lhs.GetMask() == rhs;
}

template<class Category>
inline bool operator != (const CategorySet<Category>& lhs, const CategorySet<Category>& rhs)
{
    return lhs.GetMask() != rhs.GetMask();
}

template<class Category>
inline bool operator != (const typename std::underlying_type<Category>::type& lhs, const CategorySet<Category>& rhs)
{
    return lhs != rhs.GetMask();
}

template<class Category>
inline bool operator != (const CategorySet<Category>& lhs, const typename std::underlying_type<Category>::type& rhs)
{
    return lhs.GetMask() != rhs;
}

template<class Category>
std::ostream& operator << (std::ostream& stream, const CategorySet<Category>& value)
{
    return stream << serialization::Serialize(value);
}

} // namespace tracing

namespace serialization {

template<class Category>
std::string Serialize(const tracing::CategorySet<Category>& value)
{
    std::ostringstream stream;
    stream << Serialize(value.GetMask(), 0, 16, true);
    return stream.str();
}

} // namespace serialization
