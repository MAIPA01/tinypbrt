#pragma once
#ifndef _TINYPBRT_COMMON_HPP_
#define _TINYPBRT_COMMON_HPP_

#include <tinypbrt/detail/cpp/fwd.hpp>

#include <ranges>
#include <span>
#include <string_view>

namespace tpbrt {
	/**
	 * @defgroup common-cpp-public Common
	 * @ingroup tinypbrt-cpp-public
	 */

	/**
	 * @brief Converts tpbrt_string_t to std::string_view.
	 * @param string C library string.
	 * @return Converted tpbrt_string_t to std::string_view.
	 * @ingroup common-cpp-public
	 */
	[[nodiscard]] constexpr std::string_view to_string_view(const tpbrt_string_t string) noexcept {
			if (string.data == Null) { return {}; }
		return std::string_view(string.data, string.size);
	}

	/**
	 * @brief Converts tpbrt_*_array_t to std::span<const *>.
	 * @param c_array C library array.
	 * @return Converted tpbrt_*_array_t to std::span<const *>.
	 * @ingroup common-cpp-public
	 */
	[[nodiscard]] constexpr auto to_span(const auto& c_array) noexcept {
		using element_type = std::remove_pointer_t<decltype(c_array.data)>;

			if (c_array.data == Null || c_array.count == 0) { return std::span<const element_type>(); }
		return std::span<const element_type>(c_array.data, c_array.count);
	}

	/**
	 * @brief Converts tpbrt_string_array_t to string_views.
	 * @param c_array C library string array.
	 * @return Converted tpbrt_string_array_t to string_views.
	 * @ingroup common-cpp-public
	 */
	[[nodiscard]] constexpr auto to_string_views(const tpbrt_string_array_t& c_array) noexcept {
		return to_span(c_array) | std::views::transform(to_string_view);
	}
} // namespace tpbrt
#endif // _TINYPBRT_TYPES_HPP_
