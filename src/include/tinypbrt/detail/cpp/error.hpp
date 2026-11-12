#pragma once
#ifndef _TINYPBRT_ERROR_HPP_
#define _TINYPBRT_ERROR_HPP_

#include <tinypbrt/detail/cpp/fwd.hpp>

#include <tinypbrt/detail/error.h>

#include <concepts>
#include <string>
#include <type_traits>
#include <utility>
#include <variant>

namespace tpbrt {
	/**
	 * @defgroup error-cpp-public Error
	 * @ingroup tinypbrt-cpp-public
	 */

	/**
	 * @copybrief tpbrt_error_t
	 * @ingroup error-cpp-public
	 */
	enum class error : uchar_t {
		/// @copybrief TPBRT_ERROR_NONE
		NONE									  = TPBRT_ERROR_NONE,
		/// @copybrief TPBRT_ERROR_END_OF_FILE
		END_OF_FILE								  = TPBRT_ERROR_END_OF_FILE,
		/// @copybrief TPBRT_ERROR_INVALID_TOKEN
		INVALID_TOKEN							  = TPBRT_ERROR_INVALID_TOKEN,
		/// @copybrief TPBRT_ERROR_PARSE_FLOAT
		PARSE_FLOAT								  = TPBRT_ERROR_PARSE_FLOAT,
		/// @copybrief TPBRT_ERROR_PARSE_INT
		PARSE_INT								  = TPBRT_ERROR_PARSE_INT,
		/// @copybrief TPBRT_ERROR_PARSE_BOOL
		PARSE_BOOL								  = TPBRT_ERROR_PARSE_BOOL,
		/// @copybrief TPBRT_ERROR_INVALID_DIRECTIVE
		INVALID_DIRECTIVE						  = TPBRT_ERROR_INVALID_DIRECTIVE,
		/// @copybrief TPBRT_ERROR_INVALID_STRING
		INVALID_STRING							  = TPBRT_ERROR_INVALID_STRING,
		/// @copybrief TPBRT_ERROR_INVALID_OPTION
		INVALID_OPTION							  = TPBRT_ERROR_INVALID_OPTION,
		/// @copybrief TPBRT_ERROR_INVALID_COORD_SYSTEM_TYPE
		INVALID_COORD_SYSTEM_TYPE				  = TPBRT_ERROR_INVALID_COORD_SYSTEM_TYPE,
		/// @copybrief TPBRT_ERROR_INVALID_PARAM_NAME
		INVALID_PARAM_NAME						  = TPBRT_ERROR_INVALID_PARAM_NAME,
		/// @copybrief TPBRT_ERROR_INVALID_PARAM_TYPE
		INVALID_PARAM_TYPE						  = TPBRT_ERROR_INVALID_PARAM_TYPE,
		/// @copybrief TPBRT_ERROR_DUPLICATE_PARAM_NAME
		DUPLICATE_PARAM_NAME					  = TPBRT_ERROR_DUPLICATE_PARAM_NAME,
		/// @copybrief TPBRT_ERROR_WORLD_ALREADY_STARTED
		WORLD_ALREADY_STARTED					  = TPBRT_ERROR_WORLD_ALREADY_STARTED,
		/// @copybrief TPBRT_ERROR_TOO_MANY_END_ATTRIBUTES
		TOO_MANY_END_ATTRIBUTES					  = TPBRT_ERROR_TOO_MANY_END_ATTRIBUTES,
		/// @copybrief TPBRT_ERROR_INVALID_OBJECT_TYPE
		INVALID_OBJECT_TYPE						  = TPBRT_ERROR_INVALID_OBJECT_TYPE,
		/// @copybrief TPBRT_ERROR_UNEXPECTED_TOKEN
		UNEXPECTED_TOKEN						  = TPBRT_ERROR_UNEXPECTED_TOKEN,
		/// @copybrief TPBRT_ERROR_MISSING_REQUIRED_PARAMETER
		MISSING_REQUIRED_PARAMETER				  = TPBRT_ERROR_MISSING_REQUIRED_PARAMETER,
		/// @copybrief TPBRT_ERROR_NOT_FOUND
		NOT_FOUND								  = TPBRT_ERROR_NOT_FOUND,
		/// @copybrief TPBRT_ERROR_OUT_OF_MEMORY
		OUT_OF_MEMORY							  = TPBRT_ERROR_OUT_OF_MEMORY,
		/// @copybrief TPBRT_ERROR_INVALID_POINTER
		INVALID_POINTER							  = TPBRT_ERROR_INVALID_POINTER,
		/// @copybrief TPBRT_ERROR_INVALID_FILM_TYPE
		INVALID_FILM_TYPE						  = TPBRT_ERROR_INVALID_FILM_TYPE,
		/// @copybrief TPBRT_ERROR_PARSE_UINT
		PARSE_UINT								  = TPBRT_ERROR_PARSE_UINT,
		/// @copybrief TPBRT_ERROR_INVALID_COLOR_SPACE
		INVALID_COLOR_SPACE						  = TPBRT_ERROR_INVALID_COLOR_SPACE,
		/// @copybrief TPBRT_ERROR_INVALID_SPECTRUM_BUILTIN
		INVALID_SPECTRUM_BUILTIN				  = TPBRT_ERROR_INVALID_SPECTRUM_BUILTIN,
		/// @copybrief TPBRT_ERROR_TOO_MANY_VALUES
		TOO_MANY_VALUES							  = TPBRT_ERROR_TOO_MANY_VALUES,
		/// @copybrief TPBRT_ERROR_INVALID_FILM_SENSOR
		INVALID_FILM_SENSOR						  = TPBRT_ERROR_INVALID_FILM_SENSOR,
		/// @copybrief TPBRT_ERROR_INVALID_CAMERA_TYPE
		INVALID_CAMERA_TYPE						  = TPBRT_ERROR_INVALID_CAMERA_TYPE,
		/// @copybrief TPBRT_ERROR_INVALID_CAMERA_SPHERICAL_MAPPING
		INVALID_CAMERA_SPHERICAL_MAPPING		  = TPBRT_ERROR_INVALID_CAMERA_SPHERICAL_MAPPING,
		/// @copybrief TPBRT_ERROR_INVALID_CAMERA_REALISTIC_APERTURE_BUILTIN
		INVALID_CAMERA_REALISTIC_APERTURE_BUILTIN = TPBRT_ERROR_INVALID_CAMERA_REALISTIC_APERTURE_BUILTIN,
		/// @copybrief TPBRT_ERROR_INVALID_INTEGRATOR_TYPE
		INVALID_INTEGRATOR_TYPE					  = TPBRT_ERROR_INVALID_INTEGRATOR_TYPE,
		/// @copybrief TPBRT_ERROR_INVALID_INTEGRATOR_LIGHT_SAMPLER
		INVALID_INTEGRATOR_LIGHT_SAMPLER		  = TPBRT_ERROR_INVALID_INTEGRATOR_LIGHT_SAMPLER,
		/// @copybrief TPBRT_ERROR_INVALID_ACCELERATOR_TYPE
		INVALID_ACCELERATOR_TYPE				  = TPBRT_ERROR_INVALID_ACCELERATOR_TYPE,
		/// @copybrief TPBRT_ERROR_INVALID_ACCELERATOR_BVH_SPLIT_METHOD
		INVALID_ACCELERATOR_BVH_SPLIT_METHOD	  = TPBRT_ERROR_INVALID_ACCELERATOR_BVH_SPLIT_METHOD,
		/// @copybrief TPBRT_ERROR_INVALID_SAMPLER_TYPE
		INVALID_SAMPLER_TYPE					  = TPBRT_ERROR_INVALID_SAMPLER_TYPE,
		/// @copybrief TPBRT_ERROR_INVALID_SAMPLER_RANDOM
		INVALID_SAMPLER_RANDOM					  = TPBRT_ERROR_INVALID_SAMPLER_RANDOM,
		/// @copybrief TPBRT_ERROR_INVALID_LIGHT_SOURCE_TYPE
		INVALID_LIGHT_SOURCE_TYPE				  = TPBRT_ERROR_INVALID_LIGHT_SOURCE_TYPE,
		/// @copybrief TPBRT_ERROR_INVALID_AREA_LIGHT_TYPE
		INVALID_AREA_LIGHT_TYPE					  = TPBRT_ERROR_INVALID_AREA_LIGHT_TYPE,
		/// @copybrief TPBRT_ERROR_INVALID_TEXTURE_TYPE
		INVALID_TEXTURE_TYPE					  = TPBRT_ERROR_INVALID_TEXTURE_TYPE,
		/// @copybrief TPBRT_ERROR_INVALID_TEXTURE_CLASS
		INVALID_TEXTURE_CLASS					  = TPBRT_ERROR_INVALID_TEXTURE_CLASS,
		/// @copybrief TPBRT_ERROR_INVALID_TEXTURE_WRAP
		INVALID_TEXTURE_WRAP					  = TPBRT_ERROR_INVALID_TEXTURE_WRAP,
		/// @copybrief TPBRT_ERROR_INVALID_TEXTURE_FILTER
		INVALID_TEXTURE_FILTER					  = TPBRT_ERROR_INVALID_TEXTURE_FILTER,
		/// @copybrief TPBRT_ERROR_INVALID_TEXTURE_ENCODING
		INVALID_TEXTURE_ENCODING				  = TPBRT_ERROR_INVALID_TEXTURE_ENCODING,
		/// @copybrief TPBRT_ERROR_INVALID_TEXTURE_MAPPING
		INVALID_TEXTURE_MAPPING					  = TPBRT_ERROR_INVALID_TEXTURE_MAPPING,
		/// @copybrief TPBRT_ERROR_DUPLICATE_TEXTURE_NAME
		DUPLICATE_TEXTURE_NAME					  = TPBRT_ERROR_DUPLICATE_TEXTURE_NAME,
		/// @copybrief TPBRT_ERROR_INVALID_HANDLE
		INVALID_HANDLE							  = TPBRT_ERROR_INVALID_HANDLE,
		/// @copybrief TPBRT_ERROR_INVALID_MATERIAL_TYPE
		INVALID_MATERIAL_TYPE					  = TPBRT_ERROR_INVALID_MATERIAL_TYPE,
		/// @copybrief TPBRT_ERROR_PARSE_STRING
		PARSE_STRING							  = TPBRT_ERROR_PARSE_STRING,
		/// @copybrief TPBRT_ERROR_INVALID_SHAPE_TYPE
		INVALID_SHAPE_TYPE						  = TPBRT_ERROR_INVALID_SHAPE_TYPE,
		/// @copybrief TPBRT_ERROR_INVALID_SHAPE_CURVE_BASIS
		INVALID_SHAPE_CURVE_BASIS				  = TPBRT_ERROR_INVALID_SHAPE_CURVE_BASIS,
		/// @copybrief TPBRT_ERROR_INVALID_SHAPE_CURVE_TYPE
		INVALID_SHAPE_CURVE_TYPE				  = TPBRT_ERROR_INVALID_SHAPE_CURVE_TYPE,
		/// @copybrief TPBRT_ERROR_INVALID_PARAM_VALUE
		INVALID_PARAM_VALUE						  = TPBRT_ERROR_INVALID_PARAM_VALUE,
		/// @copybrief TPBRT_ERROR_INVALID_MEDIA_TYPE
		INVALID_MEDIA_TYPE						  = TPBRT_ERROR_INVALID_MEDIA_TYPE,
		/// @copybrief TPBRT_ERROR_FILE_NOT_FOUND
		FILE_NOT_FOUND							  = TPBRT_ERROR_FILE_NOT_FOUND,
		/// @copybrief TPBRT_ERROR_DUPLICATE_OBJECT_NAME
		DUPLICATE_OBJECT_NAME					  = TPBRT_ERROR_DUPLICATE_OBJECT_NAME,
		/// @copybrief TPBRT_ERROR_INVALID_FILTER_TYPE
		INVALID_FILTER_TYPE						  = TPBRT_ERROR_INVALID_FILTER_TYPE,
		/// @copybrief TPBRT_ERROR_WRONG_TEXTURE_TYPE
		WRONG_TEXTURE_TYPE						  = TPBRT_ERROR_WRONG_TEXTURE_TYPE,
		/// @copybrief TPBRT_ERROR_INVALID_COORD_SYSTEM_NAME
		INVALID_COORD_SYSTEM_NAME				  = TPBRT_ERROR_INVALID_COORD_SYSTEM_NAME,
		/// @copybrief TPBRT_ERROR_INDEX_OUT_OF_RANGE
		INDEX_OUT_OF_RANGE						  = TPBRT_ERROR_INDEX_OUT_OF_RANGE,
		/// @copybrief TPBRT_ERROR_INCLUDES_STACK_OVERFLOW
		INCLUDES_STACK_OVERFLOW					  = TPBRT_ERROR_INCLUDES_STACK_OVERFLOW,
		/// @copybrief TPBRT_ERROR_PARAM_TYPE_MISMATCH
		PARAM_TYPE_MISMATCH						  = TPBRT_ERROR_PARAM_TYPE_MISMATCH,
		/// @copybrief TPBRT_ERROR_DUPLICATE_MATERIAL_NAME
		DUPLICATE_MATERIAL_NAME					  = TPBRT_ERROR_DUPLICATE_MATERIAL_NAME,
		/// @copybrief TPBRT_ERROR_DUPLICATE_MEDIUM_NAME
		DUPLICATE_MEDIUM_NAME					  = TPBRT_ERROR_DUPLICATE_MEDIUM_NAME,
		/// @copybrief TPBRT_ERROR_INVALID_SCATTERING_BUILTIN
		INVALID_SCATTERING_BUILTIN				  = TPBRT_ERROR_INVALID_SCATTERING_BUILTIN,
		/// @copybrief TPBRT_ERROR_MAX_NUM
		MAX_NUM									  = TPBRT_ERROR_MAX_NUM,
	};

