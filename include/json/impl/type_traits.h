#ifndef SCISRS_IMPL_TYPE_TRAITS_H
#define SCISRS_IMPL_TYPE_TRAITS_H

#include <type_traits>
#include <string>
#include <string_view>

namespace json {

template<typename T>
struct is_string
    : public std::disjunction<
            std::is_same<char *, typename std::decay_t<T>>,
            std::is_same<const char *, typename std::decay_t<T>>,
            std::is_same<std::string, typename std::decay_t<T>>,
            std::is_same<std::string_view, typename std::decay_t<T>>
    > 
{};
template<typename T>
inline constexpr bool is_string_v = is_string<T>::value; 


template<typename T>
struct is_char_array
    : public std::disjunction<
            std::is_same<char *, typename std::decay_t<T>>,
            std::is_same<const char *, typename std::decay_t<T>>
    > 
{
    using type = char [];
};
template<typename T>
inline constexpr bool is_char_array_v = is_char_array<T>::value; 

template<class T>
struct is_refwrap : public std::false_type
{};
template<class T>
struct is_refwrap<std::reference_wrapper<T>> : public std::true_type
{
    using reftype = T;
};
template<typename T>
inline constexpr bool is_refwrap_v = is_refwrap<T>::value; 
template<typename T>
using is_refwrap_refty = typename is_refwrap<T>::reftype; 


}
#endif