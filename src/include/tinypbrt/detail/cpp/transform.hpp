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
	struct transform_handle {
		/// @brief Native C library type for this struct.
		using native_type  = tpbrt_transform_handle_t;

		/// @brief Type used for reflection.
		using reflect_type = std::tuple<transform_idx_t>;

	private:
		/// @brief C native struct.
		const native_type& _native;

	public:
		/// @brief C to C++ wrapper conversion constructor.
		constexpr transform_handle(const native_type& native) noexcept : _native(native) {} // NOLINT

		/// @brief Returns transform_idx value.
		[[nodiscard]] constexpr transform_idx_t transform_idx() const noexcept { return _native.transform_idx; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept { return std::make_tuple(transform_idx()); }

		/// @brief Conversion operator to native type.
		[[nodiscard]] constexpr operator const native_type&() const noexcept { return _native; } // NOLINT

		/// @brief Conversion operator to pointer to native type.
		[[nodiscard]] constexpr operator const native_type*() const noexcept { return &_native; } // NOLINT

		/// @brief Space ship operator.
		[[nodiscard]] constexpr auto operator<=>(const transform_handle& rhs) const noexcept {
			return transform_idx() <=> rhs.transform_idx();
		}

		/// @brief Equal operator.
		[[nodiscard]] constexpr bool operator==(const transform_handle& rhs) const noexcept {
			return transform_idx() == rhs.transform_idx();
		}
	};

	/**
	 * @brief C++ wrapper for tpbrt_transform_animated_t.
	 * @ingroup transform-cpp-public
	 */
	struct transform_animated {
		/// @brief Native C library type for this struct.
		using native_type  = tpbrt_transform_animated_t;

		/// @brief Type used for reflection.
		using reflect_type = std::tuple<transform_handle, transform_handle>;

	private:
		/// @brief C native struct.
		const native_type& _native;

	public:
		/// @brief C to C++ wrapper conversion constructor.
		constexpr transform_animated(const native_type& native) noexcept : _native(native) {} // NOLINT

		/// @brief Returns start_transform value.
		[[nodiscard]] constexpr transform_handle start_transform() const noexcept { return _native.start_transform; }

		/// @brief Returns end_transform value.
		[[nodiscard]] constexpr transform_handle end_transform() const noexcept { return _native.end_transform; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept {
			return std::make_tuple(start_transform(), end_transform());
		}

		/// @brief Conversion operator to native type.
		[[nodiscard]] constexpr operator const native_type&() const noexcept { return _native; } // NOLINT

		/// @brief Conversion operator to pointer to native type.
		[[nodiscard]] constexpr operator const native_type*() const noexcept { return &_native; } // NOLINT

		/// @brief Space ship operator.
		[[nodiscard]] constexpr auto operator<=>(const transform_animated& rhs) const noexcept {
			return reflect() <=> rhs.reflect();
		}

		/// @brief Equal operator.
		[[nodiscard]] constexpr bool operator==(const transform_animated& rhs) const noexcept {
			return start_transform() == rhs.start_transform() && end_transform() == rhs.end_transform();
		}
	};

	/**
	 * @brief Returns transform to which handle is provided.
	 * @param transforms_list List of transforms in which transform given by handle should be.
	 * @param handle Handle to transform.
	 * @return Returns result with error code (most possible TPBRT_ERROR_NOT_FOUND, TPBRT_ERROR_NONE) or with transform.
	 * @ingroup transform-cpp-public
	 */
	[[nodiscard]] constexpr result<mat4> get_transform(transforms_list transforms_list, transform_handle handle) noexcept;

	/**
	 * @brief C++ wrapper for tpbrt_transforms_list_t.
	 * @ingroup transform-cpp-public
	 */
	struct transforms_list : list<tpbrt_transforms_list_t, mat4, &tpbrt_transforms_list_t::transforms> {
		/// @brief Base list constructor.
		using list::list;

		/// @brief Returns element pointed by given handle.
		[[nodiscard]] constexpr result<element_type> at(const transform_handle& handle) const noexcept {
			return get_transform(*this, handle);
		}

		/// @brief Returns element pointed by given handle.
		[[nodiscard]] constexpr result<element_type> operator[](const transform_handle& handle) const noexcept {
			return at(handle);
		}
	};

	constexpr result<mat4> get_transform(const transforms_list transforms_list, const transform_handle handle) noexcept {
		const tpbrt_mat4_t* c_transform = nullptr;

		const auto err					= static_cast<error>(tpbrt_get_transform(transforms_list, handle, &c_transform));

			if (err == error::NONE && c_transform != nullptr) { return mat4(*c_transform); }

		return err;
	}
} // namespace tpbrt
#endif // _TINYPBRT_TRANSFORM_HPP_
