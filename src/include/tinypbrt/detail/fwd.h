#pragma once
#ifndef _TINYPBRT_FWD_H_
#define _TINYPBRT_FWD_H_

#include <math.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#pragma region COMMON_DEFINES
#define TPBRT_NULL NULL

#define TPBRT_FALSE 0
#define TPBRT_TRUE	1

#define TPBRT_INFINITY INFINITY
#define TPBRT_PI	   M_PI
#define TPBRT_EPSILON  1e-6
#pragma endregion

#pragma region COMMON
	typedef char tpbrt_char_t;
	typedef struct tpbrt_string_t tpbrt_string_t;

	typedef struct tpbrt_string_array_t tpbrt_string_array_t;
#pragma endregion

#pragma region MATH
	typedef uint8_t tpbrt_bool_t;
	typedef int32_t tpbrt_int_t;
	typedef uint32_t tpbrt_uint_t;
	typedef float tpbrt_float_t;
	typedef size_t tpbrt_size_t;
	typedef long tpbrt_long_t;

	typedef tpbrt_size_t tpbrt_idx_t;
#define TPBRT_IDX_INVALID ~(tpbrt_idx_t)0

	typedef struct tpbrt_opt_float_t tpbrt_opt_float_t;

	typedef struct tpbrt_bool_array_t tpbrt_bool_array_t;
	typedef struct tpbrt_int_array_t tpbrt_int_array_t;
	typedef struct tpbrt_uint_array_t tpbrt_uint_array_t;
	typedef struct tpbrt_float_array_t tpbrt_float_array_t;

	typedef struct tpbrt_vec2_t tpbrt_vec2_t;
	typedef tpbrt_vec2_t tpbrt_point2_t;

	typedef struct tpbrt_vec3_t tpbrt_vec3_t;
	typedef tpbrt_vec3_t tpbrt_vec_t;
	typedef tpbrt_vec3_t tpbrt_point3_t;
	typedef tpbrt_point3_t tpbrt_point_t;
	typedef tpbrt_vec3_t tpbrt_normal3_t;
	typedef tpbrt_normal3_t tpbrt_normal_t;

	typedef struct tpbrt_vec2_array_t tpbrt_vec2_array_t;
	typedef tpbrt_vec2_array_t tpbrt_point2_array_t;

	typedef struct tpbrt_vec3_array_t tpbrt_vec3_array_t;
	typedef tpbrt_vec3_array_t tpbrt_vec_array_t;
	typedef tpbrt_vec3_array_t tpbrt_point3_array_t;
	typedef tpbrt_point3_array_t tpbrt_point_array_t;
	typedef tpbrt_vec3_array_t tpbrt_normal3_array_t;
	typedef tpbrt_normal3_array_t tpbrt_normal_array_t;

	typedef struct tpbrt_mat4_t tpbrt_mat4_t;
	typedef struct tpbrt_mat4_animated_t tpbrt_mat4_animated_t;
#pragma endregion

#pragma region TEXTURE
	typedef tpbrt_idx_t tpbrt_texture_idx_t;
#define TPBRT_TEXTURE_IDX_INVALID TPBRT_IDX_INVALID

	typedef struct tpbrt_texture_handle_t tpbrt_texture_handle_t;

	typedef struct tpbrt_texture_t tpbrt_texture_t;
	typedef struct tpbrt_textures_list_t tpbrt_textures_list_t;
#pragma endregion

#pragma region SHAPE
	typedef struct tpbrt_shape_t tpbrt_shape_t;
	typedef struct tpbrt_shapes_list_t tpbrt_shapes_list_t;
#pragma endregion

#pragma region SCENE
	typedef struct tpbrt_scene_t tpbrt_scene_t;
#pragma endregion

#pragma region SAMPLER
	typedef struct tpbrt_sampler_t tpbrt_sampler_t;
#pragma endregion

#pragma region OPTIONS
	typedef struct tpbrt_options_t tpbrt_options_t;
#pragma endregion

#pragma region OBJECT
	typedef tpbrt_idx_t tpbrt_object_idx_t;
#define TPBRT_OBJECT_IDX_INVALID TPBRT_IDX_INVALID

	typedef struct tpbrt_object_handle_t tpbrt_object_handle_t;

	typedef struct tpbrt_object_t tpbrt_object_t;
	typedef struct tpbrt_objects_list_t tpbrt_objects_list_t;
#pragma endregion

#pragma region OBJECT_INSTANCE
	typedef struct tpbrt_object_instance_t tpbrt_object_instance_t;
	typedef struct tpbrt_objects_instances_list_t tpbrt_objects_instances_list_t;
#pragma endregion

#pragma region MEDIA
	typedef tpbrt_idx_t tpbrt_media_idx_t;
#define TPBRT_MEDIA_IDX_INVALID TPBRT_IDX_INVALID

	typedef struct tpbrt_media_handle_t tpbrt_media_handle_t;

	typedef struct tpbrt_media_t tpbrt_media_t;
	typedef struct tpbrt_medias_list_t tpbrt_medias_list_t;
#pragma endregion

#pragma region MATERIAL
	typedef tpbrt_idx_t tpbrt_material_idx_t;
#define TPBRT_MATERIAL_IDX_INVALID TPBRT_IDX_INVALID

	typedef struct tpbrt_material_handle_t tpbrt_material_handle_t;

	typedef struct tpbrt_material_t tpbrt_material_t;
	typedef struct tpbrt_materials_list_t tpbrt_materials_list_t;
#pragma endregion

#pragma region LIGHT
	typedef struct tpbrt_light_source_t tpbrt_light_source_t;
	typedef struct tpbrt_lights_sources_list_t tpbrt_lights_sources_list_t;

	typedef tpbrt_idx_t tpbrt_area_light_idx_t;
#define TPBRT_AREA_LIGHT_IDX_INVALID TPBRT_IDX_INVALID

	typedef struct tpbrt_area_light_handle_t tpbrt_area_light_handle_t;

	typedef struct tpbrt_area_light_t tpbrt_area_light_t;
	typedef struct tpbrt_area_lights_list_t tpbrt_area_lights_list_t;

	typedef struct tpbrt_lights_list_t tpbrt_lights_list_t;
#pragma endregion

#pragma region INTEGRATOR
	typedef struct tpbrt_integrator_t tpbrt_integrator_t;
#pragma endregion

#pragma region FILTER
	typedef struct tpbrt_filter_t tpbrt_filter_t;
#pragma endregion

#pragma region FILM
	typedef struct tpbrt_film_t tpbrt_film_t;
#pragma endregion

#pragma region COLOR
	typedef struct tpbrt_wavelength_t tpbrt_wavelength_t;
	typedef struct tpbrt_wavelength_array_t tpbrt_wavelength_array_t;

	typedef tpbrt_float_t tpbrt_blackbody_t;

	typedef tpbrt_vec3_t tpbrt_rgb_t;
	typedef tpbrt_vec3_array_t tpbrt_rgb_array_t;

	typedef struct tpbrt_spectrum_t tpbrt_spectrum_t;
#pragma endregion

#pragma region CAMERA
	typedef struct tpbrt_camera_t tpbrt_camera_t;
#pragma endregion

#pragma region ACCELERATOR
	typedef struct tpbrt_accelerator_t tpbrt_accelerator_t;
#pragma endregion

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_FWD_H_