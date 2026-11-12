#pragma once
#ifndef _TINYPBRT_SHAPE_INTERNAL_H_
#define _TINYPBRT_SHAPE_INTERNAL_H_

#include <tinypbrt/detail/fwd_internal.h>

#include <tinypbrt/detail/shape.h>

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * @defgroup shape-private Shape
	 * @ingroup tinypbrt-private
	 */

#pragma region SHAPE

	/**
	 * @brief Returns shape type based on string value.
	 * @param type_str String which is convertible to shape type.
	 * @param type Output shape type value.
	 * @return Error code (most possible TPBRT_ERROR_INVALID_SHAPE_TYPE, TPBRT_ERROR_NONE).
	 * @ingroup shape-private
	 */
	tpbrt_error_t tpbrt_shape_type_from_string(tpbrt_string_t type_str, tpbrt_shape_type_t* type);

	/**
	 * @brief Returns shape curve basis based on string value.
	 * @param basis_str String which is convertible to shape curve basis.
	 * @param basis Output shape curve basis value.
	 * @return Error code (most possible TPBRT_ERROR_INVALID_SHAPE_CURVE_BASIS, TPBRT_ERROR_NONE).
	 * @ingroup shape-private
	 */
	tpbrt_error_t tpbrt_shape_curve_basis_from_string(tpbrt_string_t basis_str, tpbrt_shape_curve_basis_t* basis);

	/**
	 * @brief Returns shape curve type based on string value.
	 * @param type_str String which is convertible to shape curve type.
	 * @param type Output shape curve type value.
	 * @return Error code (most possible TPBRT_ERROR_INVALID_SHAPE_CURVE_TYPE, TPBRT_ERROR_NONE).
	 * @ingroup shape-private
	 */
	tpbrt_error_t tpbrt_shape_curve_type_from_string(tpbrt_string_t type_str, tpbrt_shape_curve_type_t* type);

	/**
	 * @brief Initializes shape.
	 * @param type_str Shape type string.
	 * @param params Parameters list.
	 * @param textures Textures list.
	 * @param transform Animated transform.
	 * @param material_handle Handle to material.
	 * @param area_light_handle Handle to area light.
	 * @param interior_media_handle Interior media handle.
	 * @param exterior_media_handle Exterior media handle.
	 * @param reverse_orientation Reverse shape orientation.
	 * @param shape Output shape data.
	 * @return Error code (most possible TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_SHAPE_TYPE, TPBRT_ERROR_INVALID_SHAPE_CURVE_BASIS, TPBRT_ERROR_INVALID_SHAPE_CURVE_TYPE, TPBRT_ERROR_NONE).
	 * @ingroup shape-private
	 */
	tpbrt_error_t tpbrt_shape_init(tpbrt_string_t type_str, const tpbrt_params_list_t* params,
	  const tpbrt_textures_list_t* textures, tpbrt_transform_animated_t transform, tpbrt_material_handle_t material_handle,
	  tpbrt_area_light_handle_t area_light_handle, tpbrt_media_handle_t interior_media_handle,
	  tpbrt_media_handle_t exterior_media_handle, tpbrt_bool_t reverse_orientation, tpbrt_shape_t* shape);
	/**
	 * @brief Frees all shape resources.
	 * @param shape Shape data containing resources to free.
	 * @ingroup shape-private
	 */
	void tpbrt_shape_free(tpbrt_shape_t* shape);

#pragma endregion

#pragma region SHAPES_LIST
	/**
	 * @brief Initializes shapes list.
	 * @param shapes_list Shapes list data.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
	 * @ingroup shape-private
	 */
	tpbrt_error_t tpbrt_shapes_list_init(tpbrt_shapes_list_t* shapes_list);
	/**
	 * @brief Adds shape to list.
	 * @param shapes_list Shapes list data.
	 * @param shape Shape to add.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_OUT_OF_MEMORY, TPBRT_ERROR_NONE).
	 * @ingroup shape-private
	 */
	tpbrt_error_t tpbrt_shapes_list_append(tpbrt_shapes_list_t* shapes_list, const tpbrt_shape_t* shape);
	/**
	 * @brief Shrinks list to minimal size.
	 * @param shapes_list Shapes list data.
	 * @ingroup shape-private
	 */
	void tpbrt_shapes_list_shrink_to_fit(tpbrt_shapes_list_t* shapes_list);
	/**
	 * @brief Frees shapes list.
	 * @param shapes_list Shapes list to free.
	 * @ingroup shape-private
	 */
	void tpbrt_shapes_list_free(tpbrt_shapes_list_t* shapes_list);

#pragma endregion

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_SHAPE_INTERNAL_H_
