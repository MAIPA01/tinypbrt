#pragma once
#ifndef _TINYPBRT_TEXTURE_INTERNAL_H_
#define _TINYPBRT_TEXTURE_INTERNAL_H_

#include <tinypbrt/detail/texture.h>

#include <tinypbrt/detail/error.h>
#include <tinypbrt/detail/param_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

#pragma region TEXTURE
	tpbrt_error_t tpbrt_create_texture(const tpbrt_string_t* name, const tpbrt_string_t* type_str,
	  const tpbrt_string_t* class_str, const tpbrt_params_list_t* params, const tpbrt_textures_list_t* textures,
	  const tpbrt_mat4_animated_t* ctm, tpbrt_texture_t* texture);
	void tpbrt_free_texture(tpbrt_texture_t* texture);
#pragma endregion

#pragma region TEXTURES_LIST
	tpbrt_error_t tpbrt_init_textures_list(tpbrt_textures_list_t* textures_list);
	tpbrt_error_t tpbrt_textures_list_add_texture(tpbrt_textures_list_t* textures_list, const tpbrt_texture_t* texture);
	tpbrt_error_t tpbrt_textures_list_get_texture_by_name(const tpbrt_textures_list_t* textures_list,
	  const tpbrt_string_t* texture_name, const tpbrt_texture_t** texture);
	tpbrt_error_t tpbrt_textures_list_get_texture_handle(const tpbrt_textures_list_t* textures_list,
	  const tpbrt_string_t* texture_name, tpbrt_float_t default_val, tpbrt_texture_handle_t* texture_handle);
	tpbrt_error_t tpbrt_textures_list_get_opt_texture_handle_of_type(const tpbrt_textures_list_t* textures_list,
	  const tpbrt_string_t* texture_name, tpbrt_texture_type_t type, tpbrt_texture_handle_t* texture_handle);
	tpbrt_error_t tpbrt_textures_list_get_texture_handle_of_type(const tpbrt_textures_list_t* textures_list,
	  const tpbrt_string_t* texture_name, tpbrt_texture_type_t type, tpbrt_float_t default_val,
	  tpbrt_texture_handle_t* texture_handle);
	void tpbrt_free_textures_list(tpbrt_textures_list_t* textures_list);

	tpbrt_size_t tpbrt_textures_list_size(const tpbrt_textures_list_t* textures_list);
	tpbrt_bool_t tpbrt_textures_list_is_empty(const tpbrt_textures_list_t* textures_list);
#pragma endregion

#ifdef __cplusplus
}
#endif

#endif // _TINYPBRT_TEXTURE_INTERNAL_H_
