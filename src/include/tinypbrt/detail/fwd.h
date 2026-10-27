#pragma once
#ifndef _TINYPBRT_FWD_H_
#define _TINYPBRT_FWD_H_

#include <tinypbrt/detail/export.h>

#include <math.h>
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
 * @brief Infinity.
 * @ingroup tinypbrt-public
 */
#define TPBRT_INFINITY INFINITY
/**
 * @brief PI.
 * @ingroup tinypbrt-public
 */
#define TPBRT_PI 3.14159265358979323846f
/**
 * @brief Epsilon.
 * @ingroup tinypbrt-public
 */
#define TPBRT_EPSILON 1e-6

/**
 * @brief Min int value.
 * @ingroup tinypbrt-public
 */
#define TPBRT_INT_MIN INT32_MIN
/**
 * @brief Max int value.
 * @ingroup tinypbrt-public
 */
#define TPBRT_INT_MAX INT32_MAX
/**
 * @brief Max uint value.
 * @ingroup tinypbrt-public
 */
#define TPBRT_UINT_MAX UINT32_MAX
/**
 * @brief Max float value.
 * @ingroup tinypbrt-public
 */
#define TPBRT_FLT_MAX FLT_MAX

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
	/**
	 * @brief String type.
	 * @ingroup common-public
	 */
	typedef struct tpbrt_string_t tpbrt_string_t;

	/**
	 * @brief Array of strings type.
	 * @ingroup common-public
	 */
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

	/**
	 * @brief Optional float type.
	 * @ingroup math-public
	 */
	typedef struct tpbrt_opt_float_t tpbrt_opt_float_t;

	/**
	 * @brief Array of bools.
	 * @ingroup math-public
	 */
	typedef struct tpbrt_bool_array_t tpbrt_bool_array_t;
	/**
	 * @brief Array of ints.
	 * @ingroup math-public
	 */
	typedef struct tpbrt_int_array_t tpbrt_int_array_t;
	/**
	 * @brief Array of unsigned ints.
	 * @ingroup math-public
	 */
	typedef struct tpbrt_uint_array_t tpbrt_uint_array_t;
	/**
	 * @brief Array of floats.
	 * @ingroup math-public
	 */
	typedef struct tpbrt_float_array_t tpbrt_float_array_t;

	/**
	 * @brief 2 component vector.
	 * @ingroup math-public
	 */
	typedef struct tpbrt_vec2_t tpbrt_vec2_t;
	/**
	 * @brief 2 component point.
	 * @ingroup math-public
	 */
	typedef tpbrt_vec2_t tpbrt_point2_t;

	/**
	 * @brief 3 component vector.
	 * @ingroup math-public
	 */
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

	/**
	 * @brief 4 component vector.
	 * @ingroup math-public
	 */
	typedef struct tpbrt_vec4_t tpbrt_vec4_t;

	/**
	 * @brief Array of 2 component vectors.
	 * @ingroup math-public
	 */
	typedef struct tpbrt_vec2_array_t tpbrt_vec2_array_t;
	/**
	 * @brief Array of 2 component points.
	 * @ingroup math-public
	 */
	typedef tpbrt_vec2_array_t tpbrt_point2_array_t;

	/**
	 * @brief Array of 3 component vectors.
	 * @ingroup math-public
	 */
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
	/**
	 * @brief 4 by 4 matrix type.
	 * @ingroup math-public
	 */
	typedef struct tpbrt_mat4_t tpbrt_mat4_t;
	/**
	 * @brief 4 by 4 animated matrix type.
	 * @ingroup math-public
	 */
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

	/**
	 * @brief Transform handle type.
	 * @ingroup transform-public
	 */
	typedef struct tpbrt_transform_handle_t tpbrt_transform_handle_t;

	/**
	 * @brief Animated transform type.
	 * @ingroup transform-public
	 */
	typedef struct tpbrt_transform_animated_t tpbrt_transform_animated_t;

	/**
	 * @brief List of transforms type.
	 * @ingroup transform-public
	 */
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

	/**
	 * @brief Texture handle type.
	 * @ingroup texture-public
	 */
	typedef struct tpbrt_texture_handle_t tpbrt_texture_handle_t;

	/**
	 * @brief Texture type.
	 * @ingroup texture-public
	 */
	typedef struct tpbrt_texture_t tpbrt_texture_t;
	/**
	 * @brief List of textures type.
	 * @ingroup texture-public
	 */
	typedef struct tpbrt_textures_list_t tpbrt_textures_list_t;
#pragma endregion

#pragma region SHAPE
	/**
	 * @brief Shape type.
	 * @ingroup shape-public
	 */
	typedef struct tpbrt_shape_t tpbrt_shape_t;
	/**
	 * @brief List of shapes type.
	 * @ingroup shape-public
	 */
	typedef struct tpbrt_shapes_list_t tpbrt_shapes_list_t;
