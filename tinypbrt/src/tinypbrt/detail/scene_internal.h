#pragma once
#ifndef _TINYPBRT_SCENE_INTERNAL_H_
#define _TINYPBRT_SCENE_INTERNAL_H_

#include <tinypbrt/detail/scene.h>

#include <tinypbrt/detail/error.h>
#include <tinypbrt/detail/param_internal.h>
#include <tinypbrt/detail/parser_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

	tpbrt_error_t tpbrt_create_options(tpbrt_options_t** options);
	tpbrt_error_t tpbrt_options_apply(tpbrt_options_t* options, const tpbrt_param_t* param);
	void tpbrt_free_options(tpbrt_options_t** options);

	typedef struct {
		tpbrt_bool_t reverse_orientation;
		tpbrt_mat4_t ctm;
		tpbrt_color_space_t current_color_space;

		tpbrt_string_t current_inside_medium;
		tpbrt_string_t current_outside_medium;

		tpbrt_material_handle_t material_handle;

		tpbrt_object_t* active_object;

		tpbrt_params_list_t* shape_params;
		tpbrt_params_list_t* light_params;
		tpbrt_params_list_t* material_params;
		tpbrt_params_list_t* medium_params;
		tpbrt_params_list_t* texture_params;
	} tpbrt_state_t;

	tpbrt_error_t tpbrt_state_init_default(tpbrt_state_t* state);
	tpbrt_error_t tpbrt_state_clone(tpbrt_state_t* dest, const tpbrt_state_t* src);
	void tpbrt_state_free(tpbrt_state_t* state);

	typedef struct {
		tpbrt_string_t name;
		float transform[16];
	} tpbrt_coord_sys_entry_t;

	typedef struct {
		tpbrt_coord_sys_entry_t* entries;
		tpbrt_size_t count;
		tpbrt_size_t capacity;
	} tpbrt_coord_sys_map_t;

	tpbrt_error_t tpbrt_coord_sys_map_init(tpbrt_coord_sys_map_t* map);
	tpbrt_error_t tpbrt_coord_sys_map_insert(tpbrt_coord_sys_map_t* map, const tpbrt_string_t* name,
	  const tpbrt_mat4_t* transform);
	tpbrt_bool_t tpbrt_coord_sys_map_get(const tpbrt_coord_sys_map_t* map, const tpbrt_string_t* name,
	  tpbrt_mat4_t* out_transform);
	void tpbrt_coord_sys_map_free(const tpbrt_coord_sys_map_t* map);

	typedef struct {
		tpbrt_parser_t parser;
		char* file_data;
	} tpbrt_parser_node_t;

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_SCENE_INTERNAL_H_