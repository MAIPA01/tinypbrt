#pragma once
#ifndef _TINYPBRT_MATERIAL_INTERNAL_H_
#define _TINYPBRT_MATERIAL_INTERNAL_H_

#include <tinypbrt/detail/fwd_internal.h>

#include <tinypbrt/detail/name_index_internal.h>

#include <tinypbrt/detail/material.h>

#ifdef __cplusplus
extern "C" {
#endif

#pragma region MATERIAL
	tpbrt_error_t tpbrt_material_type_from_string(tpbrt_string_t type_str, tpbrt_material_type_t* type);

	tpbrt_error_t tpbrt_material_init(tpbrt_string_t type_str, const tpbrt_params_list_t* params,
	  const tpbrt_textures_list_t* textures, const tpbrt_materials_list_t* materials, tpbrt_material_t* material);
	tpbrt_error_t tpbrt_material_init_named(tpbrt_string_t name, const tpbrt_params_list_t* params,
	  const tpbrt_textures_list_t* textures, const tpbrt_materials_list_t* materials, tpbrt_material_t* material);
	void tpbrt_material_free(tpbrt_material_t* material);
#pragma endregion

#pragma region MATERIALS_LIST
	tpbrt_error_t tpbrt_materials_list_init(tpbrt_materials_list_t* materials_list);
	tpbrt_error_t tpbrt_materials_list_append(tpbrt_materials_list_t* materials_list, const tpbrt_material_t* material,
	  tpbrt_material_handle_t* handle);
	tpbrt_error_t tpbrt_materials_list_get(const tpbrt_materials_list_t* materials_list, tpbrt_string_t material_name,
	  const tpbrt_material_t** material);
	tpbrt_error_t tpbrt_materials_list_get_handle(const tpbrt_materials_list_t* materials_list, tpbrt_string_t material_name,
	  tpbrt_material_handle_t* material_handle);
	tpbrt_error_t tpbrt_materials_list_shrink_to_fit(tpbrt_materials_list_t* materials_list);
	void tpbrt_materials_list_free(tpbrt_materials_list_t* materials_list);

	tpbrt_bool_t tpbrt_materials_list_empty(const tpbrt_materials_list_t* materials_list);
#pragma endregion

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_MATERIAL_INTERNAL_H_