#pragma endregion

#pragma region SCENE
	/**
	 * @brief Scene type.
	 * @ingroup scene-public
	 */
	typedef struct tpbrt_scene_t tpbrt_scene_t;
#pragma endregion

#pragma region SAMPLER
	/**
	 * @brief Sampler type.
	 * @ingroup sampler-public
	 */
	typedef struct tpbrt_sampler_t tpbrt_sampler_t;
#pragma endregion

#pragma region OPTIONS
	/**
	 * @brief Options type.
	 * @ingroup options-public
	 */
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

	/**
	 * @brief Object handle type.
	 * @ingroup object-public
	 */
	typedef struct tpbrt_object_handle_t tpbrt_object_handle_t;

	/**
	 * @brief Object type.
	 * @ingroup object-public
	 */
	typedef struct tpbrt_object_t tpbrt_object_t;
	/**
	 * @brief List of objects type.
	 * @ingroup object-public
	 */
	typedef struct tpbrt_objects_list_t tpbrt_objects_list_t;
#pragma endregion

#pragma region OBJECT_INSTANCE
	/**
	 * @brief Object instance type.
	 * @ingroup object-instance-public
	 */
	typedef struct tpbrt_object_instance_t tpbrt_object_instance_t;
	/**
	 * @brief List of objects instances type.
	 * @ingroup object-instance-public
	 */
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

	/**
	 * @brief Media handle type.
	 * @ingroup media-public
	 */
	typedef struct tpbrt_media_handle_t tpbrt_media_handle_t;

	/**
	 * @brief Media type.
	 * @ingroup media-public
	 */
	typedef struct tpbrt_media_t tpbrt_media_t;
	/**
	 * @brief List of medias type.
	 * @ingroup media-public
	 */
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

	/**
	 * @brief Material handle type.
	 * @ingroup material-public
	 */
	typedef struct tpbrt_material_handle_t tpbrt_material_handle_t;

	/**
	 * @brief Material type.
	 * @ingroup material-public
	 */
	typedef struct tpbrt_material_t tpbrt_material_t;
	/**
	 * @brief List of materials type.
	 * @ingroup material-public
	 */
	typedef struct tpbrt_materials_list_t tpbrt_materials_list_t;
#pragma endregion

#pragma region LIGHT
	/**
	 * @brief Light source type.
	 * @ingroup light-public
	 */
	typedef struct tpbrt_light_source_t tpbrt_light_source_t;
	/**
	 * @brief List of light sources type.
	 * @ingroup light-public
	 */
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

	/**
	 * @brief Area light handle type.
	 * @ingroup light-public
	 */
	typedef struct tpbrt_area_light_handle_t tpbrt_area_light_handle_t;

	/**
	 * @brief Area light type.
	 * @ingroup light-public
	 */
	typedef struct tpbrt_area_light_t tpbrt_area_light_t;
	/**
	 * @brief List of area lights type.
	 * @ingroup light-public
	 */
	typedef struct tpbrt_area_lights_list_t tpbrt_area_lights_list_t;

	/**
	 * @brief List of lights type.
	 * @ingroup light-public
	 */
	typedef struct tpbrt_lights_list_t tpbrt_lights_list_t;
#pragma endregion

#pragma region INTEGRATOR
	/**
	 * @brief Integrator type.
	 * @ingroup integrator-public
	 */
	typedef struct tpbrt_integrator_t tpbrt_integrator_t;
#pragma endregion

#pragma region FILTER
	/**
	 * @brief Filter type.
	 * @ingroup filter-public
	 */
	typedef struct tpbrt_filter_t tpbrt_filter_t;
#pragma endregion

#pragma region FILM
	/**
	 * @brief Film type.
	 * @ingroup film-public
	 */
	typedef struct tpbrt_film_t tpbrt_film_t;
#pragma endregion

#pragma region COLOR
	/**
	 * @brief Wavelength-value pair type.
	 * @ingroup color-public
	 */
	typedef struct tpbrt_wavelength_t tpbrt_wavelength_t;
	/**
	 * @brief Array of wavelengths type.
	 * @ingroup color-public
	 */
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

	/**
	 * @brief Spectrum type.
	 * @ingroup color-public
	 */
	typedef struct tpbrt_spectrum_t tpbrt_spectrum_t;
#pragma endregion

#pragma region CAMERA
	/**
	 * @brief Camera type.
	 * @ingroup camera-public
	 */
	typedef struct tpbrt_camera_t tpbrt_camera_t;
#pragma endregion

#pragma region ACCELERATOR
	/**
	 * @brief Accelerator type.
	 * @ingroup accelerator-public
	 */
	typedef struct tpbrt_accelerator_t tpbrt_accelerator_t;
#pragma endregion

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_FWD_H_
