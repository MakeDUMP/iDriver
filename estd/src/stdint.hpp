namespace estd {

using nullptr_t = decltype(nullptr);
using ptrdiff_t = __PTRDIFF_TYPE__;
using intptr_t = __INTPTR_TYPE__;
using uintptr_t = __UINTPTR_TYPE__;

using size_t = __SIZE_TYPE__;
// using ssize_t = signed decltype(__SIZE_TYPE__);

using int8_t = signed char;
using int16_t = signed short;
using int32_t = signed int;
using int64_t = signed long;

using uint8_t = unsigned char;
using uint16_t = unsigned short;
using uint32_t = unsigned int;
using uint64_t = unsigned long long;

#define PRIX8 "hhX"
#define PRIX16 "hX"
#define PRIX32 "X"
#define PRIX64 "llX"

} // namespace estd
