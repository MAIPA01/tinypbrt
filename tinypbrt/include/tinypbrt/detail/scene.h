#pragma once
#ifndef _TINYPBRT_SCENE_H_
#define _TINYPBRT_SCENE_H_

#include <tinypbrt/detail/common.h>
#include <tinypbrt/detail/coord.h>

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct {
		tpbrt_bool_t disable_pixel_jitter;
		tpbrt_bool_t disable_texture_filtering;
		tpbrt_bool_t disable_wave_length_jitter;
		tpbrt_float_t displacement_edge_scale;
		tpbrt_string_t mse_reference_image;
		tpbrt_string_t mse_reference_out;
		tpbrt_coordinate_system_t render_coord_sys;
		tpbrt_int_t seed;
		tpbrt_bool_t force_diffuse;
		tpbrt_bool_t pixel_stats;
		tpbrt_bool_t wavefront;
	} tpbrt_general_options_t;

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_SCENE_H_
