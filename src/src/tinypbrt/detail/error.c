#include <tinypbrt/pch.h>

#include <tinypbrt/detail/error.h>

#include <tinypbrt/detail/common_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

	tpbrt_size_t tpbrt_get_error_message(const tpbrt_error_t error, tpbrt_char_t* const buffer, const tpbrt_size_t buffer_size) {
		static const tpbrt_string_t TPBRT_ERROR_MSGS[TPBRT_ERROR_MAX_NUM + 1] = {
			TPBRT_STRING_INIT("No error"),						  // TPBRT_ERROR_NONE
			TPBRT_STRING_INIT("No tokens"),						  // TPBRT_ERROR_END_OF_FILE
			TPBRT_STRING_INIT("Invalid token"),					  // TPBRT_ERROR_INVALID_TOKEN
			TPBRT_STRING_INIT("Unable to parse float"),			  // TPBRT_ERROR_PARSE_FLOAT
			TPBRT_STRING_INIT("Unable to parse integer"),		  // TPBRT_ERROR_PARSE_INT
			TPBRT_STRING_INIT("Unable to parse bool"),			  // TPBRT_ERROR_PARSE_BOOL
			TPBRT_STRING_INIT("Unsupported directive"),			  // TPBRT_ERROR_INVALID_DIRECTIVE
			TPBRT_STRING_INIT("Expected string token"),			  // TPBRT_ERROR_INVALID_STRING
			TPBRT_STRING_INIT("Unable to parse option value"),	  // TPBRT_ERROR_INVALID_OPTION
			TPBRT_STRING_INIT("Unsupported coordinate system"),	  // TPBRT_ERROR_INVALID_COORD_SYSTEM_TYPE
			TPBRT_STRING_INIT("Invalid parameter name"),		  // TPBRT_ERROR_INVALID_PARAM_NAME
			TPBRT_STRING_INIT("Parameter type is invalid"),		  // TPBRT_ERROR_INVALID_PARAM_TYPE
			TPBRT_STRING_INIT("Found duplicated parameter"),	  // TPBRT_ERROR_DUPLICATE_PARAM_NAME
			TPBRT_STRING_INIT("Duplicated WorldBegin statement"), // TPBRT_ERROR_WORLD_ALREADY_STARTED
			TPBRT_STRING_INIT("Too many AttributeEnd"),			  // TPBRT_ERROR_TOO_MANY_END_ATTRIBUTES
			TPBRT_STRING_INIT(
			  "Parameter value has a wrong form (e.g. a quoted string where numbers are expected)"
			),														   // TPBRT_ERROR_INVALID_OBJECT_TYPE
			TPBRT_STRING_INIT("Unexpected token received"),			   // TPBRT_ERROR_UNEXPECTED_TOKEN
			TPBRT_STRING_INIT("Required param is missing"),			   // TPBRT_ERROR_MISSING_REQUIRED_PARAMETER
			TPBRT_STRING_INIT("Not found"),							   // TPBRT_ERROR_NOT_FOUND
			TPBRT_STRING_INIT("Out of memory"),						   // TPBRT_ERROR_OUT_OF_MEMORY
			TPBRT_STRING_INIT("Invalid pointer"),					   // TPBRT_ERROR_INVALID_POINTER
			TPBRT_STRING_INIT("Invalid film type"),					   // TPBRT_ERROR_INVALID_FILM_TYPE
			TPBRT_STRING_INIT("Unable to parse uint"),				   // TPBRT_ERROR_PARSE_UINT
			TPBRT_STRING_INIT("Unsupported color space"),			   // TPBRT_ERROR_INVALID_COLOR_SPACE
			TPBRT_STRING_INIT("Unsupported spectrum builtin"),		   // TPBRT_ERROR_INVALID_SPECTRUM_BUILTIN
			TPBRT_STRING_INIT("Too many values provided"),			   // TPBRT_ERROR_TOO_MANY_VALUES
			TPBRT_STRING_INIT("Unsupported film sensor"),			   // TPBRT_ERROR_INVALID_FILM_SENSOR
			TPBRT_STRING_INIT("Unsupported camera type"),			   // TPBRT_ERROR_INVALID_CAMERA_TYPE
			TPBRT_STRING_INIT("Unsupported spherical camera mapping"), // TPBRT_ERROR_INVALID_CAMERA_SPHERICAL_MAPPING
			TPBRT_STRING_INIT(
			  "Unsupported realistic camera aperture builtin"
			),														   // TPBRT_ERROR_INVALID_CAMERA_REALISTIC_APERTURE_BUILTIN
			TPBRT_STRING_INIT("Unsupported integrator type"),		   // TPBRT_ERROR_INVALID_INTEGRATOR_TYPE
			TPBRT_STRING_INIT("Unsupported integrator light sampler"), // TPBRT_ERROR_INVALID_INTEGRATOR_LIGHT_SAMPLER
			TPBRT_STRING_INIT("Unsupported accelerator type"),		   // TPBRT_ERROR_INVALID_ACCELERATOR_TYPE
			TPBRT_STRING_INIT("Unsupported bvh accelerator split method"), // TPBRT_ERROR_INVALID_ACCELERATOR_BVH_SPLIT_METHOD
			TPBRT_STRING_INIT("Unsupported sampler type"),				   // TPBRT_ERROR_INVALID_SAMPLER_TYPE
			TPBRT_STRING_INIT("Unsupported sampler random"),			   // TPBRT_ERROR_INVALID_SAMPLER_RANDOM
			TPBRT_STRING_INIT("Unsupported light source type"),			   // TPBRT_ERROR_INVALID_LIGHT_SOURCE_TYPE
			TPBRT_STRING_INIT("Unsupported area light type"),			   // TPBRT_ERROR_INVALID_AREA_LIGHT_TYPE
			TPBRT_STRING_INIT("Unsupported texture type"),				   // TPBRT_ERROR_INVALID_TEXTURE_TYPE
			TPBRT_STRING_INIT("Unsupported texture class"),				   // TPBRT_ERROR_INVALID_TEXTURE_CLASS
			TPBRT_STRING_INIT("Unsupported texture wrap"),				   // TPBRT_ERROR_INVALID_TEXTURE_WRAP
			TPBRT_STRING_INIT("Unsupported texture filter"),			   // TPBRT_ERROR_INVALID_TEXTURE_FILTER
			TPBRT_STRING_INIT("Unsupported texture encoding"),			   // TPBRT_ERROR_INVALID_TEXTURE_ENCODING
			TPBRT_STRING_INIT("Unsupported texture mapping"),			   // TPBRT_ERROR_INVALID_TEXTURE_MAPPING
			TPBRT_STRING_INIT("Texture redefined (same name and type)"),   // TPBRT_ERROR_DUPLICATE_TEXTURE_NAME
			TPBRT_STRING_INIT("Provided handle is invalid"),			   // TPBRT_ERROR_INVALID_HANDLE
			TPBRT_STRING_INIT("Unsupported material type"),				   // TPBRT_ERROR_INVALID_MATERIAL_TYPE
			TPBRT_STRING_INIT("Unable to parse string"),				   // TPBRT_ERROR_PARSE_STRING
			TPBRT_STRING_INIT("Unsupported shape type"),				   // TPBRT_ERROR_INVALID_SHAPE_TYPE
			TPBRT_STRING_INIT("Unsupported shape curve basis"),			   // TPBRT_ERROR_INVALID_SHAPE_CURVE_BASIS
			TPBRT_STRING_INIT("Unsupported shape curve type"),			   // TPBRT_ERROR_INVALID_SHAPE_CURVE_TYPE
			TPBRT_STRING_INIT("Invalid parameter value"),				   // TPBRT_ERROR_INVALID_PARAM_VALUE
			TPBRT_STRING_INIT("Unsupported media type"),				   // TPBRT_ERROR_INVALID_MEDIA_TYPE
			TPBRT_STRING_INIT("File could not be opened or found"),		   // TPBRT_ERROR_FILE_NOT_FOUND
			TPBRT_STRING_INIT("Object (ObjectBegin) redefined"),		   // TPBRT_ERROR_DUPLICATE_OBJECT_NAME
			TPBRT_STRING_INIT("Unsupported filter type"),				   // TPBRT_ERROR_INVALID_FILTER_TYPE
			TPBRT_STRING_INIT("Provided texture is of wrong type"),		   // TPBRT_ERROR_WRONG_TEXTURE_TYPE
			TPBRT_STRING_INIT("Provided coord system name is invalid"),	   // TPBRT_ERROR_INVALID_COORD_SYSTEM_NAME
			TPBRT_STRING_INIT("Index was out of range"),				   // TPBRT_INDEX_OUT_OF_RANGE
			TPBRT_STRING_INIT("Includes depth was greater than maximum depth"), // TPBRT_ERROR_INCLUDES_STACK_OVERFLOW
			TPBRT_STRING_INIT(
			  "Parameter is declared with a different type than expected (e.g. \"string radius\" instead of \"float radius\")"
			),														  // TPBRT_ERROR_PARAM_TYPE_MISMATCH
			TPBRT_STRING_INIT("Named material redefined"),			  // TPBRT_ERROR_DUPLICATE_MATERIAL_NAME
			TPBRT_STRING_INIT("Named medium redefined"),			  // TPBRT_ERROR_DUPLICATE_MEDIUM_NAME
			TPBRT_STRING_INIT("Unsupported scattering builtin"), // TPBRT_ERROR_INVALID_SCATTERING_BUILTIN
			TPBRT_STRING_INIT("Undefined error"),					  // DEFAULT
		};

			if (buffer == TPBRT_NULL || buffer_size == 0) { return 0; }

		const tpbrt_size_t error_idx = TPBRT_MIN(error, TPBRT_ERROR_MAX_NUM);
		tpbrt_size_t size			 = 0;
		const tpbrt_error_t err		 = tpbrt_string_copy_to(TPBRT_ERROR_MSGS[error_idx], buffer, buffer_size, &size);
		return err == TPBRT_ERROR_NONE ? size : 0;
	}

#ifdef __cplusplus
}
#endif
