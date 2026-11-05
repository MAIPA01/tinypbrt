#pragma once
#ifndef _TINYPBRT_STATE_INTERNAL_H_
#define _TINYPBRT_STATE_INTERNAL_H_

#include <tinypbrt/detail/fwd_internal.h>

#include <tinypbrt/detail/error.h>

#include <tinypbrt/detail/light_internal.h>
#include <tinypbrt/detail/material_internal.h>
#include <tinypbrt/detail/math_internal.h>
#include <tinypbrt/detail/media_internal.h>
#include <tinypbrt/detail/param_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * @defgroup state-private State
	 * @ingroup tinypbrt-private
	 */

	/**
	 * @brief Active ctm type. (ctm -> Current Transform Matrix).
	 * @ingroup state-private
	 */
	typedef enum tpbrt_active_ctm_t {
		/// @brief All ctms are active.
		TPBRT_ACTIVE_CTM_ALL		= 0,
		/// @brief Only start time ctm is active.
		TPBRT_ACTIVE_CTM_START_TIME = 1,
		/// @brief Only end time ctm is active.
		TPBRT_ACTIVE_CTM_END_TIME	= 2,
		/// @brief Number of all active ctms types.
		TPBRT_ACTIVE_CTM_MAX_NUM	= 3,
	} tpbrt_active_ctm_t;

	/**
	 * @brief State type.
	 * @ingroup state-private
	 */
	typedef enum tpbrt_state_type_t {
		/// @brief World state.
		TPBRT_STATE_TYPE_GLOBAL	   = 0,
		/// @brief ObjectBegin state. Ends with ObjectEnd.
		TPBRT_STATE_TYPE_OBJECT	   = 1,
		/// @brief AttributeBegin state. Ends with AttributeEnd.
		TPBRT_STATE_TYPE_ATTRIBUTE = 2,
		/// @brief Number of all state types.
		TPBRT_STATE_TYPE_MAX_NUM   = 3,
	} tpbrt_state_type_t;

	/**
	 * @brief State data.
	 * @ingroup state-private
	 */
	typedef struct tpbrt_state_t {
		/// @brief Current animated transform matrices.
		tpbrt_mat4_animated_t ctm;

		/// @brief Current interior medium.
		tpbrt_media_handle_t current_inside_medium;
		/// @brief Current exterior medium.
		tpbrt_media_handle_t current_outside_medium;

		/// @brief Current area light.
		tpbrt_area_light_handle_t current_area_light;

		/// @brief Current material.
		tpbrt_material_handle_t material_handle;

		/// @brief Current object.
		tpbrt_object_t* active_object;

		/// @brief Shape parameters list.
		tpbrt_params_list_t shape_params;
		/// @brief Light parameters list.
		tpbrt_params_list_t light_params;
		/// @brief Material parameters list.
		tpbrt_params_list_t material_params;
		/// @brief Medium parameters list.
		tpbrt_params_list_t medium_params;
		/// @brief Texture parameters list.
		tpbrt_params_list_t texture_params;

		/// @brief TPBRT_TRUE if reverse orientation is active.
		tpbrt_bool_t reverse_orientation		: 1;
		/// @brief Active ctm type.
		tpbrt_active_ctm_t active_ctm			: 3;
		/// @brief Current color space.
		tpbrt_color_space_t current_color_space : 4;
		/// @brief Current state type.
		tpbrt_state_type_t type					: 3;
	} tpbrt_state_t;

	/**
	 * @brief Initializes state data.
	 * @param state State data.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
	 * @ingroup state-private
	 */
	tpbrt_error_t tpbrt_state_init(tpbrt_state_t* state);
	/**
	 * @brief Clones state data.
	 * @param dest Destination state data.
	 * @param src Source state data.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
	 * @ingroup state-private
	 */
	tpbrt_error_t tpbrt_state_clone(tpbrt_state_t* dest, const tpbrt_state_t* src);
	/**
	 * @brief Frees state data.
	 * @param state State data to free.
	 * @ingroup state-private
	 */
	void tpbrt_state_free(tpbrt_state_t* state);

	/**
	 * @brief Stack of states.
	 * @ingroup state-private
	 */
	typedef struct tpbrt_states_stack_t {
		/// @brief All states on stack.
		tpbrt_state_t* states;
		/// @brief Number of states on stack.
		tpbrt_size_t count;
		/// @brief Capacity of stack.
		tpbrt_size_t capacity;
	} tpbrt_states_stack_t;

	/**
	 * @brief Initializes states stack.
	 * @param stack States stack data.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
	 * @ingroup state-private
	 */
	tpbrt_error_t tpbrt_states_stack_init(tpbrt_states_stack_t* stack);
	/**
	 * @brief Push state to stack.
	 * @param stack States stack data.
	 * @param state State to push on stack.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
	 * @ingroup state-private
	 */
	tpbrt_error_t tpbrt_states_stack_push(tpbrt_states_stack_t* stack, const tpbrt_state_t* state);
	/**
	 * @brief Pops state from stack.
	 * @param stack States stack data.
	 * @param out_state Output state data.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
	 * @ingroup state-private
	 */
	tpbrt_error_t tpbrt_states_stack_pop(tpbrt_states_stack_t* stack, tpbrt_state_t* out_state);
	/**
	 * @brief Frees states stack.
	 * @param stack States stack to free.
	 * @ingroup state-private
	 */
	void tpbrt_states_stack_free(tpbrt_states_stack_t* stack);

#ifdef __cplusplus
}
#endif

#endif // _TINYPBRT_STATE_INTERNAL_H_
