//-@Main155 
#pragma once

//Embedded available header file.
//Compile-time type checking.

#include <type_traits>

namespace lib
{
	//Compile-time check whether data is an integer.
	template <typename T>
	constexpr void assert_is_integer(T&&t)
	{
		static_assert(
			std::is_integral_v<std::decay_t<T>>,
			"error : T must be an integral type. ");
	}
	//------------------------------------------
	//Compile-time check for whether it is a floating-point type.
	template <typename T>
	constexpr void assert_is_floating_point(T&&t)
	{
		static_assert(
			std::is_floating_point_v<std::decay_t<T>>,
			"error : T must be a floating point type. ");
	}
	//------------------------------------------
	//Compile-time check for whether it is a character.
	template<typename T>
	constexpr void assert_is_character(T&&t)
	{
		static_assert(
			std::is_same_v<std::decay_t<T>, char    > ||
			std::is_same_v<std::decay_t<T>, wchar_t > ||
			std::is_same_v<std::decay_t<T>, char8_t > ||
			std::is_same_v<std::decay_t<T>, char16_t> ||
			std::is_same_v<std::decay_t<T>, char32_t>,
			"error : T must be a character type. ");
	}
	//------------------------------------------
	// Compile-time check whether T is a numeric type (integer or floating-point).
	template <typename T>
	constexpr void assert_is_numeric(T&&t) {
		static_assert(
			std::is_integral_v<std::decay_t<T>> ||
			std::is_floating_point_v<std::decay_t<T>>,
			"error : T must be a numeric type (integer or floating-point). ");
	}
	//------------------------------------------
	// Compile-time check whether T is an arithmetic type (numeric + bool).
	template <typename T>
	constexpr void assert_is_arithmetic(T&&t) {
		static_assert(
			std::is_arithmetic_v<std::decay_t<T>>,
			"error : T must be an arithmetic type. ");
	}
	//------------------------------------------
	//Compile-time check if a type is bool
	template <typename T>
	constexpr void assert_is_bool(T&&t)
	{
		static_assert(
			std::is_same_v<std::decay_t<T>, bool>,
			"error : T must be a bool type. "
			);
	}
	//------------------------------------------
	//Compile-time check whether T is const.
	template <typename T>
	constexpr void assert_is_const(T&&t) {
		static_assert(
			std::is_const_v    <std::remove_reference_t<T>> &&
			!std::is_volatile_v<std::remove_reference_t<T>>,
			"error : T must be const. ");
	}
	//------------------------------------------
	// Compile-time check for whether it is volatile.
	template <typename T>
	constexpr void assert_is_volatile(T&&t)
	{
		static_assert(
			std::is_volatile_v  <std::remove_reference_t<T>>,
			"error : T must be volatile. "
			);
	}
	//------------------------------------------
	//Compile-time check for whether it is const or volatile.
	template <typename T>
	constexpr void assert_is_const_or_volatile(T&&t)
	{
		static_assert(
			std::is_const_v    <std::remove_reference_t<T>> ||
			std::is_volatile_v<std::remove_reference_t<T>>,
			"error : T must be const or volatile.  ");
	}
	//------------------------------------------
	//Compile-time check to determine if it is a pure integer (excluding bool and character types).
	template <typename T>
	constexpr void assert_is_integer_excluding_character(T&&t)
	{
		static_assert(
			std::is_integral_v<std::decay_t<T>> &&
			!std::is_same_v<std::decay_t<T>, char> &&
			!std::is_same_v<std::decay_t<T>, wchar_t> &&
			!std::is_same_v<std::decay_t<T>, char8_t> &&
			!std::is_same_v<std::decay_t<T>, char16_t> &&
			!std::is_same_v<std::decay_t<T>, char32_t> &&
			!std::is_same_v<std::decay_t<T>, bool>,
			"error : T must be an integral type (excluding character and bool). "
			);
	}
	//------------------------------------------
	//Compile-time check for whether it is a pointer type.
	template <typename T>
	constexpr void assert_is_pointer(T&&t)
	{
		static_assert(
			std::is_pointer_v<std::decay_t<T>>,
			"error : T must be a pointer type. "
			);
	}
	//------------------------------------------
	//Compile-time check for null pointer type.
	template <typename T>
	constexpr void assert_is_null_pointer(T&&t)
	{
		static_assert(
			std::is_null_pointer_v<std::decay_t<T>>,
			"error : T must be std::nullptr_t. "
			);
	}
	//------------------------------------------
	//Compile-time check for whether it is a reference type.
	template <typename T>
	constexpr void assert_is_reference(T&&t) {
		static_assert(
			std::is_reference_v<T>,
			"error : T must be a reference type. ");
	}
	//------------------------------------------
	//Compile-time check for whether it is an lvalue reference.
	template <typename T>
	constexpr void assert_is_lvalue_reference(T&&t) {
		static_assert(
			std::is_lvalue_reference_v<T>,
			"error : T must be an lvalue reference. ");
	}
	//------------------------------------------
	//Compile-time check for whether it is an rvalue reference.
	template <typename T>
	constexpr void assert_is_rvalue_reference(T&&t) {
		static_assert(
			std::is_rvalue_reference_v<T>,
			"error : T must be an rvalue reference. ");
	}
	//------------------------------------------
	//Compile-time check for whether it is an array type.
	template <typename T>
	constexpr void assert_is_array(T&&t)
	{
		static_assert(
			std::is_array_v<std::remove_reference_t<T>>,
			"error : T must be an array type. ");
	}
	//------------------------------------------
	//Compile-time check for whether it is a bounded array type.
	template <typename T>
	constexpr void assert_is_bounded_array(T&&t) 
	{
		static_assert(
			std::is_bounded_array_v<std::remove_reference_t<T>>,
			"error : T must be an array with known bounds. ");
	}
	//------------------------------------------
	//Compile_time check for whether it is a unknown bounds array type.
	template <typename T>
	constexpr void assert_is_unbounded_array(T&&t) 
	{
		static_assert(
			std::is_unbounded_array_v<std::remove_reference_t<T>>,
			"error : T must be an array with unknown bounds. ");
	}
	//------------------------------------------
	//Compile-time check for whether it is a class or struct.
	template <typename T>
	constexpr void assert_is_class_or_struct(T&&t)
	{
		static_assert(
			std::is_class_v<std::decay_t<T>>,
			"error : T must be a class type or struct type. "
			);
	}
	//-------------------------------------------
	//Compile-time check for whether it is a union
	template <typename T>
	constexpr void assert_is_union(T&&t)
	{
		static_assert(
			std::is_union_v<std::decay_t<T>>,
			"error : T must be a union type. ");
	}
	//-------------------------------------------
	//Compile-time check for whether it is an enum
	template <typename T>
	constexpr void assert_is_enum(T&&t)
	{
		static_assert(
			std::is_enum_v<std::decay_t<T>>,
			"error : T must be an enumeration type. ");
	}
	//-------------------------------------------
	//Compile-time check whether it is standard-layout type
	template <typename T>
	constexpr void assert_is_standard_layout(T&&t) 
	{
		static_assert(
			std::is_standard_layout_v<std::decay_t<T>>,
			"error : T must be a standard-layout type. ");
	}
	//-------------------------------------------
	//Compile-time check whether it is trivial type
	template <typename T>
	constexpr void assert_is_trivial(T&&t)
	{
		static_assert(
			std::is_trivial_v<std::decay_t<T>>,
			"error : T must be a trivial type. ");
	}
	//-------------------------------------------
	//Compile-time check for function
	template <typename T>
	constexpr void assert_is_function(T &&t)
	{
		static_assert(
			std::is_function_v<std::remove_reference_t<T>>,
			"error : T must be a function type. ");
	}
	//-------------------------------------------
	//Compile-time check for void
	template <typename T>
	constexpr void assert_is_void(T&&t)
	{
		static_assert(
			std::is_void_v<std::decay_t<T>>,
			" error : T must be a void type. "
			);
	}
	//-------------------------------------------
	//Compile-time check for signed type
	template <typename T>
	constexpr void assert_is_signed(T&& t)
	{
		static_assert(
			std::is_signed_v<std::decay_t<T>>,
			"error : T must be a signed type. "
			);
	}
	//------------------------------------------
	//Compile - time check for whether it is an unsigned type
	template <typename T>
	constexpr void assert_is_unsigned(T&& t)
	{
		static_assert(
			std::is_unsigned_v<std::decay_t<T>>,
			"error : T must be a unsignd type. "
			);
	}
}
// 27
