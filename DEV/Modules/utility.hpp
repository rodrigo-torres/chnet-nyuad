/** UTILITY: Swiss-army of utility 'things'
 *  Copyright (C) 2020 Rodrigo Torres and contributors
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef INCLUDE_UTILITY_HPP_
#define INCLUDE_UTILITY_HPP_

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



#endif /* INCLUDE_UTILITY_HPP_ */
