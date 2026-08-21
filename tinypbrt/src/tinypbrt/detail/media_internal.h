#pragma once
#ifndef _TINYPBRT_MEDIA_INTERNAL_H_
#define _TINYPBRT_MEDIA_INTERNAL_H_

#include <tinypbrt/detail/media.h>

#include <tinypbrt/detail/error.h>
#include <tinypbrt/detail/param_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

#pragma region MEDIA
	tpbrt_error_t tpbrt_create_media(const tpbrt_string_t* name, const tpbrt_string_t* type_str,
	  const tpbrt_params_list_t* params, tpbrt_media_t** media);
	void tpbrt_free_media(tpbrt_media_t** media);
#pragma endregion

#pragma region MEDIAS_LIST
	tpbrt_error_t tpbrt_create_empty_medias_list(tpbrt_medias_list_t** medias_list);
	tpbrt_error_t tpbrt_medias_list_add_media(tpbrt_medias_list_t* medias_list, const tpbrt_media_t* media);
	tpbrt_error_t tpbrt_medias_list_get_media(const tpbrt_medias_list_t* medias_list, const tpbrt_string_t* media_name,
	  const tpbrt_media_t** media);
	tpbrt_error_t tpbrt_medias_list_get_media_handle(const tpbrt_medias_list_t* medias_list, const tpbrt_string_t* media_name,
	  tpbrt_media_handle_t* media_handle);
	void tpbrt_free_medias_list(tpbrt_medias_list_t** medias_list);

	tpbrt_size_t tpbrt_medias_list_size(const tpbrt_medias_list_t* medias_list);
	tpbrt_bool_t tpbrt_medias_list_is_empty(const tpbrt_medias_list_t* medias_list);
#pragma endregion

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_MEDIA_INTERNAL_H_
