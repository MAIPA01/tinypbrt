#pragma once
#ifndef _TINYPBRT_LIGHT_INTERNAL_H_
#define _TINYPBRT_LIGHT_INTERNAL_H_

#include <tinypbrt/detail/light.h>
#include <tinypbrt/detail/param_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

#pragma region LIGHT_SOURCE
	tpbrt_error_t tpbrt_create_light_source(const tpbrt_string_t* type_str, const tpbrt_params_list_t* params,
	  tpbrt_color_space_t color_space, tpbrt_light_source_t** light_source);
	void tpbrt_free_light_source(tpbrt_light_source_t** light_source);
#pragma endregion

#pragma region AREA_LIGHT
	tpbrt_error_t tpbrt_create_area_light(const tpbrt_string_t* type_str, const tpbrt_params_list_t* params,
	  tpbrt_color_space_t color_space, tpbrt_area_light_t** area_light);
	void tpbrt_free_area_light(tpbrt_area_light_t** area_light);
#pragma endregion

#pragma region LIGHTS_LIST
	tpbrt_error_t tpbrt_create_empty_lights_list(tpbrt_lights_list_t** lights_list);
	tpbrt_error_t tpbrt_lights_list_add_light_source(tpbrt_lights_list_t* lights_list, const tpbrt_light_source_t* light_source);
	tpbrt_error_t tpbrt_lights_list_add_area_light(tpbrt_lights_list_t* lights_list, const tpbrt_area_light_t* area_light);
	void tpbrt_free_lights_list(tpbrt_lights_list_t** lights_list);

	tpbrt_size_t tpbrt_lights_list_size(const tpbrt_lights_list_t* lights_list);
	tpbrt_bool_t tpbrt_lights_list_is_empty(const tpbrt_lights_list_t* lights_list);
#pragma endregion

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_LIGHT_INTERNAL_H_