	/**
	 * @brief Result of an operation: either a value or an error code (similar to C++23 std::expected).
	 * @details A result never holds error::NONE: it is either a value or a real error.
	 * @tparam T Type of value returned on success.
	 * @ingroup error-cpp-public
	 */
	template<class T>
	struct result {
		/// @brief Value type.
		using value_type   = T;

		/// @brief Error type.
		using error_type   = error;

		/// @brief Variant type of result.
		using variant_type = std::variant<value_type, error_type>;

	private:
		/// @brief Holds either value or error.
		variant_type _data;

	public:
		/// @brief Conversion constructor for error.
		constexpr result(const error_type err) noexcept : _data(std::in_place_type<error_type>, err) {} // NOLINT

		/// @brief Conversion constructor for value.
		constexpr result(const value_type& val) : _data(std::in_place_type<value_type>, val) {} // NOLINT

		/// @brief Conversion constructor for r-value value.
		constexpr result(value_type&& val) noexcept(std::is_nothrow_move_constructible_v<value_type>) // NOLINT
			: _data(std::in_place_type<value_type>, std::move(val)) {}

		/// @brief Returns true if result has value.
		[[nodiscard]] constexpr bool_t has_value() const noexcept { return std::holds_alternative<value_type>(_data); }

		/// @brief Returns true if result has value.
		[[nodiscard]] constexpr explicit operator bool() const noexcept { return has_value(); }

