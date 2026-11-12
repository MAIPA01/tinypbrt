#pragma once
#ifndef _TINYPBRT_TRANSFORM_HPP_
#define _TINYPBRT_TRANSFORM_HPP_

#include <tinypbrt/detail/cpp/fwd.hpp>

#include <tinypbrt/detail/transform.h>

#include <tuple>

#include <tinypbrt/detail/cpp/common.hpp>
#include <tinypbrt/detail/cpp/math.hpp>

namespace tpbrt {
	/**
	 * @defgroup transform-cpp-public Transform
	 * @ingroup tinypbrt-cpp-public
	 */

	/**
	 * @brief C++ wrapper for tpbrt_transform_handle_t.
	 * @ingroup transform-cpp-public
	 */
	struct transform_handle : detail::wrapper<transform_handle, tpbrt_transform_handle_t> {
		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns transform_idx value.
		[[nodiscard]] constexpr transform_idx_t transform_idx() const noexcept { return _native->transform_idx; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr auto reflect() const noexcept { return std::make_tuple(transform_idx()); }
	};

	/**
	 * @brief C++ wrapper for tpbrt_transform_animated_t.
	 * @ingroup transform-cpp-public
	 */
	struct transform_animated : detail::wrapper<transform_animated, tpbrt_transform_animated_t> {
		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns start_transform value.
		[[nodiscard]] constexpr transform_handle start_transform() const noexcept { return _native->start_transform; }

		/// @brief Returns end_transform value.
		[[nodiscard]] constexpr transform_handle end_transform() const noexcept { return _native->end_transform; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr auto reflect() const noexcept { return std::make_tuple(start_transform(), end_transform()); }
	};

	/**
	 * @brief C++ wrapper for tpbrt_transforms_list_t.
	 * @details Index access (operator[], at(size_t), iteration) works as in tpbrt::array, at(handle) use
	 * the C lookups and return error::NOT_FOUND (or the C error code) when there is no such transform.
	 * @ingroup transform-cpp-public
	 */
	struct transforms_list : array<tpbrt_transforms_list_t, mat4, &tpbrt_transforms_list_t::transforms> {
		/// @brief Base list constructor.
		using array::array;

		/// @brief Index based access from tpbrt::array.
		using array::at;

		/// @brief Operator for index based access from tpbrt::array.
		using array::operator[];

		/// @brief Returns transform pointed by given handle.
		[[nodiscard]] result<element_type> at(const transform_handle handle) const noexcept {
			return detail::find_in<element_type, tpbrt_mat4_t>(tpbrt_get_transform, _native, handle.native());
		}

		/// @brief Returns transform pointed by given handle.
		[[nodiscard]] result<element_type> operator[](const transform_handle handle) const noexcept { return at(handle); }
	};

	/**
	 * @brief Returns transform to which handle is provided.
	 * @param list List of transforms in which transform given by handle should be.
	 * @param handle Handle to transform.
	 * @return Result with the transform or with error code (most likely error::NOT_FOUND).
	 * @ingroup transform-cpp-public
	 */
	[[nodiscard]] inline result<mat4> get_transform(const transforms_list list, const transform_handle handle) noexcept {
		return list.at(handle);
	}
} // namespace tpbrt
#endif // _TINYPBRT_TRANSFORM_HPP_
