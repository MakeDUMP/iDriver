#pragma once
// clang-format off
#include <stdint.hpp>
#include <char_traits.hpp>
#include <algorithm.hpp>
// clang-format on

namespace estd {

template <typename T, typename TTraits = estd::char_traits<T>>
class basic_string_view {
public:
    using const_pointer = T const*;
    using const_iterator = T const*;
    using const_reference = T const&;

    enum { npos = estd::size_t(-1) };

    constexpr basic_string_view() noexcept
        : m_begin(nullptr)
        , m_end(nullptr)
    {
    }

    constexpr explicit basic_string_view(T const* begin)
        : m_begin(begin)
        , m_end(begin + TTraits::length(begin))
    {
    }

    constexpr basic_string_view(T const* begin, T const* end)
        : m_begin(begin)
        , m_end(end)
    {
    }

    constexpr basic_string_view(T const* begin, estd::size_t size)
        : m_begin(begin)
        , m_end(begin + size)
    {
    }

    constexpr basic_string_view(basic_string_view const& other) noexcept
        : m_begin(other.m_begin)
        , m_end(other.m_end)
    {
    }

    constexpr const_pointer data() const { return m_begin; }
    constexpr const_iterator begin() const { return m_begin; }
    constexpr const_iterator cbegin() const { return m_begin; }
    constexpr const_iterator rbegin() const { return m_end; }
    constexpr const_iterator crbegin() const { return m_end; }
    constexpr const_iterator end() const { return m_end; }
    constexpr const_iterator cend() const { return m_end; }
    constexpr const_iterator rend() const { return m_begin; }
    constexpr const_iterator crend() const { return m_begin; }
    constexpr bool empty() const { return (m_begin == m_end); }
    constexpr estd::size_t size() const { return (m_end - m_begin); }
    constexpr estd::size_t length() const { return size(); }
    constexpr estd::size_t max_size() const { return size(); }
    constexpr const_reference front() const { return *m_begin; }
    constexpr const_reference back() const { return *(m_end - 1); }

    constexpr basic_string_view substr(estd::size_t position = 0, estd::size_t count = npos) const
    {
        return (position < size()) ? basic_string_view(m_begin + position, estd::min(count, size() - position)) : basic_string_view {};
    }

    friend constexpr bool operator==(basic_string_view<T, TTraits> const& left, basic_string_view<T, TTraits> const& right)
    {
        return (left.size() == right.size()) && estd::equal(left.begin(), left.end(), right.begin());
    }

    constexpr const_reference operator[](estd::size_t index) const
    {
        return m_begin[index];
    }

    constexpr estd::size_t find(basic_string_view<T, TTraits> view, estd::size_t position = 0) const
    {
        if ((size() < view.size()))
            return npos;
        const_iterator iposition = estd::search(begin() + position, end(), view.begin(), view.end());
        if (iposition == end())
            return npos;
        else
            return iposition - begin();
    }

    // constexpr const_reference at(estd::size_t i) const
    // {
    //   ETL_ASSERT((mbegin != ETL_NULLPTR && mend != ETL_NULLPTR), ETL_ERROR(string_view_uninitialised));
    //   ETL_ASSERT(i < size(), ETL_ERROR(string_view_bounds));
    //   return mbegin[i];
    // }

private:
    const_pointer m_begin;
    const_pointer m_end;
};

using string_view = estd::basic_string_view<char>;

template <estd::size_t ARRAY_SIZE>
constexpr string_view make_string_view(const char (&text)[ARRAY_SIZE])
{
    estd::size_t length = estd::char_traits<char>::length(text, ARRAY_SIZE - 1U);
    return string_view { text, length };
}

inline namespace literals {
    inline namespace string_literals {
        constexpr estd::string_view operator"" sv(char const* str, estd::size_t length) noexcept
        {
            return estd::string_view { str, length };
        }
    }
}

} // namespace estd
