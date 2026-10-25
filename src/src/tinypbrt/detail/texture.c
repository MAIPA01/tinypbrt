#include <tinypbrt/pch.h>

#include <tinypbrt/detail/name_index_internal.h>
#include <tinypbrt/detail/texture_internal.h>

#include <tinypbrt/detail/common_internal.h>
#include <tinypbrt/detail/math_internal.h>
#include <tinypbrt/detail/param_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

#pragma region TEXTURE

	void tpbrt_texture_handle_free(tpbrt_texture_handle_t* const texture_handle) {
			if (texture_handle == TPBRT_NULL) { return; }
			if (texture_handle->value_type == TPBRT_TEXTURE_HANDLE_VALUE_TYPE_SPECTRUM) {
				tpbrt_spectrum_free(&texture_handle->as.spectrum);
			}
		*texture_handle = TPBRT_TEXTURE_HANDLE_INVALID;
	}

	tpbrt_error_t tpbrt_texture_type_from_string(const tpbrt_string_t type_str, tpbrt_texture_type_t* const type_out) {
		static const tpbrt_string_t TYPES_STRS[TPBRT_TEXTURE_TYPE_MAX_NUM] = {
			TPBRT_STRING_INIT("float"),
			TPBRT_STRING_INIT("spectrum"),
		};

			if (type_out == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (tpbrt_string_empty(type_str) == TPBRT_TRUE) { return TPBRT_ERROR_INVALID_TEXTURE_TYPE; }

			for (tpbrt_texture_type_t t = 0; t < TPBRT_TEXTURE_TYPE_MAX_NUM; ++t) {
					if (tpbrt_string_equals(type_str, TYPES_STRS[t]) == TPBRT_TRUE) {
						*type_out = t;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_INVALID_TEXTURE_TYPE;
	}

	tpbrt_error_t tpbrt_texture_class_from_string(const tpbrt_string_t class_str, tpbrt_texture_class_t* const class_out) {
		static const tpbrt_string_t CLASSES_STRS[TPBRT_TEXTURE_CLASS_MAX_NUM] = {
			TPBRT_STRING_INIT("bilerp"),
			TPBRT_STRING_INIT("checkerboard"),
			TPBRT_STRING_INIT("constant"),
			TPBRT_STRING_INIT("directionmix"),
			TPBRT_STRING_INIT("dots"),
			TPBRT_STRING_INIT("fbm"),
			TPBRT_STRING_INIT("imagemap"),
			TPBRT_STRING_INIT("marble"),
			TPBRT_STRING_INIT("mix"),
			TPBRT_STRING_INIT("ptex"),
			TPBRT_STRING_INIT("scale"),
			TPBRT_STRING_INIT("windy"),
			TPBRT_STRING_INIT("wrinkled"),
		};

			if (class_out == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (tpbrt_string_empty(class_str) == TPBRT_TRUE) { return TPBRT_ERROR_INVALID_TEXTURE_CLASS; }

			for (tpbrt_texture_class_t c = 0; c < TPBRT_TEXTURE_CLASS_MAX_NUM; ++c) {
					if (tpbrt_string_equals(class_str, CLASSES_STRS[c]) == TPBRT_TRUE) {
						*class_out = c;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_INVALID_TEXTURE_CLASS;
	}

	tpbrt_error_t tpbrt_texture_wrap_from_string(const tpbrt_string_t wrap_str, tpbrt_texture_wrap_t* const wrap_out) {
		static const tpbrt_string_t WRAPS_STRS[TPBRT_TEXTURE_WRAP_MAX_NUM] = {
			TPBRT_STRING_INIT("repeat"),
			TPBRT_STRING_INIT("black"),
			TPBRT_STRING_INIT("clamp"),
		};

			if (wrap_out == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (tpbrt_string_empty(wrap_str) == TPBRT_TRUE) { return TPBRT_ERROR_INVALID_TEXTURE_WRAP; }

			for (tpbrt_texture_wrap_t w = 0; w < TPBRT_TEXTURE_WRAP_MAX_NUM; ++w) {
					if (tpbrt_string_equals(wrap_str, WRAPS_STRS[w]) == TPBRT_TRUE) {
						*wrap_out = w;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_INVALID_TEXTURE_WRAP;
	}

	tpbrt_error_t tpbrt_texture_filter_from_string(const tpbrt_string_t filter_str, tpbrt_texture_filter_t* const filter_out) {
		static const tpbrt_string_t FILTERS_STRS[TPBRT_TEXTURE_FILTER_MAX_NUM] = {
			TPBRT_STRING_INIT("bilinear"),
			TPBRT_STRING_INIT("ewa"),
			TPBRT_STRING_INIT("trilinear"),
			TPBRT_STRING_INIT("point"),
		};

			if (filter_out == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (tpbrt_string_empty(filter_str) == TPBRT_TRUE) { return TPBRT_ERROR_INVALID_TEXTURE_FILTER; }

			for (tpbrt_texture_filter_t f = 0; f < TPBRT_TEXTURE_FILTER_MAX_NUM; ++f) {
					if (tpbrt_string_equals(filter_str, FILTERS_STRS[f]) == TPBRT_TRUE) {
						*filter_out = f;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_INVALID_TEXTURE_FILTER;
	}

	tpbrt_error_t tpbrt_texture_encoding_from_string(const tpbrt_string_t encoding_str,
	  tpbrt_texture_encoding_t* const encoding_out, tpbrt_opt_float_t* const gamma_val_out) {
		static const tpbrt_string_t ENCODINGS_STRS[TPBRT_TEXTURE_ENCODING_MAX_NUM - 1] = {
			TPBRT_STRING_INIT("sRGB"),
			TPBRT_STRING_INIT("linear"),
		};
		static const tpbrt_string_t GAMMA_STR = TPBRT_STRING_INIT("gamma");

			if (encoding_out == TPBRT_NULL || gamma_val_out == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (tpbrt_string_empty(encoding_str) == TPBRT_TRUE) { return TPBRT_ERROR_INVALID_TEXTURE_ENCODING; }

			for (tpbrt_texture_encoding_t e = 0; e < TPBRT_TEXTURE_ENCODING_MAX_NUM - 1; ++e) {
					if (tpbrt_string_equals(encoding_str, ENCODINGS_STRS[e]) == TPBRT_TRUE) {
						*encoding_out			 = e;
						gamma_val_out->has_value = TPBRT_FALSE;
						return TPBRT_ERROR_NONE;
					}
			}

		const tpbrt_char_t* const end = encoding_str.data + encoding_str.size;

		const tpbrt_string_t keyword  = tpbrt_next_value(encoding_str.data, end);
			if (tpbrt_string_equals(keyword, GAMMA_STR) == TPBRT_FALSE) { return TPBRT_ERROR_INVALID_TEXTURE_ENCODING; }

		const tpbrt_string_t value_str = tpbrt_next_value(keyword.data + keyword.size, end);
		tpbrt_float_t gamma			   = 0.0f;
			if (value_str.data == TPBRT_NULL || tpbrt_read_float_value(value_str, &gamma) == TPBRT_FALSE || gamma == 0.0f) {
				return TPBRT_ERROR_PARSE_FLOAT;
			}

			if (tpbrt_next_value(value_str.data + value_str.size, end).data != TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_TEXTURE_ENCODING;
			}

		gamma_val_out->has_value = TPBRT_TRUE;
		gamma_val_out->value	 = gamma;
		*encoding_out			 = TPBRT_TEXTURE_ENCODING_GAMMA;
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_texture_mapping_from_string(const tpbrt_string_t mapping_str,
	  tpbrt_texture_mapping_t* const mapping_out) {
		static const tpbrt_string_t MAPPINGS_STRS[TPBRT_TEXTURE_MAPPING_MAX_NUM] = {
			TPBRT_STRING_INIT("uv"),
			TPBRT_STRING_INIT("spherical"),
			TPBRT_STRING_INIT("cylindrical"),
			TPBRT_STRING_INIT("planar"),
		};

			if (mapping_out == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (tpbrt_string_empty(mapping_str) == TPBRT_TRUE) { return TPBRT_ERROR_INVALID_TEXTURE_MAPPING; }

			for (tpbrt_texture_mapping_t m = 0; m < TPBRT_TEXTURE_MAPPING_MAX_NUM; ++m) {
					if (tpbrt_string_equals(mapping_str, MAPPINGS_STRS[m]) == TPBRT_TRUE) {
						*mapping_out = m;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_INVALID_TEXTURE_MAPPING;
	}

	static tpbrt_error_t tpbrt_texture_read_mapping_uv(tpbrt_texture_mapping_uv_params_t* const uv,
	  const tpbrt_params_list_t* const params) {
			if (uv == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t U_SCALE_STR	   = TPBRT_STRING_INIT("uscale");
		static const tpbrt_string_t V_SCALE_STR	   = TPBRT_STRING_INIT("vscale");
		static const tpbrt_string_t U_DELTA_STR	   = TPBRT_STRING_INIT("udelta");
		static const tpbrt_string_t V_DELTA_STR	   = TPBRT_STRING_INIT("vdelta");

		static const tpbrt_float_t U_SCALE_DEFAULT = 1.0f;
		static const tpbrt_float_t V_SCALE_DEFAULT = 1.0f;
		static const tpbrt_float_t U_DELTA_DEFAULT = 0.0f;
		static const tpbrt_float_t V_DELTA_DEFAULT = 0.0f;

		tpbrt_error_t err = tpbrt_params_list_get_float(params, U_SCALE_STR, U_SCALE_DEFAULT, &uv->u_scale);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_float(params, V_SCALE_STR, V_SCALE_DEFAULT, &uv->v_scale);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_float(params, U_DELTA_STR, U_DELTA_DEFAULT, &uv->u_delta);
			if (err != TPBRT_ERROR_NONE) { return err; }

		return tpbrt_params_list_get_float(params, V_DELTA_STR, V_DELTA_DEFAULT, &uv->v_delta);
	}

	static tpbrt_error_t tpbrt_texture_read_mapping_spherical(tpbrt_texture_mapping_spherical_params_t* const spherical,
	  const tpbrt_transform_handle_t transform) {
			if (spherical == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		spherical->transform = transform;

		return TPBRT_ERROR_NONE;
	}

	static tpbrt_error_t tpbrt_texture_read_mapping_cylindrical(tpbrt_texture_mapping_cylindrical_params_t* const cylindrical,
	  const tpbrt_transform_handle_t transform) {
			if (cylindrical == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		cylindrical->transform = transform;

		return TPBRT_ERROR_NONE;
	}

	static tpbrt_error_t tpbrt_texture_read_mapping_planar(tpbrt_texture_mapping_planar_params_t* const planar,
	  const tpbrt_params_list_t* const params) {
			if (planar == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t U_DELTA_STR	   = TPBRT_STRING_INIT("udelta");
		static const tpbrt_string_t V_DELTA_STR	   = TPBRT_STRING_INIT("vdelta");
		static const tpbrt_string_t V1_STR		   = TPBRT_STRING_INIT("v1");
		static const tpbrt_string_t V2_STR		   = TPBRT_STRING_INIT("v2");

		static const tpbrt_float_t U_DELTA_DEFAULT = 0.0f;
		static const tpbrt_float_t V_DELTA_DEFAULT = 0.0f;
		static const tpbrt_vec3_t V1_DEFAULT	   = TPBRT_VEC3_INIT(1.0f, 0.0f, 0.0f);
		static const tpbrt_vec3_t V2_DEFAULT	   = TPBRT_VEC3_INIT(0.0f, 1.0f, 0.0f);

		tpbrt_error_t err = tpbrt_params_list_get_float(params, U_DELTA_STR, U_DELTA_DEFAULT, &planar->u_delta);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_float(params, V_DELTA_STR, V_DELTA_DEFAULT, &planar->v_delta);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_vec(params, V1_STR, V1_DEFAULT, &planar->v1);
			if (err != TPBRT_ERROR_NONE) { return err; }

		return tpbrt_params_list_get_vec(params, V2_STR, V2_DEFAULT, &planar->v2);
	}

	static tpbrt_error_t tpbrt_texture_read_bilerp(tpbrt_texture_bilerp_params_t* const bilerp,
	  const tpbrt_params_list_t* const params, const tpbrt_textures_list_t* const textures, const tpbrt_texture_type_t type) {
			if (bilerp == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t V00_STR				= TPBRT_STRING_INIT("v00");
		static const tpbrt_string_t V01_STR				= TPBRT_STRING_INIT("v01");
		static const tpbrt_string_t V10_STR				= TPBRT_STRING_INIT("v10");
		static const tpbrt_string_t V11_STR				= TPBRT_STRING_INIT("v11");

		static const tpbrt_texture_handle_t V00_DEFAULT = TPBRT_TEXTURE_HANDLE_SPECTRUM_CONSTANT_INIT(0.0f);
		static const tpbrt_texture_handle_t V01_DEFAULT = TPBRT_TEXTURE_HANDLE_SPECTRUM_CONSTANT_INIT(1.0f);
		static const tpbrt_texture_handle_t V10_DEFAULT = TPBRT_TEXTURE_HANDLE_SPECTRUM_CONSTANT_INIT(0.0f);
		static const tpbrt_texture_handle_t V11_DEFAULT = TPBRT_TEXTURE_HANDLE_SPECTRUM_CONSTANT_INIT(1.0f);

		tpbrt_error_t err = tpbrt_params_list_get_texture_of_type(params, V00_STR, textures, type, &V00_DEFAULT, &bilerp->v00);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_texture_of_type(params, V01_STR, textures, type, &V01_DEFAULT, &bilerp->v01);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_texture_of_type(params, V10_STR, textures, type, &V10_DEFAULT, &bilerp->v10);
			if (err != TPBRT_ERROR_NONE) { return err; }

		return tpbrt_params_list_get_texture_of_type(params, V11_STR, textures, type, &V11_DEFAULT, &bilerp->v11);
	}

	static tpbrt_error_t tpbrt_texture_read_checkerboard(tpbrt_texture_checkerboard_params_t* const checkerboard,
	  const tpbrt_params_list_t* const params, const tpbrt_textures_list_t* const textures, const tpbrt_texture_type_t type) {
			if (checkerboard == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t DIMENSION_STR		 = TPBRT_STRING_INIT("dimension");
		static const tpbrt_string_t TEX1_STR			 = TPBRT_STRING_INIT("tex1");
		static const tpbrt_string_t TEX2_STR			 = TPBRT_STRING_INIT("tex2");

		static const tpbrt_uint_t DIMENSION_DEFAULT		 = 2u;
		static const tpbrt_texture_handle_t TEX1_DEFAULT = TPBRT_TEXTURE_HANDLE_SPECTRUM_CONSTANT_INIT(1.0f);
		static const tpbrt_texture_handle_t TEX2_DEFAULT = TPBRT_TEXTURE_HANDLE_SPECTRUM_CONSTANT_INIT(0.0f);

		tpbrt_error_t err = tpbrt_params_list_get_uint(params, DIMENSION_STR, DIMENSION_DEFAULT, &checkerboard->dimension);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_texture_of_type(params, TEX1_STR, textures, type, &TEX1_DEFAULT, &checkerboard->tex1);
			if (err != TPBRT_ERROR_NONE) { return err; }

		return tpbrt_params_list_get_texture_of_type(params, TEX2_STR, textures, type, &TEX2_DEFAULT, &checkerboard->tex2);
	}

	static tpbrt_error_t tpbrt_texture_read_constant(tpbrt_texture_constant_params_t* const constant,
	  const tpbrt_params_list_t* const params, const tpbrt_textures_list_t* const textures, const tpbrt_texture_type_t type) {
			if (constant == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t VALUE_STR			  = TPBRT_STRING_INIT("value");

		static const tpbrt_texture_handle_t VALUE_DEFAULT = TPBRT_TEXTURE_HANDLE_SPECTRUM_CONSTANT_INIT(1.0f);

		return tpbrt_params_list_get_texture_of_type(params, VALUE_STR, textures, type, &VALUE_DEFAULT, &constant->value);
	}

	static tpbrt_error_t tpbrt_texture_read_direction_mix(tpbrt_texture_direction_mix_params_t* const direction_mix,
	  const tpbrt_params_list_t* const params, const tpbrt_textures_list_t* const textures, const tpbrt_texture_type_t type) {
			if (direction_mix == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t TEX1_STR			 = TPBRT_STRING_INIT("tex1");
		static const tpbrt_string_t TEX2_STR			 = TPBRT_STRING_INIT("tex2");
		static const tpbrt_string_t DIR_STR				 = TPBRT_STRING_INIT("dir");

		static const tpbrt_texture_handle_t TEX1_DEFAULT = TPBRT_TEXTURE_HANDLE_SPECTRUM_CONSTANT_INIT(0.0f);
		static const tpbrt_texture_handle_t TEX2_DEFAULT = TPBRT_TEXTURE_HANDLE_SPECTRUM_CONSTANT_INIT(1.0f);
		static const tpbrt_vec3_t DIR_DEFAULT			 = TPBRT_VEC3_INIT(0.0f, 1.0f, 0.0f);

		tpbrt_error_t err =
		  tpbrt_params_list_get_texture_of_type(params, TEX1_STR, textures, type, &TEX1_DEFAULT, &direction_mix->tex1);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_texture_of_type(params, TEX2_STR, textures, type, &TEX2_DEFAULT, &direction_mix->tex2);
			if (err != TPBRT_ERROR_NONE) { return err; }

		return tpbrt_params_list_get_vec(params, DIR_STR, DIR_DEFAULT, &direction_mix->dir);
	}

	static tpbrt_error_t tpbrt_texture_read_dots(tpbrt_texture_dots_params_t* const dots, const tpbrt_params_list_t* const params,
	  const tpbrt_textures_list_t* const textures, const tpbrt_texture_type_t type) {
			if (dots == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t INSIDE_STR				= TPBRT_STRING_INIT("inside");
		static const tpbrt_string_t OUTSIDE_STR				= TPBRT_STRING_INIT("outside");

		static const tpbrt_texture_handle_t INSIDE_DEFAULT	= TPBRT_TEXTURE_HANDLE_SPECTRUM_CONSTANT_INIT(1.0f);
		static const tpbrt_texture_handle_t OUTSIDE_DEFAULT = TPBRT_TEXTURE_HANDLE_SPECTRUM_CONSTANT_INIT(0.0f);

		const tpbrt_error_t err =
		  tpbrt_params_list_get_texture_of_type(params, INSIDE_STR, textures, type, &INSIDE_DEFAULT, &dots->inside);
			if (err != TPBRT_ERROR_NONE) { return err; }

		return tpbrt_params_list_get_texture_of_type(params, OUTSIDE_STR, textures, type, &OUTSIDE_DEFAULT, &dots->outside);
	}

	static tpbrt_error_t tpbrt_texture_read_fbm(tpbrt_texture_fbm_params_t* const fbm, const tpbrt_params_list_t* const params,
	  const tpbrt_transform_handle_t transform) {
			if (fbm == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t OCTAVES_STR		 = TPBRT_STRING_INIT("octaves");
		static const tpbrt_string_t ROUGHNESS_STR	 = TPBRT_STRING_INIT("roughness");

		static const tpbrt_uint_t OCTAVES_DEFAULT	 = 8u;
		static const tpbrt_float_t ROUGHNESS_DEFAULT = 0.5f;

		fbm->transform								 = transform;

		const tpbrt_error_t err = tpbrt_params_list_get_uint(params, OCTAVES_STR, OCTAVES_DEFAULT, &fbm->octaves);
			if (err != TPBRT_ERROR_NONE) { return err; }

		return tpbrt_params_list_get_float(params, ROUGHNESS_STR, ROUGHNESS_DEFAULT, &fbm->roughness);
	}

	static tpbrt_error_t tpbrt_texture_read_wrinkled(tpbrt_texture_wrinkled_params_t* const wrinkled,
	  const tpbrt_params_list_t* const params, const tpbrt_transform_handle_t transform) {
			if (wrinkled == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		return tpbrt_texture_read_fbm(wrinkled, params, transform);
	}

	static tpbrt_error_t tpbrt_texture_read_windy(tpbrt_texture_windy_params_t* const windy,
	  const tpbrt_params_list_t* const params, const tpbrt_transform_handle_t transform) {
			if (windy == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		return tpbrt_texture_read_fbm(windy, params, transform);
	}

	static tpbrt_bool_t tpbrt_texture_has_png_extension(const tpbrt_string_t file_name) {
		static const tpbrt_string_t PNG_EXTENSION = TPBRT_STRING_INIT(".png");

			if (file_name.data == TPBRT_NULL || file_name.size < PNG_EXTENSION.size) { return TPBRT_FALSE; }

		const tpbrt_char_t* const extension = file_name.data + (file_name.size - PNG_EXTENSION.size);
			for (tpbrt_size_t i = 0u; i < PNG_EXTENSION.size; ++i) {
					if (tolower((unsigned char)extension[i]) != PNG_EXTENSION.data[i]) { return TPBRT_FALSE; }
			}
		return TPBRT_TRUE;
	}

	static tpbrt_error_t tpbrt_texture_read_image_map(tpbrt_texture_image_map_params_t* const image_map,
	  const tpbrt_params_list_t* const params) {
			if (image_map == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t FILE_NAME_STR		  = TPBRT_STRING_INIT("filename");
		static const tpbrt_string_t WRAP_STR			  = TPBRT_STRING_INIT("wrap");
		static const tpbrt_string_t MAX_ANISOTROPY_STR	  = TPBRT_STRING_INIT("maxanisotropy");
		static const tpbrt_string_t FILTER_STR			  = TPBRT_STRING_INIT("filter");
		static const tpbrt_string_t ENCODING_STR		  = TPBRT_STRING_INIT("encoding");
		static const tpbrt_string_t SCALE_STR			  = TPBRT_STRING_INIT("scale");
		static const tpbrt_string_t INVERT_STR			  = TPBRT_STRING_INIT("invert");

		static const tpbrt_string_t WRAP_DEFAULT		  = TPBRT_EMPTY_STRING_INIT;
		static const tpbrt_float_t MAX_ANISOTROPY_DEFAULT = 8.0f;
		static const tpbrt_string_t FILTER_DEFAULT		  = TPBRT_EMPTY_STRING_INIT;
		static const tpbrt_string_t ENCODING_DEFAULT	  = TPBRT_EMPTY_STRING_INIT;
		static const tpbrt_float_t SCALE_DEFAULT		  = 1.0f;
		static const tpbrt_bool_t INVERT_DEFAULT		  = TPBRT_FALSE;

		tpbrt_error_t err = tpbrt_params_list_get_string_required(params, FILE_NAME_STR, &image_map->file_name);
			if (err != TPBRT_ERROR_NONE) { return err; }

		tpbrt_string_t temp_str;
		err = tpbrt_params_list_get_string(params, WRAP_STR, WRAP_DEFAULT, &temp_str);

			if (tpbrt_string_empty(temp_str) == TPBRT_TRUE) { image_map->wrap = TPBRT_TEXTURE_WRAP_REPEAT; }
			else {
				tpbrt_texture_wrap_t wrap = TPBRT_TEXTURE_WRAP_MAX_NUM;
				err						  = tpbrt_texture_wrap_from_string(temp_str, &wrap);
				tpbrt_string_free(&temp_str);
				image_map->wrap = wrap;
			}

			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_float(params, MAX_ANISOTROPY_STR, MAX_ANISOTROPY_DEFAULT, &image_map->max_anisotropy);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_string(params, FILTER_STR, FILTER_DEFAULT, &temp_str);

			if (tpbrt_string_empty(temp_str) == TPBRT_TRUE) { image_map->filter = TPBRT_TEXTURE_FILTER_BILINEAR; }
			else {
				tpbrt_texture_filter_t filter = TPBRT_TEXTURE_FILTER_MAX_NUM;
				err							  = tpbrt_texture_filter_from_string(temp_str, &filter);
				tpbrt_string_free(&temp_str);
				image_map->filter = filter;
			}

			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_string(params, ENCODING_STR, ENCODING_DEFAULT, &temp_str);

			if (tpbrt_string_empty(temp_str) == TPBRT_TRUE) {
				image_map->encoding		   = tpbrt_texture_has_png_extension(image_map->file_name) == TPBRT_TRUE ?
											   TPBRT_TEXTURE_ENCODING_SRGB :
											   TPBRT_TEXTURE_ENCODING_LINEAR;
				image_map->gamma.has_value = TPBRT_FALSE;
			}
			else {
				tpbrt_texture_encoding_t encoding = TPBRT_TEXTURE_ENCODING_MAX_NUM;
				err								  = tpbrt_texture_encoding_from_string(temp_str, &encoding, &image_map->gamma);
				tpbrt_string_free(&temp_str);
				image_map->encoding = encoding;
			}

			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_float(params, SCALE_STR, SCALE_DEFAULT, &image_map->scale);
			if (err != TPBRT_ERROR_NONE) { return err; }

		tpbrt_bool_t invert = TPBRT_FALSE;
		err					= tpbrt_params_list_get_bool(params, INVERT_STR, INVERT_DEFAULT, &invert);
		image_map->invert	= invert;

		return err;
	}

	static tpbrt_error_t tpbrt_texture_read_marble(tpbrt_texture_marble_params_t* const marble,
	  const tpbrt_params_list_t* const params) {
			if (marble == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t OCTAVES_STR		 = TPBRT_STRING_INIT("octaves");
		static const tpbrt_string_t ROUGHNESS_STR	 = TPBRT_STRING_INIT("roughness");
		static const tpbrt_string_t SCALE_STR		 = TPBRT_STRING_INIT("scale");
		static const tpbrt_string_t VARIATION_STR	 = TPBRT_STRING_INIT("variation");

		static const tpbrt_uint_t OCTAVES_DEFAULT	 = 8u;
		static const tpbrt_float_t ROUGHNESS_DEFAULT = 0.5f;
		static const tpbrt_float_t SCALE_DEFAULT	 = 1.0f;
		static const tpbrt_float_t VARIATION_DEFAULT = 0.2f;

		tpbrt_error_t err = tpbrt_params_list_get_uint(params, OCTAVES_STR, OCTAVES_DEFAULT, &marble->octaves);
			if (err != TPBRT_ERROR_NONE) { return err; }


		err = tpbrt_params_list_get_float(params, ROUGHNESS_STR, ROUGHNESS_DEFAULT, &marble->roughness);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_float(params, SCALE_STR, SCALE_DEFAULT, &marble->scale);
			if (err != TPBRT_ERROR_NONE) { return err; }

		return tpbrt_params_list_get_float(params, VARIATION_STR, VARIATION_DEFAULT, &marble->variation);
	}

	static tpbrt_error_t tpbrt_texture_read_mix(tpbrt_texture_mix_params_t* const mix, const tpbrt_params_list_t* const params,
	  const tpbrt_textures_list_t* const textures, const tpbrt_texture_type_t type) {
			if (mix == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t TEX1_STR			   = TPBRT_STRING_INIT("tex1");
		static const tpbrt_string_t TEX2_STR			   = TPBRT_STRING_INIT("tex2");
		static const tpbrt_string_t AMOUNT_STR			   = TPBRT_STRING_INIT("amount");

		static const tpbrt_texture_handle_t TEX1_DEFAULT   = TPBRT_TEXTURE_HANDLE_SPECTRUM_CONSTANT_INIT(0.0f);
		static const tpbrt_texture_handle_t TEX2_DEFAULT   = TPBRT_TEXTURE_HANDLE_SPECTRUM_CONSTANT_INIT(1.0f);
		static const tpbrt_texture_handle_t AMOUNT_DEFAULT = TPBRT_TEXTURE_HANDLE_SPECTRUM_CONSTANT_INIT(0.5f);

		tpbrt_error_t err = tpbrt_params_list_get_texture_of_type(params, TEX1_STR, textures, type, &TEX1_DEFAULT, &mix->tex1);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_texture_of_type(params, TEX2_STR, textures, type, &TEX2_DEFAULT, &mix->tex2);
			if (err != TPBRT_ERROR_NONE) { return err; }

		return tpbrt_params_list_get_texture_of_type(params, AMOUNT_STR, textures, TPBRT_TEXTURE_TYPE_FLOAT, &AMOUNT_DEFAULT,
		  &mix->amount);
	}

	static tpbrt_error_t tpbrt_texture_read_ptex(tpbrt_texture_ptex_params_t* const ptex,
	  const tpbrt_params_list_t* const params) {
			if (ptex == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t ENCODING_STR	  = TPBRT_STRING_INIT("encoding");
		static const tpbrt_string_t FILE_NAME_STR	  = TPBRT_STRING_INIT("filename");
		static const tpbrt_string_t SCALE_STR		  = TPBRT_STRING_INIT("scale");

		static const tpbrt_string_t ENCODING_DEFAULT  = TPBRT_EMPTY_STRING_INIT;
		static const tpbrt_opt_float_t GAMMA_DEFAULT  = TPBRT_OPT_FLOAT_VALUE_INIT(2.2f);
		static const tpbrt_string_t FILE_NAME_DEFAULT = TPBRT_EMPTY_STRING_INIT;
		static const tpbrt_float_t SCALE_DEFAULT	  = 1.0f;

		tpbrt_string_t temp_str;
		tpbrt_error_t err = tpbrt_params_list_get_string(params, ENCODING_STR, ENCODING_DEFAULT, &temp_str);

			if (tpbrt_string_empty(temp_str) == TPBRT_TRUE) {
				ptex->encoding = TPBRT_TEXTURE_ENCODING_GAMMA;
				ptex->gamma	   = GAMMA_DEFAULT;
			}
			else {
				tpbrt_texture_encoding_t encoding = TPBRT_TEXTURE_ENCODING_MAX_NUM;
				err								  = tpbrt_texture_encoding_from_string(temp_str, &encoding, &ptex->gamma);
				tpbrt_string_free(&temp_str);
				ptex->encoding = encoding;
			}

			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_string(params, FILE_NAME_STR, FILE_NAME_DEFAULT, &ptex->file_name);
			if (err != TPBRT_ERROR_NONE) { return err; }

		return tpbrt_params_list_get_float(params, SCALE_STR, SCALE_DEFAULT, &ptex->scale);
	}

	static tpbrt_error_t tpbrt_texture_read_scale(tpbrt_texture_scale_params_t* const scale,
	  const tpbrt_params_list_t* const params, const tpbrt_textures_list_t* const textures, const tpbrt_texture_type_t type) {
			if (scale == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t TEX_STR				  = TPBRT_STRING_INIT("tex");
		static const tpbrt_string_t SCALE_STR			  = TPBRT_STRING_INIT("scale");

		static const tpbrt_texture_handle_t TEX_DEFAULT	  = TPBRT_TEXTURE_HANDLE_SPECTRUM_CONSTANT_INIT(1.0f);
		static const tpbrt_texture_handle_t SCALE_DEFAULT = TPBRT_TEXTURE_HANDLE_SPECTRUM_CONSTANT_INIT(1.0f);

		const tpbrt_error_t err =
		  tpbrt_params_list_get_texture_of_type(params, TEX_STR, textures, type, &TEX_DEFAULT, &scale->tex);
			if (err != TPBRT_ERROR_NONE) { return err; }

		return tpbrt_params_list_get_texture_of_type(params, SCALE_STR, textures, TPBRT_TEXTURE_TYPE_FLOAT, &SCALE_DEFAULT,
		  &scale->scale);
	}

	tpbrt_error_t tpbrt_texture_init(const tpbrt_string_t name, const tpbrt_string_t type_str, const tpbrt_string_t class_str,
	  const tpbrt_params_list_t* const params, const tpbrt_textures_list_t* const textures,
	  const tpbrt_transform_handle_t transform, tpbrt_texture_t* const texture) {
			if (texture == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (memset(texture, 0, sizeof(tpbrt_texture_t)) == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		texture->idx	  = TPBRT_TEXTURE_IDX_INVALID;

		tpbrt_error_t err = tpbrt_string_copy(&texture->name, name);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_texture_free(texture);
				return err;
			}

		tpbrt_texture_type_t type;
		err = tpbrt_texture_type_from_string(type_str, &type);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_texture_free(texture);
				return err;
			}
		texture->type = type;

		tpbrt_texture_class_t tex_class;
		err = tpbrt_texture_class_from_string(class_str, &tex_class);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_texture_free(texture);
				return err;
			}
		texture->tex_class							= tex_class;

		static const tpbrt_string_t MAPPING_STR		= TPBRT_STRING_INIT("mapping");

		static const tpbrt_string_t MAPPING_DEFAULT = TPBRT_EMPTY_STRING_INIT;

		tpbrt_string_t mapping_str;
		err = tpbrt_params_list_get_string(params, MAPPING_STR, MAPPING_DEFAULT, &mapping_str);

			if (tpbrt_string_empty(mapping_str) == TPBRT_TRUE) { texture->mapping = TPBRT_TEXTURE_MAPPING_UV; }
			else {
				tpbrt_texture_mapping_t mapping = TPBRT_TEXTURE_MAPPING_MAX_NUM;
				err								= tpbrt_texture_mapping_from_string(mapping_str, &mapping);
				tpbrt_string_free(&mapping_str);
				texture->mapping = mapping;
			}

			if (err != TPBRT_ERROR_NONE) {
				tpbrt_texture_free(texture);
				return err;
			}

			// mapping
			switch (texture->mapping) {
			default:
				case TPBRT_TEXTURE_MAPPING_UV: {
					err = tpbrt_texture_read_mapping_uv(&texture->mapping_as.uv, params);
					break;
				}
				case TPBRT_TEXTURE_MAPPING_SPHERICAL: {
					err = tpbrt_texture_read_mapping_spherical(&texture->mapping_as.spherical, transform);
					break;
				}
				case TPBRT_TEXTURE_MAPPING_CYLINDRICAL: {
					err = tpbrt_texture_read_mapping_cylindrical(&texture->mapping_as.cylindrical, transform);
					break;
				}
				case TPBRT_TEXTURE_MAPPING_PLANAR: {
					err = tpbrt_texture_read_mapping_planar(&texture->mapping_as.planar, params);
					break;
				}
			}

			if (err != TPBRT_ERROR_NONE) {
				tpbrt_texture_free(texture);
				return err;
			}

			// class
			switch (texture->tex_class) {
			default:
				case TPBRT_TEXTURE_CLASS_BILERP: {
					err = tpbrt_texture_read_bilerp(&texture->as.bilerp, params, textures, texture->type);
					break;
				}
				case TPBRT_TEXTURE_CLASS_CHECKER_BOARD: {
					err = tpbrt_texture_read_checkerboard(&texture->as.checkerboard, params, textures, texture->type);
					break;
				}
				case TPBRT_TEXTURE_CLASS_CONSTANT: {
					err = tpbrt_texture_read_constant(&texture->as.constant, params, textures, texture->type);
					break;
				}
				case TPBRT_TEXTURE_CLASS_DIRECTION_MIX: {
					err = tpbrt_texture_read_direction_mix(&texture->as.direction_mix, params, textures, texture->type);
					break;
				}
				case TPBRT_TEXTURE_CLASS_DOTS: {
					err = tpbrt_texture_read_dots(&texture->as.dots, params, textures, texture->type);
					break;
				}
				case TPBRT_TEXTURE_CLASS_FBM: {
					err = tpbrt_texture_read_fbm(&texture->as.fbm, params, transform);
					break;
				}
				case TPBRT_TEXTURE_CLASS_WRINKLED: {
					err = tpbrt_texture_read_wrinkled(&texture->as.wrinkled, params, transform);
					break;
				}
				case TPBRT_TEXTURE_CLASS_WINDY: {
					err = tpbrt_texture_read_windy(&texture->as.windy, params, transform);
					break;
				}
				case TPBRT_TEXTURE_CLASS_IMAGE_MAP: {
					err = tpbrt_texture_read_image_map(&texture->as.image_map, params);
					break;
				}
				case TPBRT_TEXTURE_CLASS_MARBLE: {
					err = tpbrt_texture_read_marble(&texture->as.marble, params);
					break;
				}
				case TPBRT_TEXTURE_CLASS_MIX: {
					err = tpbrt_texture_read_mix(&texture->as.mix, params, textures, texture->type);
					break;
				}
				case TPBRT_TEXTURE_CLASS_PTEX: {
					err = tpbrt_texture_read_ptex(&texture->as.ptex, params);
					break;
				}
				case TPBRT_TEXTURE_CLASS_SCALE: {
					err = tpbrt_texture_read_scale(&texture->as.scale, params, textures, texture->type);
					break;
				}
			}

			if (err != TPBRT_ERROR_NONE) {
				tpbrt_texture_free(texture);
				return err;
			}

		return TPBRT_ERROR_NONE;
	}

	void tpbrt_texture_free(tpbrt_texture_t* const texture) {
			if (texture == TPBRT_NULL) { return; }

			switch (texture->tex_class) {
				case TPBRT_TEXTURE_CLASS_BILERP: {
					tpbrt_texture_handle_free(&texture->as.bilerp.v00);
					tpbrt_texture_handle_free(&texture->as.bilerp.v01);
					tpbrt_texture_handle_free(&texture->as.bilerp.v10);
					tpbrt_texture_handle_free(&texture->as.bilerp.v11);
					break;
				}
				case TPBRT_TEXTURE_CLASS_DIRECTION_MIX: {
					tpbrt_texture_handle_free(&texture->as.direction_mix.tex1);
					tpbrt_texture_handle_free(&texture->as.direction_mix.tex2);
					break;
				}
				case TPBRT_TEXTURE_CLASS_DOTS: {
					tpbrt_texture_handle_free(&texture->as.dots.inside);
					tpbrt_texture_handle_free(&texture->as.dots.outside);
					break;
				}
				case TPBRT_TEXTURE_CLASS_MIX: {
					tpbrt_texture_handle_free(&texture->as.mix.tex1);
					tpbrt_texture_handle_free(&texture->as.mix.tex2);
					tpbrt_texture_handle_free(&texture->as.mix.amount);
					break;
				}
				case TPBRT_TEXTURE_CLASS_SCALE: {
					tpbrt_texture_handle_free(&texture->as.scale.scale);
					tpbrt_texture_handle_free(&texture->as.scale.tex);
					break;
				}
				case TPBRT_TEXTURE_CLASS_CHECKER_BOARD: {
					tpbrt_texture_handle_free(&texture->as.checkerboard.tex1);
					tpbrt_texture_handle_free(&texture->as.checkerboard.tex2);
					break;
				}
				case TPBRT_TEXTURE_CLASS_CONSTANT: {
					tpbrt_texture_handle_free(&texture->as.constant.value);
					break;
				}
				case TPBRT_TEXTURE_CLASS_IMAGE_MAP: {
					tpbrt_string_free(&texture->as.image_map.file_name);
					break;
				}
				case TPBRT_TEXTURE_CLASS_PTEX: {
					tpbrt_string_free(&texture->as.ptex.file_name);
					break;
				}
				default: {
					break;
				}
			}

		tpbrt_string_free(&texture->name);
	}

#pragma endregion

#pragma region TEXTURES_LIST
#define TPBRT_EMPTY_TEXTURES_LIST (tpbrt_textures_list_t) { .textures = TPBRT_NULL, .count = 0 }

	tpbrt_error_t tpbrt_textures_list_init(tpbrt_textures_list_t* const textures_list) {
			if (textures_list == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		*textures_list = TPBRT_EMPTY_TEXTURES_LIST;
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_textures_list_append(tpbrt_textures_list_t* const textures_list, const tpbrt_texture_t* const texture) {
			if (texture == TPBRT_NULL || textures_list == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		tpbrt_texture_t* const new_list =
		  tpbrt_array_reserve_append(textures_list->textures, textures_list->count, sizeof(tpbrt_texture_t));
			if (new_list == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }

		const tpbrt_size_t idx	= textures_list->count;
		new_list[idx]			= *texture;
		new_list[idx].idx		= (tpbrt_idx_t)idx;
		textures_list->textures = new_list;
		textures_list->count	= idx + 1u;

		return tpbrt_name_index_insert(&textures_list->names, &(TPBRT_NAME_INDEX_VIEW(new_list, idx + 1u, tpbrt_texture_t)),
		  (tpbrt_idx_t)idx);
	}

	tpbrt_error_t tpbrt_textures_list_get(const tpbrt_textures_list_t* const textures_list, const tpbrt_string_t texture_name,
	  const tpbrt_texture_t** const texture) {
			if (texture == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		const tpbrt_idx_t idx = tpbrt_name_index_find(&textures_list->names,
		  &(TPBRT_NAME_INDEX_VIEW(textures_list->textures, textures_list->count, tpbrt_texture_t)), texture_name, TPBRT_NULL,
		  TPBRT_NULL);
			if (idx == TPBRT_IDX_INVALID) {
				*texture = TPBRT_NULL;
				return TPBRT_ERROR_NOT_FOUND;
			}

		*texture = &textures_list->textures[idx];
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_textures_list_shrink_to_fit(tpbrt_textures_list_t* const textures_list) {
			if (textures_list == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		textures_list->textures =
		  tpbrt_array_shrink_to_fit(textures_list->textures, textures_list->count, sizeof(tpbrt_texture_t));
		return tpbrt_name_index_shrink(&textures_list->names,
		  &(TPBRT_NAME_INDEX_VIEW(textures_list->textures, textures_list->count, tpbrt_texture_t)));
	}

	tpbrt_error_t tpbrt_textures_list_get_handle(const tpbrt_textures_list_t* const textures_list,
	  const tpbrt_string_t texture_name, tpbrt_texture_handle_t* const texture_handle) {
			if (texture_handle == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		const tpbrt_texture_t* texture;
		const tpbrt_error_t err = tpbrt_textures_list_get(textures_list, texture_name, &texture);
			if (err != TPBRT_ERROR_NONE) {
				*texture_handle = TPBRT_TEXTURE_HANDLE_INVALID;
				return err;
			}

		*texture_handle = TPBRT_TEXTURE_HANDLE_TEXTURE(texture->idx);
		return TPBRT_ERROR_NONE;
	}

	static tpbrt_bool_t tpbrt_texture_has_type(const void* const element, const void* const context) {
		return ((const tpbrt_texture_t*)element)->type == *(const tpbrt_texture_type_t*)context ? TPBRT_TRUE : TPBRT_FALSE;
	}

	tpbrt_error_t tpbrt_textures_list_get_handle_with_type(const tpbrt_textures_list_t* const textures_list,
	  const tpbrt_string_t texture_name, const tpbrt_texture_type_t type, tpbrt_texture_handle_t* const texture_handle) {
			if (texture_handle == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		const tpbrt_name_index_view_t view =
		  TPBRT_NAME_INDEX_VIEW(textures_list->textures, textures_list->count, tpbrt_texture_t);
		const tpbrt_idx_t idx = tpbrt_name_index_find(&textures_list->names, &view, texture_name, tpbrt_texture_has_type, &type);
			if (idx == TPBRT_IDX_INVALID) {
				*texture_handle = TPBRT_TEXTURE_HANDLE_INVALID;
				return tpbrt_name_index_find(&textures_list->names, &view, texture_name, TPBRT_NULL, TPBRT_NULL) !=
						   TPBRT_IDX_INVALID ?
						 TPBRT_ERROR_WRONG_TEXTURE_TYPE :
						 TPBRT_ERROR_NOT_FOUND;
			}

		*texture_handle = TPBRT_TEXTURE_HANDLE_TEXTURE(textures_list->textures[idx].idx);
		return TPBRT_ERROR_NONE;
	}

	tpbrt_bool_t tpbrt_textures_list_contains(const tpbrt_textures_list_t* const textures_list, const tpbrt_string_t texture_name,
	  const tpbrt_texture_type_t type) {
		return tpbrt_name_index_find(&textures_list->names,
				 &(TPBRT_NAME_INDEX_VIEW(textures_list->textures, textures_list->count, tpbrt_texture_t)), texture_name,
				 tpbrt_texture_has_type, &type) != TPBRT_IDX_INVALID ?
				 TPBRT_TRUE :
				 TPBRT_FALSE;
	}

	void tpbrt_textures_list_free(tpbrt_textures_list_t* const textures_list) {
			if (textures_list != TPBRT_NULL) { tpbrt_name_index_free(&textures_list->names); }
			if (textures_list == TPBRT_NULL) { return; }

			if (textures_list->textures != TPBRT_NULL) {
					for (tpbrt_size_t i = 0; i < textures_list->count; ++i) { tpbrt_texture_free(textures_list->textures + i); }

				free(textures_list->textures);
			}

		*textures_list = TPBRT_EMPTY_TEXTURES_LIST;
	}

	tpbrt_bool_t tpbrt_textures_list_empty(const tpbrt_textures_list_t* const textures_list) {
		return textures_list->textures == TPBRT_NULL || textures_list->count == 0 ? TPBRT_TRUE : TPBRT_FALSE;
	}

#pragma endregion

#pragma region API

	tpbrt_error_t tpbrt_get_texture_by_name(const tpbrt_textures_list_t* const textures, const tpbrt_string_t name,
	  const tpbrt_texture_t** const texture) {
		return tpbrt_textures_list_get(textures, name, texture);
	}

	tpbrt_error_t tpbrt_get_texture_by_handle(const tpbrt_textures_list_t* const textures,
	  const tpbrt_texture_handle_t* const handle, const tpbrt_texture_t** const texture) {
			if (texture == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (handle->value_type != TPBRT_TEXTURE_HANDLE_VALUE_TYPE_TEXTURE_IDX) {
				*texture = TPBRT_NULL;
				return TPBRT_ERROR_INVALID_HANDLE;
			}

			if (handle->as.texture_idx == TPBRT_TEXTURE_IDX_INVALID || handle->as.texture_idx >= textures->count) {
				*texture = TPBRT_NULL;
				return TPBRT_ERROR_INVALID_HANDLE;
			}

		*texture = &textures->textures[handle->as.texture_idx];
		return TPBRT_ERROR_NONE;
	}

#pragma endregion
#ifdef __cplusplus
}
#endif
