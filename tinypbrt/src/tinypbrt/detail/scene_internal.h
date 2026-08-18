#pragma once
#ifndef _TINYPBRT_SCENE_INTERNAL_H_
#define _TINYPBRT_SCENE_INTERNAL_H_

#include <tinypbrt/detail/scene.h>
#include <tinypbrt/detail/error.h>

#ifdef __cplusplus
extern "C" {
#endif

	tpbrt_error_t tpbrt_create_options(tpbrt_options_t** options);

	void tpbrt_free_options(tpbrt_options_t** options);

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_SCENE_INTERNAL_H_