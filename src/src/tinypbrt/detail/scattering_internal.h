#pragma once
#ifndef _TINYPBRT_SCATTERING_INTERNAL_H_
#define _TINYPBRT_SCATTERING_INTERNAL_H_

#include <tinypbrt/detail/fwd_internal.h>

#include <tinypbrt/detail/scattering.h>

#include <tinypbrt/detail/error.h>

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * @defgroup scattering-private Scattering
	 * @ingroup tinypbrt-private
	 */

	/**
	 * @brief Returns scattering builtin based on string value.
	 * @param builtin_str String which is convertible to scattering builtin.
	 * @param builtin Output scattering builtin value.
	 * @return Error code (most possible TPBRT_ERROR_INVALID_SCATTERING_BUILTIN, TPBRT_ERROR_NONE).
	 * @ingroup scattering-private
	 */
	tpbrt_error_t tpbrt_scattering_builtin_from_string(tpbrt_string_t builtin_str, tpbrt_scattering_builtin_t* builtin);

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_SCATTERING_INTERNAL_H_
