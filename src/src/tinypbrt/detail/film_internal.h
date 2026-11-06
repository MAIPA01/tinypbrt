#pragma once
#ifndef _TINYPBRT_FILM_INTERNAL_H_
#define _TINYPBRT_FILM_INTERNAL_H_

#include <tinypbrt/detail/fwd_internal.h>

#include <tinypbrt/detail/film.h>

#include <tinypbrt/detail/error.h>

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * @defgroup film-private Film
	 * @ingroup tinypbrt-private
	 */

	/**
	 * @brief Returns film type based on string value.
	 * @param type_str String which is convertable to film type.
	 * @param type Output film type value.
	 * @return Error code (most possible TPBRT_ERROR_INVALID_FILM_TYPE, TPBRT_ERROR_NONE).
	 * @ingroup film-private
	 */
	tpbrt_error_t tpbrt_film_type_from_string(tpbrt_string_t type_str, tpbrt_film_type_t* type);

	/**
	 * @brief Returns film sensor based on string value.
	 * @param sensor_str String which is convertable to film sensor.
	 * @param sensor Output film sensor value.
	 * @return Error code (most possible TPBRT_ERROR_INVALID_FILM_SENSOR, TPBRT_ERROR_NONE).
	 * @ingroup film-private
	 */
	tpbrt_error_t tpbrt_film_sensor_from_string(tpbrt_string_t sensor_str, tpbrt_film_sensor_t* sensor);

	/**
	 * @brief Initializes default film.
	 * @param film Output film data.
	 * @return Error code (most possible TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
	 * @ingroup film-private
	 */
	tpbrt_error_t tpbrt_film_init_default(tpbrt_film_t* film);

	/**
	 * @brief Initializes film.
	 * @param type_str Film type string.
	 * @param params Parameters list.
	 * @param film Output film data.
	 * @return Error code (most possible TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_FILM_TYPE, TPBRT_ERROR_INVALID_FILM_SENSOR, TPBRT_ERROR_NONE).
	 * @ingroup film-private
	 */
	tpbrt_error_t tpbrt_film_init(tpbrt_string_t type_str, const tpbrt_params_list_t* params, tpbrt_film_t* film);

	/**
	 * @brief Frees all film resources.
	 * @param film Film data containing resources to free.
	 * @ingroup film-private
	 */
	void tpbrt_film_free(tpbrt_film_t* film);

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_FILM_INTERNAL_H_