		/// @brief Returns error code (error::NONE when result has value).
		[[nodiscard]] constexpr error_type get_error() const noexcept {
				if (has_value()) { return error_type::NONE; }
			return *std::get_if<error_type>(&_data);
		}

		/// @brief Returns value (throws std::bad_variant_access when result holds an error).
		[[nodiscard]] constexpr const value_type& get_value() const& { return std::get<value_type>(_data); }

		/// @brief Returns value (throws std::bad_variant_access when result holds an error).
		[[nodiscard]] constexpr value_type& get_value() & { return std::get<value_type>(_data); }

		/// @brief Returns value (throws std::bad_variant_access when result holds an error).
		[[nodiscard]] constexpr value_type&& get_value() && { return std::get<value_type>(std::move(_data)); }

		/// @brief Returns value or given default value when result holds an error.
		template<class U>
		[[nodiscard]] constexpr value_type value_or(U&& default_value) const& {
			return has_value() ? *std::get_if<value_type>(&_data) : static_cast<value_type>(std::forward<U>(default_value));
		}

		/// @brief Conversion to variant.
		[[nodiscard]] constexpr const variant_type& to_variant() const noexcept { return _data; }

		/// @brief Visits value or error.
		template<class Visitor>
		constexpr decltype(auto) visit(Visitor&& visitor) const {
			return std::visit(std::forward<Visitor>(visitor), _data);
		}

