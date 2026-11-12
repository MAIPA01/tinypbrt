#include <tinypbrt/pch.h>

#include <tinypbrt/detail/filter_internal.h>

#include <tinypbrt/detail/common_internal.h>
#include <tinypbrt/detail/param_internal.h>

tpbrt_error_t tpbrt_filter_type_from_string(const tpbrt_string_t type_str, tpbrt_filter_type_t* const type) {
	static const tpbrt_string_t TypesStrs[TPBRT_FILTER_TYPE_MAX_NUM] = {
		TPBRT_STRING_INIT("box"),
		TPBRT_STRING_INIT("gaussian"),
		TPBRT_STRING_INIT("mitchell"),
		TPBRT_STRING_INIT("sinc"),
		TPBRT_STRING_INIT("triangle"),
	};

		if (type == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		if (tpbrt_string_empty(type_str) == TPBRT_TRUE) { return TPBRT_ERROR_INVALID_FILTER_TYPE; }

		for (tpbrt_filter_type_t t = 0; t < TPBRT_FILTER_TYPE_MAX_NUM; ++t) {
				if (tpbrt_string_equals(type_str, TypesStrs[t]) == TPBRT_TRUE) {
					*type = t;
					return TPBRT_ERROR_NONE;
				}
		}

	return TPBRT_ERROR_INVALID_FILTER_TYPE;
}

tpbrt_error_t tpbrt_filter_init_default(tpbrt_filter_t* const filter) {
		if (filter == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	static const tpbrt_float_t XRadiusDefault = 1.5f;
	static const tpbrt_float_t YRadiusDefault = 1.5f;
	static const tpbrt_float_t SigmaDefault	  = 0.5f;

	*filter									  = (tpbrt_filter_t) {
		.type			   = TPBRT_FILTER_TYPE_GAUSSIAN,
		.x_radius		   = XRadiusDefault,
		.y_radius		   = YRadiusDefault,
		.as.gaussian.sigma = SigmaDefault,
	};
	return TPBRT_ERROR_NONE;
}

/**
 * @brief Reads box parameters.
 * @param default_radius Output default radius.
 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
 * @ingroup filter-private
 */
static tpbrt_error_t tpbrt_filter_read_box(tpbrt_float_t* const default_radius) {
		if (default_radius == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	static const tpbrt_float_t BoxRadius = 0.5f;

	*default_radius						 = BoxRadius;
	return TPBRT_ERROR_NONE;
}

/**
 * @brief Reads gaussian parameters.
 * @param gaussian Gaussian data.
 * @param params Parameters list.
 * @param default_radius Output default radius.
 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
 * @ingroup filter-private
 */
static tpbrt_error_t tpbrt_filter_read_gaussian(tpbrt_filter_gaussian_params_t* const gaussian,
  const tpbrt_params_list_t* const params, tpbrt_float_t* const default_radius) {
		if (gaussian == TPBRT_NULL || default_radius == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	static const tpbrt_string_t SigmaStr	  = TPBRT_STRING_INIT("sigma");

	static const tpbrt_float_t GaussianRadius = 1.5f;
	static const tpbrt_float_t SigmaDefault	  = 0.5f;

	*default_radius							  = GaussianRadius;

	return tpbrt_params_list_get_float(params, SigmaStr, SigmaDefault, &gaussian->sigma);
}

/**
 * @brief Reads mitchell parameters.
 * @param mitchell Mitchell data.
 * @param params Parameters list.
 * @param default_radius Output default radius.
 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
 * @ingroup filter-private
 */
static tpbrt_error_t tpbrt_filter_read_mitchell(tpbrt_filter_mitchell_params_t* const mitchell,
  const tpbrt_params_list_t* const params, tpbrt_float_t* const default_radius) {
		if (mitchell == TPBRT_NULL || default_radius == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	static const tpbrt_string_t BStr		  = TPBRT_STRING_INIT("B");
	static const tpbrt_string_t CStr		  = TPBRT_STRING_INIT("C");

	static const tpbrt_float_t MitchellRadius = 2.f;
	static const tpbrt_float_t BDefault		  = 1.f / 3.f;
	static const tpbrt_float_t CDefault		  = 1.f / 3.f;

	*default_radius							  = MitchellRadius;

	const tpbrt_error_t err					  = tpbrt_params_list_get_float(params, BStr, BDefault, &mitchell->b);
		if (err != TPBRT_ERROR_NONE) { return err; }

	return tpbrt_params_list_get_float(params, CStr, CDefault, &mitchell->c);
}

/**
 * @brief Reads sinc parameters.
 * @param sinc Sinc data.
 * @param params Parameters list.
 * @param default_radius Output default radius.
 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
 * @ingroup filter-private
 */
static tpbrt_error_t tpbrt_filter_read_sinc(tpbrt_filter_sinc_params_t* const sinc, const tpbrt_params_list_t* const params,
  tpbrt_float_t* const default_radius) {
		if (sinc == TPBRT_NULL || default_radius == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	static const tpbrt_string_t TauStr	  = TPBRT_STRING_INIT("tau");

	static const tpbrt_float_t SincRadius = 4.f;
	static const tpbrt_float_t TauDefault = 3.f;

	*default_radius						  = SincRadius;

	return tpbrt_params_list_get_float(params, TauStr, TauDefault, &sinc->tau);
}

/**
 * @brief Reads triangle parameters.
 * @param default_radius Output default radius.
 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
 * @ingroup filter-private
 */
static tpbrt_error_t tpbrt_filter_read_triangle(tpbrt_float_t* const default_radius) {
		if (default_radius == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	static const tpbrt_float_t TriangleRadius = 2.f;

	*default_radius							  = TriangleRadius;
	return TPBRT_ERROR_NONE;
}

tpbrt_error_t tpbrt_filter_init(const tpbrt_string_t type, const tpbrt_params_list_t* const params,
  tpbrt_filter_t* const filter) {
		if (filter == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		if (memset(filter, 0, sizeof(tpbrt_filter_t)) == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	static const tpbrt_string_t XRadiusStr = TPBRT_STRING_INIT("xradius");
	static const tpbrt_string_t YRadiusStr = TPBRT_STRING_INIT("yradius");

	tpbrt_filter_type_t filter_type;
	tpbrt_error_t err = tpbrt_filter_type_from_string(type, &filter_type);
		if (err != TPBRT_ERROR_NONE) { return err; }
	filter->type				 = filter_type;

	tpbrt_float_t default_radius = 0.0f;
		switch (filter->type) {
			case TPBRT_FILTER_TYPE_BOX: {
				err = tpbrt_filter_read_box(&default_radius);
				break;
			}
		default:
			case TPBRT_FILTER_TYPE_GAUSSIAN: {
				err = tpbrt_filter_read_gaussian(&filter->as.gaussian, params, &default_radius);
				break;
			}
			case TPBRT_FILTER_TYPE_MITCHELL: {
				err = tpbrt_filter_read_mitchell(&filter->as.mitchell, params, &default_radius);
				break;
			}
			case TPBRT_FILTER_TYPE_SINC: {
				err = tpbrt_filter_read_sinc(&filter->as.sinc, params, &default_radius);
				break;
			}
			case TPBRT_FILTER_TYPE_TRIANGLE: {
				err = tpbrt_filter_read_triangle(&default_radius);
				break;
			}
		}

		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_float(params, XRadiusStr, default_radius, &filter->x_radius);
		if (err != TPBRT_ERROR_NONE) { return err; }


	return tpbrt_params_list_get_float(params, YRadiusStr, default_radius, &filter->y_radius);
}
