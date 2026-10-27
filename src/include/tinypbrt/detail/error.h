#pragma once
#ifndef _TINYPBRT_ERROR_H_
#define _TINYPBRT_ERROR_H_

#include <tinypbrt/detail/fwd.h>

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * @defgroup error-public Error
	 * @ingroup tinypbrt-public
	 */

	/**
	 * @brief Error type (used as result type where success = TPBRT_ERROR_NONE = 0)
	 * @ingroup error-public
	 */
	typedef enum tpbrt_error_t {
		/// @brief No error -> Success.
		TPBRT_ERROR_NONE									  = 0,
		/// @brief End of file.
		TPBRT_ERROR_END_OF_FILE								  = 1,
		/// @brief Invalid token.
		TPBRT_ERROR_INVALID_TOKEN							  = 2,
		/// @brief Error while parsing float.
		TPBRT_ERROR_PARSE_FLOAT								  = 3,
		/// @brief Error while parsing int.
		TPBRT_ERROR_PARSE_INT								  = 4,
		/// @brief Error while parsing bool.
		TPBRT_ERROR_PARSE_BOOL								  = 5,
		/// @brief Invalid directive.
		TPBRT_ERROR_INVALID_DIRECTIVE						  = 6,
		/// @brief Invalid string.
		TPBRT_ERROR_INVALID_STRING							  = 7,
		/// @brief Invalid option.
		TPBRT_ERROR_INVALID_OPTION							  = 8,
		/// @brief Invalid coordinate system type.
		TPBRT_ERROR_INVALID_COORD_SYSTEM_TYPE				  = 9,
		/// @brief Invalid parameter name.
		TPBRT_ERROR_INVALID_PARAM_NAME						  = 10,
		/// @brief Invalid parameter type.
		TPBRT_ERROR_INVALID_PARAM_TYPE						  = 11,
		/// @brief Duplicate parameter name.
		TPBRT_ERROR_DUPLICATE_PARAM_NAME					  = 12,
		/// @brief WorldBegin directive already called.
		TPBRT_ERROR_WORLD_ALREADY_STARTED					  = 13,
		/// @brief Too many AttributeEnd directives called.
		TPBRT_ERROR_TOO_MANY_END_ATTRIBUTES					  = 14,
		/// @brief Invalid object type.
		TPBRT_ERROR_INVALID_OBJECT_TYPE						  = 15,
		/// @brief Unexpected token.
		TPBRT_ERROR_UNEXPECTED_TOKEN						  = 16,
		/// @brief Required parameter is missing.
		TPBRT_ERROR_MISSING_REQUIRED_PARAMETER				  = 17,
		/// @brief Something was not found. For example texture with given name.
		TPBRT_ERROR_NOT_FOUND								  = 18,
		/// @brief Out of memory.
		TPBRT_ERROR_OUT_OF_MEMORY							  = 19,
		/// @brief Provided NULL where it is not allowed.
		TPBRT_ERROR_INVALID_POINTER							  = 20,
		/// @brief Invalid film type.
		TPBRT_ERROR_INVALID_FILM_TYPE						  = 21,
		/// @brief Error while parsing uint.
		TPBRT_ERROR_PARSE_UINT								  = 22,
		/// @brief Invalid color space.
		TPBRT_ERROR_INVALID_COLOR_SPACE						  = 23,
		/// @brief Invalid spectrum builtin.
		TPBRT_ERROR_INVALID_SPECTRUM_BUILTIN				  = 24,
		/// @brief Too many values provided.
		TPBRT_ERROR_TOO_MANY_VALUES							  = 25,
		/// @brief Invalid film sensor.
		TPBRT_ERROR_INVALID_FILM_SENSOR						  = 26,
		/// @brief Invalid camera type.
		TPBRT_ERROR_INVALID_CAMERA_TYPE						  = 27,
		/// @brief Invalid spherical camera mapping.
		TPBRT_ERROR_INVALID_CAMERA_SPHERICAL_MAPPING		  = 28,
		/// @brief Invalid realistic camera aperture builtin.
		TPBRT_ERROR_INVALID_CAMERA_REALISTIC_APERTURE_BUILTIN = 29,
		/// @brief Invalid integrator type.
		TPBRT_ERROR_INVALID_INTEGRATOR_TYPE					  = 30,
		/// @brief Invalid integrator light sampler.
		TPBRT_ERROR_INVALID_INTEGRATOR_LIGHT_SAMPLER		  = 31,
		/// @brief Invalid accelerator type.
		TPBRT_ERROR_INVALID_ACCELERATOR_TYPE				  = 32,
		/// @brief Invalid accelerator bvh split method.
		TPBRT_ERROR_INVALID_ACCELERATOR_BVH_SPLIT_METHOD	  = 33,
		/// @brief Invalid sampler type.
		TPBRT_ERROR_INVALID_SAMPLER_TYPE					  = 34,
		/// @brief Invalid sampler random method.
		TPBRT_ERROR_INVALID_SAMPLER_RANDOM					  = 35,
		/// @brief Invalid light source type.
		TPBRT_ERROR_INVALID_LIGHT_SOURCE_TYPE				  = 36,
		/// @brief Invalid area light type.
		TPBRT_ERROR_INVALID_AREA_LIGHT_TYPE					  = 37,
		/// @brief Invalid texture type.
		TPBRT_ERROR_INVALID_TEXTURE_TYPE					  = 38,
		/// @brief Invalid texture class.
		TPBRT_ERROR_INVALID_TEXTURE_CLASS					  = 39,
		/// @brief Invalid texture wrap.
		TPBRT_ERROR_INVALID_TEXTURE_WRAP					  = 40,
		/// @brief Invalid texture filter.
		TPBRT_ERROR_INVALID_TEXTURE_FILTER					  = 41,
		/// @brief Invalid texture encoding.
		TPBRT_ERROR_INVALID_TEXTURE_ENCODING				  = 42,
		/// @brief Invalid texture mapping.
		TPBRT_ERROR_INVALID_TEXTURE_MAPPING					  = 43,
		/// @brief Duplicate texture name.
		TPBRT_ERROR_DUPLICATE_TEXTURE_NAME					  = 44,
		/// @brief Invalid handle.
		TPBRT_ERROR_INVALID_HANDLE							  = 45,
		/// @brief Invalid material type.
		TPBRT_ERROR_INVALID_MATERIAL_TYPE					  = 46,
		/// @brief Error while parsing string.
		TPBRT_ERROR_PARSE_STRING							  = 47,
		/// @brief Invalid shape type.
		TPBRT_ERROR_INVALID_SHAPE_TYPE						  = 48,
		/// @brief Invalid curve shape basis.
		TPBRT_ERROR_INVALID_SHAPE_CURVE_BASIS				  = 49,
		/// @brief Invalid curve shape type.
		TPBRT_ERROR_INVALID_SHAPE_CURVE_TYPE				  = 50,
		/// @brief Invalid parameter value.
		TPBRT_ERROR_INVALID_PARAM_VALUE						  = 51,
		/// @brief Invalid media type.
		TPBRT_ERROR_INVALID_MEDIA_TYPE						  = 52,
		/// @brief Provided file not found.
		TPBRT_ERROR_FILE_NOT_FOUND							  = 53,
		/// @brief Duplicate object name.
		TPBRT_ERROR_DUPLICATE_OBJECT_NAME					  = 54,
		/// @brief Invalid filter type.
		TPBRT_ERROR_INVALID_FILTER_TYPE						  = 55,
		/// @brief Wrong texture type.
		TPBRT_ERROR_WRONG_TEXTURE_TYPE						  = 56,
		/// @brief Invalid coordinate system name.
		TPBRT_ERROR_INVALID_COORD_SYSTEM_NAME				  = 57,
		/// @brief Index out of range.
		TPBRT_ERROR_INDEX_OUT_OF_RANGE						  = 58,
		/// @brief Includes stack overflown.
		TPBRT_ERROR_INCLUDES_STACK_OVERFLOW					  = 59,
		/// @brief Parameter type mismatch.
		TPBRT_ERROR_PARAM_TYPE_MISMATCH						  = 60,
		/// @brief Duplicate material name.
		TPBRT_ERROR_DUPLICATE_MATERIAL_NAME					  = 61,
		/// @brief Duplicate medium name.
		TPBRT_ERROR_DUPLICATE_MEDIUM_NAME					  = 62,
		/// @brief Invalid scattering builtin.
		TPBRT_ERROR_INVALID_SCATTERING_BUILTIN				  = 63,
		/// @brief Count of available error codes.
		TPBRT_ERROR_MAX_NUM									  = 64,
	} tpbrt_error_t;

	/**
	 * @brief Writes error message to given buffer.
	 * @param error Error code.
	 * @param buffer Pointer to buffer in which error message will be stored.
	 * @param buffer_size Size of buffer.
	 * @return Size of actual message.
	 * @ingroup error-public
	 */
	TPBRT_API tpbrt_size_t tpbrt_get_error_message(tpbrt_error_t error, tpbrt_char_t* buffer, tpbrt_size_t buffer_size);

#ifdef __cplusplus
}
#endif

#endif // _TINYPBRT_ERROR_H_
