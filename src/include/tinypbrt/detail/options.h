#pragma once
#ifndef _TINYPBRT_OPTIONS_H_
#define _TINYPBRT_OPTIONS_H_

#include <tinypbrt/detail/fwd.h>

#include <tinypbrt/detail/common.h>
#include <tinypbrt/detail/coord.h>

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct tpbrt_options_t {
		tpbrt_string_t mse_reference_image;
		tpbrt_string_t mse_reference_out;
		tpbrt_int_t seed;
		tpbrt_float_t displacement_edge_scale;
		tpbrt_coord_system_type_t render_coord_sys : 2;
		tpbrt_bool_t disable_pixel_jitter		   : 1;
		tpbrt_bool_t disable_texture_filtering	   : 1;
		tpbrt_bool_t disable_wave_length_jitter	   : 1;
		tpbrt_bool_t force_diffuse				   : 1;
		tpbrt_bool_t pixel_stats				   : 1;
		tpbrt_bool_t wavefront					   : 1;
	} tpbrt_options_t;

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_OPTIONS_H_
