#pragma once

#include "pch.hpp"

using i8  = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using f32 = float;
using f64 = double;

using std::array;
using std::span;
using std::set;
using std::map;

template<typename T>
using shared = std::shared_ptr<T>;

template<typename T>
using weak = std::weak_ptr<T>;

template<typename T>
using unique = std::unique_ptr<T>;

template<typename T>
using func = std::function<T>;

template<typename T>
using vec = std::vector<T>;

using str   = std::string;
using str_v = std::string_view;

using logger = spdlog::logger;

template<typename F>
struct remove_noexcept
{
    using type = F;
};

template<typename F, typename... S>
struct remove_noexcept<F(S...) noexcept>
{
    using type = F(S...);
};

template<typename T>
using rm_noexcept = typename remove_noexcept<T>::type;

template<typename X>
using rm_ptr = typename std::remove_pointer<X>::type;

template<typename X>
using rm_ref = typename std::remove_reference<X>::type;

template<bool condition, typename A, typename B>
using if_type = typename std::conditional<condition, A, B>::type;

template<typename T>
struct opt final
{
private:
    static constexpr auto is_fund = std::is_fundamental<T>::value;
    static constexpr auto is_ptr  = std::is_pointer<T>::value;
    static constexpr auto is_ref  = std::is_reference<T>::value;

    static constexpr auto is_simple = is_fund || is_ptr || is_ref;

public:
    using ptr = if_type<is_simple, T, T*>;
    using ref = if_type<is_simple, T, T&>;

    using cptr = if_type<is_fund, const T, if_type<is_ptr, const rm_ptr<T>*, if_type<is_ref, const rm_ref<T>*, const T*>>>;
    using cref = if_type<is_fund, const T, if_type<is_ref, const rm_ref<T>&, if_type<is_ptr, const rm_ptr<T>*, const T&>>>;
};

namespace nd::src::tools
{
} // namespace nd::src::tools
