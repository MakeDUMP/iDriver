#pragma once

namespace estd {

template <typename T, const T VALUE>
struct integral_constant {
    static const T value = VALUE;

    typedef T value_type;
    typedef integral_constant<T, VALUE> type;

    operator value_type() const
    {
        return value;
    }
};

typedef integral_constant<bool, false> false_type;
typedef integral_constant<bool, true> true_type;

template <typename T1, typename T2>
struct is_same : public false_type { };
template <typename T>
struct is_same<T, T> : public true_type { };

template <typename T1, typename T2>
inline constexpr bool is_same_v = is_same<T1, T2>::value;

template <typename T>
struct add_rvalue_reference {
    using type = T&&;
};

template <typename T>
typename add_rvalue_reference<T>::type declval() noexcept;

template <typename T>
struct remove_reference {
    typedef T type;
};

template <typename T>
struct remove_reference<T&> {
    typedef T type;
};

template <typename T>
struct remove_reference<T&&> {
    typedef T type;
};

template <typename T>
using remove_reference_t = typename remove_reference<T>::type;

} // namespace estd
