#include <tinypbrt/pch.h>

#include <tinypbrt/detail/filter_internal.h>

#include <tinypbrt/detail/common_internal.h>
#include <tinypbrt/detail/param_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

	tpbrt_error_t tpbrt_filter_type_from_string(const tpbrt_string_t type_str, tpbrt_filter_type_t* const type) {
		static const tpbrt_string_t TYPES_STRS[TPBRT_FILTER_TYPE_MAX_NUM] = {
			TPBRT_STRING("box"),
			TPBRT_STRING("gaussian"),
			TPBRT_STRING("mitchell"),
			TPBRT_STRING("sinc"),
			TPBRT_STRING("triangle"),
		};

			if (type == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (tpbrt_string_empty(type_str) == TPBRT_TRUE) { return TPBRT_ERROR_INVALID_FILTER_TYPE; }

			for (tpbrt_filter_type_t t = 0; t < TPBRT_FILTER_TYPE_MAX_NUM; ++t) {
					if (tpbrt_string_equals(type_str, TYPES_STRS[t]) == TPBRT_TRUE) {
						*type = t;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_INVALID_FILTER_TYPE;
	}

	tpbrt_error_t tpbrt_filter_init_default(tpbrt_filter_t* const filter) {
			if (filter == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_float_t X_RADIUS_DEFAULT = 1.5f;
		static const tpbrt_float_t Y_RADIUS_DEFAULT = 1.5f;
		static const tpbrt_float_t SIGMA_DEFAULT	= 0.5f;

		*filter										= (tpbrt_filter_t) {
			.type			   = TPBRT_FILTER_TYPE_GAUSSIAN,
			.x_radius		   = X_RADIUS_DEFAULT,
			.y_radius		   = Y_RADIUS_DEFAULT,
			.as.gaussian.sigma = SIGMA_DEFAULT,
		};
		return TPBRT_ERROR_NONE;
	}

	static tpbrt_error_t tpbrt_filter_read_box(tpbrt_float_t* const default_radius) {
			if (default_radius == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_float_t BOX_RADIUS = 0.5f;

		*default_radius						  = BOX_RADIUS;
		return TPBRT_ERROR_NONE;
	}

	static tpbrt_error_t tpbrt_filter_read_gaussian(tpbrt_filter_gaussian_params_t* const gaussian,
	  const tpbrt_params_list_t params, tpbrt_float_t* const default_radius) {
			if (gaussian == TPBRT_NULL || default_radius == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t SIGMA_STR	   = TPBRT_STRING("sigma");

		static const tpbrt_float_t GAUSSIAN_RADIUS = 1.5f;
		static const tpbrt_float_t SIGMA_DEFAULT   = 0.5f;

		*default_radius							   = GAUSSIAN_RADIUS;

		return tpbrt_params_list_get_float(params, SIGMA_STR, SIGMA_DEFAULT, &gaussian->sigma);
	}

	static tpbrt_error_t tpbrt_filter_read_mitchell(tpbrt_filter_mitchell_params_t* const mitchell,
	  const tpbrt_params_list_t params, tpbrt_float_t* const default_radius) {
			if (mitchell == TPBRT_NULL || default_radius == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t B_STR		   = TPBRT_STRING("B");
		static const tpbrt_string_t C_STR		   = TPBRT_STRING("C");

		static const tpbrt_float_t MITCHELL_RADIUS = 2.f;
		static const tpbrt_float_t B_DEFAULT	   = 1.f / 3.f;
		static const tpbrt_float_t C_DEFAULT	   = 1.f / 3.f;

		*default_radius							   = MITCHELL_RADIUS;

		const tpbrt_error_t err					   = tpbrt_params_list_get_float(params, B_STR, B_DEFAULT, &mitchell->b);
			if (err != TPBRT_ERROR_NONE) { return err; }

		return tpbrt_params_list_get_float(params, C_STR, C_DEFAULT, &mitchell->c);
	}

	static tpbrt_error_t tpbrt_filter_read_sinc(tpbrt_filter_sinc_params_t* const sinc, const tpbrt_params_list_t params,
	  tpbrt_float_t* const default_radius) {
			if (sinc == TPBRT_NULL || default_radius == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t TAU_STR	   = TPBRT_STRING("tau");

		static const tpbrt_float_t SINC_RADIUS = 4.f;
		static const tpbrt_float_t TAU_DEFAULT = 3.f;

		*default_radius						   = SINC_RADIUS;

		return tpbrt_params_list_get_float(params, TAU_STR, TAU_DEFAULT, &sinc->tau);
	}

	static tpbrt_error_t tpbrt_filter_read_triangle(tpbrt_float_t* const default_radius) {
			if (default_radius == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_float_t TRIANGLE_RADIUS = 2.f;

		*default_radius							   = TRIANGLE_RADIUS;
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_filter_init(const tpbrt_string_t type, const tpbrt_params_list_t params, tpbrt_filter_t* const filter) {
			if (filter == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (memset(filter, 0, sizeof(tpbrt_filter_t)) == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t X_RADIUS_STR = TPBRT_STRING("xradius");
		static const tpbrt_string_t Y_RADIUS_STR = TPBRT_STRING("yradius");

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

		err = tpbrt_params_list_get_float(params, X_RADIUS_STR, default_radius, &filter->x_radius);
			if (err != TPBRT_ERROR_NONE) { return err; }


		return tpbrt_params_list_get_float(params, Y_RADIUS_STR, default_radius, &filter->y_radius);
	}

#ifdef __cplusplus
}
#endif