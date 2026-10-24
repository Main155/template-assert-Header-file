
//-@Main155
#pragma once

//Suitable for APP development
//Compile-time checked container utility

#include <type_traits>
#include <utility>
#include <string>
#include <string_view>
#include <array>
#include <concepts>
#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <set>
#include <queue>
#include <deque>

namespace lib
{
	//=========================================
	//Check if there are value type.
	template <typename T , typename = void>
	struct has_value_type : std::false_type {};
	template <typename T>
	struct has_value_type<T,std::void_t<typename T::value_type>> : std::true_type {};
	//----------------------------------
	//Check if it is an iterator.
	template <typename T , typename = void>
	struct has_iterator : std::false_type {};
	template <typename T>
	struct has_iterator<T,std::void_t<typename T::iterator>> : std::true_type {};
	//----------------------------------
	//Check if it is an begin()
	template <typename T , typename = void>
	struct has_begin : std::false_type {};
	template <typename T>
	struct has_begin<T, std::void_t<decltype(std::declval<T&>().begin())>> : std::true_type {};
	//----------------------------------
	//Check if it is an end()
	template <typename T , typename = void>
	struct has_end : std::false_type {};
	template <typename T>
	struct has_end<T,std::void_t<decltype(std::declval<T&>().end())>> : std::true_type {};
	//----------------------------------
	//Check if it is an size()
	template <typename T , typename = void>
	struct has_size : std::false_type {};
	template <typename T>
	struct has_size<T, std::void_t<decltype(std::declval<T&>().size())>> : std::true_type {};
	//-----------------------------------
	//Check if it is an empty()
	template <typename T, typename = void>
	struct has_empty : std::false_type {};
	template <typename T>
	struct has_empty<T, std::void_t<decltype(std::declval<T&>().empty())>> : std::true_type {};
	//------------------------------------
	// Check if it is std::array
	template <typename T>
	struct is_std_array : std::false_type {};
	template <typename T , std::size_t N>
	struct  is_std_array <std::array<T , N >> : std::true_type {};
	template <typename T>
	concept std_array_concept = is_std_array <T>::value;
	//------------------------------------
	// Check if it is std::vector
	template <typename T>
	struct is_std_vector : std::false_type {};
	template <typename T,typename... U>
	struct is_std_vector <std::vector<T , U...>> : std::true_type {};
	template <typename T>
	concept std_vector_concept = is_std_vector <T>::value ;
	//------------------------------------
	// Check if it is std::list
	template <typename T>
	struct  is_std_list : std::false_type {};
	template <typename T, typename U>
	struct is_std_list <std::list<T,U>> : std::true_type {};
	template <typename T>
	concept std_list_concept = is_std_list<T>::value;
	//------------------------------------
	// Check if it is std::map
	template <typename T>
	struct is_std_map : std::false_type {};
	template <typename Key, typename T, typename... Rest>
	struct is_std_map<std::map<Key, T, Rest...>> : std::true_type {};
	template <typename T>
	concept std_map_concept = is_std_map<T>::value;
	//------------------------------------
	// Check if it is std::unordered_map
	template <typename T>
	struct is_std_unordered : std::false_type {};
	template <typename T,typename U,typename... Rest>
	struct is_std_unordered<std::unordered_map<T,U, Rest...>> : std::true_type {};
	template <typename T>
	concept std_unordered_concept = is_std_unordered<T>::value;
	//-----------------------------------
	//Check if it is std::set
	template <typename T>
	struct is_std_set : std::false_type {};
	template <typename T, typename... U>
	struct is_std_set<std::set<T, U...>> : std::true_type {};
	template <typename T>
	concept std_set_concept = is_std_set<T>::value;
	//-----------------------------------
	//Check if it is std::queue
	template <typename T>
	struct is_std_queue : std::false_type {};
	template <typename T,typename... U>
	struct is_std_queue<std::queue<T,U...>> : std::true_type {};
	template <typename T>
	concept std_queue_concept = is_std_queue<T>::value;
	//-----------------------------------
	//Check if it is std::deque
	template <typename T>
	struct is_std_deque : std::false_type {};
	template <typename T,typename... U>
	struct is_std_deque<std::deque<T,U...>> : std::true_type {};
	template<typename T>
	concept std_deque_concept = is_std_deque<T>::value;
	//=========================================
	
