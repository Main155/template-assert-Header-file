//-@Main155
#pragma once

#include <type_traits>
#include <concepts>

namespace lib
{
	//======================================
	//Constraint to be an integer
	template <typename T>
	concept Integral = std::is_integral_v<std::remove_cvref_t<T>>;
	//--------------------------------------
	//Constraints are floating-point
	template <typename T>
	concept Floating_point = std::is_floating_point_v<std::remove_cvref_t<T>>;
	//--------------------------------------
	//The constraint is character
	template <typename T>
	concept character = std::is_same_v<std::decay_t<T>, char > ||
		      std::is_same_v<std::decay_t<T>, wchar_t > ||
		    std::is_same_v<std::decay_t<T>, char8_t > ||
		  std::is_same_v<std::decay_t<T>, char16_t> ||
		std::is_same_v<std::decay_t<T>, char32_t>;
	//--------------------------------------
	//Restricted to pointers
	template <typename T>
	concept Pointers = std::is_pointer_v<std::remove_cvref_t<T>>;
	//--------------------------------------
	//The constraint is islvalue reference 
	template <typename T>
	concept Lvalue_reference = std::is_lvalue_reference_v<T>;
	//======================================
	//======================================
	//True if it is an integer
	template <Integral T>
	constexpr bool is_integral_concept(T &&t) { return true; }
	//--------------------------------------
	//Returns true if it is a floating-point number
	template <Floating_point T>
	constexpr bool is_floating_point_concept(T &&t) { return true; }
	//--------------------------------------
	//If it is character, return true
	template <character T>
	constexpr bool is_char_concept(T &&t) { return true; }
	//--------------------------------------
	//The pointer returns true
	template <Pointers T>
	constexpr bool is_pointer_concept(T&& t) { return true; }
	//--------------------------------------
	//The lvalue reference returns true
	template <Lvalue_reference T>
	constexpr bool is_lvalue_reference_concept(T&&) { return true; }
}
