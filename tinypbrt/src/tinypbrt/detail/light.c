#include "tinypbrt/detail/camera.h"


#include <tinypbrt/pch.h>

#include <tinypbrt/detail/light_internal.h>

#include <tinypbrt/detail/color_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

#pragma region LIGHT_SOURCE

	static tpbrt_error_t tpbrt_light_source_type_from_string(const tpbrt_string_t* const type_str,
	  tpbrt_light_source_type_t* const type) {
		static const tpbrt_string_t TYPES_STRS[TPBRT_LIGHT_SOURCE_TYPE_MAX_NUM] = {
			{ .chars = "distant",	  .size = 7	},
			{ .chars = "goniometric", .size = 11 },
			{ .chars = "infinite",	   .size = 8	 },
			{ .chars = "point",		.size = 5  },
			{ .chars = "projection",	 .size = 10 },
			{ .chars = "spot",		   .size = 4	 },
		};

			if (type_str == TPBRT_NULL || type_str->chars == TPBRT_NULL || type == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

			for (tpbrt_light_source_type_t t = 0; t < TPBRT_LIGHT_SOURCE_TYPE_MAX_NUM; ++t) {
					if (type_str->size == TYPES_STRS[t].size &&
						strncmp(type_str->chars, TYPES_STRS[t].chars, TYPES_STRS[t].size) == 0) {
						*type = t;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_UNKNOWN_LIGHT_SOURCE_TYPE;
	}

	tpbrt_error_t tpbrt_create_light_source(const tpbrt_string_t* const type_str, const tpbrt_params_list_t* const params,
	  const tpbrt_color_space_t color_space, tpbrt_light_source_t** const light_source) {
			if (type_str == TPBRT_NULL || type_str->chars == TPBRT_NULL || params == TPBRT_NULL || light_source == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

		*light_source = malloc(sizeof(tpbrt_light_source_t));
			if (*light_source == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }

		tpbrt_error_t err = tpbrt_light_source_type_from_string(type_str, &(*light_source)->type);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_free_light_source(light_source);
				return err;
			}

		static const tpbrt_string_t POWER_STR		= { .chars = "power", .size = 5 };
		static const tpbrt_string_t ILLUMINANCE_STR = { .chars = "illuminance", .size = 11 };
		static const tpbrt_string_t SCALE_STR		= { .chars = "scale", .size = 5 };

		err = tpbrt_params_list_get_opt_float(params, &POWER_STR, &(*light_source)->power_illuminance);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_free_light_source(light_source);
				return err;
			}

			if (!(*light_source)->power_illuminance.has_value) {
				err = tpbrt_params_list_get_opt_float(params, &ILLUMINANCE_STR, &(*light_source)->power_illuminance);
					if (err != TPBRT_ERROR_NONE) {
						tpbrt_free_light_source(light_source);
						return err;
					}
			}

		err = tpbrt_params_list_get_float(params, &SCALE_STR, 1.0f, &(*light_source)->scale);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_free_light_source(light_source);
				return err;
			}

		tpbrt_spectrum_builtin_t current_color_space_illuminant;
			switch (color_space) {
				case TPBRT_COLOR_SPACE_ACES_2065_1: {
					current_color_space_illuminant = TPBRT_SPECTRUM_BUILTIN_ILLUM_ACES_D60;
				}
				case TPBRT_COLOR_SPACE_REC_2020: {
					current_color_space_illuminant = TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_D65;
				}
				case TPBRT_COLOR_SPACE_DCI_P3: {
					current_color_space_illuminant = TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_D65;
				}
			default:
				case TPBRT_COLOR_SPACE_SRGB: {
					current_color_space_illuminant = TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_D65;
				}
			}

			switch ((*light_source)->type) {
			default:
				case TPBRT_LIGHT_SOURCE_TYPE_DISTANT: {
					static const tpbrt_string_t L_STR	 = { .chars = "L", .size = 1 };
					static const tpbrt_string_t FROM_STR = { .chars = "from", .size = 4 };
					static const tpbrt_string_t TO_STR	 = { .chars = "to", .size = 2 };

					err = tpbrt_params_list_get_spectrum(params, &L_STR, &(*light_source)->distant_params.l);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_light_source(light_source);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							(*light_source)->distant_params.l.type	  = TPBRT_SPECTRUM_TYPE_BUILTIN;
							(*light_source)->distant_params.l.builtin = current_color_space_illuminant;
						}

					err = tpbrt_params_list_get_point(params, &FROM_STR, &(*light_source)->distant_params.from);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_light_source(light_source);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							(*light_source)->distant_params.from.x = 0.0f;
							(*light_source)->distant_params.from.y = 0.0f;
							(*light_source)->distant_params.from.z = 0.0f;
						}

					err = tpbrt_params_list_get_point(params, &TO_STR, &(*light_source)->distant_params.to);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_light_source(light_source);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							(*light_source)->distant_params.to.x = 0.0f;
							(*light_source)->distant_params.to.y = 0.0f;
							(*light_source)->distant_params.to.z = 1.0f;
						}
					break;
				}
				case TPBRT_LIGHT_SOURCE_TYPE_GONIOMETRIC: {
					static const tpbrt_string_t FILE_NAME_STR = { .chars = "filename", .size = 8 };
					static const tpbrt_string_t I_STR		  = { .chars = "I", .size = 1 };

					err = tpbrt_params_list_get_string(params, &FILE_NAME_STR, &(*light_source)->goniometric_params.file_name);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_light_source(light_source);
							return err;
						}

					err = tpbrt_params_list_get_spectrum(params, &I_STR, &(*light_source)->goniometric_params.i);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_light_source(light_source);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							(*light_source)->distant_params.l.type	  = TPBRT_SPECTRUM_TYPE_BUILTIN;
							(*light_source)->distant_params.l.builtin = current_color_space_illuminant;
						}
					break;
				}
				case TPBRT_LIGHT_SOURCE_TYPE_INFINITE: {
					static const tpbrt_string_t FILE_NAME_STR = { .chars = "filename", .size = 8 };
					static const tpbrt_string_t PORTAL_STR	  = { .chars = "portal", .size = 6 };
					static const tpbrt_string_t L_STR		  = { .chars = "L", .size = 1 };

					err = tpbrt_params_list_get_string(params, &FILE_NAME_STR, &(*light_source)->infinite_params.file_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_light_source(light_source);
							return err;
						}

					tpbrt_point3_array_t points;
					err = tpbrt_params_list_get_point3s(params, &PORTAL_STR, &points);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_light_source(light_source);
							return err;
						}

						if (err == TPBRT_ERROR_NONE) {
								if (points.count < 4) {
										if (points.values != TPBRT_NULL) { free(points.values); }
									tpbrt_free_light_source(light_source);
									return TPBRT_ERROR_MISSING_REQUIRED_PARAMETER;
								}

								if (points.count > 4) {
									free(points.values);
									tpbrt_free_light_source(light_source);
									return TPBRT_ERROR_TOO_MANY_VALUES;
								}

							(*light_source)->infinite_params.portal[0] = points.values[0];
							(*light_source)->infinite_params.portal[1] = points.values[1];
							(*light_source)->infinite_params.portal[2] = points.values[2];
							(*light_source)->infinite_params.portal[3] = points.values[3];
							free(points.values);
						}

					err = tpbrt_params_list_get_spectrum(params, &L_STR, &(*light_source)->infinite_params.l);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_light_source(light_source);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							(*light_source)->infinite_params.l.type	   = TPBRT_SPECTRUM_TYPE_BUILTIN;
							(*light_source)->infinite_params.l.builtin = current_color_space_illuminant;
						}
					break;
				}
				case TPBRT_LIGHT_SOURCE_TYPE_POINT: {
					static const tpbrt_string_t I_STR	 = { .chars = "I", .size = 1 };
					static const tpbrt_string_t FROM_STR = { .chars = "from", .size = 4 };

					err = tpbrt_params_list_get_spectrum(params, &I_STR, &(*light_source)->point_params.i);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_light_source(light_source);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							(*light_source)->point_params.i.type	= TPBRT_SPECTRUM_TYPE_BUILTIN;
							(*light_source)->point_params.i.builtin = current_color_space_illuminant;
						}

					err = tpbrt_params_list_get_point(params, &FROM_STR, &(*light_source)->point_params.from);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_light_source(light_source);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							(*light_source)->point_params.from.x = 0.0f;
							(*light_source)->point_params.from.y = 0.0f;
							(*light_source)->point_params.from.z = 0.0f;
						}
					break;
				}
				case TPBRT_LIGHT_SOURCE_TYPE_PROJECTION: {
					static const tpbrt_string_t I_STR		  = { .chars = "I", .size = 1 };
					static const tpbrt_string_t FOV_STR		  = { .chars = "fov", .size = 3 };
					static const tpbrt_string_t FILE_NAME_STR = { .chars = "filename", .size = 8 };

					err = tpbrt_params_list_get_spectrum(params, &I_STR, &(*light_source)->projection_params.i);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_light_source(light_source);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							(*light_source)->projection_params.i.type	 = TPBRT_SPECTRUM_TYPE_BUILTIN;
							(*light_source)->projection_params.i.builtin = current_color_space_illuminant;
						}

					static const tpbrt_float_t FOV_DEFAULT = 90.0f;
					err = tpbrt_params_list_get_float(params, &FOV_STR, FOV_DEFAULT, &(*light_source)->projection_params.fov);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_light_source(light_source);
							return err;
						}

					err = tpbrt_params_list_get_string(params, &FILE_NAME_STR, &(*light_source)->projection_params.file_name);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_light_source(light_source);
							return err;
						}
					break;
				}
				case TPBRT_LIGHT_SOURCE_TYPE_SPOT: {
					static const tpbrt_string_t I_STR				 = { .chars = "I", .size = 1 };
					static const tpbrt_string_t FROM_STR			 = { .chars = "from", .size = 4 };
					static const tpbrt_string_t TO_STR				 = { .chars = "to", .size = 2 };
					static const tpbrt_string_t CONE_ANGLE_STR		 = { .chars = "coneangle", .size = 9 };
					static const tpbrt_string_t CONE_DELTA_ANGLE_STR = { .chars = "conedeltaangle", .size = 14 };

					err = tpbrt_params_list_get_spectrum(params, &I_STR, &(*light_source)->spot_params.i);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_light_source(light_source);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							(*light_source)->spot_params.i.type	   = TPBRT_SPECTRUM_TYPE_BUILTIN;
							(*light_source)->spot_params.i.builtin = current_color_space_illuminant;
						}

					err = tpbrt_params_list_get_point(params, &FROM_STR, &(*light_source)->spot_params.from);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_light_source(light_source);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							(*light_source)->spot_params.from.x = 0.0f;
							(*light_source)->spot_params.from.y = 0.0f;
							(*light_source)->spot_params.from.z = 0.0f;
						}

					err = tpbrt_params_list_get_point(params, &TO_STR, &(*light_source)->spot_params.to);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_light_source(light_source);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							(*light_source)->spot_params.to.x = 0.0f;
							(*light_source)->spot_params.to.y = 0.0f;
							(*light_source)->spot_params.to.z = 1.0f;
						}

					static const tpbrt_float_t CONE_ANGLE_DEFAULT = 30.0f;
					err = tpbrt_params_list_get_float(params, &CONE_ANGLE_STR, CONE_ANGLE_DEFAULT,
					  &(*light_source)->spot_params.cone_angle);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_light_source(light_source);
							return err;
						}

					static const tpbrt_float_t CONE_DELTA_ANGLE_DEFAULT = 5.0f;
					err = tpbrt_params_list_get_float(params, &CONE_DELTA_ANGLE_STR, CONE_DELTA_ANGLE_DEFAULT,
					  &(*light_source)->spot_params.cone_delta_angle);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_light_source(light_source);
							return err;
						}
					break;
				}
			}

		return TPBRT_ERROR_NONE;
	}

	void tpbrt_free_light_source(tpbrt_light_source_t** const light_source) {
			if (light_source == TPBRT_NULL || *light_source == TPBRT_NULL) { return; }

			switch ((*light_source)->type) {
				case TPBRT_LIGHT_SOURCE_TYPE_DISTANT: {
					tpbrt_free_spectrum(&(*light_source)->distant_params.l);
					break;
				}
				case TPBRT_LIGHT_SOURCE_TYPE_GONIOMETRIC: {
					free((*light_source)->goniometric_params.file_name.chars);
					tpbrt_free_spectrum(&(*light_source)->goniometric_params.i);
					break;
				}
				case TPBRT_LIGHT_SOURCE_TYPE_INFINITE: {
					free((*light_source)->infinite_params.file_name.chars);
					tpbrt_free_spectrum(&(*light_source)->infinite_params.l);
					break;
				}
				case TPBRT_LIGHT_SOURCE_TYPE_POINT: {
					tpbrt_free_spectrum(&(*light_source)->point_params.i);
					break;
				}
				case TPBRT_LIGHT_SOURCE_TYPE_PROJECTION: {
					free((*light_source)->projection_params.file_name.chars);
					tpbrt_free_spectrum(&(*light_source)->projection_params.i);
					break;
				}
				case TPBRT_LIGHT_SOURCE_TYPE_SPOT: {
					tpbrt_free_spectrum(&(*light_source)->spot_params.i);
					break;
				}
			default: break;
			}

		free(*light_source);
		*light_source = TPBRT_NULL;
	}

