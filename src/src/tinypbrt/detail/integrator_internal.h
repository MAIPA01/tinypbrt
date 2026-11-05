#pragma once
#ifndef _TINYPBRT_INTEGRATOR_INTERNAL_H_
#define _TINYPBRT_INTEGRATOR_INTERNAL_H_

#include <tinypbrt/detail/fwd_internal.h>

#include <tinypbrt/detail/integrator.h>

#include <tinypbrt/detail/error.h>

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * @defgroup integrator-private Integrator
	 * @ingroup tinypbrt-private
	 */

	/**
	 * @brief Returns integrator type based on string value.
	 * @param type_str String which is convertable to integrator type.
	 * @param type Output integrator type value.
	 * @return Error code (most possible TPBRT_ERROR_INVALID_INTEGRATOR_TYPE, TPBRT_ERROR_NONE).
	 * @ingroup integrator-private
	 */
	tpbrt_error_t tpbrt_integrator_type_from_string(tpbrt_string_t type_str, tpbrt_integrator_type_t* type);

	/**
	 * @brief Returns integrator light sampler type based on string value.
	 * @param sampler_str String which is convertable to integrator light sampler type.
	 * @param sampler Output integrator light sampler type value.
	 * @return Error code (most possible TPBRT_ERROR_INVALID_INTEGRATOR_LIGHT_SAMPLER, TPBRT_ERROR_NONE).
	 * @ingroup integrator-private
	 */
	tpbrt_error_t tpbrt_integrator_light_sampler_from_string(tpbrt_string_t sampler_str,
	  tpbrt_integrator_light_sampler_t* sampler);

	/**
	 * @brief Initializes default integrator.
	 * @param integrator Output integrator data.
	 * @return Error code (most possible TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
	 * @ingroup integrator-private
	 */
	tpbrt_error_t tpbrt_integrator_init_default(tpbrt_integrator_t* integrator);

	/**
	 * @brief Initializes integrator.
	 * @param type_str Integrator type string.
	 * @param params Parameters list.
	 * @param integrator Output integrator data.
	 * @return Error code (most possible TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_INTEGRATOR_TYPE, TPBRT_ERROR_INVALID_INTEGRATOR_LIGHT_SAMPLER, TPBRT_ERROR_NONE).
	 * @ingroup integrator-private
	 */
	tpbrt_error_t tpbrt_integrator_init(tpbrt_string_t type_str, const tpbrt_params_list_t* params,
	  tpbrt_integrator_t* integrator);

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_INTEGRATOR_INTERNAL_H_
