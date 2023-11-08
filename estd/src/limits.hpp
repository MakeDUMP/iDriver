#pragma once
#include <stdint.hpp>

namespace estd {

template <typename T>
struct numeric_limits;

struct estd_integral_limits {
    static constexpr bool is_integer = true;
};

struct estd_floating_point_limits {
    static constexpr bool is_integer = false;
};

template <>
struct numeric_limits<estd::int8_t> : public estd_integral_limits { };
template <>
struct numeric_limits<estd::int16_t> : public estd_integral_limits { };
template <>
struct numeric_limits<estd::int32_t> : public estd_integral_limits { };
template <>
struct numeric_limits<estd::int64_t> : public estd_integral_limits { };
template <>
struct numeric_limits<estd::uint8_t> : public estd_integral_limits { };
template <>
struct numeric_limits<estd::uint16_t> : public estd_integral_limits { };
template <>
struct numeric_limits<estd::uint32_t> : public estd_integral_limits { };
template <>
struct numeric_limits<estd::uint64_t> : public estd_integral_limits { };

template <>
struct numeric_limits<long long> : public estd_integral_limits { };

template <>
struct numeric_limits<float> : public estd_floating_point_limits { };
template <>
struct numeric_limits<double> : public estd_floating_point_limits { };
template <>
struct numeric_limits<long double> : public estd_floating_point_limits { };

// template <>
// struct numeric_limits<estd::size_t> {
//     static constexpr estd::size_t(min)() noexcept { return 0; }
//     static constexpr estd::size_t(max)() noexcept { return 0xffff'ffff'ffff'ffffui64; }
// };

} // estd
