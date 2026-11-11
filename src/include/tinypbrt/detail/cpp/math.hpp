#pragma once
#ifndef _TINYPBRT_MATH_HPP_
#define _TINYPBRT_MATH_HPP_

#include <tinypbrt/detail/cpp/fwd.hpp>

#include <tinypbrt/detail/math.h>

#include <algorithm>
#include <optional>

#include <tinypbrt/detail/cpp/common.hpp>

namespace tpbrt {
	/**
	 * @defgroup math-cpp-public Math
	 * @ingroup tinypbrt-public
	 */

	/**
	 * @brief C++ wrapper for tpbrt_opt_float_t.
	 * @ingroup math-cpp-public
	 */
	struct opt_float : wrapper<opt_float, tpbrt_opt_float_t> {
		/// @brief std::optional type.
		using optional_type = std::optional<float_t>;

		/// @brief Type used for reflection.
		using reflect_type	= std::tuple<float_t, bool_t>;

		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns value value.
		[[nodiscard]] constexpr float_t value() const noexcept { return _native.value; }

		/// @brief Returns has_value value.
		[[nodiscard]] constexpr bool_t has_value() const noexcept { return _native.has_value == True; }

		[[nodiscard]] constexpr optional_type to_optional() const noexcept {
				if (!has_value()) { return std::nullopt; }
			return value();
		}

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept { return std::make_tuple(value(), has_value()); }
	};

	/**
	 * @brief C++ wrapper for tpbrt_bool_array_t.
	 * @ingroup math-cpp-public
	 */
	using bool_array  = array<tpbrt_bool_array_t, bool_t>;

	/**
	 * @brief C++ wrapper for tpbrt_int_array_t.
	 * @ingroup math-cpp-public
	 */
	using int_array	  = array<tpbrt_int_array_t, int_t>;

	/**
	 * @brief C++ wrapper for tpbrt_uint_array_t.
	 * @ingroup math-cpp-public
	 */
	using uint_array  = array<tpbrt_uint_array_t, uint_t>;

	/**
	 * @brief C++ wrapper for tpbrt_float_array_t.
	 * @ingroup math-cpp-public
	 */
	using float_array = array<tpbrt_float_array_t, float_t>;

	/**
	 * @brief Generic C++ wrapper for all tpbrt_vec*_t types.
	 * @tparam NativeVec Type of the C struct.
	 */
	template<class NativeVec>
	struct vec_base : array<NativeVec, float_t, &NativeVec::v> {
		/// @brief Vec length.
		static constexpr size_t Length = array<NativeVec, float_t, &NativeVec::v>::Extent;

		/// @brief Default array constructor.
		using array<NativeVec, float_t, &NativeVec::v>::array;

	protected:
		/// @brief C native struct.
		using array<NativeVec, float_t, &NativeVec::v>::_native;

	public:
		/// @brief Returns vec length.
		[[nodiscard]] static constexpr size_t length() noexcept { return array<NativeVec, float_t, &NativeVec::v>::size(); }
	};

	/**
	 * @brief C++ wrapper for tpbrt_vec2_t
	 * @ingroup math-cpp-public
	 */
	struct vec2 : vec_base<tpbrt_vec2_t> {
		/// @brief Type used for reflection.
		using reflect_type = std::tuple<float_t, float_t>;

		/// @brief Base conversion constructor.
		using vec_base::vec_base;

		/// @brief Returns x value.
		[[nodiscard]] constexpr float_t x() const noexcept { return _native.x; }

		/// @brief Returns r value.
		[[nodiscard]] constexpr float_t r() const noexcept { return _native.r; }

		/// @brief Returns y value.
		[[nodiscard]] constexpr float_t y() const noexcept { return _native.y; }

		/// @brief Returns g value.
		[[nodiscard]] constexpr float_t g() const noexcept { return _native.g; }

		/// @brief Conversion operator to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept { return std::make_tuple(x(), y()); }
	};

	/**
	 * @brief C++ wrapper for tpbrt_vec3_t
	 * @ingroup math-cpp-public
	 */
	struct vec3 : vec_base<tpbrt_vec3_t> {
		/// @brief Type used for reflection.
		using reflect_type = std::tuple<float_t, float_t, float_t>;

		/// @brief Base conversion constructor.
		using vec_base::vec_base;

		/// @brief Returns x value.
		[[nodiscard]] constexpr float_t x() const noexcept { return _native.x; }

		/// @brief Returns r value.
		[[nodiscard]] constexpr float_t r() const noexcept { return _native.r; }

		/// @brief Returns y value.
		[[nodiscard]] constexpr float_t y() const noexcept { return _native.y; }

		/// @brief Returns g value.
		[[nodiscard]] constexpr float_t g() const noexcept { return _native.g; }

		/// @brief Returns z value.
		[[nodiscard]] constexpr float_t z() const noexcept { return _native.z; }

