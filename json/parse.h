#ifndef PARSE_H_ 
#define PARSE_H_

#include <string>
#include <sstream>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cerrno>

#include <boost/optional.hpp>

namespace Json {
namespace detail {

template<typename T, typename std::enable_if<
    std::is_integral<T>::value && std::is_same<T, bool>::value, T>::type* = nullptr>
inline boost::optional<T> parse(const std::string& value) {
    boost::optional<bool> res;
    if( not value.empty() ) {
        std::string lower_value_string = common::lower_string(value);
        if (lower_value_string == "true" or lower_value_string == "false") {
            res = ("true" == lower_value_string);
        }
    }
    return res;
}

template<typename T, typename std::enable_if<
    std::is_integral<T>::value && std::is_signed<T>::value, T>::type* = nullptr>
inline boost::optional<T> parse(const std::string& value) {
    boost::optional<T> res;
    if ( not value.empty() ) {
        const char* const value_c_str = value.c_str();
        char* end;
        errno = 0;
        const long lvalue = strtol(value_c_str, &end, 10);
        if ( end != value_c_str and '\0' == *end
                and lvalue <= std::numeric_limits<T>::max()
                and lvalue >= std::numeric_limits<T>::min()
                and errno != ERANGE)    // overflow check
        {
            res = static_cast<T>(lvalue);
        }
    }
    return res;
}

template<typename T, typename std::enable_if<
    std::is_integral<T>::value && !std::is_same<T, bool>::value && std::is_unsigned<T>::value, T>::type* = nullptr>
inline boost::optional<T> parse(const std::string& value) {
    boost::optional<T> res;
    if ( not value.empty() and value.find("-") == std::string::npos ) {
        const char* const value_c_str = value.c_str();
        char* end;
        errno = 0;
        const unsigned long lvalue = strtoul(value_c_str, &end, 10);
        if ( end != value_c_str and '\0' == *end
                and lvalue <= std::numeric_limits<T>::max()
                and lvalue >= std::numeric_limits<T>::min()
                and errno != ERANGE)    // overflow check
        {
            res = static_cast<T>(lvalue);
        }
    }
    return res;
}

template<typename T, typename std::enable_if<std::is_floating_point<T>::value, T>::type* = nullptr>
inline boost::optional<T> parse(const std::string& value) {
    boost::optional<T> res;
    if ( not value.empty() ) {
        const char* const value_c_str = value.c_str();
        char* end;
        errno = 0;
        const double dvalue = strtod(value_c_str, &end);
        if (end != value_c_str and '\0' == *end and errno != ERANGE) {
            res = static_cast<T>(dvalue);
        }
    }
    return res;
}

template <typename T>
inline T parse(const std::string& value, const T& default_value)
{
    boost::optional<T> res = parse<T>(value);
    if ( not res ) {
        res = default_value;
    }
    return res.get();
}

} // namespace detail
} // namespace Json

#endif // PARSE_H_
