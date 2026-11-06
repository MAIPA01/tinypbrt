#pragma once
#ifndef _TINYPBRT_TEXTURE_INTERNAL_H_
#define _TINYPBRT_TEXTURE_INTERNAL_H_

#include <tinypbrt/detail/fwd_internal.h>

#include <tinypbrt/detail/name_index_internal.h>

#include <tinypbrt/detail/texture.h>

#include <tinypbrt/detail/color_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * @defgroup texture-private Texture
	 * @ingroup tinypbrt-private
	 */

/**
 * @brief Texture handle initialization with texture index.
 * @param idx Texture index.
 * @ingroup texture-private
 */
#define TPBRT_TEXTURE_HANDLE_TEXTURE_INIT(idx)                                             \
	{ .as.texture_idx = (idx), .value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_TEXTURE_IDX }
	/**
	 * @brief Texture handle initialization with texture index and with type conversion.
	 * @param idx Texture index.
	 * @ingroup texture-private
	 */
#define TPBRT_TEXTURE_HANDLE_TEXTURE(idx) (tpbrt_texture_handle_t) TPBRT_TEXTURE_HANDLE_TEXTURE_INIT(idx)

	/**
	 * @brief Texture handle initialization with spectrum.
	 * @param spectrum_val Spectrum value.
	 * @ingroup texture-private
	 */
#define TPBRT_TEXTURE_HANDLE_SPECTRUM_INIT(spectrum_val)                                    \
	{ .as.spectrum = spectrum_val, .value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_SPECTRUM }
	/**
	 * @brief Texture handle initialization with spectrum and with type conversion.
	 * @param spectrum_val Spectrum value.
	 * @ingroup texture-private
	 */
#define TPBRT_TEXTURE_HANDLE_SPECTRUM(spectrum_val) (tpbrt_texture_handle_t) TPBRT_TEXTURE_HANDLE_SPECTRUM_INIT(spectrum_val)

	/**
	 * @brief Texture handle initialization with empty wavelengths list.
	 * @ingroup texture-private
	 */
#define TPBRT_TEXTURE_HANDLE_SPECTRUM_WAVELENGTHS_INIT TPBRT_TEXTURE_HANDLE_SPECTRUM_INIT(TPBRT_SPECTRUM_WAVELENGTHS_INIT)
	/**
	 * @brief Texture handle initialization with empty wavelengths list and with type conversion.
	 * @ingroup texture-private
	 */
#define TPBRT_TEXTURE_HANDLE_SPECTRUM_WAVELENGTHS (tpbrt_texture_handle_t) TPBRT_TEXTURE_HANDLE_SPECTRUM_WAVELENGTHS_INIT

	/**
	 * @brief Texture handle initialization with rgb values.
	 * @param r_val Red value.
	 * @param g_val Green value.
	 * @param b_val Blue value.
	 * @ingroup texture-private
	 */
#define TPBRT_TEXTURE_HANDLE_SPECTRUM_RGB_INIT(r_val, g_val, b_val)                  \
	TPBRT_TEXTURE_HANDLE_SPECTRUM_INIT(TPBRT_SPECTRUM_RGB_INIT(r_val, g_val, b_val))
	/**
	 * @brief Texture handle initialization with rgb values and with type conversion.
	 * @param r_val Red value.
	 * @param g_val Green value.
	 * @param b_val Blue value.
	 * @ingroup texture-private
	 */
#define TPBRT_TEXTURE_HANDLE_SPECTRUM_RGB(r_val, g_val, b_val)                           \
	(tpbrt_texture_handle_t) TPBRT_TEXTURE_HANDLE_SPECTRUM_RGB_INIT(r_val, g_val, b_val)

	/**
	 * @brief Texture handle initialization with blackbody temperature.
	 * @param value Blackbody temperature value.
	 * @ingroup texture-private
	 */
#define TPBRT_TEXTURE_HANDLE_SPECTRUM_BLACKBODY_INIT(value)                  \
	TPBRT_TEXTURE_HANDLE_SPECTRUM_INIT(TPBRT_SPECTRUM_BLACKBODY_INIT(value))
	/**
	 * @brief Texture handle initialization with blackbody temperature and with type conversion.
	 * @param value Blackbody temperature value.
	 * @ingroup texture-private
	 */
#define TPBRT_TEXTURE_HANDLE_SPECTRUM_BLACKBODY(value)                           \
	(tpbrt_texture_handle_t) TPBRT_TEXTURE_HANDLE_SPECTRUM_BLACKBODY_INIT(value)

	/**
	 * @brief Texture handle initialization with empty filename.
	 * @ingroup texture-private
	 */
#define TPBRT_TEXTURE_HANDLE_SPECTRUM_FILE_NAME_INIT TPBRT_TEXTURE_HANDLE_SPECTRUM_INIT(TPBRT_SPECTRUM_FILE_NAME_INIT)
	/**
	 * @brief Texture handle initialization with filename and with type conversion.
	 * @ingroup texture-private
	 */
