#pragma once
#ifndef _TINYPBRT_FWD_H_
#define _TINYPBRT_FWD_H_

#include <tinypbrt/detail/export.h>

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#pragma region COMMON_DEFINES
/**
 * @brief NULL value.
 * @ingroup tinypbrt-public
 */
#define TPBRT_NULL NULL

/**
 * @brief False value.
 * @ingroup tinypbrt-public
 */
#define TPBRT_FALSE 0
/**
 * @brief True value.
 * @ingroup tinypbrt-public
 */
#define TPBRT_TRUE 1

/**
 * @brief Anonymous union holding anonymous structs (`TPBRT_ANONYMOUS_UNION { struct { float x, y; }; float v[2]; }`).
 * Standard in C11, but anonymous structs are an extension in ISO C++: `__extension__` silences -Wpedantic
 * for GCC/Clang when the headers are included from C++ (the C++ wrapper or user code built with -Wpedantic).
 * @ingroup tinypbrt-public
 */
#if defined(__GNUC__) || defined(__clang__)
#define TPBRT_ANONYMOUS_UNION __extension__ union
#else
#define TPBRT_ANONYMOUS_UNION union
#endif

#pragma endregion

#pragma region COMMON
	/**
	 * @brief Char type.
	 * @ingroup common-public
	 */
	typedef char tpbrt_char_t;
	/**
	 * @brief Unsigned char type.
	 * @ingroup common-public
	 */
	typedef unsigned char tpbrt_uchar_t;

	typedef struct tpbrt_string_t tpbrt_string_t;

	typedef struct tpbrt_string_array_t tpbrt_string_array_t;
#pragma endregion

#pragma region MATH
	/**
	 * @brief Bool type.
	 * @ingroup math-public
	 */
	typedef uint8_t tpbrt_bool_t;
	/**
	 * @brief Int type.
	 * @ingroup math-public
	 */
	typedef int32_t tpbrt_int_t;
	/**
	 * @brief Unsigned int type.
	 * @ingroup math-public
	 */
	typedef uint32_t tpbrt_uint_t;
	/**
	 * @brief Float type.
	 * @ingroup math-public
	 */
	typedef float tpbrt_float_t;
	/**
	 * @brief Double type.
	 * @ingroup math-public
	 */
	typedef double tpbrt_double_t;
	/**
	 * @brief Size type.
	 * @ingroup math-public
	 */
	typedef size_t tpbrt_size_t;
	/**
	 * @brief Long type.
	 * @ingroup math-public
	 */
	typedef long tpbrt_long_t;
	/**
	 * @brief Unsigned long type.
	 * @ingroup math-public
	 */
	typedef unsigned long tpbrt_ulong_t;
	/**
	 * @brief Long long type.
	 * @ingroup math-public
	 */
	typedef long long tpbrt_longlong_t;
	/**
	 * @brief Unsigned long long type.
	 * @ingroup math-public
	 */
	typedef unsigned long long tpbrt_ulonglong_t;

	/**
	 * @brief Index type.
	 * @ingroup math-public
	 */
	typedef tpbrt_uint_t tpbrt_idx_t;
/**
 * @brief Invalid index.
 * @ingroup math-public
 */
