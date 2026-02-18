#pragma once
//
#if defined(_MSC_VER)
#define rem_finline      __forceinline
#define rem_finline_cxpr __forceinline constexpr
#elif defined(__GNUC__) || defined(__clang__)
#define rem_finline      __attribute__((always_inline)) inline
#define rem_finline_cxpr __attribute__((always_inline)) inline constexpr
#else
#define rem_finline      inline
#define rem_finline_cxpr inline constexpr
#endif

#define rem_disable_copy(Class) \
    Class(const Class&) = delete; \
    Class& operator=(const Class&) = delete;

#define rem_disable_move(Class) \
    Class(Class&&) = delete; \
    Class& operator=(Class&&) = delete;

#define rem_disable_copy_and_move(Class) \
    rem_disable_copy(Class) \
    rem_disable_move(Class)