#pragma endregion

#pragma region AREA_LIGHT

	static tpbrt_error_t tpbrt_area_light_type_from_string(const tpbrt_string_t* const type_str,
	  tpbrt_area_light_type_t* const type) {
		static const tpbrt_string_t TYPES_STRS[TPBRT_AREA_LIGHT_TYPE_MAX_NUM] = {
			{ .chars = "diffuse", .size = 7 },
		};

			if (type_str == TPBRT_NULL || type_str->chars == TPBRT_NULL || type == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

			for (tpbrt_area_light_type_t t = 0; t < TPBRT_AREA_LIGHT_TYPE_MAX_NUM; ++t) {
					if (type_str->size == TYPES_STRS[t].size &&
						strncmp(type_str->chars, TYPES_STRS[t].chars, TYPES_STRS[t].size) == 0) {
						*type = t;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_UNKNOWN_AREA_LIGHT_TYPE;
	}

	tpbrt_error_t tpbrt_create_area_light(const tpbrt_string_t* const type_str, const tpbrt_params_list_t* const params,
	  const tpbrt_color_space_t color_space, tpbrt_area_light_t** const area_light) {
			if (type_str == TPBRT_NULL || type_str->chars == TPBRT_NULL || params == TPBRT_NULL || area_light == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

		*area_light = malloc(sizeof(tpbrt_area_light_t));
			if (*area_light == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }

		tpbrt_error_t err = tpbrt_area_light_type_from_string(type_str, &(*area_light)->type);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_free_area_light(area_light);
				return err;
			}

		tpbrt_spectrum_builtin_t current_color_space_illuminant;
			switch (color_space) {
				case TPBRT_COLOR_SPACE_ACES_2065_1: {
					current_color_space_illuminant = TPBRT_SPECTRUM_BUILTIN_ILLUM_ACES_D60;
				}
				case TPBRT_COLOR_SPACE_REC_2020: {
					current_color_space_illuminant = TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_D65;
				}
				case TPBRT_COLOR_SPACE_DCI_P3: {
					current_color_space_illuminant = TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_D65;
				}
			default:
				case TPBRT_COLOR_SPACE_SRGB: {
					current_color_space_illuminant = TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_D65;
				}
			}

			switch ((*area_light)->type) {
			default:
				case TPBRT_AREA_LIGHT_TYPE_DIFFUSE: {
					static const tpbrt_string_t FILE_NAME_STR = { .chars = "filename", .size = 8 };
					static const tpbrt_string_t L_STR		  = { .chars = "L", .size = 1 };
					static const tpbrt_string_t TWO_SIDED_STR = { .chars = "twosided", .size = 8 };

					err = tpbrt_params_list_get_string(params, &FILE_NAME_STR, &(*area_light)->diffuse_params.file_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_area_light(area_light);
							return err;
						}

					err = tpbrt_params_list_get_spectrum(params, &L_STR, &(*area_light)->diffuse_params.l);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_area_light(area_light);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							(*area_light)->diffuse_params.l.type	= TPBRT_SPECTRUM_TYPE_BUILTIN;
							(*area_light)->diffuse_params.l.builtin = current_color_space_illuminant;
						}

					err =
					  tpbrt_params_list_get_bool(params, &TWO_SIDED_STR, TPBRT_FALSE, &(*area_light)->diffuse_params.two_sided);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_area_light(area_light);
							return err;
						}
					break;
				}
			}

		return TPBRT_ERROR_NONE;
	}

	void tpbrt_free_area_light(tpbrt_area_light_t** const area_light) {
			if (area_light == TPBRT_NULL || *area_light == TPBRT_NULL) { return; }

			if ((*area_light)->type == TPBRT_AREA_LIGHT_TYPE_DIFFUSE) {
					if ((*area_light)->diffuse_params.file_name.chars != TPBRT_NULL) {
						free((*area_light)->diffuse_params.file_name.chars);
					}


				tpbrt_free_spectrum(&(*area_light)->diffuse_params.l);
			}

		free(*area_light);
		*area_light = TPBRT_NULL;
	}

#pragma endregion

#ifdef __cplusplus
}
#endif