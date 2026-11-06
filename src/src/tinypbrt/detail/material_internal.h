#pragma once
#ifndef _TINYPBRT_MATERIAL_INTERNAL_H_
#define _TINYPBRT_MATERIAL_INTERNAL_H_

#include <tinypbrt/detail/fwd_internal.h>

#include <tinypbrt/detail/name_index_internal.h>

#include <tinypbrt/detail/material.h>

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * @defgroup material-private Material
	 * @ingroup tinypbrt-private
	 */

#pragma region MATERIAL
	/**
	 * @brief Returns material type based on string value.
	 * @param type_str String which is convertable to material type.
	 * @param type Output material type value.
	 * @return Error code (most possible TPBRT_ERROR_INVALID_MATERIAL_TYPE, TPBRT_ERROR_NONE).
	 * @ingroup material-private
	 */
	tpbrt_error_t tpbrt_material_type_from_string(tpbrt_string_t type_str, tpbrt_material_type_t* type);

	/**
	 * @brief Initializes material.
	 * @param type_str Material type string.
	 * @param params Parameters list.
	 * @param textures Textures list.
	 * @param materials Materials list.
	 * @param material Output material data.
	 * @return Error code (most possible TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_MATERIAL_TYPE, TPBRT_ERROR_NONE).
	 * @ingroup material-private
	 */
	tpbrt_error_t tpbrt_material_init(tpbrt_string_t type_str, const tpbrt_params_list_t* params,
	  const tpbrt_textures_list_t* textures, const tpbrt_materials_list_t* materials, tpbrt_material_t* material);
	/**
	 * @brief Initializes named material.
	 * @param name Material name string.
	 * @param params Parameters list.
	 * @param textures Textures list.
	 * @param materials Materials list.
	 * @param material Output material data.
	 * @return Error code (most possible TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_MATERIAL_TYPE, TPBRT_ERROR_NONE).
	 * @ingroup material-private
	 */
	tpbrt_error_t tpbrt_material_init_named(tpbrt_string_t name, const tpbrt_params_list_t* params,
	  const tpbrt_textures_list_t* textures, const tpbrt_materials_list_t* materials, tpbrt_material_t* material);
	/**
	 * @brief Frees all material resources.
	 * @param material Material data containing resources to free.
	 * @ingroup material-private
	 */
	void tpbrt_material_free(tpbrt_material_t* material);
#pragma endregion

#pragma region MATERIALS_LIST
	/**
	 * @brief Initializes materials list.
	 * @param materials_list Materials list data.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
	 * @ingroup material-private
	 */
	tpbrt_error_t tpbrt_materials_list_init(tpbrt_materials_list_t* materials_list);
	/**
	 * @brief Adds material and returns handle to added material.
	 * @param materials_list Materials list data.
	 * @param material Material to add.
	 * @param handle Output handle.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_OUT_OF_MEMORY, TPBRT_ERROR_NONE).
	 * @ingroup material-private
	 */
	tpbrt_error_t tpbrt_materials_list_append(tpbrt_materials_list_t* materials_list, const tpbrt_material_t* material,
	  tpbrt_material_handle_t* handle);
	/**
	 * @brief Returns material from list with given name.
	 * @param materials_list Materials list data.
	 * @param material_name Material name.
	 * @param material Output material.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NOT_FOUND, TPBRT_ERROR_NONE).
	 * @ingroup material-private
	 */
	tpbrt_error_t tpbrt_materials_list_get(const tpbrt_materials_list_t* materials_list, tpbrt_string_t material_name,
	  const tpbrt_material_t** material);
	/**
	 * @brief Returns material handle from list with given name.
	 * @param materials_list Materials list data.
	 * @param material_name Material name.
	 * @param material_handle Material handle.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NOT_FOUND, TPBRT_ERROR_NONE).
	 * @ingroup material-private
	 */
	tpbrt_error_t tpbrt_materials_list_get_handle(const tpbrt_materials_list_t* materials_list, tpbrt_string_t material_name,
	  tpbrt_material_handle_t* material_handle);
	/**
	 * @brief Shrinks materials list to fit final size.
	 * @param materials_list Materials list data.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_OUT_OF_MEMORY, TPBRT_ERROR_NONE).
	 * @ingroup material-private
	 */
	tpbrt_error_t tpbrt_materials_list_shrink_to_fit(tpbrt_materials_list_t* materials_list);
	/**
	 * @brief Frees materials list and all materials.
	 * @param materials_list Materials list data to free.
	 * @ingroup material-private
	 */
	void tpbrt_materials_list_free(tpbrt_materials_list_t* materials_list);

	/**
	 * @brief Checks if materials list is empty.
	 * @param materials_list Materials list to check.
	 * @return TPBRT_TRUE if materials list is empty else TPBRT_FALSE.
	 * @ingroup material-private
	 */
	tpbrt_bool_t tpbrt_materials_list_empty(const tpbrt_materials_list_t* materials_list);
#pragma endregion

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_MATERIAL_INTERNAL_H_
