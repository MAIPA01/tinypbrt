#include <tinypbrt/pch.h>

#include <tinypbrt/detail/light_internal.h>
#include <tinypbrt/detail/name_index_internal.h>

#include <tinypbrt/detail/color_internal.h>
#include <tinypbrt/detail/common_internal.h>
#include <tinypbrt/detail/math_internal.h>
#include <tinypbrt/detail/param_internal.h>

#pragma region LIGHT_SOURCE

tpbrt_error_t tpbrt_light_source_type_from_string(const tpbrt_string_t type_str, tpbrt_light_source_type_t* const type) {
	static const tpbrt_string_t TypesStrs[TPBRT_LIGHT_SOURCE_TYPE_MAX_NUM] = {
		TPBRT_STRING_INIT("distant"),
		TPBRT_STRING_INIT("goniometric"),
		TPBRT_STRING_INIT("infinite"),
		TPBRT_STRING_INIT("point"),
		TPBRT_STRING_INIT("projection"),
		TPBRT_STRING_INIT("spot"),
	};

		if (type == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		if (tpbrt_string_empty(type_str) == TPBRT_TRUE) { return TPBRT_ERROR_INVALID_LIGHT_SOURCE_TYPE; }

		for (tpbrt_light_source_type_t t = 0; t < TPBRT_LIGHT_SOURCE_TYPE_MAX_NUM; ++t) {
				if (tpbrt_string_equals(type_str, TypesStrs[t]) == TPBRT_TRUE) {
					*type = t;
					return TPBRT_ERROR_NONE;
				}
		}

	return TPBRT_ERROR_INVALID_LIGHT_SOURCE_TYPE;
}

/**
 * @brief Reads distant parameters.
 * @param distant Distant data.
 * @param params Parameters list.
 * @param current_color_space_illuminant Current color space illuminant.
 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
 * @ingroup light-private
 */
static tpbrt_error_t tpbrt_light_source_read_distant(tpbrt_light_source_distant_params_t* const distant,
  const tpbrt_params_list_t* const params, const tpbrt_spectrum_builtin_t current_color_space_illuminant) {
		if (distant == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	static const tpbrt_string_t LStr	  = TPBRT_STRING_INIT("L");
	static const tpbrt_string_t FromStr	  = TPBRT_STRING_INIT("from");
	static const tpbrt_string_t ToStr	  = TPBRT_STRING_INIT("to");

	const tpbrt_spectrum_t l_default	  = TPBRT_SPECTRUM_BUILTIN_INIT(current_color_space_illuminant);
	static const tpbrt_vec3_t FromDefault = TPBRT_VEC3_INIT(0.0f, 0.0f, 0.0f);
	static const tpbrt_vec3_t ToDefault	  = TPBRT_VEC3_INIT(0.0f, 0.0f, 1.0f);

	tpbrt_error_t err					  = tpbrt_params_list_get_spectrum(params, LStr, &l_default, &distant->l);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_point(params, FromStr, FromDefault, &distant->from);
		if (err != TPBRT_ERROR_NONE) { return err; }

	return tpbrt_params_list_get_point(params, ToStr, ToDefault, &distant->to);
}

/**
 * @brief Reads goniometric parameters.
 * @param goniometric Goniometric data.
 * @param params Parameters list.
 * @param current_color_space_illuminant Current color space illuminant.
 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
 * @ingroup light-private
 */
static tpbrt_error_t tpbrt_light_source_read_goniometric(tpbrt_light_source_goniometric_params_t* const goniometric,
  const tpbrt_params_list_t* const params, const tpbrt_spectrum_builtin_t current_color_space_illuminant) {
		if (goniometric == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	static const tpbrt_string_t FileNameStr = TPBRT_STRING_INIT("filename");
	static const tpbrt_string_t IStr		= TPBRT_STRING_INIT("I");

	const tpbrt_spectrum_t i_default		= TPBRT_SPECTRUM_BUILTIN_INIT(current_color_space_illuminant);

	const tpbrt_error_t err					= tpbrt_params_list_get_string_required(params, FileNameStr, &goniometric->file_name);
		if (err != TPBRT_ERROR_NONE) { return err; }

	return tpbrt_params_list_get_spectrum(params, IStr, &i_default, &goniometric->i);
}

/**
 * @brief Reads infinite parameters.
 * @param infinite Infinite data.
 * @param params Parameters list.
 * @param current_color_space_illuminant Current color space illuminant.
 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
 * @ingroup light-private
 */
static tpbrt_error_t tpbrt_light_source_read_infinite(tpbrt_light_source_infinite_params_t* const infinite,
  const tpbrt_params_list_t* const params, const tpbrt_spectrum_builtin_t current_color_space_illuminant) {
		if (infinite == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	static const tpbrt_string_t FileNameStr		= TPBRT_STRING_INIT("filename");
	static const tpbrt_string_t PortalStr		= TPBRT_STRING_INIT("portal");
	static const tpbrt_string_t LStr			= TPBRT_STRING_INIT("L");

	static const tpbrt_string_t FileNameDefault = TPBRT_EMPTY_STRING_INIT;
	const tpbrt_spectrum_t l_default			= TPBRT_SPECTRUM_BUILTIN_INIT(current_color_space_illuminant);

	tpbrt_error_t err = tpbrt_params_list_get_string(params, FileNameStr, FileNameDefault, &infinite->file_name);
		if (err != TPBRT_ERROR_NONE) { return err; }

	tpbrt_point3_array_t points;
	err = tpbrt_params_list_get_point3s_of_size(params, PortalStr, 4u, &points);
		if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

		if (err == TPBRT_ERROR_NONE) {
			memcpy(infinite->portal, points.data, sizeof(tpbrt_point3_t) * 4u);
			tpbrt_vec3_array_free(&points);
		}

	return tpbrt_params_list_get_spectrum(params, LStr, &l_default, &infinite->l);
}

/**
 * @brief Reads point parameters.
 * @param point Point data.
 * @param params Parameters list.
 * @param current_color_space_illuminant Current color space illuminant.
 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
 * @ingroup light-private
 */
static tpbrt_error_t tpbrt_light_source_read_point(tpbrt_light_source_point_params_t* const point,
  const tpbrt_params_list_t* const params, const tpbrt_spectrum_builtin_t current_color_space_illuminant) {
		if (point == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	static const tpbrt_string_t IStr	  = TPBRT_STRING_INIT("I");
	static const tpbrt_string_t FromStr	  = TPBRT_STRING_INIT("from");

	const tpbrt_spectrum_t i_default	  = TPBRT_SPECTRUM_BUILTIN_INIT(current_color_space_illuminant);
	static const tpbrt_vec3_t FromDefault = TPBRT_VEC3_INIT(0.0f, 0.0f, 0.0f);

	const tpbrt_error_t err				  = tpbrt_params_list_get_spectrum(params, IStr, &i_default, &point->i);
		if (err != TPBRT_ERROR_NONE) { return err; }

	return tpbrt_params_list_get_point(params, FromStr, FromDefault, &point->from);
}

/**
 * @brief Reads projection parameters.
 * @param projection Projection data.
 * @param params Parameters list.
 * @param current_color_space_illuminant Current color space illuminant.
 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
 * @ingroup light-private
 */
static tpbrt_error_t tpbrt_light_source_read_projection(tpbrt_light_source_projection_params_t* const projection,
  const tpbrt_params_list_t* const params, const tpbrt_spectrum_builtin_t current_color_space_illuminant) {
		if (projection == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	static const tpbrt_string_t IStr		= TPBRT_STRING_INIT("I");
	static const tpbrt_string_t FovStr		= TPBRT_STRING_INIT("fov");
	static const tpbrt_string_t FileNameStr = TPBRT_STRING_INIT("filename");

	const tpbrt_spectrum_t i_default		= TPBRT_SPECTRUM_BUILTIN_INIT(current_color_space_illuminant);
	static const tpbrt_float_t FovDefault	= 90.0f;

	tpbrt_error_t err						= tpbrt_params_list_get_spectrum(params, IStr, &i_default, &projection->i);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_float(params, FovStr, FovDefault, &projection->fov);
		if (err != TPBRT_ERROR_NONE) { return err; }

	return tpbrt_params_list_get_string_required(params, FileNameStr, &projection->file_name);
}

/**
 * @brief Reads spot parameters.
 * @param spot Spot data.
 * @param params Parameters list.
 * @param current_color_space_illuminant Current color space illuminant.
 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
 * @ingroup light-private
 */
static tpbrt_error_t tpbrt_light_source_read_spot(tpbrt_light_source_spot_params_t* const spot,
  const tpbrt_params_list_t* const params, const tpbrt_spectrum_builtin_t current_color_space_illuminant) {
		if (spot == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	static const tpbrt_string_t IStr				 = TPBRT_STRING_INIT("I");
	static const tpbrt_string_t FromStr				 = TPBRT_STRING_INIT("from");
	static const tpbrt_string_t ToStr				 = TPBRT_STRING_INIT("to");
	static const tpbrt_string_t ConeAngleStr		 = TPBRT_STRING_INIT("coneangle");
	static const tpbrt_string_t ConeDeltaAngleStr	 = TPBRT_STRING_INIT("conedeltaangle");

	const tpbrt_spectrum_t i_default				 = TPBRT_SPECTRUM_BUILTIN_INIT(current_color_space_illuminant);
	static const tpbrt_vec3_t FromDefault			 = TPBRT_VEC3_INIT(0.0f, 0.0f, 0.0f);
	static const tpbrt_vec3_t ToDefault				 = TPBRT_VEC3_INIT(0.0f, 0.0f, 1.0f);
	static const tpbrt_float_t ConeAngleDefault		 = 30.0f;
	static const tpbrt_float_t ConeDeltaAngleDefault = 5.0f;

	tpbrt_error_t err								 = tpbrt_params_list_get_spectrum(params, IStr, &i_default, &spot->i);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_point(params, FromStr, FromDefault, &spot->from);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_point(params, ToStr, ToDefault, &spot->to);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_float(params, ConeAngleStr, ConeAngleDefault, &spot->cone_angle);
		if (err != TPBRT_ERROR_NONE) { return err; }

	return tpbrt_params_list_get_float(params, ConeDeltaAngleStr, ConeDeltaAngleDefault, &spot->cone_delta_angle);
}

tpbrt_error_t tpbrt_light_source_init(const tpbrt_string_t type_str, const tpbrt_params_list_t* const params,
  const tpbrt_color_space_t color_space, const tpbrt_transform_handle_t transform,
  const tpbrt_media_handle_t interior_media_handle, const tpbrt_media_handle_t exterior_media_handle,
  tpbrt_light_source_t* const light_source) {
		if (light_source == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		if (memset(light_source, 0, sizeof(tpbrt_light_source_t)) == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	light_source->transform				= transform;
	light_source->interior_media_handle = interior_media_handle;
	light_source->exterior_media_handle = exterior_media_handle;

	tpbrt_light_source_type_t light_type;
	tpbrt_error_t err = tpbrt_light_source_type_from_string(type_str, &light_type);
		if (err != TPBRT_ERROR_NONE) {
			tpbrt_light_source_free(light_source);
			return err;
		}
	light_source->type						   = light_type;

	static const tpbrt_string_t PowerStr	   = TPBRT_STRING_INIT("power");
	static const tpbrt_string_t IlluminanceStr = TPBRT_STRING_INIT("illuminance");
	static const tpbrt_string_t ScaleStr	   = TPBRT_STRING_INIT("scale");

	static const tpbrt_float_t ScaleDefault	   = 1.0f;

	err = tpbrt_params_list_get_opt_float(params, PowerStr, &light_source->power_illuminance);
		if (err != TPBRT_ERROR_NONE) {
			tpbrt_light_source_free(light_source);
			return err;
		}

		if (light_source->power_illuminance.has_value == TPBRT_FALSE) {
			err = tpbrt_params_list_get_opt_float(params, IlluminanceStr, &light_source->power_illuminance);
				if (err != TPBRT_ERROR_NONE) {
					tpbrt_light_source_free(light_source);
					return err;
				}
		}

	err = tpbrt_params_list_get_float(params, ScaleStr, ScaleDefault, &light_source->scale);
		if (err != TPBRT_ERROR_NONE) {
			tpbrt_light_source_free(light_source);
			return err;
		}

	tpbrt_spectrum_builtin_t current_color_space_illuminant;
		switch (color_space) {
			case TPBRT_COLOR_SPACE_ACES_2065_1: {
				current_color_space_illuminant = TPBRT_SPECTRUM_BUILTIN_ILLUM_ACES_D60;
				break;
			}
		case TPBRT_COLOR_SPACE_REC_2020:
		case TPBRT_COLOR_SPACE_DCI_P3:
		default:
			case TPBRT_COLOR_SPACE_SRGB: {
				current_color_space_illuminant = TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_D65;
				break;
			}
		}

		switch (light_source->type) {
		default:
			case TPBRT_LIGHT_SOURCE_TYPE_DISTANT: {
				err = tpbrt_light_source_read_distant(&light_source->as.distant, params, current_color_space_illuminant);
				break;
			}
			case TPBRT_LIGHT_SOURCE_TYPE_GONIOMETRIC: {
				err = tpbrt_light_source_read_goniometric(&light_source->as.goniometric, params, current_color_space_illuminant);
				break;
			}
			case TPBRT_LIGHT_SOURCE_TYPE_INFINITE: {
				err = tpbrt_light_source_read_infinite(&light_source->as.infinite, params, current_color_space_illuminant);
				break;
			}
			case TPBRT_LIGHT_SOURCE_TYPE_POINT: {
				err = tpbrt_light_source_read_point(&light_source->as.point, params, current_color_space_illuminant);
				break;
			}
			case TPBRT_LIGHT_SOURCE_TYPE_PROJECTION: {
				err = tpbrt_light_source_read_projection(&light_source->as.projection, params, current_color_space_illuminant);
				break;
			}
			case TPBRT_LIGHT_SOURCE_TYPE_SPOT: {
				err = tpbrt_light_source_read_spot(&light_source->as.spot, params, current_color_space_illuminant);
				break;
			}
		}

		if (err != TPBRT_ERROR_NONE) {
			tpbrt_light_source_free(light_source);
			return err;
		}

	return TPBRT_ERROR_NONE;
}

void tpbrt_light_source_free(tpbrt_light_source_t* const light_source) {
		if (light_source == TPBRT_NULL) { return; }

		switch (light_source->type) {
			case TPBRT_LIGHT_SOURCE_TYPE_DISTANT: {
				tpbrt_light_source_distant_params_t* params = &light_source->as.distant;

				tpbrt_spectrum_free(&params->l);
				break;
			}
			case TPBRT_LIGHT_SOURCE_TYPE_GONIOMETRIC: {
				tpbrt_light_source_goniometric_params_t* params = &light_source->as.goniometric;

				tpbrt_string_free(&params->file_name);
				tpbrt_spectrum_free(&params->i);
				break;
			}
			case TPBRT_LIGHT_SOURCE_TYPE_INFINITE: {
				tpbrt_light_source_infinite_params_t* params = &light_source->as.infinite;

				tpbrt_string_free(&params->file_name);
				tpbrt_spectrum_free(&params->l);
				break;
			}
			case TPBRT_LIGHT_SOURCE_TYPE_POINT: {
				tpbrt_light_source_point_params_t* params = &light_source->as.point;

				tpbrt_spectrum_free(&params->i);
				break;
			}
			case TPBRT_LIGHT_SOURCE_TYPE_PROJECTION: {
				tpbrt_light_source_projection_params_t* params = &light_source->as.projection;

				tpbrt_string_free(&params->file_name);
				tpbrt_spectrum_free(&params->i);
				break;
			}
			case TPBRT_LIGHT_SOURCE_TYPE_SPOT: {
				tpbrt_light_source_spot_params_t* params = &light_source->as.spot;

				tpbrt_spectrum_free(&params->i);
				break;
			}
			default: {
				break;
			}
		}
}

#pragma endregion

#pragma region AREA_LIGHT

tpbrt_error_t tpbrt_area_light_type_from_string(const tpbrt_string_t type_str, tpbrt_area_light_type_t* const type) {
	static const tpbrt_string_t TypesStrs[TPBRT_AREA_LIGHT_TYPE_MAX_NUM] = {
		TPBRT_STRING_INIT("diffuse"),
	};

		if (type == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		if (tpbrt_string_empty(type_str) == TPBRT_TRUE) { return TPBRT_ERROR_INVALID_AREA_LIGHT_TYPE; }

		for (tpbrt_area_light_type_t t = 0; t < TPBRT_AREA_LIGHT_TYPE_MAX_NUM; ++t) {
				if (tpbrt_string_equals(type_str, TypesStrs[t]) == TPBRT_TRUE) {
					*type = t;
					return TPBRT_ERROR_NONE;
				}
		}

	return TPBRT_ERROR_INVALID_AREA_LIGHT_TYPE;
}

/**
 * @brief Reads diffuse parameters.
 * @param diffuse Diffuse data.
 * @param params Parameters list.
 * @param current_color_space_illuminant Current color space illuminant.
 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
 * @ingroup light-private
 */
static tpbrt_error_t tpbrt_area_light_read_diffuse(tpbrt_area_light_diffuse_params_t* const diffuse,
  const tpbrt_params_list_t* const params, const tpbrt_spectrum_builtin_t current_color_space_illuminant) {
		if (diffuse == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	static const tpbrt_string_t FileNameStr		= TPBRT_STRING_INIT("filename");
	static const tpbrt_string_t LStr			= TPBRT_STRING_INIT("L");
	static const tpbrt_string_t TwoSidedStr		= TPBRT_STRING_INIT("twosided");

	static const tpbrt_string_t FileNameDefault = TPBRT_EMPTY_STRING_INIT;
	const tpbrt_spectrum_t l_default			= TPBRT_SPECTRUM_BUILTIN_INIT(current_color_space_illuminant);
	static const tpbrt_bool_t TwoSidedDefault	= TPBRT_FALSE;

	tpbrt_error_t err = tpbrt_params_list_get_string(params, FileNameStr, FileNameDefault, &diffuse->file_name);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_spectrum(params, LStr, &l_default, &diffuse->l);
		if (err != TPBRT_ERROR_NONE) { return err; }

	tpbrt_bool_t two_sided = TwoSidedDefault;
	err					   = tpbrt_params_list_get_bool(params, TwoSidedStr, TwoSidedDefault, &two_sided);
	diffuse->two_sided	   = two_sided;

	return err;
}

tpbrt_error_t tpbrt_area_light_init(const tpbrt_string_t type_str, const tpbrt_params_list_t* const params,
  const tpbrt_color_space_t color_space, tpbrt_area_light_t* const area_light) {
		if (area_light == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		if (memset(area_light, 0, sizeof(tpbrt_area_light_t)) == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	tpbrt_area_light_type_t area_light_type;
	tpbrt_error_t err = tpbrt_area_light_type_from_string(type_str, &area_light_type);
		if (err != TPBRT_ERROR_NONE) {
			tpbrt_area_light_free(area_light);
			return err;
		}
	area_light->type						= area_light_type;

	static const tpbrt_string_t PowerStr	= TPBRT_STRING_INIT("power");
	static const tpbrt_string_t ScaleStr	= TPBRT_STRING_INIT("scale");

	static const tpbrt_float_t ScaleDefault = 1.0f;

	err										= tpbrt_params_list_get_opt_float(params, PowerStr, &area_light->power);
		if (err != TPBRT_ERROR_NONE) {
			tpbrt_area_light_free(area_light);
			return err;
		}

	err = tpbrt_params_list_get_float(params, ScaleStr, ScaleDefault, &area_light->scale);
		if (err != TPBRT_ERROR_NONE) {
			tpbrt_area_light_free(area_light);
			return err;
		}

	tpbrt_spectrum_builtin_t current_color_space_illuminant;
		switch (color_space) {
			case TPBRT_COLOR_SPACE_ACES_2065_1: {
				current_color_space_illuminant = TPBRT_SPECTRUM_BUILTIN_ILLUM_ACES_D60;
				break;
			}
		case TPBRT_COLOR_SPACE_REC_2020:
		case TPBRT_COLOR_SPACE_DCI_P3:
		default:
			case TPBRT_COLOR_SPACE_SRGB: {
				current_color_space_illuminant = TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_D65;
				break;
			}
		}

		switch (area_light->type) {
		default:
			case TPBRT_AREA_LIGHT_TYPE_DIFFUSE: {
				err = tpbrt_area_light_read_diffuse(&area_light->as.diffuse, params, current_color_space_illuminant);
				break;
			}
		}

		if (err != TPBRT_ERROR_NONE) {
			tpbrt_area_light_free(area_light);
			return err;
		}

	return TPBRT_ERROR_NONE;
}

void tpbrt_area_light_free(tpbrt_area_light_t* const area_light) {
		if (area_light == TPBRT_NULL) { return; }

		if (area_light->type == TPBRT_AREA_LIGHT_TYPE_DIFFUSE) {
			tpbrt_area_light_diffuse_params_t* params = &area_light->as.diffuse;

			tpbrt_string_free(&params->file_name);
			tpbrt_spectrum_free(&params->l);
		}
}

#pragma endregion

#pragma region LIGHTS_LIST
/**
 * @brief Initializes empty light sources list with type conversion.
 * @ingroup light-private
 */
#define TPBRT_EMPTY_LIGHTS_SOURCES_LIST (tpbrt_lights_sources_list_t) { .lights = TPBRT_NULL, .count = 0 }
/**
 * @brief Initializes empty area lights list with type conversion.
 * @ingroup light-private
 */
#define TPBRT_EMPTY_AREA_LIGHTS_LIST (tpbrt_area_lights_list_t) { .lights = TPBRT_NULL, .count = 0 }

/**
 * @brief Initializes empty lights with type conversion.
 * @ingroup light-private
 */
#define TPBRT_EMPTY_LIGHTS_LIST                                                                                              \
	(tpbrt_lights_list_t) { .lights_sources = TPBRT_EMPTY_LIGHTS_SOURCES_LIST, .area_lights = TPBRT_EMPTY_AREA_LIGHTS_LIST }

tpbrt_error_t tpbrt_lights_list_init(tpbrt_lights_list_t* const lights_list) {
		if (lights_list == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	*lights_list = TPBRT_EMPTY_LIGHTS_LIST;
	return TPBRT_ERROR_NONE;
}

tpbrt_error_t tpbrt_lights_list_append_light_source(tpbrt_lights_list_t* const lights_list,
  const tpbrt_light_source_t* const light_source) {
		if (light_source == TPBRT_NULL || lights_list == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	tpbrt_lights_sources_list_t* const lights_sources = &lights_list->lights_sources;

	tpbrt_light_source_t* const new_list =
	  tpbrt_array_reserve_append(lights_sources->lights, lights_sources->count, sizeof(tpbrt_light_source_t));
		if (new_list == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }

	lights_sources->lights						  = new_list;
	lights_sources->lights[lights_sources->count] = *light_source;
	++lights_sources->count;
	return TPBRT_ERROR_NONE;
}

tpbrt_error_t tpbrt_lights_list_append_area_light(tpbrt_lights_list_t* const lights_list, tpbrt_area_light_handle_t* const handle,
  const tpbrt_area_light_t* const area_light) {
		if (area_light == TPBRT_NULL || lights_list == TPBRT_NULL || handle == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	tpbrt_area_lights_list_t* const area_lights = &lights_list->area_lights;

	const tpbrt_size_t new_count				= area_lights->count + 1u;

	tpbrt_area_light_t* const new_list =
	  tpbrt_array_reserve_append(area_lights->lights, area_lights->count, sizeof(tpbrt_area_light_t));
		if (new_list == TPBRT_NULL) {
			*handle = TPBRT_AREA_LIGHT_HANDLE_INVALID;
			return TPBRT_ERROR_OUT_OF_MEMORY;
		}

	area_lights->lights							= new_list;
	area_lights->lights[area_lights->count]		= *area_light;
	area_lights->lights[area_lights->count].idx = area_lights->count;
	handle->area_light_idx						= area_lights->count;
	area_lights->count							= new_count;
	return TPBRT_ERROR_NONE;
}

void tpbrt_lights_list_shrink_to_fit(tpbrt_lights_list_t* const lights_list) {
		if (lights_list == TPBRT_NULL) { return; }
	lights_list->lights_sources.lights = tpbrt_array_shrink_to_fit(lights_list->lights_sources.lights,
	  lights_list->lights_sources.count, sizeof(tpbrt_light_source_t));
	lights_list->area_lights.lights =
	  tpbrt_array_shrink_to_fit(lights_list->area_lights.lights, lights_list->area_lights.count, sizeof(tpbrt_area_light_t));
}

void tpbrt_lights_list_free(tpbrt_lights_list_t* const lights_list) {
		if (lights_list == TPBRT_NULL) { return; }

	const tpbrt_lights_sources_list_t* const lights_sources = &lights_list->lights_sources;
		if (lights_sources->lights != TPBRT_NULL) {
				for (tpbrt_size_t i = 0; i < lights_sources->count; ++i) { tpbrt_light_source_free(lights_sources->lights + i); }

			free(lights_sources->lights);
		}

	const tpbrt_area_lights_list_t* const area_lights = &lights_list->area_lights;
		if (area_lights->lights != TPBRT_NULL) {
				for (tpbrt_size_t i = 0; i < area_lights->count; ++i) { tpbrt_area_light_free(area_lights->lights + i); }

			free(area_lights->lights);
		}

	*lights_list = TPBRT_EMPTY_LIGHTS_LIST;
}

/**
 * @brief Checks if light sources list is empty.
 * @param lights_sources Light sources list to check.
 * @return TPBRT_TRUE if light sources list is empty else TPBRT_FALSE.
 * @ingroup light-private
 */
static tpbrt_bool_t tpbrt_lights_list_lights_sources_empty(const tpbrt_lights_sources_list_t lights_sources) {
	return lights_sources.lights == TPBRT_NULL || lights_sources.count == 0 ? TPBRT_TRUE : TPBRT_FALSE;
}

/**
 * @brief Checks if area lights list is empty.
 * @param area_lights Area lights list to check.
 * @return TPBRT_TRUE if area lights list is empty else TPBRT_FALSE.
 * @ingroup light-private
 */
static tpbrt_bool_t tpbrt_lights_list_area_lights_empty(const tpbrt_area_lights_list_t* const area_lights) {
	return area_lights->lights == TPBRT_NULL || area_lights->count == 0 ? TPBRT_TRUE : TPBRT_FALSE;
}

tpbrt_bool_t tpbrt_lights_list_empty(const tpbrt_lights_list_t* const lights_list) {
		if (lights_list == TPBRT_NULL) { return TPBRT_TRUE; }
	return tpbrt_lights_list_lights_sources_empty(lights_list->lights_sources) == TPBRT_TRUE &&
			   tpbrt_lights_list_area_lights_empty(&lights_list->area_lights) == TPBRT_TRUE ?
			 TPBRT_TRUE :
			 TPBRT_FALSE;
}

#pragma endregion

#pragma region API

tpbrt_error_t tpbrt_get_area_light_by_handle(const tpbrt_area_lights_list_t* const area_lights_list,
  const tpbrt_area_light_handle_t handle, const tpbrt_area_light_t** const area_light) {
		if (area_light == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		if (handle.area_light_idx == TPBRT_AREA_LIGHT_IDX_INVALID || handle.area_light_idx >= area_lights_list->count) {
			*area_light = TPBRT_NULL;
			return TPBRT_ERROR_INVALID_HANDLE;
		}

	*area_light = area_lights_list->lights + handle.area_light_idx;
	return TPBRT_ERROR_NONE;
}

#pragma endregion
