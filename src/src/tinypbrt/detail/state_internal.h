#pragma once
#ifndef _TINYPBRT_STATE_INTERNAL_H_
#define _TINYPBRT_STATE_INTERNAL_H_

#include <tinypbrt/detail/color_internal.h>
#include <tinypbrt/detail/common_internal.h>
#include <tinypbrt/detail/fwd.h>
#include <tinypbrt/detail/light_internal.h>
#include <tinypbrt/detail/material_internal.h>
#include <tinypbrt/detail/math_internal.h>
#include <tinypbrt/detail/media_internal.h>
#include <tinypbrt/detail/object_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

	typedef enum tpbrt_active_ctm_t : uint8_t {
		TPBRT_ACTIVE_CTM_ALL		= 0,
		TPBRT_ACTIVE_CTM_START_TIME = 1,
		TPBRT_ACTIVE_CTM_END_TIME	= 2,
	} tpbrt_active_ctm_t;

	typedef struct tpbrt_state_t {
		tpbrt_bool_t reverse_orientation;
		tpbrt_active_ctm_t active_ctm;
		tpbrt_mat4_animated_t ctm;
		tpbrt_color_space_t current_color_space;

		tpbrt_media_handle_t current_inside_medium;
		tpbrt_media_handle_t current_outside_medium;

		tpbrt_area_light_handle_t current_area_light;

		tpbrt_material_handle_t material_handle;

		tpbrt_object_t* active_object;

		tpbrt_params_list_t shape_params;
		tpbrt_params_list_t light_params;
		tpbrt_params_list_t material_params;
		tpbrt_params_list_t medium_params;
		tpbrt_params_list_t texture_params;
	} tpbrt_state_t;

	tpbrt_error_t tpbrt_state_init(tpbrt_state_t* state);
	tpbrt_error_t tpbrt_state_clone(tpbrt_state_t* dest, const tpbrt_state_t* src);
	void tpbrt_state_free(tpbrt_state_t* state);

	typedef struct tpbrt_states_stack_t {
		tpbrt_state_t* states;
		tpbrt_size_t count;
		tpbrt_size_t capacity;
	} tpbrt_states_stack_t;

	tpbrt_error_t tpbrt_states_stack_init(tpbrt_states_stack_t* stack);
	tpbrt_error_t tpbrt_states_stack_push(tpbrt_states_stack_t* stack, const tpbrt_state_t* state);
	tpbrt_error_t tpbrt_states_stack_pop(tpbrt_states_stack_t* stack, tpbrt_state_t* out_state);
	void tpbrt_states_stack_free(tpbrt_states_stack_t* stack);

#ifdef __cplusplus
}
#endif

#endif // _TINYPBRT_STATE_INTERNAL_H_
