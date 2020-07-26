/*
 * utility.hpp
 *
 *  Created on: Jun 10, 2020
 *      Author: rt135
 */

#ifndef INCLUDE_RODRIGOTORRES_UTILITY_HPP_
#define INCLUDE_RODRIGOTORRES_UTILITY_HPP_

#include <type_traits>

/// ENABLE BITWISE OPERATIONS ON SCOPED ENUMS

// Now we need a MACRO to reduce so much coding

#define DECLARE_BITWISE_OPERATORS(x) 	\
template<>                            \
struct EnableBitwiseOperators<x>      \
{                                     \
  static bool const enable = true;    \
};

template <typename Enum>
struct EnableBitwiseOperators
{
  static bool const enable = false;
};

template <typename Enum, 
typename = std::enable_if_t<EnableBitwiseOperators<Enum>::enable, Enum>>
Enum operator  | (Enum  lhs, Enum  rhs)
{
  // We assert the template argument is of type enum
  static_assert(std::is_enum<Enum>::value,
                     "The template parameter is not an enumeration type!");
  using underlying = typename std::underlying_type<Enum>::type;
  // Could we use underlying_type_t<Enum> instead?

  return static_cast<Enum> (
        static_cast<underlying>(lhs) |
        static_cast<underlying>(rhs)
        );
}

/// Then we do the same for the other operators

template<typename Enum, 
         typename = std::enable_if_t<EnableBitwiseOperators<Enum>::enable, Enum>>
Enum operator  & (Enum  lhs, Enum  rhs)
{
  // We assert the template argument is of type enum
  static_assert(std::is_enum<Enum>::value,
                     "The template parameter is not an enumeration type!");
  using underlying = typename std::underlying_type<Enum>::type;
  // Could we use underlying_type_t<Enum> instead?

  return static_cast<Enum> (
        static_cast<underlying>(lhs) &
        static_cast<underlying>(rhs)
        );
}

template<typename Enum, 
         typename = std::enable_if_t<EnableBitwiseOperators<Enum>::enable, Enum>>
Enum operator  ^ (Enum lhs, Enum rhs)
{
  // We assert the template argument is of type enum
  static_assert(std::is_enum<Enum>::value,
                     "The template parameter is not an enumeration type!");
  using underlying = typename std::underlying_type<Enum>::type;
  // Could we use underlying_type_t<Enum> instead?

  return static_cast<Enum> (
        static_cast<underlying>(lhs) ^
        static_cast<underlying>(rhs)
        );
}

template<typename Enum, 
         typename = std::enable_if_t<EnableBitwiseOperators<Enum>::enable, Enum>>
Enum operator  ~ (Enum rhs)
{
  // We assert the template argument is of type enum
  static_assert(std::is_enum<Enum>::value,
                     "The template parameter is not an enumeration type!");
  using underlying = typename std::underlying_type<Enum>::type;
  // Could we use underlying_type_t<Enum> instead?

  return static_cast<Enum>(~static_cast<underlying>(rhs));
}

template<typename Enum, 
         typename = std::enable_if_t<EnableBitwiseOperators<Enum>::enable, Enum>>
Enum operator  |= (Enum & lhs, Enum rhs)
{
  // We assert the template argument is of type enum
  static_assert(std::is_enum<Enum>::value,
                     "The template parameter is not an enumeration type!");
  using underlying = typename std::underlying_type<Enum>::type;
  // Could we use underlying_type_t<Enum> instead?

  lhs = static_cast<Enum>(
        static_cast<underlying>(lhs) |
        static_cast<underlying>(rhs)
        );
  return lhs;
}

template<typename Enum, 
         typename = std::enable_if_t<EnableBitwiseOperators<Enum>::enable, Enum>>
Enum operator  &= (Enum & lhs, Enum rhs)
{
  // We assert the template argument is of type enum
  static_assert(std::is_enum<Enum>::value,
                     "The template parameter is not an enumeration type!");
  using underlying = typename std::underlying_type<Enum>::type;
  // Could we use underlying_type_t<Enum> instead?

  lhs = static_cast<Enum>(
        static_cast<underlying>(lhs) &
        static_cast<underlying>(rhs)
        );
  return lhs;
}

template<typename Enum, 
         typename = std::enable_if_t<EnableBitwiseOperators<Enum>::enable, Enum>>
Enum operator  ^= (Enum & lhs, Enum rhs)
{
  // We assert the template argument is of type enum
  static_assert(std::is_enum<Enum>::value,
                     "The template parameter is not an enumeration type!");
  using underlying = typename std::underlying_type<Enum>::type;
  // Could we use underlying_type_t<Enum> instead?

  lhs = static_cast<Enum>(
        static_cast<underlying>(lhs) ^
        static_cast<underlying>(rhs)
        );
  return lhs;
}



#endif /* INCLUDE_RODRIGOTORRES_UTILITY_HPP_ */
