#include <tinypbrt/pch.h>

#include <tinypbrt/detail/light_internal.h>

#include <tinypbrt/detail/color_internal.h>
#include <tinypbrt/detail/common_internal.h>
#include <tinypbrt/detail/math_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

#pragma region LIGHT_SOURCE

	static tpbrt_error_t tpbrt_light_source_type_from_string(const tpbrt_string_t* const type_str,
	  tpbrt_light_source_type_t* const type) {
		static const tpbrt_string_t TYPES_STRS[TPBRT_LIGHT_SOURCE_TYPE_MAX_NUM] = {
			TPBRT_STRING("distant"),
			TPBRT_STRING("goniometric"),
			TPBRT_STRING("infinite"),
			TPBRT_STRING("point"),
			TPBRT_STRING("projection"),
			TPBRT_STRING("spot"),
		};

			if (type_str == TPBRT_NULL || type_str->data == TPBRT_NULL || type == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

			for (tpbrt_light_source_type_t t = 0; t < TPBRT_LIGHT_SOURCE_TYPE_MAX_NUM; ++t) {
					if (tpbrt_string_equals(type_str, TYPES_STRS + t)) {
						*type = t;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_UNKNOWN_LIGHT_SOURCE_TYPE;
	}

	tpbrt_error_t tpbrt_create_light_source(const tpbrt_string_t* const type_str, const tpbrt_params_list_t* const params,
	  const tpbrt_color_space_t color_space, tpbrt_light_source_t* const light_source) {
			if (type_str == TPBRT_NULL || type_str->data == TPBRT_NULL || params == TPBRT_NULL || light_source == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

		tpbrt_error_t err = tpbrt_light_source_type_from_string(type_str, &light_source->type);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_free_light_source(light_source);
				return err;
			}

		static const tpbrt_string_t POWER_STR		= TPBRT_STRING("power");
		static const tpbrt_string_t ILLUMINANCE_STR = TPBRT_STRING("illuminance");
		static const tpbrt_string_t SCALE_STR		= TPBRT_STRING("scale");

		err = tpbrt_params_list_get_opt_float(params, &POWER_STR, &light_source->power_illuminance);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_free_light_source(light_source);
				return err;
			}

			if (!light_source->power_illuminance.has_value) {
				err = tpbrt_params_list_get_opt_float(params, &ILLUMINANCE_STR, &light_source->power_illuminance);
					if (err != TPBRT_ERROR_NONE) {
						tpbrt_free_light_source(light_source);
						return err;
					}
			}

		err = tpbrt_params_list_get_float(params, &SCALE_STR, 1.0f, &light_source->scale);
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

			switch (light_source->type) {
			default:
				case TPBRT_LIGHT_SOURCE_TYPE_DISTANT: {
					static const tpbrt_string_t L_STR				  = TPBRT_STRING("L");
					static const tpbrt_string_t FROM_STR			  = TPBRT_STRING("from");
					static const tpbrt_string_t TO_STR				  = TPBRT_STRING("to");

					tpbrt_light_source_distant_params_t* light_params = &light_source->as.distant;

					err = tpbrt_params_list_get_spectrum(params, &L_STR, &light_params->l);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_light_source(light_source);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							light_params->l.type	   = TPBRT_SPECTRUM_TYPE_BUILTIN;
							light_params->l.as.builtin = current_color_space_illuminant;
						}

					err = tpbrt_params_list_get_point(params, &FROM_STR, &light_params->from);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_light_source(light_source);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							light_params->from.x = 0.0f;
							light_params->from.y = 0.0f;
							light_params->from.z = 0.0f;
						}

					err = tpbrt_params_list_get_point(params, &TO_STR, &light_params->to);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_light_source(light_source);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							light_params->to.x = 0.0f;
							light_params->to.y = 0.0f;
							light_params->to.z = 1.0f;
						}
					break;
				}
				case TPBRT_LIGHT_SOURCE_TYPE_GONIOMETRIC: {
					static const tpbrt_string_t FILE_NAME_STR			  = TPBRT_STRING("filename");
					static const tpbrt_string_t I_STR					  = TPBRT_STRING("I");

					tpbrt_light_source_goniometric_params_t* light_params = &light_source->as.goniometric;

					err = tpbrt_params_list_get_string(params, &FILE_NAME_STR, &light_params->file_name);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_light_source(light_source);
							return err;
						}

					err = tpbrt_params_list_get_spectrum(params, &I_STR, &light_params->i);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_light_source(light_source);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							light_params->i.type	   = TPBRT_SPECTRUM_TYPE_BUILTIN;
							light_params->i.as.builtin = current_color_space_illuminant;
						}
					break;
				}
				case TPBRT_LIGHT_SOURCE_TYPE_INFINITE: {
					static const tpbrt_string_t FILE_NAME_STR		   = TPBRT_STRING("filename");
					static const tpbrt_string_t PORTAL_STR			   = TPBRT_STRING("portal");
					static const tpbrt_string_t L_STR				   = TPBRT_STRING("L");

					tpbrt_light_source_infinite_params_t* light_params = &light_source->as.infinite;

					err = tpbrt_params_list_get_string(params, &FILE_NAME_STR, &light_params->file_name);
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
									tpbrt_free_vec3_array(&points);
									tpbrt_free_light_source(light_source);
									return TPBRT_ERROR_MISSING_REQUIRED_PARAMETER;
								}

								if (points.count > 4) {
									tpbrt_free_vec3_array(&points);
									tpbrt_free_light_source(light_source);
									return TPBRT_ERROR_TOO_MANY_VALUES;
								}

							light_params->portal[0] = points.data[0];
							light_params->portal[1] = points.data[1];
							light_params->portal[2] = points.data[2];
							light_params->portal[3] = points.data[3];
							tpbrt_free_vec3_array(&points);
						}

					err = tpbrt_params_list_get_spectrum(params, &L_STR, &light_params->l);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_light_source(light_source);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							light_params->l.type	   = TPBRT_SPECTRUM_TYPE_BUILTIN;
							light_params->l.as.builtin = current_color_space_illuminant;
						}
					break;
				}
				case TPBRT_LIGHT_SOURCE_TYPE_POINT: {
					static const tpbrt_string_t I_STR				= TPBRT_STRING("I");
					static const tpbrt_string_t FROM_STR			= TPBRT_STRING("from");

					tpbrt_light_source_point_params_t* light_params = &light_source->as.point;

					err = tpbrt_params_list_get_spectrum(params, &I_STR, &light_params->i);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_light_source(light_source);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							light_params->i.type	   = TPBRT_SPECTRUM_TYPE_BUILTIN;
							light_params->i.as.builtin = current_color_space_illuminant;
						}

					err = tpbrt_params_list_get_point(params, &FROM_STR, &light_params->from);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_light_source(light_source);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							light_params->from.x = 0.0f;
							light_params->from.y = 0.0f;
							light_params->from.z = 0.0f;
						}
					break;
				}
				case TPBRT_LIGHT_SOURCE_TYPE_PROJECTION: {
					static const tpbrt_string_t I_STR					 = TPBRT_STRING("I");
					static const tpbrt_string_t FOV_STR					 = TPBRT_STRING("fov");
					static const tpbrt_string_t FILE_NAME_STR			 = TPBRT_STRING("filename");

					tpbrt_light_source_projection_params_t* light_params = &light_source->as.projection;

					err = tpbrt_params_list_get_spectrum(params, &I_STR, &light_params->i);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_light_source(light_source);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							light_params->i.type	   = TPBRT_SPECTRUM_TYPE_BUILTIN;
							light_params->i.as.builtin = current_color_space_illuminant;
						}

					static const tpbrt_float_t FOV_DEFAULT = 90.0f;
					err = tpbrt_params_list_get_float(params, &FOV_STR, FOV_DEFAULT, &light_params->fov);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_light_source(light_source);
							return err;
						}

					err = tpbrt_params_list_get_string(params, &FILE_NAME_STR, &light_params->file_name);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_light_source(light_source);
							return err;
						}
					break;
				}
				case TPBRT_LIGHT_SOURCE_TYPE_SPOT: {
					static const tpbrt_string_t I_STR				 = TPBRT_STRING("I");
					static const tpbrt_string_t FROM_STR			 = TPBRT_STRING("from");
					static const tpbrt_string_t TO_STR				 = TPBRT_STRING("to");
					static const tpbrt_string_t CONE_ANGLE_STR		 = TPBRT_STRING("coneangle");
					static const tpbrt_string_t CONE_DELTA_ANGLE_STR = TPBRT_STRING("conedeltaangle");

					tpbrt_light_source_spot_params_t* light_params	 = &light_source->as.spot;

					err = tpbrt_params_list_get_spectrum(params, &I_STR, &light_params->i);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_light_source(light_source);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							light_params->i.type	   = TPBRT_SPECTRUM_TYPE_BUILTIN;
							light_params->i.as.builtin = current_color_space_illuminant;
						}

					err = tpbrt_params_list_get_point(params, &FROM_STR, &light_params->from);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_light_source(light_source);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							light_params->from.x = 0.0f;
							light_params->from.y = 0.0f;
							light_params->from.z = 0.0f;
						}

					err = tpbrt_params_list_get_point(params, &TO_STR, &light_params->to);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_light_source(light_source);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							light_params->to.x = 0.0f;
							light_params->to.y = 0.0f;
							light_params->to.z = 1.0f;
						}

					static const tpbrt_float_t CONE_ANGLE_DEFAULT = 30.0f;
					err = tpbrt_params_list_get_float(params, &CONE_ANGLE_STR, CONE_ANGLE_DEFAULT, &light_params->cone_angle);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_light_source(light_source);
							return err;
						}

					static const tpbrt_float_t CONE_DELTA_ANGLE_DEFAULT = 5.0f;
					err = tpbrt_params_list_get_float(params, &CONE_DELTA_ANGLE_STR, CONE_DELTA_ANGLE_DEFAULT,
					  &light_params->cone_delta_angle);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_light_source(light_source);
							return err;
						}
					break;
				}
			}

		return TPBRT_ERROR_NONE;
	}

	void tpbrt_free_light_source(tpbrt_light_source_t* const light_source) {
			if (light_source == TPBRT_NULL) { return; }

			switch (light_source->type) {
				case TPBRT_LIGHT_SOURCE_TYPE_DISTANT: {
					tpbrt_light_source_distant_params_t* params = &light_source->as.distant;

					tpbrt_free_spectrum(&params->l);
					break;
				}
				case TPBRT_LIGHT_SOURCE_TYPE_GONIOMETRIC: {
					tpbrt_light_source_goniometric_params_t* params = &light_source->as.goniometric;

					tpbrt_free_string(&params->file_name);
					tpbrt_free_spectrum(&params->i);
					break;
				}
				case TPBRT_LIGHT_SOURCE_TYPE_INFINITE: {
					tpbrt_light_source_infinite_params_t* params = &light_source->as.infinite;

					tpbrt_free_string(&params->file_name);
					tpbrt_free_spectrum(&params->l);
					break;
				}
				case TPBRT_LIGHT_SOURCE_TYPE_POINT: {
					tpbrt_light_source_point_params_t* params = &light_source->as.point;

					tpbrt_free_spectrum(&params->i);
					break;
				}
				case TPBRT_LIGHT_SOURCE_TYPE_PROJECTION: {
					tpbrt_light_source_projection_params_t* params = &light_source->as.projection;

					tpbrt_free_string(&params->file_name);
					tpbrt_free_spectrum(&params->i);
					break;
				}
				case TPBRT_LIGHT_SOURCE_TYPE_SPOT: {
					tpbrt_light_source_spot_params_t* params = &light_source->as.spot;

					tpbrt_free_spectrum(&params->i);
					break;
				}
			default: break;
			}
	}

