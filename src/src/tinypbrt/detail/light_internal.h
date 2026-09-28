#pragma once
#ifndef _TINYPBRT_LIGHT_INTERNAL_H_
#define _TINYPBRT_LIGHT_INTERNAL_H_

#include <tinypbrt/detail/light.h>
#include <tinypbrt/detail/param_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

#pragma region LIGHT_SOURCE
	tpbrt_error_t tpbrt_light_source_type_from_string(tpbrt_string_t type_str, tpbrt_light_source_type_t* type);

	tpbrt_error_t tpbrt_light_source_init(tpbrt_string_t type_str, tpbrt_params_list_t params, tpbrt_color_space_t color_space,
	  const tpbrt_mat4_animated_t* ctm, tpbrt_media_handle_t interior_media_handle, tpbrt_media_handle_t exterior_media_handle,
	  tpbrt_light_source_t* light_source);
	void tpbrt_light_source_free(tpbrt_light_source_t* light_source);
#pragma endregion

#pragma region AREA_LIGHT
	tpbrt_error_t tpbrt_area_light_init(tpbrt_string_t type_str, tpbrt_params_list_t params, tpbrt_color_space_t color_space,
	  tpbrt_area_light_t* area_light);
	void tpbrt_area_light_free(tpbrt_area_light_t* area_light);
#pragma endregion

#pragma region LIGHTS_LIST
	tpbrt_error_t tpbrt_lights_list_init(tpbrt_lights_list_t* lights_list);
	tpbrt_error_t tpbrt_lights_list_append_light_source(tpbrt_lights_list_t* lights_list,
	  const tpbrt_light_source_t* light_source);
	tpbrt_error_t tpbrt_lights_list_append_area_light(tpbrt_lights_list_t* lights_list, tpbrt_area_light_handle_t* handle,
	  const tpbrt_area_light_t* area_light);
	void tpbrt_lights_list_free(tpbrt_lights_list_t* lights_list);

	tpbrt_bool_t tpbrt_lights_list_empty(const tpbrt_lights_list_t* lights_list);
#pragma endregion

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_LIGHT_INTERNAL_H_
