#include <tinypbrt/pch.h>

#include <tinypbrt/detail/texture_internal.h>

#include <tinypbrt/detail/common_internal.h>
#include <tinypbrt/detail/math_internal.h>
#include <tinypbrt/detail/param_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

#pragma region TEXTURE

	tpbrt_error_t tpbrt_texture_type_from_string(const tpbrt_string_t type_str, tpbrt_texture_type_t* const type_out) {
		static const tpbrt_string_t TYPES_STRS[TPBRT_TEXTURE_TYPE_MAX_NUM] = {
			TPBRT_STRING("float"),
			TPBRT_STRING("spectrum"),
		};

			if (type_out == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (tpbrt_string_empty(type_str)) { return TPBRT_ERROR_INVALID_TEXTURE_TYPE; }

			for (tpbrt_texture_type_t t = 0; t < TPBRT_TEXTURE_TYPE_MAX_NUM; ++t) {
					if (tpbrt_string_equals(type_str, TYPES_STRS[t])) {
						*type_out = t;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_INVALID_TEXTURE_TYPE;
	}

	tpbrt_error_t tpbrt_texture_class_from_string(const tpbrt_string_t class_str, tpbrt_texture_class_t* const class_out) {
		static const tpbrt_string_t CLASSES_STRS[TPBRT_TEXTURE_CLASS_MAX_NUM] = {
			TPBRT_STRING("bilerp"),
			TPBRT_STRING("checkerboard"),
			TPBRT_STRING("constant"),
			TPBRT_STRING("directionmix"),
			TPBRT_STRING("dots"),
			TPBRT_STRING("fbm"),
			TPBRT_STRING("imagemap"),
			TPBRT_STRING("marble"),
			TPBRT_STRING("mix"),
			TPBRT_STRING("ptex"),
			TPBRT_STRING("scale"),
			TPBRT_STRING("windy"),
			TPBRT_STRING("wrinkled"),
		};

			if (class_out == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (tpbrt_string_empty(class_str)) { return TPBRT_ERROR_INVALID_TEXTURE_CLASS; }

			for (tpbrt_texture_class_t c = 0; c < TPBRT_TEXTURE_CLASS_MAX_NUM; ++c) {
					if (tpbrt_string_equals(class_str, CLASSES_STRS[c])) {
						*class_out = c;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_INVALID_TEXTURE_CLASS;
	}

	tpbrt_error_t tpbrt_texture_wrap_from_string(const tpbrt_string_t wrap_str, tpbrt_texture_wrap_t* const wrap_out) {
		static const tpbrt_string_t WRAPS_STRS[TPBRT_TEXTURE_WRAP_MAX_NUM] = {
			TPBRT_STRING("repeat"),
			TPBRT_STRING("black"),
			TPBRT_STRING("clamp"),
		};

			if (wrap_out == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (tpbrt_string_empty(wrap_str)) { return TPBRT_ERROR_INVALID_TEXTURE_WRAP; }

			for (tpbrt_texture_wrap_t w = 0; w < TPBRT_TEXTURE_WRAP_MAX_NUM; ++w) {
					if (tpbrt_string_equals(wrap_str, WRAPS_STRS[w])) {
						*wrap_out = w;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_INVALID_TEXTURE_WRAP;
	}

	tpbrt_error_t tpbrt_texture_filter_from_string(const tpbrt_string_t filter_str, tpbrt_texture_filter_t* const filter_out) {
		static const tpbrt_string_t FILTERS_STRS[TPBRT_TEXTURE_FILTER_MAX_NUM] = {
			TPBRT_STRING("bilinear"),
			TPBRT_STRING("ewa"),
			TPBRT_STRING("trilinear"),
			TPBRT_STRING("point"),
		};

			if (filter_out == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (tpbrt_string_empty(filter_str)) { return TPBRT_ERROR_INVALID_TEXTURE_FILTER; }

			for (tpbrt_texture_filter_t f = 0; f < TPBRT_TEXTURE_FILTER_MAX_NUM; ++f) {
					if (tpbrt_string_equals(filter_str, FILTERS_STRS[f])) {
						*filter_out = f;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_INVALID_TEXTURE_FILTER;
	}

	tpbrt_error_t tpbrt_texture_encoding_from_string(const tpbrt_string_t encoding_str,
	  tpbrt_texture_encoding_t* const encoding_out, tpbrt_opt_float_t* const gamma_val_out) {
		static const tpbrt_string_t ENCODINGS_STRS[TPBRT_TEXTURE_ENCODING_MAX_NUM - 1] = {
			TPBRT_STRING("sRGB"),
			TPBRT_STRING("linear"),
		};
		static const tpbrt_string_t GAMMA_STR = TPBRT_STRING("gamma");

			if (encoding_out == TPBRT_NULL || gamma_val_out == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (tpbrt_string_empty(encoding_str)) { return TPBRT_ERROR_INVALID_TEXTURE_ENCODING; }

			for (tpbrt_texture_encoding_t e = 0; e < TPBRT_TEXTURE_ENCODING_MAX_NUM - 1; ++e) {
					if (tpbrt_string_equals(encoding_str, ENCODINGS_STRS[e])) {
						*encoding_out			 = e;
						gamma_val_out->has_value = TPBRT_FALSE;
						return TPBRT_ERROR_NONE;
					}
			}

		tpbrt_string_t token;
			if ((token.data = strstr(encoding_str.data, GAMMA_STR.data)) == TPBRT_NULL) {
				gamma_val_out->has_value = TPBRT_FALSE;
				return TPBRT_ERROR_INVALID_TEXTURE_ENCODING;
			}

			if (token.data - encoding_str.data >= GAMMA_STR.size) {
				gamma_val_out->has_value = TPBRT_FALSE;
				return TPBRT_ERROR_INVALID_TEXTURE_ENCODING;
			}

		token.data += GAMMA_STR.size;


			if ((token = tpbrt_next_value(token.data, encoding_str.data + encoding_str.size)).data == TPBRT_NULL) {
				gamma_val_out->has_value = TPBRT_FALSE;
				return TPBRT_ERROR_PARSE_FLOAT;
			}

			if (!tpbrt_read_float_value(token, &gamma_val_out->value)) {
				gamma_val_out->has_value = TPBRT_FALSE;
				return TPBRT_ERROR_PARSE_FLOAT;
			}

		gamma_val_out->has_value = TPBRT_TRUE;
		*encoding_out			 = TPBRT_TEXTURE_ENCODING_GAMMA;
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_texture_mapping_from_string(const tpbrt_string_t mapping_str,
	  tpbrt_texture_mapping_t* const mapping_out) {
		static const tpbrt_string_t MAPPINGS_STRS[TPBRT_TEXTURE_MAPPING_MAX_NUM] = {
			TPBRT_STRING("uv"),
			TPBRT_STRING("spherical"),
			TPBRT_STRING("cylindrical"),
			TPBRT_STRING("planar"),
		};

			if (mapping_out == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (tpbrt_string_empty(mapping_str)) { return TPBRT_ERROR_INVALID_TEXTURE_MAPPING; }

			for (tpbrt_texture_mapping_t m = 0; m < TPBRT_TEXTURE_MAPPING_MAX_NUM; ++m) {
					if (tpbrt_string_equals(mapping_str, MAPPINGS_STRS[m])) {
						*mapping_out = m;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_INVALID_TEXTURE_MAPPING;
	}

	static tpbrt_error_t tpbrt_texture_read_mapping_uv(tpbrt_texture_mapping_uv_params_t* const uv,
	  const tpbrt_params_list_t params) {
			if (uv == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t U_SCALE_STR = TPBRT_STRING("uscale");
		static const tpbrt_string_t V_SCALE_STR = TPBRT_STRING("vscale");
		static const tpbrt_string_t U_DELTA_STR = TPBRT_STRING("udelta");
		static const tpbrt_string_t V_DELTA_STR = TPBRT_STRING("vdelta");

		tpbrt_error_t err						= tpbrt_params_list_get_float(params, U_SCALE_STR, 1.0f, &uv->u_scale);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_float(params, V_SCALE_STR, 1.0f, &uv->v_scale);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_float(params, U_DELTA_STR, 0.0f, &uv->u_delta);
			if (err != TPBRT_ERROR_NONE) { return err; }

		return tpbrt_params_list_get_float(params, V_DELTA_STR, 0.0f, &uv->v_delta);
	}

	static tpbrt_error_t tpbrt_texture_read_mapping_spherical(tpbrt_texture_mapping_spherical_params_t* const spherical,
	  const tpbrt_mat4_animated_t* ctm) {
			if (spherical == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		spherical->transform = *ctm;

		return TPBRT_ERROR_NONE;
	}

	static tpbrt_error_t tpbrt_texture_read_mapping_cylindrical(tpbrt_texture_mapping_cylindrical_params_t* const cylindrical,
	  const tpbrt_mat4_animated_t* ctm) {
			if (cylindrical == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		cylindrical->transform = *ctm;

		return TPBRT_ERROR_NONE;
	}

	static tpbrt_error_t tpbrt_texture_read_mapping_planar(tpbrt_texture_mapping_planar_params_t* const planar,
	  const tpbrt_params_list_t params) {
			if (planar == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t U_DELTA_STR = TPBRT_STRING("udelta");
		static const tpbrt_string_t V_DELTA_STR = TPBRT_STRING("vdelta");
		static const tpbrt_string_t V1_STR		= TPBRT_STRING("v1");
		static const tpbrt_string_t V2_STR		= TPBRT_STRING("v2");

		tpbrt_error_t err						= tpbrt_params_list_get_float(params, U_DELTA_STR, 0.0f, &planar->u_delta);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_float(params, V_DELTA_STR, 0.0f, &planar->v_delta);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_vec(params, V1_STR, &planar->v1);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

			if (err == TPBRT_ERROR_NOT_FOUND) { planar->v1 = TPBRT_VEC3(1.0f, 0.0f, 0.0f); }

		err = tpbrt_params_list_get_vec(params, V2_STR, &planar->v2);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

			if (err == TPBRT_ERROR_NOT_FOUND) { planar->v2 = TPBRT_VEC3(0.0f, 1.0f, 0.0f); }

		return TPBRT_ERROR_NONE;
	}

	static tpbrt_error_t tpbrt_texture_read_bilerp(tpbrt_texture_bilerp_params_t* const bilerp, const tpbrt_params_list_t params,
	  const tpbrt_textures_list_t textures) {
			if (bilerp == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t V00_STR = TPBRT_STRING("v00");
		static const tpbrt_string_t V01_STR = TPBRT_STRING("v01");
		static const tpbrt_string_t V10_STR = TPBRT_STRING("v10");
		static const tpbrt_string_t V11_STR = TPBRT_STRING("v11");

		tpbrt_error_t err					= tpbrt_params_list_get_texture(params, V00_STR, textures, &bilerp->v00);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

			if (err == TPBRT_ERROR_NOT_FOUND) { bilerp->v00 = TPBRT_TEXTURE_HANDLE_FLOAT(0.0f); }

		err = tpbrt_params_list_get_texture(params, V01_STR, textures, &bilerp->v01);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

			if (err == TPBRT_ERROR_NOT_FOUND) { bilerp->v01 = TPBRT_TEXTURE_HANDLE_FLOAT(1.0f); }

		err = tpbrt_params_list_get_texture(params, V10_STR, textures, &bilerp->v10);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

			if (err == TPBRT_ERROR_NOT_FOUND) { bilerp->v10 = TPBRT_TEXTURE_HANDLE_FLOAT(0.0f); }

		err = tpbrt_params_list_get_texture(params, V11_STR, textures, &bilerp->v11);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

			if (err == TPBRT_ERROR_NOT_FOUND) { bilerp->v11 = TPBRT_TEXTURE_HANDLE_FLOAT(1.0f); }

		return TPBRT_ERROR_NONE;
	}

	static tpbrt_error_t tpbrt_texture_read_checkerboard(tpbrt_texture_checkerboard_params_t* const checkerboard,
	  const tpbrt_params_list_t params, const tpbrt_textures_list_t textures) {
			if (checkerboard == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t DIMENSION_STR = TPBRT_STRING("dimension");
		static const tpbrt_string_t TEX1_STR	  = TPBRT_STRING("tex1");
		static const tpbrt_string_t TEX2_STR	  = TPBRT_STRING("tex2");

		tpbrt_error_t err = tpbrt_params_list_get_uint(params, DIMENSION_STR, 2u, &checkerboard->dimension);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_texture(params, TEX1_STR, textures, &checkerboard->tex1);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

			if (err == TPBRT_ERROR_NOT_FOUND) { checkerboard->tex1 = TPBRT_TEXTURE_HANDLE_FLOAT(1.0f); }

		err = tpbrt_params_list_get_texture(params, TEX2_STR, textures, &checkerboard->tex2);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

			if (err == TPBRT_ERROR_NOT_FOUND) { checkerboard->tex2 = TPBRT_TEXTURE_HANDLE_FLOAT(0.0f); }

		return TPBRT_ERROR_NONE;
	}

	static tpbrt_error_t tpbrt_texture_read_constant(tpbrt_texture_constant_params_t* const constant,
	  const tpbrt_params_list_t params, const tpbrt_textures_list_t textures) {
			if (constant == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t VALUE_STR = TPBRT_STRING("value");

		const tpbrt_error_t err				  = tpbrt_params_list_get_texture(params, VALUE_STR, textures, &constant->value);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

			if (err == TPBRT_ERROR_NOT_FOUND) { constant->value = TPBRT_TEXTURE_HANDLE_FLOAT(1.0f); }

		return TPBRT_ERROR_NONE;
	}

	static tpbrt_error_t tpbrt_texture_read_direction_mix(tpbrt_texture_direction_mix_params_t* const direction_mix,
	  const tpbrt_params_list_t params, const tpbrt_textures_list_t textures) {
			if (direction_mix == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t TEX1_STR = TPBRT_STRING("tex1");
		static const tpbrt_string_t TEX2_STR = TPBRT_STRING("tex2");
		static const tpbrt_string_t DIR_STR	 = TPBRT_STRING("dir");

		tpbrt_error_t err					 = tpbrt_params_list_get_texture(params, TEX1_STR, textures, &direction_mix->tex1);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

			if (err == TPBRT_ERROR_NOT_FOUND) { direction_mix->tex1 = TPBRT_TEXTURE_HANDLE_FLOAT(0.0f); }

		err = tpbrt_params_list_get_texture(params, TEX2_STR, textures, &direction_mix->tex2);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

			if (err == TPBRT_ERROR_NOT_FOUND) { direction_mix->tex2 = TPBRT_TEXTURE_HANDLE_FLOAT(1.0f); }

		err = tpbrt_params_list_get_vec(params, DIR_STR, &direction_mix->dir);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

			if (err == TPBRT_ERROR_NOT_FOUND) { direction_mix->dir = TPBRT_VEC3(0.0f, 1.0f, 0.0f); }

		return TPBRT_ERROR_NONE;
	}

	static tpbrt_error_t tpbrt_texture_read_dots(tpbrt_texture_dots_params_t* const dots, const tpbrt_params_list_t params,
	  const tpbrt_textures_list_t textures) {
			if (dots == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t INSIDE_STR	= TPBRT_STRING("inside");
		static const tpbrt_string_t OUTSIDE_STR = TPBRT_STRING("outside");

		tpbrt_error_t err						= tpbrt_params_list_get_texture(params, INSIDE_STR, textures, &dots->inside);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

			if (err == TPBRT_ERROR_NOT_FOUND) { dots->inside = TPBRT_TEXTURE_HANDLE_FLOAT(1.0f); }

		err = tpbrt_params_list_get_texture(params, OUTSIDE_STR, textures, &dots->outside);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

			if (err == TPBRT_ERROR_NOT_FOUND) { dots->outside = TPBRT_TEXTURE_HANDLE_FLOAT(0.0f); }

		return TPBRT_ERROR_NONE;
	}

	static tpbrt_error_t tpbrt_texture_read_fbm(tpbrt_texture_fbm_params_t* const fbm, const tpbrt_params_list_t params,
	  const tpbrt_mat4_animated_t* ctm) {
			if (fbm == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t OCTAVES_STR	  = TPBRT_STRING("octaves");
		static const tpbrt_string_t ROUGHNESS_STR = TPBRT_STRING("roughness");

		fbm->transform							  = *ctm;

		static const tpbrt_uint_t OCTAVES_DEFAULT = 8u;
		const tpbrt_error_t err = tpbrt_params_list_get_uint(params, OCTAVES_STR, OCTAVES_DEFAULT, &fbm->octaves);
			if (err != TPBRT_ERROR_NONE) { return err; }

		static const tpbrt_float_t ROUGHNESS_DEFAULT = 0.5f;
		return tpbrt_params_list_get_float(params, ROUGHNESS_STR, ROUGHNESS_DEFAULT, &fbm->roughness);
	}

	static tpbrt_error_t tpbrt_texture_read_wrinkled(tpbrt_texture_wrinkled_params_t* const wrinkled,
	  const tpbrt_params_list_t params, const tpbrt_mat4_animated_t* ctm) {
			if (wrinkled == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		return tpbrt_texture_read_fbm(wrinkled, params, ctm);
	}

	static tpbrt_error_t tpbrt_texture_read_windy(tpbrt_texture_windy_params_t* const windy, const tpbrt_params_list_t params,
	  const tpbrt_mat4_animated_t* ctm) {
			if (windy == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		return tpbrt_texture_read_fbm(windy, params, ctm);
	}

	static tpbrt_error_t tpbrt_texture_read_image_map(tpbrt_texture_image_map_params_t* const image_map,
	  const tpbrt_params_list_t params) {
			if (image_map == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t FILE_NAME_STR	   = TPBRT_STRING("filename");
		static const tpbrt_string_t WRAP_STR		   = TPBRT_STRING("wrap");
		static const tpbrt_string_t MAX_ANISOTROPY_STR = TPBRT_STRING("maxanisotropy");
		static const tpbrt_string_t FILTER_STR		   = TPBRT_STRING("filter");
		static const tpbrt_string_t ENCODING_STR	   = TPBRT_STRING("encoding");
		static const tpbrt_string_t SCALE_STR		   = TPBRT_STRING("scale");
		static const tpbrt_string_t INVERT_STR		   = TPBRT_STRING("invert");

		tpbrt_error_t err = tpbrt_params_list_get_string(params, FILE_NAME_STR, &image_map->file_name);
			if (err != TPBRT_ERROR_NONE) { return err; }

		tpbrt_string_t temp_str;
		err = tpbrt_params_list_get_string(params, WRAP_STR, &temp_str);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

			if (err == TPBRT_ERROR_NOT_FOUND) { image_map->wrap = TPBRT_TEXTURE_WRAP_REPEAT; }
			else {
				tpbrt_texture_wrap_t wrap;
				err = tpbrt_texture_wrap_from_string(temp_str, &wrap);
				tpbrt_string_free(&temp_str);
					if (err != TPBRT_ERROR_NONE) { return err; }
				image_map->wrap = wrap;
			}

		static const tpbrt_float_t MAX_ANISOTROPY_DEFAULT = 8.0f;
		err = tpbrt_params_list_get_float(params, MAX_ANISOTROPY_STR, MAX_ANISOTROPY_DEFAULT, &image_map->max_anisotropy);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_string(params, FILTER_STR, &temp_str);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

			if (err == TPBRT_ERROR_NOT_FOUND) { image_map->filter = TPBRT_TEXTURE_FILTER_BILINEAR; }
			else {
				tpbrt_texture_filter_t filter;
				err = tpbrt_texture_filter_from_string(temp_str, &filter);
				tpbrt_string_free(&temp_str);
					if (err != TPBRT_ERROR_NONE) { return err; }
				image_map->filter = filter;
			}

		err = tpbrt_params_list_get_string(params, ENCODING_STR, &temp_str);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

			if (err == TPBRT_ERROR_NOT_FOUND) { image_map->encoding = TPBRT_TEXTURE_ENCODING_SRGB; }
			else {
				tpbrt_texture_encoding_t encoding;
				err = tpbrt_texture_encoding_from_string(temp_str, &encoding, &image_map->gamma);
				tpbrt_string_free(&temp_str);
					if (err != TPBRT_ERROR_NONE) { return err; }
				image_map->encoding = encoding;
			}

		err = tpbrt_params_list_get_float(params, SCALE_STR, 1.0f, &image_map->scale);
			if (err != TPBRT_ERROR_NONE) { return err; }

		tpbrt_bool_t invert;
		err = tpbrt_params_list_get_bool(params, INVERT_STR, TPBRT_FALSE, &invert);
			if (err != TPBRT_ERROR_NONE) { return err; }
		image_map->invert = invert;

		return TPBRT_ERROR_NONE;
	}

	static tpbrt_error_t tpbrt_texture_read_marble(tpbrt_texture_marble_params_t* const marble,
	  const tpbrt_params_list_t params) {
			if (marble == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t OCTAVES_STR	  = TPBRT_STRING("octaves");
		static const tpbrt_string_t ROUGHNESS_STR = TPBRT_STRING("roughness");
		static const tpbrt_string_t SCALE_STR	  = TPBRT_STRING("scale");
		static const tpbrt_string_t VARIATION_STR = TPBRT_STRING("variation");

		static const tpbrt_uint_t OCTAVES_DEFAULT = 8u;
		tpbrt_error_t err = tpbrt_params_list_get_uint(params, OCTAVES_STR, OCTAVES_DEFAULT, &marble->octaves);
			if (err != TPBRT_ERROR_NONE) { return err; }

		static const tpbrt_float_t ROUGHNESS_DEFAULT = 0.5f;
		err = tpbrt_params_list_get_float(params, ROUGHNESS_STR, ROUGHNESS_DEFAULT, &marble->roughness);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_float(params, SCALE_STR, 1.0f, &marble->scale);
			if (err != TPBRT_ERROR_NONE) { return err; }

		static const tpbrt_float_t VARIATION_DEFAULT = 0.2f;
		return tpbrt_params_list_get_float(params, VARIATION_STR, VARIATION_DEFAULT, &marble->variation);
	}

	static tpbrt_error_t tpbrt_texture_read_mix(tpbrt_texture_mix_params_t* const mix, const tpbrt_params_list_t params,
	  const tpbrt_textures_list_t textures) {
			if (mix == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t TEX1_STR   = TPBRT_STRING("tex1");
		static const tpbrt_string_t TEX2_STR   = TPBRT_STRING("tex2");
		static const tpbrt_string_t AMOUNT_STR = TPBRT_STRING("amount");

		tpbrt_error_t err					   = tpbrt_params_list_get_texture(params, TEX1_STR, textures, &mix->tex1);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

			if (err == TPBRT_ERROR_NOT_FOUND) { mix->tex1 = TPBRT_TEXTURE_HANDLE_FLOAT(0.0f); }

		err = tpbrt_params_list_get_texture(params, TEX2_STR, textures, &mix->tex2);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

			if (err == TPBRT_ERROR_NOT_FOUND) { mix->tex2 = TPBRT_TEXTURE_HANDLE_FLOAT(1.0f); }

		err = tpbrt_params_list_get_texture_of_type(params, AMOUNT_STR, textures, TPBRT_TEXTURE_TYPE_FLOAT, &mix->amount);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

			if (err == TPBRT_ERROR_NOT_FOUND) { mix->amount = TPBRT_TEXTURE_HANDLE_FLOAT(0.5f); }

		return TPBRT_ERROR_NONE;
	}

	static tpbrt_error_t tpbrt_texture_read_ptex(tpbrt_texture_ptex_params_t* const ptex, const tpbrt_params_list_t params) {
			if (ptex == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t ENCODING_STR  = TPBRT_STRING("encoding");
		static const tpbrt_string_t FILE_NAME_STR = TPBRT_STRING("filename");
		static const tpbrt_string_t SCALE_STR	  = TPBRT_STRING("scale");

		tpbrt_string_t temp_str;
		tpbrt_error_t err = tpbrt_params_list_get_string(params, ENCODING_STR, &temp_str);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

			if (err == TPBRT_ERROR_NOT_FOUND) {
				static const tpbrt_float_t GAMMA_DEFAULT = 2.2f;

				ptex->encoding							 = TPBRT_TEXTURE_ENCODING_GAMMA;
				ptex->gamma								 = TPBRT_OPT_FLOAT(GAMMA_DEFAULT);
			}
			else {
				tpbrt_texture_encoding_t encoding;
				err = tpbrt_texture_encoding_from_string(temp_str, &encoding, &ptex->gamma);
				tpbrt_string_free(&temp_str);
					if (err != TPBRT_ERROR_NONE) { return err; }
				ptex->encoding = encoding;
			}

		err = tpbrt_params_list_get_string(params, FILE_NAME_STR, &ptex->file_name);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

		return tpbrt_params_list_get_float(params, SCALE_STR, 1.0f, &ptex->scale);
	}

	static tpbrt_error_t tpbrt_texture_read_scale(tpbrt_texture_scale_params_t* const scale, const tpbrt_params_list_t params,
	  const tpbrt_textures_list_t textures) {
			if (scale == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t TEX_STR	  = TPBRT_STRING("tex");
		static const tpbrt_string_t SCALE_STR = TPBRT_STRING("scale");

		tpbrt_error_t err					  = tpbrt_params_list_get_texture(params, TEX_STR, textures, &scale->tex);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

			if (err == TPBRT_ERROR_NOT_FOUND) { scale->tex = TPBRT_TEXTURE_HANDLE_FLOAT(1.0f); }

		err = tpbrt_params_list_get_texture_of_type(params, SCALE_STR, textures, TPBRT_TEXTURE_TYPE_FLOAT, &scale->scale);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

			if (err == TPBRT_ERROR_NOT_FOUND) { scale->scale = TPBRT_TEXTURE_HANDLE_FLOAT(1.0f); }

		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_texture_init(const tpbrt_string_t name, const tpbrt_string_t type_str, const tpbrt_string_t class_str,
	  const tpbrt_params_list_t params, const tpbrt_textures_list_t textures, const tpbrt_mat4_animated_t* ctm,
	  tpbrt_texture_t* const texture) {
			if (ctm == TPBRT_NULL || texture == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

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
		texture->tex_class						= tex_class;

		static const tpbrt_string_t MAPPING_STR = TPBRT_STRING("mapping");

		tpbrt_string_t mapping_str;
		err = tpbrt_params_list_get_string(params, MAPPING_STR, &mapping_str);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
				tpbrt_texture_free(texture);
				return err;
			}

			if (err == TPBRT_ERROR_NOT_FOUND) { texture->mapping = TPBRT_TEXTURE_MAPPING_UV; }
			else {
				tpbrt_texture_mapping_t mapping;
				err = tpbrt_texture_mapping_from_string(mapping_str, &mapping);
				tpbrt_string_free(&mapping_str);
					if (err != TPBRT_ERROR_NONE) {
						tpbrt_texture_free(texture);
						return err;
					}
				texture->mapping = mapping;
			}

			// mapping
			switch (texture->mapping) {
			default:
				case TPBRT_TEXTURE_MAPPING_UV: {
					err = tpbrt_texture_read_mapping_uv(&texture->mapping_as.uv, params);
					break;
				}
				case TPBRT_TEXTURE_MAPPING_SPHERICAL: {
					err = tpbrt_texture_read_mapping_spherical(&texture->mapping_as.spherical, ctm);
					break;
				}
				case TPBRT_TEXTURE_MAPPING_CYLINDRICAL: {
					err = tpbrt_texture_read_mapping_cylindrical(&texture->mapping_as.cylindrical, ctm);
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
					err = tpbrt_texture_read_bilerp(&texture->as.bilerp, params, textures);
					break;
				}
				case TPBRT_TEXTURE_CLASS_CHECKER_BOARD: {
					err = tpbrt_texture_read_checkerboard(&texture->as.checkerboard, params, textures);
					break;
				}
				case TPBRT_TEXTURE_CLASS_CONSTANT: {
					err = tpbrt_texture_read_constant(&texture->as.constant, params, textures);
					break;
				}
				case TPBRT_TEXTURE_CLASS_DIRECTION_MIX: {
					err = tpbrt_texture_read_direction_mix(&texture->as.direction_mix, params, textures);
					break;
				}
				case TPBRT_TEXTURE_CLASS_DOTS: {
					err = tpbrt_texture_read_dots(&texture->as.dots, params, textures);
					break;
				}
				case TPBRT_TEXTURE_CLASS_FBM: {
					err = tpbrt_texture_read_fbm(&texture->as.fbm, params, ctm);
					break;
				}
				case TPBRT_TEXTURE_CLASS_WRINKLED: {
					err = tpbrt_texture_read_wrinkled(&texture->as.wrinkled, params, ctm);
					break;
				}
				case TPBRT_TEXTURE_CLASS_WINDY: {
					err = tpbrt_texture_read_windy(&texture->as.windy, params, ctm);
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
					err = tpbrt_texture_read_mix(&texture->as.mix, params, textures);
					break;
				}
				case TPBRT_TEXTURE_CLASS_PTEX: {
					err = tpbrt_texture_read_ptex(&texture->as.ptex, params);
					break;
				}
				case TPBRT_TEXTURE_CLASS_SCALE: {
					err = tpbrt_texture_read_scale(&texture->as.scale, params, textures);
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
				case TPBRT_TEXTURE_CLASS_IMAGE_MAP: {
					tpbrt_string_free(&texture->as.image_map.file_name);
					break;
				}
				case TPBRT_TEXTURE_CLASS_PTEX: {
					tpbrt_string_free(&texture->as.ptex.file_name);
					break;
				}
			default: break;
			}
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

		const tpbrt_size_t new_count = textures_list->count + 1;

		tpbrt_texture_t* new_list	 = realloc(textures_list->textures, sizeof(tpbrt_texture_t) * new_count);
			if (new_list == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }

		new_list[textures_list->count]	   = *texture;
		new_list[textures_list->count].idx = textures_list->count;
		*textures_list					   = (tpbrt_textures_list_t) { .textures = new_list, .count = new_count };
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_textures_list_get(const tpbrt_textures_list_t textures_list, const tpbrt_string_t texture_name,
	  const tpbrt_texture_t** const texture) {
			if (texture == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			for (tpbrt_size_t i = 0; i < textures_list.count; i++) {
					if (tpbrt_string_equals(textures_list.textures[i].name, texture_name)) {
						*texture = &textures_list.textures[i];
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_NOT_FOUND;
	}

	tpbrt_error_t tpbrt_textures_list_get_handle(const tpbrt_textures_list_t textures_list, const tpbrt_string_t texture_name,
	  tpbrt_texture_handle_t* const texture_handle) {
			if (texture_handle == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		const tpbrt_texture_t* texture;
		const tpbrt_error_t err = tpbrt_textures_list_get(textures_list, texture_name, &texture);
			if (err != TPBRT_ERROR_NONE) {
				*texture_handle = TPBRT_TEXTURE_HANDLE_NONE;
				return err;
			}

		*texture_handle = TPBRT_TEXTURE_HANDLE_TEXTURE(texture->idx);
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_textures_list_get_handle_with_type(const tpbrt_textures_list_t textures_list,
	  const tpbrt_string_t texture_name, const tpbrt_texture_type_t type, tpbrt_texture_handle_t* const texture_handle) {
			if (texture_handle == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		const tpbrt_texture_t* texture;
		const tpbrt_error_t err = tpbrt_textures_list_get(textures_list, texture_name, &texture);
			if (err != TPBRT_ERROR_NONE) {
				*texture_handle = TPBRT_TEXTURE_HANDLE_NONE;
				return err;
			}

			if (texture->type != type) {
				*texture_handle = TPBRT_TEXTURE_HANDLE_NONE;
				return TPBRT_ERROR_WRONG_TEXTURE_TYPE;
			}

		*texture_handle = TPBRT_TEXTURE_HANDLE_TEXTURE(texture->idx);
		return TPBRT_ERROR_NONE;
	}

	void tpbrt_textures_list_free(tpbrt_textures_list_t* const textures_list) {
			if (textures_list == TPBRT_NULL) { return; }

			if (textures_list->textures != TPBRT_NULL) {
					for (tpbrt_size_t i = 0; i < textures_list->count; ++i) { tpbrt_texture_free(textures_list->textures + i); }

				free(textures_list->textures);
			}

		*textures_list = TPBRT_EMPTY_TEXTURES_LIST;
	}

	tpbrt_bool_t tpbrt_textures_list_empty(const tpbrt_textures_list_t textures_list) {
		return textures_list.textures == TPBRT_NULL || textures_list.count == 0 ? TPBRT_TRUE : TPBRT_FALSE;
	}

#pragma endregion

#pragma region API

	tpbrt_error_t tpbrt_get_texture_by_name(const tpbrt_textures_list_t textures, const tpbrt_string_t name,
	  const tpbrt_texture_t** const texture) {
		return tpbrt_textures_list_get(textures, name, texture);
	}

	tpbrt_error_t tpbrt_get_texture_by_handle(const tpbrt_textures_list_t textures, const tpbrt_texture_handle_t handle,
	  const tpbrt_texture_t** const texture) {
			if (texture == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (handle.value_type != TPBRT_TEXTURE_HANDLE_VALUE_TYPE_TEXTURE) {
				*texture = TPBRT_NULL;
				return TPBRT_ERROR_INVALID_HANDLE;
			}

			if (handle.as.texture_idx == TPBRT_TEXTURE_IDX_INVALID || handle.as.texture_idx >= textures.count) {
				*texture = TPBRT_NULL;
				return TPBRT_ERROR_INVALID_HANDLE;
			}

		*texture = &textures.textures[handle.as.texture_idx];
		return TPBRT_ERROR_NONE;
	}

#pragma endregion
#ifdef __cplusplus
}
#endif