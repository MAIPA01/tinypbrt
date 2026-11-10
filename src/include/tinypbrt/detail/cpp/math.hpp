#pragma once
#ifndef _TINYPBRT_MATH_HPP_
#define _TINYPBRT_MATH_HPP_

#include <tinypbrt/detail/cpp/fwd.hpp>

#include <tinypbrt/detail/math.h>

#include <algorithm>
#include <optional>

namespace tpbrt {
	/**
	 * @defgroup math-cpp-public Math
	 * @ingroup tinypbrt-public
	 */

	/**
	 * @brief C++ wrapper for tpbrt_opt_float_t.
	 * @ingroup math-cpp-public
	 */
	struct opt_float {
		/// @brief Native C library type for this struct.
		using native_type	= tpbrt_opt_float_t;

		/// @brief std::optional type.
		using optional_type = std::optional<float_t>;

		/// @brief Type used for reflection.
		using reflect_type	= std::tuple<float_t, bool>;

	private:
		/// @brief C native struct.
		const native_type& _native;

	public:
		/// @brief C to C++ wrapper conversion constructor.
		constexpr opt_float(const native_type& native) noexcept : _native(native) {} // NOLINT

		/// @brief Returns value value.
		[[nodiscard]] constexpr float_t value() const noexcept { return _native.value; }

		/// @brief Returns has_value value.
		[[nodiscard]] constexpr bool has_value() const noexcept { return _native.has_value == True; }

		[[nodiscard]] constexpr optional_type to_optional() const noexcept {
				if (!has_value()) { return std::nullopt; }
			return value();
		}

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept { return std::make_tuple(value(), has_value()); }

		/// @brief Conversion operator to native type.
		[[nodiscard]] constexpr operator const native_type&() const noexcept { return _native; } // NOLINT

		/// @brief Conversion operator to pointer to native type.
		[[nodiscard]] constexpr operator const native_type*() const noexcept { return &_native; } // NOLINT

		/// @brief Space ship operator.
		[[nodiscard]] constexpr auto operator<=>(const opt_float& rhs) const noexcept { return reflect() <=> rhs.reflect(); }

		/// @brief Equal operator.
		[[nodiscard]] constexpr bool operator==(const opt_float& rhs) const noexcept {
			return has_value() == rhs.has_value() && value() == rhs.value();
		}
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
	 * @tparam N Number of elements in the vector.
	 */
	template<class NativeVec, size_t N>
	struct vec_base {
		/// @brief Native C library type for this struct.
		using native_type			   = NativeVec;

		/// @brief Vec length.
		static constexpr size_t Length = N;

	protected:
		/// @brief C native struct.
		const native_type& _native;

	public:
		/// @brief C to C++ wrapper conversion constructor.
		constexpr vec_base(const native_type& native) noexcept : _native(native) {} // NOLINT

		/// @brief Returns vec length.
		[[nodiscard]] static constexpr size_t size() noexcept { return Length; }

		/// @brief Returns vec length.
		[[nodiscard]] static constexpr size_t length() noexcept { return Length; }

		/// @brief Conversion to span.
		[[nodiscard]] constexpr std::span<const float_t, Length> to_span() const noexcept {
			return std::span<const float_t, Length>(_native.v);
		}

		/// @brief Returns element at given index.
		[[nodiscard]] constexpr float_t at(const size_t index) const noexcept { return _native.v[index]; }

		/// @brief Conversion operator to native type.
		[[nodiscard]] constexpr operator const native_type&() const noexcept { return _native; } // NOLINT

		/// @brief Conversion operator to pointer to native type.
		[[nodiscard]] constexpr operator const native_type*() const noexcept { return &_native; } // NOLINT

		/// @brief Conversion operator to pointer to array of floats.
		[[nodiscard]] constexpr operator const float_t*() const noexcept { return _native.v; } // NOLINT

		/// @brief Space ship operator.
		[[nodiscard]] constexpr auto operator<=>(const vec_base& rhs) const noexcept {
			auto s1 = to_span();
			auto s2 = rhs.to_span();
			return std::lexicographical_compare_three_way(s1.begin(), s1.end(), s2.begin(), s2.end());
		}

		/// @brief Equal operator.
		[[nodiscard]] constexpr bool operator==(const vec_base& rhs) const noexcept {
			return std::ranges::equal(to_span(), rhs.to_span());
		}

		/// @brief Returns element at given index.
		[[nodiscard]] constexpr float_t operator[](const size_t index) const noexcept { return at(index); }
	};

	/**
	 * @brief C++ wrapper for tpbrt_vec2_t
	 * @ingroup math-cpp-public
	 */
	struct vec2 : vec_base<tpbrt_vec2_t, 2u> {
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
	struct vec3 : vec_base<tpbrt_vec3_t, 3u> {
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
	struct vec4 : vec_base<tpbrt_vec4_t, 4u> {
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
	using vec2_array = array<tpbrt_vec2_array_t, vec2>;

	/**
	 * @brief C++ wrapper for tpbrt_vec3_array_t.
	 * @ingroup math-cpp-public
	 */
	using vec3_array = array<tpbrt_vec3_array_t, vec3>;

