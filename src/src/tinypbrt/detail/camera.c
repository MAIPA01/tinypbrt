#include <tinypbrt/pch.h>

#include <tinypbrt/detail/camera_internal.h>

#include <tinypbrt/detail/common_internal.h>
#include <tinypbrt/detail/film_internal.h>
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

	static tpbrt_error_t tpbrt_camera_read_image(tpbrt_float_t* const frame_aspect_ratio, tpbrt_float_t screen_window[4],
	  tpbrt_float_t* const lens_radius, tpbrt_float_t* const focal_distance, const tpbrt_film_t* const film,
	  const tpbrt_params_list_t params) {
			if (frame_aspect_ratio == TPBRT_NULL || screen_window == TPBRT_NULL || lens_radius == TPBRT_NULL ||
				focal_distance == TPBRT_NULL || film == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

		static const tpbrt_string_t FRAME_ASPECT_RATIO_STR = TPBRT_STRING("frameaspectratio");
		const tpbrt_float_t default_aspect_ratio	   = (tpbrt_float_t)film->x_resolution / (tpbrt_float_t)film->y_resolution;

		static const tpbrt_string_t SCREEN_WINDOW_STR  = TPBRT_STRING("screenwindow");
		static const tpbrt_string_t LENS_RADIUS_STR	   = TPBRT_STRING("lensradius");
		static const tpbrt_string_t FOCAL_DISTANCE_STR = TPBRT_STRING("focaldistance");

		tpbrt_error_t err = tpbrt_params_list_get_float(params, FRAME_ASPECT_RATIO_STR, default_aspect_ratio, frame_aspect_ratio);
			if (err != TPBRT_ERROR_NONE) { return err; }

		tpbrt_float_array_t temp_array;
		err = tpbrt_params_list_get_floats_of_size(params, SCREEN_WINDOW_STR, 4u, &temp_array);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

			if (err == TPBRT_ERROR_NOT_FOUND) {
					if (film->x_resolution < film->y_resolution) {
						const tpbrt_float_t aspect = 1.0f / default_aspect_ratio;

						screen_window[0]		   = -1.0f;
						screen_window[1]		   = 1.0f;
						screen_window[2]		   = -aspect;
						screen_window[3]		   = aspect;
					}
					else {
						const tpbrt_float_t aspect = default_aspect_ratio;

						screen_window[0]		   = -aspect;
						screen_window[1]		   = aspect;
						screen_window[2]		   = -1.0f;
						screen_window[3]		   = 1.0f;
					}
			}
			else {
				memcpy(screen_window, temp_array.data, sizeof(tpbrt_float_t) * 4u);
				tpbrt_float_array_free(&temp_array);
			}

		err = tpbrt_params_list_get_float(params, LENS_RADIUS_STR, 0.0f, lens_radius);
			if (err != TPBRT_ERROR_NONE) { return err; }

		static const tpbrt_float_t FOCAL_DISTANCE_DEFAULT = 1e+30f;
		return tpbrt_params_list_get_float(params, FOCAL_DISTANCE_STR, FOCAL_DISTANCE_DEFAULT, focal_distance);
	}

	static tpbrt_error_t tpbrt_camera_read_orthographic(tpbrt_camera_orthographic_params_t* const orthographic_params,
	  const tpbrt_film_t* const film, const tpbrt_params_list_t params) {
			if (orthographic_params == TPBRT_NULL || film == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		return tpbrt_camera_read_image(&orthographic_params->frame_aspect_ratio, orthographic_params->screen_window,
		  &orthographic_params->lens_radius, &orthographic_params->focal_distance, film, params);
	}

	static tpbrt_error_t tpbrt_camera_read_perspective(tpbrt_camera_perspective_params_t* const perspective_params,
	  const tpbrt_film_t* const film, const tpbrt_params_list_t params) {
			if (perspective_params == TPBRT_NULL || film == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t FOV_STR	   = TPBRT_STRING("fov");
		static const tpbrt_float_t FOV_DEFAULT = 90.0f;

		const tpbrt_error_t err				   = tpbrt_camera_read_image(&perspective_params->frame_aspect_ratio,
		  perspective_params->screen_window, &perspective_params->lens_radius, &perspective_params->focal_distance, film, params);
			if (err != TPBRT_ERROR_NONE) { return err; }

		return tpbrt_params_list_get_float(params, FOV_STR, FOV_DEFAULT, &perspective_params->fov);
	}

	static tpbrt_error_t tpbrt_camera_read_realistic(tpbrt_camera_realistic_params_t* const realistic_params,
	  const tpbrt_params_list_t params) {
			if (realistic_params == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t LENS_FILE_STR		  = TPBRT_STRING("lensfile");
		static const tpbrt_string_t APERTURE_DIAMETER_STR = TPBRT_STRING("aperturediameter");
		static const tpbrt_string_t FOCUS_DISTANCE_STR	  = TPBRT_STRING("focusdistance");
		static const tpbrt_string_t APERTURE_STR		  = TPBRT_STRING("aperture");

		tpbrt_error_t err = tpbrt_params_list_get_string(params, LENS_FILE_STR, &realistic_params->lens_file);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

		err = tpbrt_params_list_get_float(params, APERTURE_DIAMETER_STR, 1.0f, &realistic_params->aperture_diameter);
			if (err != TPBRT_ERROR_NONE) { return err; }

		static const tpbrt_float_t FOCUS_DISTANCE_DEFAULT = 10.0f;
		err = tpbrt_params_list_get_float(params, FOCUS_DISTANCE_STR, FOCUS_DISTANCE_DEFAULT, &realistic_params->focus_distance);
			if (err != TPBRT_ERROR_NONE) { return err; }

		tpbrt_string_t aperture_val;
		err = tpbrt_params_list_get_string(params, APERTURE_STR, &aperture_val);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

			if (err == TPBRT_ERROR_NOT_FOUND) {
				realistic_params->aperture.type		  = TPBRT_CAMERA_REALISTIC_APERTURE_TYPE_BUILTIN;
				realistic_params->aperture.as.builtin = TPBRT_CAMERA_REALISTIC_APERTURE_BUILTIN_CIRCULAR;
				return TPBRT_ERROR_NONE;
			}

		tpbrt_camera_realistic_aperture_builtin_t builtin;
		err = tpbrt_camera_realistic_aperture_builtin_from_string(aperture_val, &builtin);
			if (err == TPBRT_ERROR_NONE) {
				tpbrt_string_free(&aperture_val);
				realistic_params->aperture.as.builtin = builtin;
				realistic_params->aperture.type		  = TPBRT_CAMERA_REALISTIC_APERTURE_TYPE_BUILTIN;
				return TPBRT_ERROR_NONE;
			}

		realistic_params->aperture.type			= TPBRT_CAMERA_REALISTIC_APERTURE_TYPE_FILE_NAME;
		realistic_params->aperture.as.file_name = aperture_val;

		return TPBRT_ERROR_NONE;
	}

	static tpbrt_error_t tpbrt_camera_read_spherical(tpbrt_camera_spherical_params_t* const spherical_params,
	  const tpbrt_params_list_t params) {
			if (spherical_params == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t MAPPING_STR = TPBRT_STRING("mapping");

		tpbrt_string_t mapping_val;
		tpbrt_error_t err = tpbrt_params_list_get_string(params, MAPPING_STR, &mapping_val);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

			if (err == TPBRT_ERROR_NOT_FOUND) {
				spherical_params->mapping = TPBRT_CAMERA_SPHERICAL_MAPPING_EQUAL_AREA;
				return TPBRT_ERROR_NONE;
			}

		tpbrt_camera_spherical_mapping_t spherical_mapping;
		err = tpbrt_camera_spherical_mapping_from_string(mapping_val, &spherical_mapping);
		tpbrt_string_free(&mapping_val);
			if (err != TPBRT_ERROR_NONE) { return err; }

		spherical_params->mapping = spherical_mapping;

		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_camera_init_default(const tpbrt_film_t* const film, tpbrt_camera_t* const camera) {
			if (camera == TPBRT_NULL || film == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (memset(camera, 0, sizeof(tpbrt_camera_t)) == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		tpbrt_mat4_identity(&camera->transform.start);
		tpbrt_mat4_identity(&camera->transform.end);
		camera->interior_media_handle			  = TPBRT_MEDIA_HANDLE_INVALID;
		camera->exterior_media_handle			  = TPBRT_MEDIA_HANDLE_INVALID;
		camera->shutter_open					  = 0.0f;
		camera->shutter_close					  = 1.0f;

		const tpbrt_float_t default_aspect_ratio  = (tpbrt_float_t)film->x_resolution / (tpbrt_float_t)film->y_resolution;

		camera->type							  = TPBRT_CAMERA_TYPE_PERSPECTIVE;
		camera->as.perspective.fov				  = 90.0f;
		camera->as.perspective.frame_aspect_ratio = default_aspect_ratio;

			if (film->x_resolution < film->y_resolution) {
				const tpbrt_float_t aspect				= 1.0f / default_aspect_ratio;

				camera->as.perspective.screen_window[0] = -1.0f;
				camera->as.perspective.screen_window[1] = 1.0f;
				camera->as.perspective.screen_window[2] = -aspect;
				camera->as.perspective.screen_window[3] = aspect;
			}
			else {
				const tpbrt_float_t aspect				= default_aspect_ratio;

				camera->as.perspective.screen_window[0] = -aspect;
				camera->as.perspective.screen_window[1] = aspect;
				camera->as.perspective.screen_window[2] = -1.0f;
				camera->as.perspective.screen_window[3] = 1.0f;
			}

		camera->as.perspective.lens_radius	  = 0.0f;
		camera->as.perspective.focal_distance = 1e+30f;
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_camera_init(const tpbrt_string_t type_str, const tpbrt_params_list_t params,
	  const tpbrt_film_t* const film, const tpbrt_mat4_animated_t* const ctm, const tpbrt_media_handle_t interior_media_handle,
	  const tpbrt_media_handle_t exterior_media_handle, tpbrt_camera_t* const camera) {
			if (camera == TPBRT_NULL || film == TPBRT_NULL || ctm == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

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

		tpbrt_camera_type_t type;
		err = tpbrt_camera_type_from_string(type_str, &type);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_camera_free(camera);
				return err;
			}
		camera->type = type;

			switch (camera->type) {
			default:
				case TPBRT_CAMERA_TYPE_ORTHOGRAPHIC: {
					err = tpbrt_camera_read_orthographic(&camera->as.orthographic, film, params);
					break;
				}
				case TPBRT_CAMERA_TYPE_PERSPECTIVE: {
					err = tpbrt_camera_read_perspective(&camera->as.perspective, film, params);
					break;
				}
				case TPBRT_CAMERA_TYPE_REALISTIC: {
					err = tpbrt_camera_read_realistic(&camera->as.realistic, params);
					break;
				}
				case TPBRT_CAMERA_TYPE_SPHERICAL: {
					err = tpbrt_camera_read_spherical(&camera->as.spherical, params);
					break;
				}
			}

			if (err != TPBRT_ERROR_NONE) {
				tpbrt_camera_free(camera);
				return err;
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