#define TPBRT_IDX_INVALID (~(tpbrt_idx_t)0)

	typedef struct tpbrt_opt_float_t tpbrt_opt_float_t;


	typedef struct tpbrt_bool_array_t tpbrt_bool_array_t;

	typedef struct tpbrt_int_array_t tpbrt_int_array_t;

	typedef struct tpbrt_uint_array_t tpbrt_uint_array_t;

	typedef struct tpbrt_float_array_t tpbrt_float_array_t;


	typedef struct tpbrt_vec2_t tpbrt_vec2_t;
	/**
	 * @brief 2 component point.
	 * @ingroup math-public
	 */
	typedef tpbrt_vec2_t tpbrt_point2_t;


	typedef struct tpbrt_vec3_t tpbrt_vec3_t;
	/**
	 * @brief Vector.
	 * @ingroup math-public
	 */
	typedef tpbrt_vec3_t tpbrt_vec_t;
	/**
	 * @brief 3 component point.
	 * @ingroup math-public
	 */
	typedef tpbrt_vec3_t tpbrt_point3_t;
	/**
	 * @brief Point.
	 * @ingroup math-public
	 */
	typedef tpbrt_point3_t tpbrt_point_t;
	/**
	 * @brief 3 component normal.
	 * @ingroup math-public
	 */
	typedef tpbrt_vec3_t tpbrt_normal3_t;
	/**
	 * @brief Normal.
	 * @ingroup math-public
	 */
	typedef tpbrt_normal3_t tpbrt_normal_t;


	typedef struct tpbrt_vec4_t tpbrt_vec4_t;


	typedef struct tpbrt_vec2_array_t tpbrt_vec2_array_t;
	/**
	 * @brief Array of 2 component points.
	 * @ingroup math-public
	 */
	typedef tpbrt_vec2_array_t tpbrt_point2_array_t;


	typedef struct tpbrt_vec3_array_t tpbrt_vec3_array_t;
	/**
	 * @brief Array of vectors.
	 * @ingroup math-public
	 */
	typedef tpbrt_vec3_array_t tpbrt_vec_array_t;
	/**
	 * @brief Array of 3 component points.
	 * @ingroup math-public
	 */
	typedef tpbrt_vec3_array_t tpbrt_point3_array_t;
	/**
	 * @brief Array of points.
	 * @ingroup math-public
	 */
	typedef tpbrt_point3_array_t tpbrt_point_array_t;
	/**
	 * @brief Array of 3 component normals.
	 * @ingroup math-public
	 */
	typedef tpbrt_vec3_array_t tpbrt_normal3_array_t;
	/**
	 * @brief Array of normals.
	 * @ingroup math-public
	 */
	typedef tpbrt_normal3_array_t tpbrt_normal_array_t;

	/**
	 * @brief 4 by 4 matrix row type.
	 * @ingroup math-public
	 */
	typedef tpbrt_vec4_t tpbrt_mat4_row_t;

	typedef struct tpbrt_mat4_t tpbrt_mat4_t;

	typedef struct tpbrt_mat4_animated_t tpbrt_mat4_animated_t;
#pragma endregion

#pragma region TRANSFORM
	/**
	 * @brief Transform index.
	 * @ingroup transform-public
	 */
	typedef tpbrt_idx_t tpbrt_transform_idx_t;
/**
 * @brief Invalid transform index.
 * @ingroup transform-public
 */
#define TPBRT_TRANSFORM_IDX_INVALID TPBRT_IDX_INVALID

	typedef struct tpbrt_transform_handle_t tpbrt_transform_handle_t;

	typedef struct tpbrt_transform_animated_t tpbrt_transform_animated_t;

	typedef struct tpbrt_transforms_list_t tpbrt_transforms_list_t;
#pragma endregion

#pragma region TEXTURE
	/**
	 * @brief Texture index type.
	 * @ingroup texture-public
	 */
	typedef tpbrt_idx_t tpbrt_texture_idx_t;
/**
 * @brief Invalid texture index.
 * @ingroup texture-public
 */
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
	/**
	 * @brief Object index type.
	 * @ingroup object-public
	 */
	typedef tpbrt_idx_t tpbrt_object_idx_t;
/**
 * @brief Invalid object index.
 * @ingroup object-public
 */
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
	/**
	 * @brief Media index type.
	 * @ingroup media-public
	 */
	typedef tpbrt_idx_t tpbrt_media_idx_t;
/**
 * @brief Invalid media index.
 * @ingroup media-public
 */
#define TPBRT_MEDIA_IDX_INVALID TPBRT_IDX_INVALID

	typedef struct tpbrt_media_handle_t tpbrt_media_handle_t;

	typedef struct tpbrt_media_t tpbrt_media_t;

	typedef struct tpbrt_medias_list_t tpbrt_medias_list_t;
#pragma endregion

#pragma region MATERIAL
	/**
	 * @brief Material index type.
	 * @ingroup material-public
	 */
	typedef tpbrt_idx_t tpbrt_material_idx_t;
/**
 * @brief Invalid material index.
 * @ingroup material-public
 */
#define TPBRT_MATERIAL_IDX_INVALID TPBRT_IDX_INVALID

	typedef struct tpbrt_material_handle_t tpbrt_material_handle_t;

	typedef struct tpbrt_material_t tpbrt_material_t;

	typedef struct tpbrt_materials_list_t tpbrt_materials_list_t;
#pragma endregion

#pragma region LIGHT
	typedef struct tpbrt_light_source_t tpbrt_light_source_t;

	typedef struct tpbrt_lights_sources_list_t tpbrt_lights_sources_list_t;

	/**
	 * @brief Area light index type.
	 * @ingroup light-public
	 */
	typedef tpbrt_idx_t tpbrt_area_light_idx_t;
/**
 * @brief Invalid area light index.
 * @ingroup light-public
 */
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

	/**
	 * @brief Blackbody temperature type.
	 * @ingroup color-public
	 */
	typedef tpbrt_float_t tpbrt_blackbody_t;

	/**
	 * @brief RGB type.
	 * @ingroup color-public
	 */
	typedef tpbrt_vec3_t tpbrt_rgb_t;
	/**
	 * @brief Array of rgb type.
	 * @ingroup color-public
	 */
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