	/**
	 * @brief C++ wrapper for tpbrt_mat4_t
	 * @ingroup math-cpp-public
	 */
	struct mat4 {
		/// @brief Native C library type for this struct.
		using native_type					= tpbrt_mat4_t;

		/// @brief Native C library mat4 row type.
		using native_row_type				= tpbrt_mat4_row_t;

		/// @brief Row type.
		using row_type						= vec4;

		/// @brief Row Vec length.
		static constexpr size_t RowLength	= 4u;

		/// @brief Rows count.
		static constexpr size_t RowsCount	= 4u;

		/// @brief Values count.
		static constexpr size_t ValuesCount = RowsCount * RowLength;

	private:
		/// @brief Transforms native_row_type to row_type.
		struct rows_transformer {
			/// @brief Call operator.
			[[nodiscard]] constexpr row_type operator()(const native_row_type& row) const noexcept { return row; }
		};

	public:
		/// @brief Rows view type.
		using rows_view_type = std::ranges::transform_view<std::span<const native_row_type, RowsCount>, rows_transformer>;

		/// @brief Type used for reflection.
		using reflect_type	 = std::tuple<rows_view_type>;

	private:
		/// @brief C native struct.
		const native_type& _native;

	public:
		/// @brief C to C++ wrapper conversion constructor.
		constexpr mat4(const native_type& native) noexcept : _native(native) {} // NOLINT

		/// @brief Returns values count.
		[[nodiscard]] static constexpr size_t size() noexcept { return length(); }

		/// @brief Returns values count.
		[[nodiscard]] static constexpr size_t length() noexcept { return ValuesCount; }

		/// @brief Conversion to values span.
		[[nodiscard]] constexpr std::span<const float_t, ValuesCount> to_values_span() const noexcept {
			return std::span(_native.m);
		}

		/// @brief Conversion to rows span.
		[[nodiscard]] constexpr rows_view_type to_rows_span() const noexcept {
			return std::span(_native.rows) | std::views::transform(rows_transformer {});
		}

		/// @brief Returns element at given index.
		[[nodiscard]] constexpr float_t value_at(const size_t index) const noexcept { return _native.m[index]; }

		/// @brief Returns row at given index.
		[[nodiscard]] constexpr vec4 row_at(const size_t index) const noexcept { return _native.rows[index]; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept { return std::make_tuple(to_rows_span()); }

		/// @brief Conversion operator to native type.
		[[nodiscard]] constexpr operator const native_type&() const noexcept { return _native; } // NOLINT

		/// @brief Conversion operator to pointer to native type.
		[[nodiscard]] constexpr operator const native_type*() const noexcept { return &_native; } // NOLINT

		/// @brief Conversion operator to pointer to array of floats.
		[[nodiscard]] constexpr operator const float_t*() const noexcept { return _native.m; } // NOLINT

		/// @brief Space ship operator.
		[[nodiscard]] constexpr auto operator<=>(const mat4& rhs) const noexcept {
			auto s1 = to_values_span();
			auto s2 = rhs.to_values_span();
			return std::lexicographical_compare_three_way(s1.begin(), s1.end(), s2.begin(), s2.end());
		}

		/// @brief Equal operator.
		[[nodiscard]] constexpr bool operator==(const mat4& rhs) const noexcept {
			return std::ranges::equal(to_values_span(), rhs.to_values_span());
		}

		/// @brief Returns row at given index.
		[[nodiscard]] constexpr vec4 operator[](const size_t index) const noexcept { return row_at(index); }
	};

	/**
	 * @brief C++ wrapper for tpbrt_mat4_animated_t.
	 * @ingroup math-cpp-public
	 */
	struct mat4_animated {
		/// @brief Native C library type for this struct.
		using native_type  = tpbrt_mat4_animated_t;

		/// @brief Type used for reflection.
		using reflect_type = std::tuple<mat4, mat4>;

	private:
		/// @brief C native struct.
		const native_type& _native;

	public:
		/// @brief C to C++ wrapper conversion constructor.
		constexpr mat4_animated(const native_type& native) noexcept : _native(native) {} // NOLINT

		/// @brief Returns start value.
		[[nodiscard]] constexpr mat4 start() const noexcept { return _native.start; }

		/// @brief Returns end value.
		[[nodiscard]] constexpr mat4 end() const noexcept { return _native.end; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept { return std::make_tuple(start(), end()); }

		/// @brief Conversion operator to native type.
		[[nodiscard]] constexpr operator const native_type&() const noexcept { return _native; } // NOLINT

		/// @brief Conversion operator to pointer to native type.
		[[nodiscard]] constexpr operator const native_type*() const noexcept { return &_native; } // NOLINT

		/// @brief Space ship operator.
		[[nodiscard]] constexpr auto operator<=>(const mat4_animated& rhs) const noexcept { return reflect() <=> rhs.reflect(); }

		/// @brief Equal operator.
		[[nodiscard]] constexpr bool operator==(const mat4_animated& rhs) const noexcept {
			return start() == rhs.start() && end() == rhs.end();
		}
	};
} // namespace tpbrt
#endif // _TINYPBRT_MATH_HPP_
