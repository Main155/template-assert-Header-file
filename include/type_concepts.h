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
	concept FloatingPoint = std::is_floating_point_v<std::remove_cvref_t<T>>;
	//--------------------------------------
	//The constraint is character
	template <typename T>
	concept Character = std::is_same_v<std::decay_t<T>, char > ||
		      std::is_same_v<std::decay_t<T>, wchar_t > ||
		    std::is_same_v<std::decay_t<T>, char8_t > ||
		  std::is_same_v<std::decay_t<T>, char16_t> ||
		std::is_same_v<std::decay_t<T>, char32_t>;
	//--------------------------------------
	//Restricted to pointers
	template <typename T>
	concept Pointer = std::is_pointer_v<std::remove_cvref_t<T>>;
	//--------------------------------------
	//The constraint is is lvalue reference
	template <typename T>
	concept LvalueReference = std::is_lvalue_reference_v<T>;
	//--------------------------------------
	//Constraint is an unsigned type
	template <typename T>
	concept Unsigned = std::is_unsigned_v<std::remove_cvref_t<T>>;
	//--------------------------------------
	//Constraints are signed
	template <typename T>
	concept Signed = std::is_signed_v<std::remove_cvref_t<T>>;
	//--------------------------------------
	//Constraint is an arithmetic type
	template <typename T>
	concept Arithmetic = std::is_arithmetic_v<std::remove_cvref_t<T>>;
	//--------------------------------------
	// The constraint is a function 
	template <typename T>
	concept Function = std::is_function_v<std::remove_reference_t<T>>;
	//======================================
	//======================================
	//True if it is an integer
	template <Integral T>
	constexpr bool is_integral_concept(T &&t) { return true; }
	//--------------------------------------
	//Returns true if it is a floating-point number
	template <FloatingPoint T>
	constexpr bool is_floating_point_concept(T &&t) { return true; }
	//--------------------------------------
	//If it is character, return true
	template <Character T>
	constexpr bool is_char_concept(T &&t) { return true; }
	//--------------------------------------
	//The pointer returns true
	template <Pointer T>
	constexpr bool is_pointer_concept(T &&t) { return true; }
	//--------------------------------------
	//The lvalue reference returns true
	template <LvalueReference T>
	constexpr bool is_lvalue_reference_concept(T &&t) { return true; }
	//--------------------------------------
	//Returns true if it is unsigned 
	template <Unsigned T>
	constexpr bool is_unsigned_concept(T &&t) { return true; }
	//--------------------------------------
	//Returns true if it is signed
	template <Signed T>
	constexpr bool is_signed_concept(T &&t) { return true; }
	//--------------------------------------
	//Returns true if it is an arithmetic type
	template <Arithmetic T>
	constexpr bool is_arithmetic_concept(T &&t) { return true; }
	//--------------------------------------
	//The function returns true
	template <Function T>
	constexpr bool is_function_concept(T&& t) { return true; }

}