	//Determine whether it is a container.
	template <typename T>
	struct is_container : std::integral_constant<bool,
		has_value_type<T>::value    &&
		has_iterator  <T>::value    &&
		has_begin     <T>::value    &&
	    has_end       <T>::value    &&
		(has_size<T>::value || has_empty<T>::value)>
	{};

	template <typename T>
	constexpr bool is_container_v = is_container<T>::value;
	//----------------------------------
	//assert
	template <typename T>
	constexpr void assert_is_container(T&&)
	{
		static_assert(
			is_container_v<std::decay_t<T>>,
			"error : T must be a container type(requires value_type, begin(), end(), size()). "
			);
	}
	//----------------------------------
	//Check if it is empty.
	template <typename T>
	constexpr void assert_is_container_strict(T&&) {
		static_assert(
			is_container_v<std::decay_t<T>> &&
			has_empty<std::decay_t<T>>::value,
			"error : T must be a container type with empty() method."
			);
	}
	//--------------------------------
	//Check if it is std::string
	template <typename T>
	constexpr void assert_is_string(T&&)
	{
		static_assert(
			std::is_same_v<std::remove_cvref_t<T>, std::string> ||
			std::is_same_v<std::remove_cvref_t<T>, std::string_view> ||
			std::is_same_v<std::remove_cvref_t<T>, std::wstring> ||
			std::is_same_v<std::remove_cvref_t<T>, std::u16string> ||
			std::is_same_v<std::remove_cvref_t<T>, std::u32string>,
			"error : T must be of type string & string_view...etc "
			);
	}
	//-------------------------------
	//Check if it is std::array
	template <typename T>
	constexpr void assert_is_std_array(T&&)
	{
		static_assert(
			std_array_concept<std::remove_cvref_t<T>>,
			"error : T must be of array container. "
			);
	}
	//-------------------------------
	//Check if it is std::vector
	template <typename T>
	constexpr void assert_is_std_vector(T&&)
	{
		static_assert(
			std_vector_concept<std::remove_cvref_t<T>>,
			"error : T must be of vector container. "
			);
	}
	//--------------------------------
	//Check if it is std::list
	template <typename T>
	constexpr void assert_is_std_list(T&&)
	{
		static_assert(
			std_list_concept<std::remove_cvref_t<T>>,
			"error : T must be of list container. "
			);
	}
	//--------------------------------
	//Check if it is std::map
	template <typename T>
	constexpr void assert_is_std_map(T&&)
	{
		static_assert(
			std_map_concept<std::remove_cvref_t<T>>,
			"error : T must be of map container"
			);
	}
	//--------------------------------
	//Check if it is std::unordered_map
	template <typename T>
	constexpr void assert_is_std_unordered_map(T&&)
	{
		static_assert (
			std_unordered_concept<std::remove_cvref_t<T>>,
			"error : T must be of unordered_map container"
			);
	}
	//--------------------------------
	//Check if it is std::set
	template <typename T>
	constexpr void assert_is_std_set(T&&)
	{
		static_assert(
			std_set_concept<std::remove_cvref_t<T>>,
			"error : T must be of set container"
			);
	}
	//---------------------------------
	//Check if it is std::queue
	template <typename T>
	constexpr void assert_is_std_queue(T&&)
	{
		static_assert (
			std_queue_concept<std::remove_cvref_t<T>>,
			"error : T must be of queue container"
			);
	}
	//----------------------------------
	//Check if it is std::deque
	template <typename T>
	constexpr void assert_is_std_deque(T&&)
	{
		static_assert(
			std_deque_concept<std::remove_cvref_t<T>>,
			"error : T must be of deque container"
			);
	}
}
//35
