#pragma once
#ifndef _TINYPBRT_SCENE_H_
#define _TINYPBRT_SCENE_H_

#include <tinypbrt/detail/accelerator.h>
#include <tinypbrt/detail/camera.h>
#include <tinypbrt/detail/common.h>
#include <tinypbrt/detail/coord.h>
#include <tinypbrt/detail/error.h>
#include <tinypbrt/detail/film.h>
#include <tinypbrt/detail/filter.h>
#include <tinypbrt/detail/fwd.h>
#include <tinypbrt/detail/integrator.h>
#include <tinypbrt/detail/light.h>
#include <tinypbrt/detail/material.h>
#include <tinypbrt/detail/media.h>
#include <tinypbrt/detail/sampler.h>
#include <tinypbrt/detail/shape.h>
#include <tinypbrt/detail/texture.h>

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct tpbrt_options_t {
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
	} tpbrt_options_t;

	typedef struct tpbrt_scene_t {
		tpbrt_float_t start_time;
		tpbrt_float_t end_time;

		tpbrt_options_t options;

		tpbrt_camera_t camera;
		tpbrt_film_t film;
		tpbrt_integrator_t integrator;
		tpbrt_accelerator_t accelerator;
		tpbrt_sampler_t sampler;
		tpbrt_filter_t filter;

		tpbrt_textures_list_t textures;
		tpbrt_materials_list_t materials;
		tpbrt_medias_list_t medias;
		tpbrt_lights_list_t lights;
		tpbrt_objects_list_t objects;

		tpbrt_shape_array_t shapes;

		tpbrt_instances_list_t instances;

		tpbrt_string_array_t includes;
	} tpbrt_scene_t;

	tpbrt_error_t tpbrt_scene_load_from_file(const tpbrt_char_t* filepath, tpbrt_scene_t* out_scene);
	tpbrt_error_t tpbrt_scene_load_from_memory(const tpbrt_char_t* data, const tpbrt_char_t* working_directory,
	  tpbrt_scene_t* out_scene);
	void tpbrt_free_scene(tpbrt_scene_t* scene);

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_SCENE_H_
