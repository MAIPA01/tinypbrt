#pragma once
#ifndef _TINYPBRT_LIGHT_INTERNAL_H_
#define _TINYPBRT_LIGHT_INTERNAL_H_

#include <tinypbrt/detail/fwd_internal.h>

#include <tinypbrt/detail/light.h>

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * @defgroup light-private Light
	 * @ingroup tinypbrt-private
	 */

#pragma region LIGHT_SOURCE
	/**
	 * @brief Returns light source type based on string value.
	 * @param type_str String which is convertible to light source type.
	 * @param type Output light source type value.
	 * @return Error code (most possible TPBRT_ERROR_INVALID_LIGHT_SOURCE_TYPE, TPBRT_ERROR_NONE).
	 * @ingroup light-private
	 */
	tpbrt_error_t tpbrt_light_source_type_from_string(tpbrt_string_t type_str, tpbrt_light_source_type_t* type);

	/**
	 * @brief Initializes light source.
	 * @param type_str Light source type string.
	 * @param params Parameters list.
	 * @param color_space Active color space.
	 * @param transform Current transform.
	 * @param interior_media_handle Interior media handle.
	 * @param exterior_media_handle Exterior media handle.
	 * @param light_source Output light source data.
	 * @return Error code (most possible TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_LIGHT_SOURCE_TYPE, TPBRT_ERROR_NONE).
	 * @ingroup light-private
	 */
	tpbrt_error_t tpbrt_light_source_init(tpbrt_string_t type_str, const tpbrt_params_list_t* params,
	  tpbrt_color_space_t color_space, tpbrt_transform_handle_t transform, tpbrt_media_handle_t interior_media_handle,
	  tpbrt_media_handle_t exterior_media_handle, tpbrt_light_source_t* light_source);
	/**
	 * @brief Frees all light source resources.
	 * @param light_source Light source data containing resources to free.
	 * @ingroup light-private
	 */
	void tpbrt_light_source_free(tpbrt_light_source_t* light_source);
#pragma endregion

#pragma region AREA_LIGHT
/**
 * @brief Invalid area light handle initialization.
 * @ingroup light-private
 */
#define TPBRT_AREA_LIGHT_HANDLE_INVALID_INIT { .area_light_idx = TPBRT_AREA_LIGHT_IDX_INVALID }
/**
 * @brief Invalid area light handle initialization with type conversion.
 * @ingroup light-private
 */
#define TPBRT_AREA_LIGHT_HANDLE_INVALID (tpbrt_area_light_handle_t) TPBRT_AREA_LIGHT_HANDLE_INVALID_INIT

	/**
	 * @brief Returns area light type based on string value.
	 * @param type_str String which is convertible to area light type.
	 * @param type Output area light type value.
	 * @return Error code (most possible TPBRT_ERROR_INVALID_AREA_LIGHT_TYPE, TPBRT_ERROR_NONE).
	 * @ingroup light-private
	 */
	tpbrt_error_t tpbrt_area_light_type_from_string(tpbrt_string_t type_str, tpbrt_area_light_type_t* type);

	/**
	 * @brief Initializes area light.
	 * @param type_str Area light type string.
	 * @param params Parameters list.
	 * @param color_space Active color space.
	 * @param area_light Output area light data.
	 * @return Error code (most possible TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_AREA_LIGHT_TYPE, TPBRT_ERROR_NONE).
	 * @ingroup light-private
	 */
	tpbrt_error_t tpbrt_area_light_init(tpbrt_string_t type_str, const tpbrt_params_list_t* params,
	  tpbrt_color_space_t color_space, tpbrt_area_light_t* area_light);
	/**
	 * @brief Frees all area light resources.
	 * @param area_light Area light data containing resources to free.
	 * @ingroup light-private
	 */
	void tpbrt_area_light_free(tpbrt_area_light_t* area_light);
#pragma endregion

#pragma region LIGHTS_LIST
	/**
	 * @brief Initializes lights list.
	 * @param lights_list Lights list data.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
	 * @ingroup light-private
	 */
	tpbrt_error_t tpbrt_lights_list_init(tpbrt_lights_list_t* lights_list);
	/**
	 * @brief Adds light source.
	 * @param lights_list Lights list data.
	 * @param light_source Light source to add.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_OUT_OF_MEMORY, TPBRT_ERROR_NONE).
	 * @ingroup light-private
	 */
	tpbrt_error_t tpbrt_lights_list_append_light_source(tpbrt_lights_list_t* lights_list,
	  const tpbrt_light_source_t* light_source);
	/**
	 * @brief Adds area light and returns handle to added area light.
	 * @param lights_list Lights list data.
	 * @param handle Output handle.
	 * @param area_light Light to add.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_OUT_OF_MEMORY, TPBRT_ERROR_NONE).
	 * @ingroup light-private
	 */
	tpbrt_error_t tpbrt_lights_list_append_area_light(tpbrt_lights_list_t* lights_list, tpbrt_area_light_handle_t* handle,
	  const tpbrt_area_light_t* area_light);
	/**
	 * @brief Shrinks lights list to fit final size.
	 * @param lights_list Lights list data.
	 * @ingroup light-private
	 */
	void tpbrt_lights_list_shrink_to_fit(tpbrt_lights_list_t* lights_list);
	/**
	 * @brief Frees lights list and all lights.
	 * @param lights_list Lights list data to free.
	 * @ingroup light-private
	 */
	void tpbrt_lights_list_free(tpbrt_lights_list_t* lights_list);

	/**
	 * @brief Checks if lights list is empty.
	 * @param lights_list Lights list to check.
	 * @return TPBRT_TRUE if lights list is empty else TPBRT_FALSE.
	 * @ingroup light-private
	 */
	tpbrt_bool_t tpbrt_lights_list_empty(const tpbrt_lights_list_t* lights_list);
#pragma endregion

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_LIGHT_INTERNAL_H_
