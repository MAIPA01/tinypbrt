#pragma once
#ifndef _TINYPBRT_FILM_INTERNAL_H_
#define _TINYPBRT_FILM_INTERNAL_H_

#include <tinypbrt/detail/fwd_internal.h>

#include <tinypbrt/detail/film.h>

#include <tinypbrt/detail/error.h>

#ifdef __cplusplus
extern "C" {
#endif

	tpbrt_error_t tpbrt_film_type_from_string(tpbrt_string_t type_str, tpbrt_film_type_t* type);

	tpbrt_error_t tpbrt_film_sensor_from_string(tpbrt_string_t type_str, tpbrt_film_sensor_t* sensor);

	tpbrt_error_t tpbrt_film_init_default(tpbrt_film_t* film);

	tpbrt_error_t tpbrt_film_init(tpbrt_string_t type_str, tpbrt_params_list_t params, tpbrt_film_t* film);

	void tpbrt_film_free(tpbrt_film_t* film);

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_FILM_INTERNAL_H_