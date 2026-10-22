#pragma once
#ifndef _TINYPBRT_TEXTURE_INTERNAL_H_
#define _TINYPBRT_TEXTURE_INTERNAL_H_

#include <tinypbrt/detail/fwd_internal.h>

#include <tinypbrt/detail/texture.h>

#include <tinypbrt/detail/color_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TPBRT_TEXTURE_HANDLE_TEXTURE(idx)                                                                           \
	(tpbrt_texture_handle_t) { .as.texture_idx = (idx), .value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_TEXTURE_IDX }
#define TPBRT_TEXTURE_HANDLE_SPECTRUM(spectrum_val)                                                                    \
	(tpbrt_texture_handle_t) { .as.spectrum = (spectrum_val), .value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_SPECTRUM }

#define TPBRT_TEXTURE_HANDLE_SPECTRUM_WAVELENGTHS \
	(tpbrt_texture_handle_t) {                    \
		.as.spectrum = { .as.wavelengths = { .data = TPBRT_NULL, .count = 0u }, .type = TPBRT_SPECTRUM_TYPE_WAVELENGTHS }, .value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_SPECTRUM                                         \
	}
#define TPBRT_TEXTURE_HANDLE_SPECTRUM_RGB(r_val, g_val, b_val) \
	(tpbrt_texture_handle_t) {                                 \
		.as.spectrum = { .as.rgb = { .r = (r_val), .g = (g_val), .b = (b_val) }, .type = TPBRT_SPECTRUM_TYPE_RGB }, .value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_SPECTRUM                                                      \
	}
#define TPBRT_TEXTURE_HANDLE_SPECTRUM_BLACKBODY(value)                                     \
	(tpbrt_texture_handle_t) {                                                             \
		.as.spectrum = { .as.blackbody = (value), .type = TPBRT_SPECTRUM_TYPE_BLACKBODY }, \
		.value_type	 = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_SPECTRUM                            \
	}
#define TPBRT_TEXTURE_HANDLE_SPECTRUM_FILE_NAME \
	(tpbrt_texture_handle_t) {                  \
		.as.spectrum = { .as.file_name = { .data = TPBRT_NULL, .size = 0u }, .type = TPBRT_SPECTRUM_TYPE_FILE }, .value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_SPECTRUM                                       \
	}
#define TPBRT_TEXTURE_HANDLE_SPECTRUM_BUILTIN(builtin_value)                                   \
	(tpbrt_texture_handle_t) {                                                                 \
		.as.spectrum = { .as.builtin = (builtin_value), .type = TPBRT_SPECTRUM_TYPE_BUILTIN }, \
		.value_type	 = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_SPECTRUM                                \
	}
#define TPBRT_TEXTURE_HANDLE_SPECTRUM_CONSTANT(value)                                    \
	(tpbrt_texture_handle_t) {                                                           \
		.as.spectrum = { .as.constant = (value), .type = TPBRT_SPECTRUM_TYPE_CONSTANT }, \
		.value_type	 = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_SPECTRUM                          \
	}

#pragma region TEXTURE

	void tpbrt_texture_handle_free(tpbrt_texture_handle_t* texture_handle);

	tpbrt_error_t tpbrt_texture_type_from_string(tpbrt_string_t type_str, tpbrt_texture_type_t* type_out);

	tpbrt_error_t tpbrt_texture_class_from_string(tpbrt_string_t class_str, tpbrt_texture_class_t* class_out);

	tpbrt_error_t tpbrt_texture_wrap_from_string(tpbrt_string_t wrap_str, tpbrt_texture_wrap_t* wrap_out);

	tpbrt_error_t tpbrt_texture_filter_from_string(tpbrt_string_t filter_str, tpbrt_texture_filter_t* filter_out);

	tpbrt_error_t tpbrt_texture_encoding_from_string(tpbrt_string_t encoding_str, tpbrt_texture_encoding_t* encoding_out,
	  tpbrt_opt_float_t* gamma_val_out);

	tpbrt_error_t tpbrt_texture_mapping_from_string(tpbrt_string_t mapping_str, tpbrt_texture_mapping_t* mapping_out);

	tpbrt_error_t tpbrt_texture_init(tpbrt_string_t name, tpbrt_string_t type_str, tpbrt_string_t class_str,
	  tpbrt_params_list_t params, tpbrt_textures_list_t textures, tpbrt_transform_handle_t transform, tpbrt_texture_t* texture);
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
