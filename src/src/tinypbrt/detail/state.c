#include <tinypbrt/pch.h>

#include <tinypbrt/detail/state_internal.h>

#include <tinypbrt/detail/color_internal.h>
#include <tinypbrt/detail/light_internal.h>
#include <tinypbrt/detail/material_internal.h>
#include <tinypbrt/detail/math_internal.h>
#include <tinypbrt/detail/media_internal.h>
#include <tinypbrt/detail/object_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

	tpbrt_error_t tpbrt_state_init(tpbrt_state_t* const state) {
			if (state == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (memset(state, 0, sizeof(tpbrt_state_t)) == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		state->reverse_orientation = TPBRT_FALSE;
		state->active_ctm		   = TPBRT_ACTIVE_CTM_ALL;
		tpbrt_mat4_identity(&state->ctm.start);
		tpbrt_mat4_identity(&state->ctm.end);
		state->current_color_space	  = TPBRT_COLOR_SPACE_SRGB;
		state->current_inside_medium  = TPBRT_MEDIA_HANDLE_INVALID;
		state->current_outside_medium = TPBRT_MEDIA_HANDLE_INVALID;
		state->current_area_light	  = TPBRT_AREA_LIGHT_HANDLE_INVALID;
		state->material_handle		  = TPBRT_MATERIAL_HANDLE_INVALID;
		state->active_object		  = TPBRT_NULL;
		state->type					  = TPBRT_STATE_TYPE_GLOBAL;

		tpbrt_error_t err			  = tpbrt_params_list_init(&state->shape_params);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_state_free(state);
				return err;
			}

		err = tpbrt_params_list_init(&state->light_params);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_state_free(state);
				return err;
			}

		err = tpbrt_params_list_init(&state->material_params);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_state_free(state);
				return err;
			}

		err = tpbrt_params_list_init(&state->medium_params);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_state_free(state);
				return err;
			}

		err = tpbrt_params_list_init(&state->texture_params);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_state_free(state);
				return err;
			}

		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_state_clone(tpbrt_state_t* const dest, const tpbrt_state_t* const src) {
			if (dest == TPBRT_NULL || src == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		*dest = *src;
		tpbrt_params_list_init(&dest->shape_params);
		tpbrt_params_list_extend(&dest->shape_params, src->shape_params);
		tpbrt_params_list_init(&dest->light_params);
		tpbrt_params_list_extend(&dest->light_params, src->light_params);
		tpbrt_params_list_init(&dest->material_params);
		tpbrt_params_list_extend(&dest->material_params, src->material_params);
		tpbrt_params_list_init(&dest->medium_params);
		tpbrt_params_list_extend(&dest->medium_params, src->medium_params);
		tpbrt_params_list_init(&dest->texture_params);
		tpbrt_params_list_extend(&dest->texture_params, src->texture_params);
		return TPBRT_ERROR_NONE;
	}

	void tpbrt_state_free(tpbrt_state_t* const state) {
			if (state == TPBRT_NULL) { return; }

		tpbrt_params_list_free(&state->shape_params);
		tpbrt_params_list_free(&state->light_params);
		tpbrt_params_list_free(&state->material_params);
		tpbrt_params_list_free(&state->medium_params);
		tpbrt_params_list_free(&state->texture_params);
	}

#define TPBRT_EMPTY_STATE_STACK (tpbrt_states_stack_t) { .states = TPBRT_NULL, .count = 0, .capacity = 0 }

	tpbrt_error_t tpbrt_states_stack_init(tpbrt_states_stack_t* const stack) {
			if (stack == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		*stack = TPBRT_EMPTY_STATE_STACK;
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_states_stack_push(tpbrt_states_stack_t* const stack, const tpbrt_state_t* const state) {
			if (stack == TPBRT_NULL || state == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (stack->count >= stack->capacity) {
				stack->capacity = stack->capacity == 0 ? 8 : stack->capacity * 2;
				stack->states	= realloc(stack->states, stack->capacity * sizeof(tpbrt_state_t));
			}
		stack->states[stack->count++] = *state;
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_states_stack_pop(tpbrt_states_stack_t* const stack, tpbrt_state_t* const out_state) {
			if (stack == TPBRT_NULL || out_state == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (stack->count == 0) { return TPBRT_ERROR_MISSING_REQUIRED_PARAMETER; }

		*out_state = stack->states[--stack->count];
		return TPBRT_ERROR_NONE;
	}

	void tpbrt_states_stack_free(tpbrt_states_stack_t* const stack) {
			if (stack == TPBRT_NULL) { return; }

			for (tpbrt_uint_t i = 0; i < stack->count; i++) { tpbrt_state_free(stack->states + i); }
		free(stack->states);

		*stack = TPBRT_EMPTY_STATE_STACK;
	}

#ifdef __cplusplus
}
#endif