#include <tinypbrt/pch.h>

#include <tinypbrt/detail/filter_internal.h>

#include <tinypbrt/detail/common_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

	static tpbrt_error_t tpbrt_filter_type_from_string(const tpbrt_string_t* const type_str, tpbrt_filter_type_t* const type) {
		static const tpbrt_string_t TYPES_STRS[TPBRT_FILTER_TYPE_MAX_NUM] = {
			TPBRT_STRING("box"),
			TPBRT_STRING("gaussian"),
			TPBRT_STRING("mitchell"),
			TPBRT_STRING("sinc"),
			TPBRT_STRING("triangle"),
		};

			if (type_str == TPBRT_NULL || type_str->data == TPBRT_NULL || type == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

			for (tpbrt_filter_type_t t = 0; t < TPBRT_FILTER_TYPE_MAX_NUM; ++t) {
					if (tpbrt_string_equals(type_str, &TYPES_STRS[t])) {
						*type = t;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_UNKNOWN_FILTER_TYPE;
	}

	tpbrt_error_t tpbrt_create_default_filter(tpbrt_filter_t* const filter) {
			if (filter == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		filter->type			  = TPBRT_FILTER_TYPE_GAUSSIAN;
		filter->x_radius		  = 1.5f;
		filter->y_radius		  = 1.5f;
		filter->as.gaussian.sigma = 0.5f;
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_create_filter(const tpbrt_string_t* type, const tpbrt_params_list_t* const params,
	  tpbrt_filter_t* const filter) {
			if (params == TPBRT_NULL || filter == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (memset(filter, 0, sizeof(tpbrt_filter_t)) == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		tpbrt_error_t err = tpbrt_filter_type_from_string(type, &filter->type);
			if (err != TPBRT_ERROR_NONE) { return err; }

		tpbrt_float_t defaultRadius = 0.0f;

			switch (filter->type) {
				case TPBRT_FILTER_TYPE_BOX: {
					defaultRadius = 0.5f;
					break;
				}
			default:
				case TPBRT_FILTER_TYPE_GAUSSIAN: {
					defaultRadius						  = 1.5f;

					static const tpbrt_string_t SIGMA_STR = TPBRT_STRING("sigma");
					err = tpbrt_params_list_get_float(params, &SIGMA_STR, 0.5f, &filter->as.gaussian.sigma);
						if (err != TPBRT_ERROR_NONE) { return err; }
					break;
				}
				case TPBRT_FILTER_TYPE_MITCHELL: {
					defaultRadius					  = 2.f;

					static const tpbrt_string_t B_STR = TPBRT_STRING("B");
					static const tpbrt_string_t C_STR = TPBRT_STRING("C");

					err = tpbrt_params_list_get_float(params, &B_STR, 1.f / 3.f, &filter->as.mitchell.B);
						if (err != TPBRT_ERROR_NONE) { return err; }

					err = tpbrt_params_list_get_float(params, &C_STR, 1.f / 3.f, &filter->as.mitchell.C);
						if (err != TPBRT_ERROR_NONE) { return err; }
					break;
				}
				case TPBRT_FILTER_TYPE_SINC: {
					defaultRadius						= 4.f;

					static const tpbrt_string_t TAU_STR = TPBRT_STRING("tau");

					err = tpbrt_params_list_get_float(params, &TAU_STR, 3.f, &filter->as.sinc.tau);
						if (err != TPBRT_ERROR_NONE) { return err; }
					break;
				}
				case TPBRT_FILTER_TYPE_TRIANGLE: {
					defaultRadius = 2.f;
					break;
				}
			}

		static const tpbrt_string_t X_RADIUS_STR = TPBRT_STRING("xradius");
		err = tpbrt_params_list_get_float(params, &X_RADIUS_STR, defaultRadius, &filter->x_radius);
			if (err != TPBRT_ERROR_NONE) { return err; }

		static const tpbrt_string_t Y_RADIUS_STR = TPBRT_STRING("yradius");
		err = tpbrt_params_list_get_float(params, &Y_RADIUS_STR, defaultRadius, &filter->y_radius);
			if (err != TPBRT_ERROR_NONE) { return err; }

		return TPBRT_ERROR_NONE;
	}

#ifdef __cplusplus
}
#endif