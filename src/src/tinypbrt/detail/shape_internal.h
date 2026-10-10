#pragma once
#ifndef _TINYPBRT_SHAPE_INTERNAL_H_
#define _TINYPBRT_SHAPE_INTERNAL_H_

#include <tinypbrt/detail/fwd_internal.h>

#include <tinypbrt/detail/shape.h>

#ifdef __cplusplus
extern "C" {
#endif

#pragma region SHAPE

	tpbrt_error_t tpbrt_shape_type_from_string(tpbrt_string_t type_str, tpbrt_shape_type_t* type);

	tpbrt_error_t tpbrt_shape_curve_basis_from_string(tpbrt_string_t basis_str, tpbrt_shape_curve_basis_t* basis);

	tpbrt_error_t tpbrt_shape_curve_type_from_string(tpbrt_string_t type_str, tpbrt_shape_curve_type_t* type);

	tpbrt_error_t tpbrt_shape_init(tpbrt_string_t type_str, tpbrt_params_list_t params, tpbrt_textures_list_t textures,
	  const tpbrt_mat4_animated_t* ctm, tpbrt_material_handle_t material_handle, tpbrt_area_light_handle_t area_light_handle,
	  tpbrt_media_handle_t interior_media_handle, tpbrt_media_handle_t exterior_media_handle, tpbrt_bool_t reverse_orientation,
	  tpbrt_shape_t* shape);
	void tpbrt_shape_free(tpbrt_shape_t* shape);

#pragma endregion

#pragma region SHAPES_LIST
	tpbrt_error_t tpbrt_shapes_list_init(tpbrt_shapes_list_t* shapes_list);
	tpbrt_error_t tpbrt_shapes_list_append(tpbrt_shapes_list_t* shapes_list, const tpbrt_shape_t* shape);
	void tpbrt_shapes_list_free(tpbrt_shapes_list_t* shapes_list);

#pragma endregion

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_SHAPE_INTERNAL_H_