#pragma endregion

#pragma region AREA_LIGHT

	static tpbrt_error_t tpbrt_area_light_type_from_string(const tpbrt_string_t* const type_str,
	  tpbrt_area_light_type_t* const type) {
		static const tpbrt_string_t TYPES_STRS[TPBRT_AREA_LIGHT_TYPE_MAX_NUM] = {
			TPBRT_STRING("diffuse"),
		};

			if (type_str == TPBRT_NULL || type_str->data == TPBRT_NULL || type == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

			for (tpbrt_area_light_type_t t = 0; t < TPBRT_AREA_LIGHT_TYPE_MAX_NUM; ++t) {
					if (tpbrt_string_equals(type_str, TYPES_STRS + t)) {
						*type = t;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_UNKNOWN_AREA_LIGHT_TYPE;
	}

	tpbrt_error_t tpbrt_create_area_light(const tpbrt_string_t* const type_str, const tpbrt_params_list_t* const params,
	  const tpbrt_color_space_t color_space, tpbrt_area_light_t* const area_light) {
			if (type_str == TPBRT_NULL || type_str->data == TPBRT_NULL || params == TPBRT_NULL || area_light == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

		tpbrt_error_t err = tpbrt_area_light_type_from_string(type_str, &area_light->type);
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

			switch (area_light->type) {
			default:
				case TPBRT_AREA_LIGHT_TYPE_DIFFUSE: {
					static const tpbrt_string_t FILE_NAME_STR			 = TPBRT_STRING("filename");
					static const tpbrt_string_t L_STR					 = TPBRT_STRING("L");
					static const tpbrt_string_t TWO_SIDED_STR			 = TPBRT_STRING("twosided");

					tpbrt_area_light_diffuse_params_t* area_light_params = &area_light->as.diffuse;

					err = tpbrt_params_list_get_string(params, &FILE_NAME_STR, &area_light_params->file_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_area_light(area_light);
							return err;
						}

					err = tpbrt_params_list_get_spectrum(params, &L_STR, &area_light_params->l);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_area_light(area_light);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							area_light_params->l.type		= TPBRT_SPECTRUM_TYPE_BUILTIN;
							area_light_params->l.as.builtin = current_color_space_illuminant;
						}

					err = tpbrt_params_list_get_bool(params, &TWO_SIDED_STR, TPBRT_FALSE, &area_light_params->two_sided);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_area_light(area_light);
							return err;
						}
					break;
				}
			}

		return TPBRT_ERROR_NONE;
	}

	void tpbrt_free_area_light(tpbrt_area_light_t* const area_light) {
			if (area_light == TPBRT_NULL) { return; }

			if (area_light->type == TPBRT_AREA_LIGHT_TYPE_DIFFUSE) {
				tpbrt_area_light_diffuse_params_t* params = &area_light->as.diffuse;

				tpbrt_free_string(&params->file_name);
				tpbrt_free_spectrum(&params->l);
			}
	}

#pragma endregion

#pragma region LIGHTS_LIST

	tpbrt_error_t tpbrt_create_empty_lights_list(tpbrt_lights_list_t** const lights_list) {
			if (lights_list == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		*lights_list = malloc(sizeof(tpbrt_lights_list_t));
			if (*lights_list == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }

		(*lights_list)->lights			  = TPBRT_NULL;
		(*lights_list)->lights_count	  = 0;
		(*lights_list)->area_lights		  = TPBRT_NULL;
		(*lights_list)->area_lights_count = 0;
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_lights_list_add_light_source(tpbrt_lights_list_t* const lights_list,
	  const tpbrt_light_source_t* const light_source) {
			if (light_source == TPBRT_NULL || lights_list == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (lights_list->lights == TPBRT_NULL) {
				lights_list->lights = malloc(sizeof(tpbrt_light_source_t));
					if (lights_list->lights == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }

				lights_list->lights[0]	  = *light_source;
				lights_list->lights_count = 1;
				return TPBRT_ERROR_NONE;
			}

		tpbrt_light_source_t* new_list = malloc(sizeof(tpbrt_light_source_t) * (lights_list->lights_count + 1));
			if (new_list == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }

			for (tpbrt_size_t i = 0; i < lights_list->lights_count; i++) { new_list[i] = lights_list->lights[i]; }
		new_list[lights_list->lights_count++] = *light_source;
		free(lights_list->lights);
		lights_list->lights = new_list;
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_lights_list_add_area_light(tpbrt_lights_list_t* const lights_list,
	  const tpbrt_area_light_t* const area_light) {
			if (area_light == TPBRT_NULL || lights_list == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (lights_list->area_lights == TPBRT_NULL) {
				lights_list->area_lights = malloc(sizeof(tpbrt_area_light_t));
					if (lights_list->area_lights == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }

				lights_list->area_lights[0]	   = *area_light;
				lights_list->area_lights_count = 1;
				return TPBRT_ERROR_NONE;
			}

		tpbrt_area_light_t* new_list = malloc(sizeof(tpbrt_area_light_t) * (lights_list->area_lights_count + 1));
			if (new_list == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }

			for (tpbrt_size_t i = 0; i < lights_list->area_lights_count; i++) { new_list[i] = lights_list->area_lights[i]; }
		new_list[lights_list->area_lights_count++] = *area_light;
		free(lights_list->area_lights);
		lights_list->area_lights = new_list;
		return TPBRT_ERROR_NONE;
	}

	void tpbrt_free_lights_list(tpbrt_lights_list_t* const lights_list) {
			if (lights_list == TPBRT_NULL) { return; }

			if (lights_list->lights != TPBRT_NULL) {
					for (tpbrt_size_t i = 0; i < lights_list->lights_count; ++i) {
						tpbrt_free_light_source(lights_list->lights + i);
					}

				free(lights_list->lights);
				lights_list->lights		  = TPBRT_NULL;
				lights_list->lights_count = 0;
			}

			if (lights_list->area_lights != TPBRT_NULL) {
					for (tpbrt_size_t i = 0; i < lights_list->area_lights_count; ++i) {
						tpbrt_free_area_light(lights_list->area_lights + i);
					}

				free(lights_list->area_lights);
				lights_list->area_lights	   = TPBRT_NULL;
				lights_list->area_lights_count = 0;
			}
	}

	tpbrt_size_t tpbrt_lights_list_size(const tpbrt_lights_list_t* const lights_list) {
		return lights_list != TPBRT_NULL ? lights_list->lights_count + lights_list->area_lights_count : 0;
	}

	tpbrt_bool_t tpbrt_lights_list_is_empty(const tpbrt_lights_list_t* const lights_list) {
		return tpbrt_lights_list_size(lights_list) == 0;
	}

#pragma endregion

#ifdef __cplusplus
}
#endif