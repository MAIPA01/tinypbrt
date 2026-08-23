#include <tinypbrt/pch.h>

#include <tinypbrt/detail/common_internal.h>
#include <tinypbrt/detail/error.h>

#ifdef __cplusplus
extern "C" {
#endif

	tpbrt_size_t tpbrt_get_error_message(tpbrt_error_t error, tpbrt_char_t* const buffer, const tpbrt_size_t buffer_size) {
		static const tpbrt_string_t TPBRT_ERROR_MSGS[] = {
			TPBRT_STRING("No error"),								 // TPBRT_ERROR_NONE
			TPBRT_STRING("No tokens"),								 // TPBRT_ERROR_END_OF_FILE
			TPBRT_STRING("Token expected, got end of stream"),		 // TPBRT_ERROR_NO_TOKEN
			TPBRT_STRING("Failed to read file"),					 // TPBRT_ERROR_IO
			TPBRT_STRING("Invalid token"),							 // TPBRT_ERROR_INVALID_TOKEN
			TPBRT_STRING("Unable to parse float"),					 // TPBRT_ERROR_PARSE_FLOAT
			TPBRT_STRING("Unable to parse integer"),				 // TPBRT_ERROR_PARSE_INT
			TPBRT_STRING("Unable to parse bool"),					 // TPBRT_ERROR_PARSE_BOOL
			TPBRT_STRING("Unexpected number of arguments in array"), // TPBRT_ERROR_SLICE
			TPBRT_STRING("Unsupported directive"),					 // TPBRT_ERROR_UNKNOWN_DIRECTIVE
			TPBRT_STRING("Expected string token"),					 // TPBRT_ERROR_INVALID_STRING
			TPBRT_STRING("Unable to parse option value"),			 // TPBRT_ERROR_INVALID_OPTION_VALUE
			TPBRT_STRING("Unsupported coordinate system"),			 // TPBRT_ERROR_UNKNOWN_COORDINATE_SYSTEM
			TPBRT_STRING("Invalid parameter name"),					 // TPBRT_ERROR_INVALID_PARAM_NAME
			TPBRT_STRING("Parameter type is invalid"),				 // TPBRT_ERROR_INVALID_PARAM_TYPE
			TPBRT_STRING("Found duplicated parameter"),				 // TPBRT_ERROR_DUPLICATE_PARAM_NAME
			TPBRT_STRING("Duplicated WorldBegin statement"),		 // TPBRT_ERROR_WORLD_ALREADY_STARTED
			TPBRT_STRING("Element is not allowed"),					 // TPBRT_ERROR_ELEMENT_NOT_ALLOWED
			TPBRT_STRING("Too many AttributeEnd"),					 // TPBRT_ERROR_TOO_MANY_END_ATTRIBUTES
			TPBRT_STRING("Attempt to restore CoordSysTransform matrix with invalid name"), // TPBRT_ERROR_INVALID_MATRIX_NAME
			TPBRT_STRING("Invalid camera type"),										   // TPBRT_ERROR_INVALID_CAMERA_TYPE
			TPBRT_STRING("Unknown object type"),										   // TPBRT_ERROR_INVALID_OBJECT_TYPE
			TPBRT_STRING("Unexpected token received"),									   // TPBRT_ERROR_UNEXPECTED_TOKEN
			TPBRT_STRING("Required param is missing"),				   // TPBRT_ERROR_MISSING_REQUIRED_PARAMETER
			TPBRT_STRING("Nested object attributes are not allowed"),  // TPBRT_ERROR_NESTED_OBJECTS
			TPBRT_STRING("Not found"),								   // TPBRT_ERROR_NOT_FOUND
			TPBRT_STRING("Out of memory"),							   // TPBRT_ERROR_OUT_OF_MEMORY
			TPBRT_STRING("Invalid pointer"),						   // TPBRT_ERROR_INVALID_POINTER
			TPBRT_STRING("Invalid film type"),						   // TPBRT_ERROR_INVALID_FILM_TYPE
			TPBRT_STRING("Unable to parse uint"),					   // TPBRT_ERROR_PARSE_UINT
			TPBRT_STRING("Unknown color space"),					   // TPBRT_ERROR_UNKNOWN_COLOR_SPACE
			TPBRT_STRING("Unknown spectrum builtin"),				   // TPBRT_ERROR_UNKNOWN_SPECTRUM_BUILTIN
			TPBRT_STRING("Too many values provided"),				   // TPBRT_ERROR_TOO_MANY_VALUES
			TPBRT_STRING("Unknown camera type"),					   // TPBRT_ERROR_UNKNOWN_CAMERA_TYPE
			TPBRT_STRING("Unknown spherical camera mapping"),		   // TPBRT_ERROR_UNKNOWN_CAMERA_SPHERICAL_MAPPING
			TPBRT_STRING("Unknown realistic camera aperture builtin"), // TPBRT_ERROR_UNKNOWN_CAMERA_REALISTIC_APERTURE_BUILTIN
			TPBRT_STRING("Unknown integrator type"),				   // TPBRT_ERROR_UNKNOWN_INTEGRATOR_TYPE
			TPBRT_STRING("Unknown integrator light sampler"),		   // TPBRT_ERROR_UNKNOWN_INTEGRATOR_LIGHT_SAMPLER
			TPBRT_STRING("Unknown accelerator type"),				   // TPBRT_ERROR_UNKNOWN_ACCELERATOR_TYPE
			TPBRT_STRING("Unknown bvh accelerator split method"),	   // TPBRT_ERROR_UNKNOWN_ACCELERATOR_BVH_SPLIT_METHOD
			TPBRT_STRING("Unknown sampler type"),					   // TPBRT_ERROR_UNKNOWN_SAMPLER_TYPE
			TPBRT_STRING("Unknown sampler random"),					   // TPBRT_ERROR_UNKNOWN_SAMPLER_RANDOM
			TPBRT_STRING("Unknown texture type"),					   // TPBRT_ERROR_UNKNOWN_TEXTURE_TYPE
			TPBRT_STRING("Unknown texture class"),					   // TPBRT_ERROR_UNKNOWN_TEXTURE_CLASS
			TPBRT_STRING("Unknown texture wrap"),					   // TPBRT_ERROR_UNKNOWN_TEXTURE_WRAP
			TPBRT_STRING("Unknown texture filter"),					   // TPBRT_ERROR_UNKNOWN_TEXTURE_FILTER
			TPBRT_STRING("Unknown texture encoding"),				   // TPBRT_ERROR_UNKNOWN_TEXTURE_ENCODING
			TPBRT_STRING("Unknown texture mapping"),				   // TPBRT_ERROR_UNKNOWN_TEXTURE_MAPPING
			TPBRT_STRING("Found duplicated texture"),				   // TPBRT_ERROR_DUPLICATE_TEXTURE_NAME
			TPBRT_STRING("Invalid texture type"),					   // TPBRT_ERROR_INVALID_TEXTURE_TYPE
			TPBRT_STRING("Provided handle is invalid"),				   // TPBRT_ERROR_INVALID_HANDLE
			TPBRT_STRING("Unknown material type"),					   // TPBRT_ERROR_UNKNOWN_MATERIAL_TYPE
			TPBRT_STRING("Unable to parse string"),					   // TPBRT_ERROR_PARSE_STRING
			TPBRT_STRING("Unknown shape type"),						   // TPBRT_ERROR_UNKNOWN_SHAPE_TYPE
			TPBRT_STRING("Unknown shape curve basis"),				   // TPBRT_ERROR_UNKNOWN_SHAPE_CURVE_BASIS
			TPBRT_STRING("Unknown shape curve type"),				   // TPBRT_ERROR_UNKNOWN_SHAPE_CURVE_TYPE
			TPBRT_STRING("Invalid parameter value"),				   // TPBRT_ERROR_INVALID_PARAM_VALUE
			TPBRT_STRING("Unknown media type"),						   // TPBRT_ERROR_UNKNOWN_MEDIA_TYPE
			TPBRT_STRING("File could not be opened or found"),		   // TPBRT_ERROR_FILE_NOT_FOUND
			TPBRT_STRING("Found duplicated object"),				   // TPBRT_ERROR_DUPLICATE_OBJECT_NAME
			TPBRT_STRING("Unknown filter type"),					   // TPBRT_ERROR_UNKNOWN_FILTER_TYPE
			TPBRT_STRING("Undefined error"),						   // DEFAULT
		};

			if (buffer == TPBRT_NULL || buffer_size == 0) { return 0; }

		error					= min(error, TPBRT_ERROR_MAX_NUM);
		const tpbrt_size_t size = min(TPBRT_ERROR_MSGS[error].size, buffer_size - 1u);
		memcpy(buffer, TPBRT_ERROR_MSGS[error].data, size);
		buffer[size] = '\0';
		return size;
	}

#ifdef __cplusplus
}
#endif