#include <tinypbrt/pch.h>

#include <tinypbrt/detail/scene_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

	tpbrt_error_t tpbrt_create_options(tpbrt_options_t** options) {
			if (options == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		*options = malloc(sizeof(tpbrt_options_t));
			if (*options == NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }

		(*options)->disable_pixel_jitter	   = TPBRT_FALSE;
		(*options)->disable_texture_filtering  = TPBRT_FALSE;
		(*options)->disable_wave_length_jitter = TPBRT_FALSE;
		(*options)->displacement_edge_scale	   = 1;
		(*options)->mse_reference_image.chars  = TPBRT_NULL;
		(*options)->mse_reference_image.size   = 0;
		(*options)->mse_reference_out.chars	   = TPBRT_NULL;
		(*options)->mse_reference_out.size	   = 0;
		(*options)->render_coord_sys		   = TPBRT_COORDINATE_SYSTEM_CAMERA_WORLD;
		(*options)->seed					   = 0;
		(*options)->force_diffuse			   = TPBRT_FALSE;
		(*options)->pixel_stats				   = TPBRT_FALSE;
		(*options)->wavefront				   = TPBRT_FALSE;
		return TPBRT_ERROR_NONE;
	}

	void tpbrt_free_options(tpbrt_options_t** options) {
			if (options == TPBRT_NULL || *options == TPBRT_NULL) { return; }

			if ((*options)->mse_reference_image.chars != TPBRT_NULL) { free((*options)->mse_reference_image.chars); }

			if ((*options)->mse_reference_out.chars != TPBRT_NULL) { free((*options)->mse_reference_out.chars); }

		free(*options);
		*options = TPBRT_NULL;
	}

#ifdef __cplusplus
}
#endif