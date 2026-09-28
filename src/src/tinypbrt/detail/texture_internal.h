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

	tpbrt_error_t tpbrt_texture_type_from_string(tpbrt_string_t type_str, tpbrt_texture_type_t* type_out);

	tpbrt_error_t tpbrt_texture_class_from_string(tpbrt_string_t class_str, tpbrt_texture_class_t* class_out);

	tpbrt_error_t tpbrt_texture_wrap_from_string(tpbrt_string_t wrap_str, tpbrt_texture_wrap_t* wrap_out);

	tpbrt_error_t tpbrt_texture_filter_from_string(tpbrt_string_t filter_str, tpbrt_texture_filter_t* filter_out);

	tpbrt_error_t tpbrt_texture_encoding_from_string(tpbrt_string_t encoding_str, tpbrt_texture_encoding_t* encoding_out,
	  tpbrt_opt_float_t* gamma_val_out);

	tpbrt_error_t tpbrt_texture_mapping_from_string(tpbrt_string_t mapping_str, tpbrt_texture_mapping_t* mapping_out);

	tpbrt_error_t tpbrt_texture_init(tpbrt_string_t name, tpbrt_string_t type_str, tpbrt_string_t class_str,
	  tpbrt_params_list_t params, tpbrt_textures_list_t textures, const tpbrt_mat4_animated_t* ctm, tpbrt_texture_t* texture);
	void tpbrt_texture_free(tpbrt_texture_t* texture);

#pragma endregion

#pragma region TEXTURES_LIST
	tpbrt_error_t tpbrt_textures_list_init(tpbrt_textures_list_t* textures_list);
	tpbrt_error_t tpbrt_textures_list_append(tpbrt_textures_list_t* textures_list, const tpbrt_texture_t* texture);
	tpbrt_error_t tpbrt_textures_list_get(tpbrt_textures_list_t textures_list, tpbrt_string_t texture_name,
	  const tpbrt_texture_t** texture);
	tpbrt_error_t tpbrt_textures_list_get_handle(tpbrt_textures_list_t textures_list, tpbrt_string_t texture_name,
	  tpbrt_texture_handle_t* texture_handle);
	tpbrt_error_t tpbrt_textures_list_get_handle_with_type(tpbrt_textures_list_t textures_list, tpbrt_string_t texture_name,
	  tpbrt_texture_type_t type, tpbrt_texture_handle_t* texture_handle);
	void tpbrt_textures_list_free(tpbrt_textures_list_t* textures_list);

	tpbrt_bool_t tpbrt_textures_list_empty(tpbrt_textures_list_t textures_list);

#pragma endregion

#ifdef __cplusplus
}
#endif

#endif // _TINYPBRT_TEXTURE_INTERNAL_H_
