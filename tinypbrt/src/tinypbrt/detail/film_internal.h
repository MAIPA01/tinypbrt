#pragma once
#ifndef _TINYPBRT_FILM_INTERNAL_H_
#define _TINYPBRT_FILM_INTERNAL_H_

#include <tinypbrt/detail/film.h>

#include <tinypbrt/detail/error.h>
#include <tinypbrt/detail/param_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

	tpbrt_error_t tpbrt_create_default_film(tpbrt_film_t* film);

	tpbrt_error_t tpbrt_create_film(const tpbrt_string_t* type_str, const tpbrt_params_list_t* params, tpbrt_film_t* film);

	void tpbrt_free_film(tpbrt_film_t* film);

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_FILM_INTERNAL_H_