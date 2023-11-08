#pragma once
// clang-format off
#include <stdint.hpp>
#include <type_traits.hpp>
// clang-format on

namespace estd {

template <typename T>
constexpr T&& forward(typename estd::remove_reference<T>::type& t) noexcept { return static_cast<T&&>(t); }
template <typename T>
constexpr T&& forward(typename estd::remove_reference<T>::type&& t) noexcept { return static_cast<T&&>(t); }

template <typename TFirst, typename TSecond>
struct pair {
    using first_type = TFirst;
    using second_type = TSecond;

    TFirst first;
    TSecond second;

    // Default constructor
    // The default constructor creates first and second using their respective default constructors.
    constexpr pair() noexcept
        : first(TFirst())
        , second(TSecond())
    {
    }

    // Constructor from parameters
    // Two objects may be passed to a pair constructor to be copied.
    constexpr pair(TFirst const& a, TSecond const& b)
        : first(a)
        , second(b)
    {
    }

    // Move constructor from parameters
    template <typename T1, typename T2>
    constexpr pair(T1&& a, T2&& b)
        : first(forward<T1>(a))
        , second(forward<T2>(b))
    {
    }

    // Copy constructor
    // There is also a templated copy constructor for the pair class itself.
    template <typename T1, typename T2>
    constexpr pair(pair<T1, T2> const& other)
        : first(other.first)
        , second(other.second)
    {
    }

    // Move constructor
    template <typename U1, typename U2>
    constexpr pair(pair<U1, U2>&& other)
        : first(forward<U1>(other.first))
        , second(forward<U2>(other.second))
    {
    }
};

template <typename TFirst, typename TSecond>
inline estd::pair<TFirst, TSecond> make_pair(TFirst&& a, TSecond&& b)
{
    return pair<TFirst, TSecond>(forward<TFirst>(a), forward<TSecond>(b));
}

} // namespace estd