		/// @brief Returns value (unchecked, like std::optional).
		[[nodiscard]] constexpr const value_type& operator*() const& noexcept { return *std::get_if<value_type>(&_data); }

		/// @brief Returns value (unchecked, like std::optional).
		[[nodiscard]] constexpr value_type& operator*() & noexcept { return *std::get_if<value_type>(&_data); }

		/// @brief Returns value (unchecked, like std::optional).
		[[nodiscard]] constexpr value_type&& operator*() && noexcept { return std::move(*std::get_if<value_type>(&_data)); }

		/// @brief Returns pointer to value (unchecked, like std::optional).
		[[nodiscard]] constexpr const value_type* operator->() const noexcept { return std::get_if<value_type>(&_data); }

		/// @brief Returns pointer to value (unchecked, like std::optional).
		[[nodiscard]] constexpr value_type* operator->() noexcept { return std::get_if<value_type>(&_data); }

		/// @brief Equality operator.
		[[nodiscard]] constexpr bool_t operator==(const result& rhs) const requires std::equality_comparable<value_type>
		{
			return _data == rhs._data;
		}

		/// @brief Equality operator (true when result holds given error).
		[[nodiscard]] constexpr bool_t operator==(const error_type err) const noexcept { return get_error() == err; }
	};

	/**
	 * @brief Writes error message to given buffer.
	 * @param error_code Error code.
	 * @param buffer Pointer to buffer in which error message will be stored.
	 * @param buffer_size Size of buffer.
	 * @return Size of actual message.
	 * @ingroup error-cpp-public
	 */
	[[nodiscard]] inline size_t get_error_message(const error error_code, char_t* const buffer,
	  const size_t buffer_size) noexcept {
		return tpbrt_get_error_message(static_cast<tpbrt_error_t>(error_code), buffer, buffer_size);
	}

	/**
	 * @brief Returns error message.
	 * @param error_code Error code.
	 * @return String with error message.
	 * @ingroup error-cpp-public
	 */
	[[nodiscard]] inline std::string to_string(const error error_code) {
		constexpr size_t BufferSize = 256;
		char_t buffer[BufferSize];

		const size_t count = get_error_message(error_code, buffer, BufferSize);
		return std::string { buffer, count };
	}
} // namespace tpbrt
#endif // _TINYPBRT_ERROR_HPP_
