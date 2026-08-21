#pragma once
#ifndef _TINYPBRT_MATERIAL_INTERNAL_H_
#define _TINYPBRT_MATERIAL_INTERNAL_H_

#include <tinypbrt/detail/material.h>

#include <tinypbrt/detail/param_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

#pragma region MATERIAL
	tpbrt_error_t tpbrt_create_material(const tpbrt_string_t* type_str, const tpbrt_params_list_t* params,
	  const tpbrt_textures_list_t* textures, const tpbrt_materials_list_t* materials, tpbrt_material_t** material);
	tpbrt_error_t tpbrt_create_named_material(const tpbrt_string_t* name, const tpbrt_params_list_t* params,
	  const tpbrt_textures_list_t* textures, const tpbrt_materials_list_t* materials, tpbrt_material_t** material);
	void tpbrt_free_material(tpbrt_material_t** material);
#pragma endregion

#pragma region MATERIALS_LIST
	tpbrt_error_t tpbrt_create_empty_materials_list(tpbrt_materials_list_t** materials_list);
	tpbrt_error_t tpbrt_materials_list_add_material(tpbrt_materials_list_t* materials_list, const tpbrt_material_t* material,
	  tpbrt_material_handle_t* handle);
	tpbrt_error_t tpbrt_materials_list_get_material_by_name(const tpbrt_materials_list_t* materials_list,
	  const tpbrt_string_t* material_name, const tpbrt_material_t** material);
	tpbrt_error_t tpbrt_materials_list_get_material_handle(const tpbrt_materials_list_t* materials_list,
	  const tpbrt_string_t* material_name, tpbrt_material_handle_t* material_handle);
	void tpbrt_free_materials_list(tpbrt_materials_list_t** materials_list);

	tpbrt_size_t tpbrt_materials_list_size(const tpbrt_materials_list_t* materials_list);
	tpbrt_bool_t tpbrt_materials_list_is_empty(const tpbrt_materials_list_t* materials_list);
#pragma endregion

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_MATERIAL_INTERNAL_H_
