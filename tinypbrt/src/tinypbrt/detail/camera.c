#include <tinypbrt/pch.h>

#include <tinypbrt/detail/camera_internal.h>

#include <tinypbrt/detail/param_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

	static tpbrt_error_t tpbrt_camera_type_from_string(const tpbrt_string_t* const type_str, tpbrt_camera_type_t* const type) {
		static const tpbrt_string_t TYPES_STRS[TPBRT_CAMERA_TYPE_MAX_NUM] = {
			{ .chars = "orthographic", .size = 12 },
			{ .chars = "perspective",  .size = 11 },
			{ .chars = "realistic",	.size = 9  },
			{ .chars = "spherical",	.size = 9  },
		};

			if (type_str == TPBRT_NULL || type_str->chars == TPBRT_NULL || type == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

			for (tpbrt_camera_type_t t = 0; t < TPBRT_CAMERA_TYPE_MAX_NUM; ++t) {
					if (type_str->size == TYPES_STRS[t].size &&
						strncmp(type_str->chars, TYPES_STRS[t].chars, TYPES_STRS[t].size) == 0) {
						*type = t;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_UNKNOWN_CAMERA_TYPE;
	}

	static tpbrt_error_t tpbrt_camera_spherical_mapping_from_string(const tpbrt_string_t* const mapping_str,
	  tpbrt_camera_spherical_mapping_t* const mapping) {
		static const tpbrt_string_t MAPPINGS_STRS[TPBRT_CAMERA_SPHERICAL_MAPPING_MAX_NUM] = {
			{ .chars = "equalarea",		.size = 12 },
			{ .chars = "equirectangular", .size = 11 },
		};

			if (mapping_str == TPBRT_NULL || mapping_str->chars == TPBRT_NULL || mapping == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

			for (tpbrt_camera_spherical_mapping_t m = 0; m < TPBRT_CAMERA_SPHERICAL_MAPPING_MAX_NUM; ++m) {
					if (mapping_str->size == MAPPINGS_STRS[m].size &&
						strncmp(mapping_str->chars, MAPPINGS_STRS[m].chars, MAPPINGS_STRS[m].size) == 0) {
						*mapping = m;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_UNKNOWN_CAMERA_SPHERICAL_MAPPING;
	}

	static tpbrt_error_t tpbrt_camera_realistic_aperture_builtin_from_string(const tpbrt_string_t* const builtin_str,
	  tpbrt_camera_realistic_aperture_builtin_t* const builtin) {
		static const tpbrt_string_t BUILTINS_STRS[TPBRT_CAMERA_REALISTIC_APERTURE_BUILTIN_MAX_NUM] = {
			{ .chars = "circular", .size = 8 },
			{ .chars = "gaussian", .size = 8 },
			{ .chars = "square",	 .size = 6 },
			{ .chars = "pentagon", .size = 8 },
			{ .chars = "star",	   .size = 4 },
		};

			if (builtin_str == TPBRT_NULL || builtin_str->chars == TPBRT_NULL || builtin == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

			for (tpbrt_camera_realistic_aperture_builtin_t b = 0; b < TPBRT_CAMERA_REALISTIC_APERTURE_BUILTIN_MAX_NUM; ++b) {
					if (builtin_str->size == BUILTINS_STRS[b].size &&
						strncmp(builtin_str->chars, BUILTINS_STRS[b].chars, BUILTINS_STRS[b].size) == 0) {
						*builtin = b;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_UNKNOWN_CAMERA_REALISTIC_APERTURE_BUILTIN;
	}

	tpbrt_error_t tpbrt_create_camera(const tpbrt_string_t* const type_str, const tpbrt_params_list_t* const params,
	  const tpbrt_film_t* film, const tpbrt_mat4_t* const ctm, tpbrt_camera_t** const camera) {
			if (type_str == TPBRT_NULL || type_str->chars == TPBRT_NULL || params == TPBRT_NULL || camera == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

		*camera = malloc(sizeof(tpbrt_camera_t));
			if (*camera == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }

		(*camera)->transform						 = *ctm;

		static const tpbrt_string_t SHUTTER_OPEN_STR = { .chars = "shutteropen", .size = 11 };
		tpbrt_error_t err = tpbrt_params_list_get_float(params, &SHUTTER_OPEN_STR, 0.0f, &(*camera)->shutter_open);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_free_camera(camera);
				return err;
			}

		static const tpbrt_string_t SHUTTER_CLOSE_STR = { .chars = "shutterclose", .size = 11 };
		err = tpbrt_params_list_get_float(params, &SHUTTER_CLOSE_STR, 1.0f, &(*camera)->shutter_close);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_free_camera(camera);
				return err;
			}

		err = tpbrt_camera_type_from_string(type_str, &(*camera)->type);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_free_camera(camera);
				return err;
			}

			if ((*camera)->type == TPBRT_CAMERA_TYPE_ORTHOGRAPHIC || (*camera)->type == TPBRT_CAMERA_TYPE_PERSPECTIVE) {
				static const tpbrt_string_t FRAME_ASPECT_RATIO_STR = { .chars = "frameaspectratio", .size = 16 };
				const tpbrt_float_t default_aspect_ratio		   = film->x_resolution / film->y_resolution;

				static const tpbrt_string_t SCREEN_WINDOW_STR	   = { .chars = "screenwindow", .size = 12 };
				static const tpbrt_string_t LENS_RADIUS_STR		   = { .chars = "lensradius", .size = 10 };
				static const tpbrt_string_t FOCAL_DISTANCE_STR	   = { .chars = "focaldistance", .size = 13 };

				err = tpbrt_params_list_get_float(params, &FRAME_ASPECT_RATIO_STR, default_aspect_ratio,
				  (*camera)->type == TPBRT_CAMERA_TYPE_ORTHOGRAPHIC ? &(*camera)->orthographic_params.frame_aspect_ratio :
																	  &(*camera)->perspective_params.frame_aspect_ratio);
					if (err != TPBRT_ERROR_NONE) {
						tpbrt_free_camera(camera);
						return err;
					}

				tpbrt_float_array_t screen_window;
				err = tpbrt_params_list_get_floats(params, &SCREEN_WINDOW_STR, &screen_window);
					if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
						tpbrt_free_camera(camera);
						return err;
					}
					if (err != TPBRT_ERROR_NOT_FOUND) {
							if (screen_window.count < 4) {
									if (screen_window.values != TPBRT_NULL) { free(screen_window.values); }
								tpbrt_free_camera(camera);
								return TPBRT_ERROR_MISSING_REQUIRED_PARAMETER;
							}

							if (screen_window.count > 4) {
								free(screen_window.values);
								tpbrt_free_camera(camera);
								return TPBRT_ERROR_TOO_MANY_VALUES;
							}

							if ((*camera)->type == TPBRT_CAMERA_TYPE_ORTHOGRAPHIC) {
								(*camera)->orthographic_params.screen_window[0] = screen_window.values[0];
								(*camera)->orthographic_params.screen_window[1] = screen_window.values[1];
								(*camera)->orthographic_params.screen_window[2] = screen_window.values[2];
								(*camera)->orthographic_params.screen_window[3] = screen_window.values[3];
							}
							else {
								(*camera)->perspective_params.screen_window[0] = screen_window.values[0];
								(*camera)->perspective_params.screen_window[1] = screen_window.values[1];
								(*camera)->perspective_params.screen_window[2] = screen_window.values[2];
								(*camera)->perspective_params.screen_window[3] = screen_window.values[3];
							}
						free(screen_window.values);
					}
					else {
							if (film->x_resolution < film->y_resolution) {
								const tpbrt_float_t aspect = film->y_resolution / film->x_resolution;

									if ((*camera)->type == TPBRT_CAMERA_TYPE_ORTHOGRAPHIC) {
										(*camera)->orthographic_params.screen_window[0] = -1.0f;
										(*camera)->orthographic_params.screen_window[1] = 1.0f;
										(*camera)->orthographic_params.screen_window[2] = -aspect;
										(*camera)->orthographic_params.screen_window[3] = aspect;
									}
									else {
										(*camera)->perspective_params.screen_window[0] = -1.0f;
										(*camera)->perspective_params.screen_window[1] = 1.0f;
										(*camera)->perspective_params.screen_window[2] = -aspect;
										(*camera)->perspective_params.screen_window[3] = aspect;
									}
							}
							else {
								const tpbrt_float_t aspect = film->x_resolution / film->y_resolution;

									if ((*camera)->type == TPBRT_CAMERA_TYPE_ORTHOGRAPHIC) {
										(*camera)->orthographic_params.screen_window[0] = -aspect;
										(*camera)->orthographic_params.screen_window[1] = aspect;
										(*camera)->orthographic_params.screen_window[2] = -1.0f;
										(*camera)->orthographic_params.screen_window[3] = 1.0f;
									}
									else {
										(*camera)->perspective_params.screen_window[0] = -aspect;
										(*camera)->perspective_params.screen_window[1] = aspect;
										(*camera)->perspective_params.screen_window[2] = -1.0f;
										(*camera)->perspective_params.screen_window[3] = 1.0f;
									}
							}
					}

				err = tpbrt_params_list_get_float(params, &LENS_RADIUS_STR, 0.0f,
				  (*camera)->type == TPBRT_CAMERA_TYPE_ORTHOGRAPHIC ? &(*camera)->orthographic_params.lens_radius :
																	  &(*camera)->perspective_params.lens_radius);
					if (err != TPBRT_ERROR_NONE) {
						tpbrt_free_camera(camera);
						return err;
					}

				static const tpbrt_float_t FOCAL_DISTANCE_DEFAULT = 1e+30f;
				err = tpbrt_params_list_get_float(params, &FOCAL_DISTANCE_STR, FOCAL_DISTANCE_DEFAULT,
				  (*camera)->type == TPBRT_CAMERA_TYPE_ORTHOGRAPHIC ? &(*camera)->orthographic_params.focal_distance :
																	  &(*camera)->perspective_params.focal_distance);
					if (err != TPBRT_ERROR_NONE) {
						tpbrt_free_camera(camera);
						return err;
					}
			}

			switch ((*camera)->type) {
			default:
				case TPBRT_CAMERA_TYPE_ORTHOGRAPHIC: {
					break;
				}
				case TPBRT_CAMERA_TYPE_PERSPECTIVE: {
					static const tpbrt_string_t FOV_STR	   = { .chars = "fov", .size = 3 };
					static const tpbrt_float_t FOV_DEFAULT = 90.0f;

					err = tpbrt_params_list_get_float(params, &FOV_STR, FOV_DEFAULT, &(*camera)->perspective_params.fov);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_camera(camera);
							return err;
						}
					break;
				}
				case TPBRT_CAMERA_TYPE_REALISTIC: {
					static const tpbrt_string_t LENS_FILE_STR		  = { .chars = "lensfile", .size = 8 };
					static const tpbrt_string_t APERTURE_DIAMETER_STR = { .chars = "aperturediameter", .size = 16 };
					static const tpbrt_string_t FOCUS_DISTANCE_STR	  = { .chars = "focusdistance", .size = 13 };
					static const tpbrt_string_t APERTURE_STR		  = { .chars = "aperture", .size = 8 };

					err = tpbrt_params_list_get_string(params, &LENS_FILE_STR, &(*camera)->realistic_params.lens_file);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_camera(camera);
							return err;
						}

					err = tpbrt_params_list_get_float(params, &APERTURE_DIAMETER_STR, 1.0f,
					  &(*camera)->realistic_params.aperture_diameter);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_camera(camera);
							return err;
						}

					static const tpbrt_float_t FOCUS_DISTANCE_DEFAULT = 10.0f;
					err = tpbrt_params_list_get_float(params, &FOCUS_DISTANCE_STR, FOCUS_DISTANCE_DEFAULT,
					  &(*camera)->realistic_params.focus_distance);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_camera(camera);
							return err;
						}

					tpbrt_string_t aperture_val;
					err = tpbrt_params_list_get_string(params, &APERTURE_STR, &aperture_val);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
								if (aperture_val.chars != TPBRT_NULL) { free(aperture_val.chars); }
							tpbrt_free_camera(camera);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							(*camera)->realistic_params.aperture.type	 = TPBRT_CAMERA_REALISTIC_APERTURE_TYPE_BUILTIN;
							(*camera)->realistic_params.aperture.builtin = TPBRT_CAMERA_REALISTIC_APERTURE_BUILTIN_CIRCULAR;
							break;
						}

					err = tpbrt_camera_realistic_aperture_builtin_from_string(&aperture_val,
					  &(*camera)->realistic_params.aperture.builtin);
						if (err == TPBRT_ERROR_NONE) {
							free(aperture_val.chars);
							(*camera)->realistic_params.aperture.type = TPBRT_CAMERA_REALISTIC_APERTURE_TYPE_BUILTIN;
							break;
						}

					(*camera)->realistic_params.aperture.type	   = TPBRT_CAMERA_REALISTIC_APERTURE_TYPE_FILE_NAME;
					(*camera)->realistic_params.aperture.file_name = aperture_val;
					break;
				}
				case TPBRT_CAMERA_TYPE_SPHERICAL: {
					static const tpbrt_string_t MAPPING_STR = { .chars = "mapping", .size = 7 };

					tpbrt_string_t mapping_val;
					err = tpbrt_params_list_get_string(params, &MAPPING_STR, &mapping_val);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							free(mapping_val.chars);
							tpbrt_free_camera(camera);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							(*camera)->spherical_params.mapping = TPBRT_CAMERA_SPHERICAL_MAPPING_EQUAL_AREA;
						}
						else {
							err = tpbrt_camera_spherical_mapping_from_string(&mapping_val, &(*camera)->spherical_params.mapping);
							free(mapping_val.chars);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_camera(camera);
									return err;
								}
						}
					break;
				}
			}

		return TPBRT_ERROR_NONE;
	}

	void tpbrt_free_camera(tpbrt_camera_t** const camera) {
			if (camera == TPBRT_NULL || *camera == TPBRT_NULL) { return; }

			if ((*camera)->type == TPBRT_CAMERA_TYPE_REALISTIC) {
					if ((*camera)->realistic_params.lens_file.chars != TPBRT_NULL) {
						free((*camera)->realistic_params.lens_file.chars);
					}

					if ((*camera)->realistic_params.aperture.type == TPBRT_CAMERA_REALISTIC_APERTURE_TYPE_FILE_NAME &&
						(*camera)->realistic_params.aperture.file_name.chars != TPBRT_NULL) {
						free((*camera)->realistic_params.aperture.file_name.chars);
					}
			}

		free(*camera);
		*camera = TPBRT_NULL;
	}

#ifdef __cplusplus
}
#endif