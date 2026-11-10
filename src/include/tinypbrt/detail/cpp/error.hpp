#pragma once
#ifndef _TINYPBRT_ERROR_HPP_
#define _TINYPBRT_ERROR_HPP_

#include <tinypbrt/detail/cpp/fwd.hpp>

#include <tinypbrt/detail/error.h>

#include <string>

namespace tpbrt {
	/**
	 * @defgroup error-cpp-public Error
	 * @ingroup tinypbrt-cpp-public
	 */

	/**
	 * @copybrief tpbrt_error_t
	 * @ingroup error-cpp-public
	 */
	enum class error : char_t {
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
	 * @brief Returned result type.
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

		/// @brief Type used for reflection.
		using reflect_type = std::tuple<variant_type>;

	private:
		/// @brief Holds either value or error.
		variant_type _data;

	public:
		/// @brief Default constructor.
		constexpr result() noexcept : _data(error::NONE) {}

		/// @brief Conversion constructor for error.
		constexpr result(const error_type err) noexcept : _data(err) {} // NOLINT

		/// @brief Conversion constructor for value.
		constexpr result(const value_type& val) noexcept : _data(val) {} // NOLINT

		/// @brief Conversion constructor for r-value value.
		constexpr result(value_type&& val) noexcept : _data(std::move(val)) {} // NOLINT

		/// @brief Returns true if result has value.
		[[nodiscard]] constexpr bool has_value() const noexcept { return std::holds_alternative<value_type>(_data); }

		/// @brief Returns error code.
		[[nodiscard]] constexpr error_type get_error() const noexcept {
				if (has_value()) { return error::NONE; }
			return std::get<error_type>(_data);
		}

		/// @brief Returns value.
		[[nodiscard]] constexpr const value_type& get_value() const noexcept { return std::get<value_type>(_data); }

		/// @brief Returns value.
		[[nodiscard]] constexpr value_type& get_value() noexcept { return std::get<value_type>(_data); }

		/// @brief Conversion to variant.
		[[nodiscard]] constexpr const variant_type& to_variant() const noexcept { return _data; }

		/// @brief Visits camera parameters.
		constexpr auto visit(const auto& visitor) const noexcept {
			using return_type = decltype(std::visit(visitor, _data));

				if constexpr (std::is_void_v<return_type>) { std::visit(visitor, _data); }
				else { return std::visit(visitor, _data); }
		}

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept { return std::make_tuple(_data); }

		/// @brief Returns value.
		[[nodiscard]] constexpr const value_type& operator*() const noexcept { return get_value(); }

		/// @brief Returns value.
		[[nodiscard]] constexpr value_type& operator*() noexcept { return get_value(); }

		/// @brief Returns pointer to value.
		[[nodiscard]] constexpr const value_type* operator->() const noexcept { return &get_value(); }

		/// @brief Space ship operator.
		[[nodiscard]] constexpr auto operator<=>(const result& rhs) const noexcept { return _data <=> rhs._data; }

		/// @brief Equal operator.
		[[nodiscard]] constexpr bool operator==(const result& rhs) const noexcept { return _data == rhs._data; }
	};

	/**
	 * @brief Writes error message to given buffer.
	 * @param error_code Error code.
	 * @param buffer Pointer to buffer in which error message will be stored.
	 * @param buffer_size Size of buffer.
	 * @return Size of actual message.
	 * @ingroup error-cpp-public
	 */
	[[nodiscard]] constexpr size_t get_error_message(const error error_code, char_t* const buffer, const size_t buffer_size) {
		return tpbrt_get_error_message(static_cast<tpbrt_error_t>(error_code), buffer, buffer_size);
	}

	/**
	 * @brief Returns error message.
	 * @param error_code Error code.
	 * @return String with error message.
	 * @ingroup error-cpp-public
	 */
	[[nodiscard]] constexpr std::string to_string(const error error_code) noexcept {
		constexpr size_t BufferSize = 128;
		char_t buffer[BufferSize];

		const size_t count = get_error_message(error_code, buffer, BufferSize);
		return std::string { buffer, count };
	}
} // namespace tpbrt
#endif // _TINYPBRT_ERROR_HPP_
