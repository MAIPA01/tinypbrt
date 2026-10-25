#pragma once
#ifndef _TINYPBRT_MEDIA_INTERNAL_H_
#define _TINYPBRT_MEDIA_INTERNAL_H_

#include <tinypbrt/detail/fwd_internal.h>

#include <tinypbrt/detail/name_index_internal.h>

#include <tinypbrt/detail/media.h>

#ifdef __cplusplus
extern "C" {
#endif

#pragma region MEDIA
	tpbrt_error_t tpbrt_media_type_from_string(tpbrt_string_t type_str, tpbrt_media_type_t* type);

	tpbrt_error_t tpbrt_media_init(tpbrt_string_t name, tpbrt_string_t type_str, const tpbrt_params_list_t* params,
	  tpbrt_transform_handle_t transform, tpbrt_media_t* media);
	void tpbrt_media_free(tpbrt_media_t* media);
#pragma endregion

#pragma region MEDIAS_LIST
	tpbrt_error_t tpbrt_medias_list_init(tpbrt_medias_list_t* medias_list);
	tpbrt_error_t tpbrt_medias_list_append(tpbrt_medias_list_t* medias_list, const tpbrt_media_t* media);
	tpbrt_error_t tpbrt_medias_list_get(const tpbrt_medias_list_t* medias_list, tpbrt_string_t media_name, const tpbrt_media_t** media);
	tpbrt_error_t tpbrt_medias_list_get_handle(const tpbrt_medias_list_t* medias_list, tpbrt_string_t media_name,
	  tpbrt_media_handle_t* media_handle);
	tpbrt_error_t tpbrt_medias_list_shrink_to_fit(tpbrt_medias_list_t* medias_list);
	void tpbrt_medias_list_free(tpbrt_medias_list_t* medias_list);

	tpbrt_bool_t tpbrt_medias_list_empty(const tpbrt_medias_list_t* medias_list);
#pragma endregion

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_MEDIA_INTERNAL_H_