#define TPBRT_TEXTURE_HANDLE_SPECTRUM_FILE_NAME (tpbrt_texture_handle_t) TPBRT_TEXTURE_HANDLE_SPECTRUM_FILE_NAME_INIT

	/**
	 * @brief Texture handle initialization with spectrum builtin.
	 * @param builtin_value Spectrum builtin.
	 * @ingroup texture-private
	 */
#define TPBRT_TEXTURE_HANDLE_SPECTRUM_BUILTIN_INIT(builtin_value)                  \
	TPBRT_TEXTURE_HANDLE_SPECTRUM_INIT(TPBRT_SPECTRUM_BUILTIN_INIT(builtin_value))
	/**
	 * @brief Texture handle initialization with spectrum builtin and with type conversion.
	 * @param builtin_value Spectrum builtin.
	 * @ingroup texture-private
	 */
#define TPBRT_TEXTURE_HANDLE_SPECTRUM_BUILTIN(builtin_value)                           \
	(tpbrt_texture_handle_t) TPBRT_TEXTURE_HANDLE_SPECTRUM_BUILTIN_INIT(builtin_value)

	/**
	 * @brief Texture handle initialization with spectrum constant.
	 * @param value Constant value.
	 * @ingroup texture-private
	 */
#define TPBRT_TEXTURE_HANDLE_SPECTRUM_CONSTANT_INIT(value) TPBRT_TEXTURE_HANDLE_SPECTRUM_INIT(TPBRT_SPECTRUM_CONSTANT_INIT(value))
	/**
	 * @brief Texture handle initialization with spectrum constant and with type conversion.
	 * @param value Constant value.
	 * @ingroup texture-private
	 */
#define TPBRT_TEXTURE_HANDLE_SPECTRUM_CONSTANT(value) (tpbrt_texture_handle_t) TPBRT_TEXTURE_HANDLE_SPECTRUM_CONSTANT_INIT(value)

#pragma region TEXTURE

	/**
	 * @brief Frees texture handle data.
	 * @param texture_handle Texture handle data to free.
	 * @ingroup texture-private
	 */
	void tpbrt_texture_handle_free(tpbrt_texture_handle_t* texture_handle);

	/**
	 * @brief Returns texture type based on string value.
	 * @param type_str String which is convertable to texture type.
	 * @param type_out Output texture type value.
	 * @return Error code (most possible TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_TEXTURE_TYPE, TPBRT_ERROR_NONE).
	 * @ingroup texture-private
	 */
	tpbrt_error_t tpbrt_texture_type_from_string(tpbrt_string_t type_str, tpbrt_texture_type_t* type_out);

	/**
	 * @brief Returns texture class based on string value.
	 * @param class_str String which is convertable to texture class.
	 * @param class_out Output texture class value.
	 * @return Error code (most possible TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_TEXTURE_CLASS, TPBRT_ERROR_NONE).
	 * @ingroup texture-private
	 */
	tpbrt_error_t tpbrt_texture_class_from_string(tpbrt_string_t class_str, tpbrt_texture_class_t* class_out);

	/**
	 * @brief Returns texture wrap based on string value.
	 * @param wrap_str String which is convertable to texture wrap.
	 * @param wrap_out Output texture wrap value.
	 * @return Error code (most possible TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_TEXTURE_WRAP, TPBRT_ERROR_NONE).
	 * @ingroup texture-private
	 */
	tpbrt_error_t tpbrt_texture_wrap_from_string(tpbrt_string_t wrap_str, tpbrt_texture_wrap_t* wrap_out);

	/**
	 * @brief Returns texture filter based on string value.
	 * @param filter_str String which is convertable to texture filter.
	 * @param filter_out Output texture filter value.
	 * @return Error code (most possible TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_TEXTURE_FILTER, TPBRT_ERROR_NONE).
	 * @ingroup texture-private
	 */
	tpbrt_error_t tpbrt_texture_filter_from_string(tpbrt_string_t filter_str, tpbrt_texture_filter_t* filter_out);

	/**
	 * @brief Returns texture encoding based on string value.
	 * @param encoding_str String which is convertable to texture encoding.
	 * @param encoding_out Output texture encoding value.
	 * @param gamma_val_out Possible gamma value if encoding is (gamma val).
	 * @return Error code (most possible TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_TEXTURE_ENCODING, TPBRT_ERROR_NONE).
	 * @ingroup texture-private
	 */
	tpbrt_error_t tpbrt_texture_encoding_from_string(tpbrt_string_t encoding_str, tpbrt_texture_encoding_t* encoding_out,
	  tpbrt_opt_float_t* gamma_val_out);

	/**
	 * @brief Returns texture mapping based on string value.
	 * @param mapping_str String which is convertable to texture mapping.
	 * @param mapping_out Output texture mapping value.
	 * @return Error code (most possible TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_TEXTURE_MAPPING, TPBRT_ERROR_NONE).
	 * @ingroup texture-private
	 */
	tpbrt_error_t tpbrt_texture_mapping_from_string(tpbrt_string_t mapping_str, tpbrt_texture_mapping_t* mapping_out);

	/**
	 * @brief Initializes texture.
	 * @param name Texture name.
	 * @param type_str Texture type string.
	 * @param class_str Texture class string.
	 * @param params Parameters list.
	 * @param textures Textures list.
	 * @param transform Transform.
	 * @param texture Output texture data.
	 * @return Error code (most possible TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_TEXTURE_TYPE, TPBRT_ERROR_INVALID_TEXTURE_CLASS, TPBRT_ERROR_INVALID_TEXTURE_WRAP, TPBRT_ERROR_INVALID_TEXTURE_FILTER, TPBRT_ERROR_INVALID_TEXTURE_ENCODING, TPBRT_ERROR_INVALID_TEXTURE_MAPPING, TPBRT_ERROR_NONE).
	 * @ingroup texture-private
	 */
	tpbrt_error_t tpbrt_texture_init(tpbrt_string_t name, tpbrt_string_t type_str, tpbrt_string_t class_str,
	  const tpbrt_params_list_t* params, const tpbrt_textures_list_t* textures, tpbrt_transform_handle_t transform,
	  tpbrt_texture_t* texture);
	/**
	 * @brief Frees all texture resources.
	 * @param texture Texture data containing resources to free.
	 * @ingroup texture-private
	 */
	void tpbrt_texture_free(tpbrt_texture_t* texture);

