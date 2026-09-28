#include <tinypbrt/pch.h>

#include <tinypbrt/detail/camera_internal.h>

#include <tinypbrt/detail/common_internal.h>
#include <tinypbrt/detail/math_internal.h>
#include <tinypbrt/detail/param_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

	tpbrt_error_t tpbrt_camera_type_from_string(const tpbrt_string_t type_str, tpbrt_camera_type_t* const type) {
		static const tpbrt_string_t TYPES_STRS[TPBRT_CAMERA_TYPE_MAX_NUM] = {
			TPBRT_STRING("orthographic"),
			TPBRT_STRING("perspective"),
			TPBRT_STRING("realistic"),
			TPBRT_STRING("spherical"),
		};

			if (type == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (tpbrt_string_empty(type_str)) { return TPBRT_ERROR_INVALID_CAMERA_TYPE; }

			for (tpbrt_camera_type_t t = 0; t < TPBRT_CAMERA_TYPE_MAX_NUM; ++t) {
					if (tpbrt_string_equals(type_str, TYPES_STRS[t])) {
						*type = t;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_INVALID_CAMERA_TYPE;
	}

	tpbrt_error_t tpbrt_camera_spherical_mapping_from_string(const tpbrt_string_t mapping_str,
	  tpbrt_camera_spherical_mapping_t* const mapping) {
		static const tpbrt_string_t MAPPINGS_STRS[TPBRT_CAMERA_SPHERICAL_MAPPING_MAX_NUM] = {
			TPBRT_STRING("equalarea"),
			TPBRT_STRING("equirectangular"),
		};

			if (mapping == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (tpbrt_string_empty(mapping_str)) { return TPBRT_ERROR_INVALID_CAMERA_SPHERICAL_MAPPING; }

			for (tpbrt_camera_spherical_mapping_t m = 0; m < TPBRT_CAMERA_SPHERICAL_MAPPING_MAX_NUM; ++m) {
					if (tpbrt_string_equals(mapping_str, MAPPINGS_STRS[m])) {
						*mapping = m;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_INVALID_CAMERA_SPHERICAL_MAPPING;
	}

	tpbrt_error_t tpbrt_camera_realistic_aperture_builtin_from_string(const tpbrt_string_t builtin_str,
	  tpbrt_camera_realistic_aperture_builtin_t* const builtin) {
		static const tpbrt_string_t BUILTINS_STRS[TPBRT_CAMERA_REALISTIC_APERTURE_BUILTIN_MAX_NUM] = {
			TPBRT_STRING("circular"),
			TPBRT_STRING("gaussian"),
			TPBRT_STRING("square"),
			TPBRT_STRING("pentagon"),
			TPBRT_STRING("star"),
		};

			if (builtin == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (tpbrt_string_empty(builtin_str)) { return TPBRT_ERROR_INVALID_CAMERA_REALISTIC_APERTURE_BUILTIN; }

			for (tpbrt_camera_realistic_aperture_builtin_t b = 0; b < TPBRT_CAMERA_REALISTIC_APERTURE_BUILTIN_MAX_NUM; ++b) {
					if (tpbrt_string_equals(builtin_str, BUILTINS_STRS[b])) {
						*builtin = b;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_INVALID_CAMERA_REALISTIC_APERTURE_BUILTIN;
	}

	tpbrt_error_t tpbrt_camera_init(const tpbrt_string_t type_str, const tpbrt_params_list_t params, const tpbrt_film_t* film,
	  const tpbrt_mat4_animated_t* const ctm, const tpbrt_media_handle_t interior_media_handle,
	  const tpbrt_media_handle_t exterior_media_handle, tpbrt_camera_t* const camera) {
			if (camera == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (memset(camera, 0, sizeof(tpbrt_camera_t)) == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		camera->transform							 = *ctm;
		camera->interior_media_handle				 = interior_media_handle;
		camera->exterior_media_handle				 = exterior_media_handle;

		static const tpbrt_string_t SHUTTER_OPEN_STR = TPBRT_STRING("shutteropen");
		tpbrt_error_t err = tpbrt_params_list_get_float(params, SHUTTER_OPEN_STR, 0.0f, &camera->shutter_open);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_camera_free(camera);
				return err;
			}

		static const tpbrt_string_t SHUTTER_CLOSE_STR = TPBRT_STRING("shutterclose");
		err = tpbrt_params_list_get_float(params, SHUTTER_CLOSE_STR, 1.0f, &camera->shutter_close);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_camera_free(camera);
				return err;
			}

		err = tpbrt_camera_type_from_string(type_str, &camera->type);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_camera_free(camera);
				return err;
			}

			if (camera->type == TPBRT_CAMERA_TYPE_ORTHOGRAPHIC || camera->type == TPBRT_CAMERA_TYPE_PERSPECTIVE) {
				static const tpbrt_string_t FRAME_ASPECT_RATIO_STR = TPBRT_STRING("frameaspectratio");
				const tpbrt_float_t default_aspect_ratio		   = film->x_resolution / film->y_resolution;

				static const tpbrt_string_t SCREEN_WINDOW_STR	   = TPBRT_STRING("screenwindow");
				static const tpbrt_string_t LENS_RADIUS_STR		   = TPBRT_STRING("lensradius");
				static const tpbrt_string_t FOCAL_DISTANCE_STR	   = TPBRT_STRING("focaldistance");

				err = tpbrt_params_list_get_float(params, FRAME_ASPECT_RATIO_STR, default_aspect_ratio,
				  camera->type == TPBRT_CAMERA_TYPE_ORTHOGRAPHIC ? &camera->as.orthographic.frame_aspect_ratio :
																   &camera->as.perspective.frame_aspect_ratio);
					if (err != TPBRT_ERROR_NONE) {
						tpbrt_camera_free(camera);
						return err;
					}

				tpbrt_float_array_t screen_window;
				err = tpbrt_params_list_get_floats(params, SCREEN_WINDOW_STR, &screen_window);
					if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
						tpbrt_camera_free(camera);
						return err;
					}
					if (err == TPBRT_ERROR_NOT_FOUND) {
						tpbrt_float_t* const camera_screen_window = camera->type == TPBRT_CAMERA_TYPE_ORTHOGRAPHIC ?
																	  camera->as.orthographic.screen_window :
																	  camera->as.perspective.screen_window;

							if (film->x_resolution < film->y_resolution) {
								const tpbrt_float_t aspect = film->y_resolution / film->x_resolution;

								camera_screen_window[0]	   = -1.0f;
								camera_screen_window[1]	   = 1.0f;
								camera_screen_window[2]	   = -aspect;
								camera_screen_window[3]	   = aspect;
							}
							else {
								const tpbrt_float_t aspect = film->x_resolution / film->y_resolution;

								camera_screen_window[0]	   = -aspect;
								camera_screen_window[1]	   = aspect;
								camera_screen_window[2]	   = -1.0f;
								camera_screen_window[3]	   = 1.0f;
							}
					}
					else {
							if (screen_window.count < 4) {
								tpbrt_float_array_free(&screen_window);
								tpbrt_camera_free(camera);
								return TPBRT_ERROR_MISSING_REQUIRED_PARAMETER;
							}

							if (screen_window.count > 4) {
								tpbrt_float_array_free(&screen_window);
								tpbrt_camera_free(camera);
								return TPBRT_ERROR_TOO_MANY_VALUES;
							}

						tpbrt_float_t* const camera_screen_window = camera->type == TPBRT_CAMERA_TYPE_ORTHOGRAPHIC ?
																	  camera->as.orthographic.screen_window :
																	  camera->as.perspective.screen_window;

							for (tpbrt_uint_t i = 0; i < 4; ++i) { camera_screen_window[i] = screen_window.data[i]; }
						tpbrt_float_array_free(&screen_window);
					}

				err = tpbrt_params_list_get_float(params, LENS_RADIUS_STR, 0.0f,
				  camera->type == TPBRT_CAMERA_TYPE_ORTHOGRAPHIC ? &camera->as.orthographic.lens_radius :
																   &camera->as.perspective.lens_radius);
					if (err != TPBRT_ERROR_NONE) {
						tpbrt_camera_free(camera);
						return err;
					}

				static const tpbrt_float_t FOCAL_DISTANCE_DEFAULT = 1e+30f;
				err = tpbrt_params_list_get_float(params, FOCAL_DISTANCE_STR, FOCAL_DISTANCE_DEFAULT,
				  camera->type == TPBRT_CAMERA_TYPE_ORTHOGRAPHIC ? &camera->as.orthographic.focal_distance :
																   &camera->as.perspective.focal_distance);
					if (err != TPBRT_ERROR_NONE) {
						tpbrt_camera_free(camera);
						return err;
					}
			}

			switch (camera->type) {
			default:
				case TPBRT_CAMERA_TYPE_ORTHOGRAPHIC: {
					break;
				}
				case TPBRT_CAMERA_TYPE_PERSPECTIVE: {
					static const tpbrt_string_t FOV_STR	   = TPBRT_STRING("fov");
					static const tpbrt_float_t FOV_DEFAULT = 90.0f;

					err = tpbrt_params_list_get_float(params, FOV_STR, FOV_DEFAULT, &camera->as.perspective.fov);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_camera_free(camera);
							return err;
						}
					break;
				}
				case TPBRT_CAMERA_TYPE_REALISTIC: {
					static const tpbrt_string_t LENS_FILE_STR		  = TPBRT_STRING("lensfile");
					static const tpbrt_string_t APERTURE_DIAMETER_STR = TPBRT_STRING("aperturediameter");
					static const tpbrt_string_t FOCUS_DISTANCE_STR	  = TPBRT_STRING("focusdistance");
					static const tpbrt_string_t APERTURE_STR		  = TPBRT_STRING("aperture");

					err = tpbrt_params_list_get_string(params, LENS_FILE_STR, &camera->as.realistic.lens_file);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_camera_free(camera);
							return err;
						}

					err =
					  tpbrt_params_list_get_float(params, APERTURE_DIAMETER_STR, 1.0f, &camera->as.realistic.aperture_diameter);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_camera_free(camera);
							return err;
						}

					static const tpbrt_float_t FOCUS_DISTANCE_DEFAULT = 10.0f;
					err = tpbrt_params_list_get_float(params, FOCUS_DISTANCE_STR, FOCUS_DISTANCE_DEFAULT,
					  &camera->as.realistic.focus_distance);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_camera_free(camera);
							return err;
						}

					tpbrt_string_t aperture_val;
					err = tpbrt_params_list_get_string(params, APERTURE_STR, &aperture_val);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_camera_free(camera);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							camera->as.realistic.aperture.type		 = TPBRT_CAMERA_REALISTIC_APERTURE_TYPE_BUILTIN;
							camera->as.realistic.aperture.as.builtin = TPBRT_CAMERA_REALISTIC_APERTURE_BUILTIN_CIRCULAR;
							break;
						}

					err = tpbrt_camera_realistic_aperture_builtin_from_string(aperture_val,
					  &camera->as.realistic.aperture.as.builtin);
						if (err == TPBRT_ERROR_NONE) {
							tpbrt_string_free(&aperture_val);
							camera->as.realistic.aperture.type = TPBRT_CAMERA_REALISTIC_APERTURE_TYPE_BUILTIN;
							break;
						}

					camera->as.realistic.aperture.type		   = TPBRT_CAMERA_REALISTIC_APERTURE_TYPE_FILE_NAME;
					camera->as.realistic.aperture.as.file_name = aperture_val;
					break;
				}
				case TPBRT_CAMERA_TYPE_SPHERICAL: {
					static const tpbrt_string_t MAPPING_STR = TPBRT_STRING("mapping");

					tpbrt_string_t mapping_val;
					err = tpbrt_params_list_get_string(params, MAPPING_STR, &mapping_val);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_camera_free(camera);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							camera->as.spherical.mapping = TPBRT_CAMERA_SPHERICAL_MAPPING_EQUAL_AREA;
							break;
						}

					err = tpbrt_camera_spherical_mapping_from_string(mapping_val, &camera->as.spherical.mapping);
					tpbrt_string_free(&mapping_val);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_camera_free(camera);
							return err;
						}
					break;
				}
			}

		return TPBRT_ERROR_NONE;
	}

	void tpbrt_camera_free(tpbrt_camera_t* const camera) {
			if (camera == TPBRT_NULL) { return; }

			if (camera->type == TPBRT_CAMERA_TYPE_REALISTIC) {
				tpbrt_string_free(&camera->as.realistic.lens_file);

					if (camera->as.realistic.aperture.type == TPBRT_CAMERA_REALISTIC_APERTURE_TYPE_FILE_NAME) {
						tpbrt_string_free(&camera->as.realistic.aperture.as.file_name);
					}
			}
	}

#ifdef __cplusplus
}
#endif