		/// @brief Returns b value.
		[[nodiscard]] constexpr float_t b() const noexcept { return _native.b; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept { return std::make_tuple(x(), y(), z()); }
	};

	/**
	 * @brief C++ wrapper for tpbrt_vec4_t
	 * @ingroup math-cpp-public
	 */
	struct vec4 : vec_base<tpbrt_vec4_t> {
		/// @brief Type used for reflection.
		using reflect_type = std::tuple<float_t, float_t, float_t, float_t>;

		/// @brief Base conversion constructor.
		using vec_base::vec_base;

		/// @brief Returns x value.
		[[nodiscard]] constexpr float_t x() const noexcept { return _native.x; }

		/// @brief Returns r value.
		[[nodiscard]] constexpr float_t r() const noexcept { return _native.r; }

		/// @brief Returns y value.
		[[nodiscard]] constexpr float_t y() const noexcept { return _native.y; }

		/// @brief Returns g value.
		[[nodiscard]] constexpr float_t g() const noexcept { return _native.g; }

		/// @brief Returns z value.
		[[nodiscard]] constexpr float_t z() const noexcept { return _native.z; }

		/// @brief Returns b value.
		[[nodiscard]] constexpr float_t b() const noexcept { return _native.b; }

		/// @brief Returns w value.
		[[nodiscard]] constexpr float_t w() const noexcept { return _native.w; }

		/// @brief Returns a value.
		[[nodiscard]] constexpr float_t a() const noexcept { return _native.a; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept { return std::make_tuple(x(), y(), z(), w()); }
	};

	/**
	 * @brief C++ wrapper for tpbrt_vec2_array_t.
	 * @ingroup math-cpp-public
	 */
	using vec2_array	= array<tpbrt_vec2_array_t, vec2>;

	/**
	 * @brief C++ wrapper for tpbrt_point2_array_t
	 * @ingroup math-cpp-public
	 */
	using point2_array	= vec2_array;

	/**
	 * @brief C++ wrapper for tpbrt_vec3_array_t.
	 * @ingroup math-cpp-public
	 */
	using vec3_array	= array<tpbrt_vec3_array_t, vec3>;

	/**
	 * @brief C++ wrapper for tpbrt_vec_array_t
	 * @ingroup math-cpp-public
	 */
	using vec_array		= vec3_array;
	/**
	 * @brief C++ wrapper for tpbrt_point3_array_t
	 * @ingroup math-cpp-public
	 */
	using point3_array	= vec3_array;
	/**
	 * @brief C++ wrapper for tpbrt_point_array_t
	 * @ingroup math-cpp-public
	 */
	using point_array	= point3_array;
	/**
	 * @brief C++ wrapper for tpbrt_normal3_array_t
	 * @ingroup math-cpp-public
	 */
	using normal3_array = vec3_array;
	/**
	 * @brief C++ wrapper for tpbrt_normal_array_t
	 * @ingroup math-cpp-public
	 */
	using normal_array	= normal3_array;

	/**
	 * @brief C++ wrapper for tpbrt_mat4_t
	 * @ingroup math-cpp-public
	 */
	struct mat4 : array<tpbrt_mat4_t, vec4, &tpbrt_mat4_t::rows> {
		/// @brief Row Vec length.
		static constexpr size_t RowLength	= 4u;

		/// @brief Rows count.
		static constexpr size_t RowsCount	= Extent;

		/// @brief Values count.
		static constexpr size_t ValuesCount = RowsCount * RowLength;

		/// @brief Default array constructor.
		using array::array;

		/// @brief Returns values count.
		[[nodiscard]] constexpr size_t length() const noexcept { return size(); }

		/// @brief Conversion to values span.
		[[nodiscard]] constexpr std::span<const float_t, ValuesCount> to_values_span() const noexcept {
			return std::span(_native.m);
		}

		/// @brief Returns element at given index.
		[[nodiscard]] constexpr float_t value_at(const size_t index) const noexcept { return _native.m[index]; }

		/// @brief Conversion operator to pointer to array of floats.
		[[nodiscard]] constexpr operator const float_t*() const noexcept { return _native.m; } // NOLINT
	};

	/**
	 * @brief C++ wrapper for tpbrt_mat4_animated_t.
	 * @ingroup math-cpp-public
	 */
	struct mat4_animated : wrapper<mat4_animated, tpbrt_mat4_animated_t> {
		/// @brief Type used for reflection.
		using reflect_type = std::tuple<mat4, mat4>;

		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns start value.
		[[nodiscard]] constexpr mat4 start() const noexcept { return _native.start; }

		/// @brief Returns end value.
		[[nodiscard]] constexpr mat4 end() const noexcept { return _native.end; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept { return std::make_tuple(start(), end()); }
	};
} // namespace tpbrt
#endif // _TINYPBRT_MATH_HPP_
