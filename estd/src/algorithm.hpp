#pragma once
#include <stdint.hpp>

namespace estd {

template <typename T>
constexpr const T& min(const T& a, const T& b) { return (a < b) ? a : b; }

template <typename T>
constexpr const T& max(const T& a, const T& b) { return (a < b) ? b : a; }

template <typename TIterator1, typename TIterator2>
constexpr bool equal(TIterator1 first1, TIterator1 last1, TIterator2 first2)
{
    while (first1 != last1) {
        if (*first1 != *first2)
            return false;
        first1++;
        first2++;
    }
    return true;
}

template <typename TIterator1, typename TIterator2>
constexpr TIterator1 search(TIterator1 first, TIterator1 last, TIterator2 search_first, TIterator2 search_last)
{
    while (true) {
        TIterator1 itr = first;
        TIterator2 search_itr = search_first;
        while (true) {
            if (search_itr == search_last)
                return first;
            if (itr == last)
                return last;
            if (*itr != *search_itr)
                break;
            itr++;
            search_itr++;
        }
        first++;
    }
}

} // estd
