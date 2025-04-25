#pragma once

#include <boost/archive/basic_archive.hpp>
#include <map>
#include <unordered_map>
#include <vector>

namespace boost {
namespace archive {

namespace detail {

template <typename T> struct is_shared_ptr : std::false_type {};

template <typename T> struct is_shared_ptr<std::shared_ptr<T>> : std::true_type {};

template <typename T> struct is_weak_ptr : std::false_type {};

template <typename T> struct is_weak_ptr<std::weak_ptr<T>> : std::true_type {};

template <typename T> struct is_unique_ptr : std::false_type {};

template <typename T> struct is_unique_ptr<std::unique_ptr<T>> : std::true_type {};

template <typename T> struct is_fixed_size_old_school_array : std::false_type {};

template <typename T, std::size_t N> struct is_fixed_size_old_school_array<T[N]> : std::true_type {
  static constexpr size_t size = N;
};

template <typename T> struct is_fixed_size_array : std::integral_constant<bool, std::is_array<T>::value> {};

template <typename T, std::size_t N> struct is_fixed_size_array<std::array<T, N>> : std::true_type {};

template <typename T> struct is_std_vector : std::false_type {};

template <typename T, typename... OtherTs> struct is_std_vector<std::vector<T, OtherTs...>> : std::true_type {};

template <typename T> struct is_std_map : std::false_type {};

template <typename T, typename... OtherTs> struct is_std_map<std::map<T, OtherTs...>> : std::true_type {};

template <typename T, typename... OtherTs> struct is_std_map<std::unordered_map<T, OtherTs...>> : std::true_type {};

} // namespace detail

} // namespace archive
} // namespace boost