#pragma endregion

#pragma region TEXTURES_LIST
	/**
	 * @brief Initialize textures list.
	 * @param textures_list Textures list data.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
	 * @ingroup texture-private
	 */
	tpbrt_error_t tpbrt_textures_list_init(tpbrt_textures_list_t* textures_list);
	/**
	 * @brief Add texture to list.
	 * @param textures_list Textures list data.
	 * @param texture Texture to add.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_OUT_OF_MEMORY, TPBRT_ERROR_NONE).
	 * @ingroup texture-private
	 */
	tpbrt_error_t tpbrt_textures_list_append(tpbrt_textures_list_t* textures_list, const tpbrt_texture_t* texture);
	/**
	 * @brief Returns texture with given name.
	 * @param textures_list Textures list data.
	 * @param texture_name Texture name.
	 * @param texture Returned texture if found.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NOT_FOUND, TPBRT_ERROR_NONE).
	 * @ingroup texture-private
	 */
	tpbrt_error_t tpbrt_textures_list_get(const tpbrt_textures_list_t* textures_list, tpbrt_string_t texture_name,
	  const tpbrt_texture_t** texture);
	/**
	 * @brief Returns texture handle to texture with given name.
	 * @param textures_list Textures list data.
	 * @param texture_name Texture name.
	 * @param texture_handle Returned texture handle.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NOT_FOUND, TPBRT_ERROR_NONE).
	 * @ingroup texture-private
	 */
	tpbrt_error_t tpbrt_textures_list_get_handle(const tpbrt_textures_list_t* textures_list, tpbrt_string_t texture_name,
	  tpbrt_texture_handle_t* texture_handle);
	/**
	 * @brief Returns texture handle to texture of given type with given name.
	 * @param textures_list Textures list data.
	 * @param texture_name Texture name.
	 * @param type Desired texture type.
	 * @param texture_handle Returned texture handle.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NOT_FOUND, TPBRT_ERROR_NONE).
	 * @ingroup texture-private
	 */
	tpbrt_error_t tpbrt_textures_list_get_handle_with_type(const tpbrt_textures_list_t* textures_list,
	  tpbrt_string_t texture_name, tpbrt_texture_type_t type, tpbrt_texture_handle_t* texture_handle);
	/**
	 * @brief Checks if texture of given type with given name is in list.
	 * @param textures_list Textures list data.
	 * @param texture_name Texture name.
	 * @param type Texture type.
	 * @return TPBRT_TRUE if texture exist else TPBRT_FALSE.
	 * @ingroup texture-private
	 */
	tpbrt_bool_t tpbrt_textures_list_contains(const tpbrt_textures_list_t* textures_list, tpbrt_string_t texture_name,
	  tpbrt_texture_type_t type);
	/**
	 * @brief Shrinks textures list data size to minimal required.
	 * @param textures_list Textures list data.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_OUT_OF_MEMORY, TPBRT_ERROR_NONE).
	 * @ingroup texture-private
	 */
	tpbrt_error_t tpbrt_textures_list_shrink_to_fit(tpbrt_textures_list_t* textures_list);
	/**
	 * @brief Frees textures list data.
	 * @param textures_list Textures list to free.
	 * @ingroup texture-private
	 */
	void tpbrt_textures_list_free(tpbrt_textures_list_t* textures_list);

	/**
	 * @brief Checks if textures list is empty.
	 * @param textures_list Textures list to check.
	 * @return TPBRT_TRUE if textures list is empty else TPBRT_FALSE.
	 * @ingroup texture-private
	 */
	tpbrt_bool_t tpbrt_textures_list_empty(const tpbrt_textures_list_t* textures_list);

#pragma endregion

#ifdef __cplusplus
}
#endif

#endif // _TINYPBRT_TEXTURE_INTERNAL_H_
