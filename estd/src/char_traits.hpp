#pragma once
#include <stdint.hpp>

namespace estd {

template <typename T>
struct char_traits_types;

template <>
struct char_traits_types<char> {
    using char_type = char;
    using int_type = int;
    using off_type = estd::size_t;
    using pos_type = estd::size_t;
    using state_type = char;
};

template <typename T>
struct char_traits : public char_traits_types<T> {
    using char_type = typename char_traits_types<T>::char_type;
    using int_type = typename char_traits_types<T>::int_type;
    using off_type = typename char_traits_types<T>::off_type;
    using pos_type = typename char_traits_types<T>::pos_type;
    using state_type = typename char_traits_types<T>::state_type;

    constexpr static int_type eof() { return -1; }
    constexpr static estd::size_t length(char_type const* str)
    {
        estd::size_t count = 0;
        if (str != 0)
            while (*str++ != 0)
                ++count;
        return count;
    }
    constexpr static estd::size_t length(char_type const* str, estd::size_t max_length)
    {
        estd::size_t count = 0;
        if (str != 0)
            while ((count < max_length) && (*str++ != 0))
                ++count;
        return count;
    }
};

} // namespace estd
