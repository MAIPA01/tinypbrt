#include <tinypbrt/pch.h>

#include <tinypbrt/detail/parser_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

	void tpbrt_parser_init(tpbrt_parser_t* const parser, const tpbrt_char_t* const str, const tpbrt_size_t length) {
			if (parser == TPBRT_NULL) { return; }

		tpbrt_tokenizer_init(&parser->tokenizer, str, length);
	}

	static tpbrt_error_t read_token(tpbrt_parser_t* const parser, tpbrt_token_t* const out_token) {
			if (!tpbrt_tokenizer_next(&parser->tokenizer, out_token)) { return TPBRT_ERROR_END_OF_FILE; }
			if (!tpbrt_token_is_valid(out_token)) { return TPBRT_ERROR_INVALID_TOKEN; }
		return TPBRT_ERROR_NONE;
	}

	static tpbrt_error_t read_str(tpbrt_parser_t* const parser, tpbrt_string_t* const out_str) {
		tpbrt_token_t token;
		const tpbrt_error_t err = read_token(parser, &token);
			if (err != TPBRT_ERROR_NONE) { return err; }

			if (!tpbrt_token_is_quoted_string(&token)) { return TPBRT_ERROR_INVALID_STRING; }

		out_str->data = token.value.data + 1;
		out_str->size = token.value.size - 2;
		return TPBRT_ERROR_NONE;
	}

	static tpbrt_error_t read_float(tpbrt_parser_t* const parser, tpbrt_float_t* const out_f) {
		tpbrt_token_t token;
		const tpbrt_error_t err = read_token(parser, &token);
			if (err != TPBRT_ERROR_NONE) { return err; }

		tpbrt_char_t buf[64];
		const tpbrt_size_t n = token.value.size < 63 ? token.value.size : 63;
		strncpy_s(buf, sizeof(tpbrt_char_t) * 64, token.value.data, n);
		buf[n] = '\0';

		tpbrt_char_t* end;
		*out_f = strtof(buf, &end);
			if (end == buf) { return TPBRT_ERROR_PARSE_FLOAT; }
		return TPBRT_ERROR_NONE;
	}

	static tpbrt_error_t read_point(tpbrt_parser_t* const parser, tpbrt_float_t out_p[3]) {
		tpbrt_error_t err = read_float(parser, &out_p[0]);
			if (err != TPBRT_ERROR_NONE) { return err; }
		err = read_float(parser, &out_p[1]);
			if (err != TPBRT_ERROR_NONE) { return err; }
		return read_float(parser, &out_p[2]);
	}

	static tpbrt_error_t read_matrix(tpbrt_parser_t* const parser, tpbrt_float_t out_m[16]) {
			for (int i = 0; i < 16; ++i) {
				const tpbrt_error_t err = read_float(parser, &out_m[i]);
					if (err != TPBRT_ERROR_NONE) { return err; }
			}
		return TPBRT_ERROR_NONE;
	}

	static tpbrt_error_t skip_brace(tpbrt_parser_t* const parser, const tpbrt_bool_t expect_open) {
		tpbrt_token_t token;
		const tpbrt_error_t err = read_token(parser, &token);
			if (err != TPBRT_ERROR_NONE) { return err; }

			if (expect_open && !tpbrt_token_is_open_brace(&token)) { return TPBRT_ERROR_UNEXPECTED_TOKEN; }
			if (!expect_open && !tpbrt_token_is_close_brace(&token)) { return TPBRT_ERROR_UNEXPECTED_TOKEN; }
		return TPBRT_ERROR_NONE;
	}

	static tpbrt_error_t read_param(tpbrt_parser_t* const parser, tpbrt_param_t** const out_param) {
		tpbrt_string_t type_and_name;
		tpbrt_error_t err = read_str(parser, &type_and_name);
			if (err != TPBRT_ERROR_NONE) { return err; }

		tpbrt_token_t value_tok;
		err = read_token(parser, &value_tok);
			if (err != TPBRT_ERROR_NONE) { return err; }

		tpbrt_size_t start_offset;
		tpbrt_size_t end_offset;

		tpbrt_param_value_type_t value_type = TPBRT_PARAM_VALUE_TYPE_SINGLE;

			if (tpbrt_token_is_open_brace(&value_tok)) {
				start_offset = tpbrt_tokenizer_offset(&parser->tokenizer);

					while (TPBRT_TRUE) {
						tpbrt_token_t inner;
						err = read_token(parser, &inner);
							if (err != TPBRT_ERROR_NONE) { return err; }

							if (tpbrt_token_is_close_brace(&inner)) {
								end_offset = (tpbrt_size_t)(inner.value.data - parser->tokenizer.str.data);
								value_type = TPBRT_PARAM_VALUE_TYPE_ARRAY;
								break;
							}
							if (tpbrt_token_is_directive(&inner)) { return TPBRT_ERROR_UNEXPECTED_TOKEN; }
					}
			}
			else {
				start_offset = (tpbrt_size_t)(value_tok.value.data - parser->tokenizer.str.data);
				end_offset	 = start_offset + value_tok.value.size;
			}

		const tpbrt_string_t value_str = { .data = parser->tokenizer.str.data + start_offset, .size = end_offset - start_offset };
			if (value_str.size >= 2u && value_str.data[0] == '\"' && value_str.data[value_str.size - 1] == '\"') {
				value_type = TPBRT_PARAM_VALUE_TYPE_STRING;
			}

		return tpbrt_create_param(&type_and_name, value_type, &value_str, out_param);
	}

	static tpbrt_error_t read_param_list(tpbrt_parser_t* const parser, tpbrt_params_list_t** const out_list) {
		tpbrt_error_t err = tpbrt_create_empty_params_list(out_list);
			if (err != TPBRT_ERROR_NONE) { return err; }

			while (TPBRT_TRUE) {
				tpbrt_token_t next_tok;
					if (!tpbrt_tokenizer_peek(&parser->tokenizer, &next_tok)) { break; }

					if (!tpbrt_token_is_quoted_string(&next_tok)) { break; }

				tpbrt_param_t* param = TPBRT_NULL;
				err					 = read_param(parser, &param);
					if (err != TPBRT_ERROR_NONE) {
						tpbrt_free_params_list(out_list);
						return err;
					}

				err = tpbrt_params_list_add_param(*out_list, param);
				tpbrt_free_param(&param);

					if (err != TPBRT_ERROR_NONE) {
						tpbrt_free_params_list(out_list);
						return err;
					}
			}
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_parser_parse_next(tpbrt_parser_t* const parser, tpbrt_element_t* const out_element) {
			if (!parser || !out_element) { return TPBRT_ERROR_INVALID_POINTER; }

		tpbrt_token_t token;
		tpbrt_error_t err = read_token(parser, &token);
			if (err != TPBRT_ERROR_NONE) { return err; }

			if (!tpbrt_token_is_directive(&token)) { return TPBRT_ERROR_UNKNOWN_DIRECTIVE; }

		out_element->type = token.directive;

			switch (token.directive) {
			case TPBRT_DIRECTIVE_INCLUDE:
			case TPBRT_DIRECTIVE_IMPORT:	 return read_str(parser, &out_element->as.include_import.path);

				case TPBRT_DIRECTIVE_OPTION: {
					tpbrt_param_t* p;
					err = read_param(parser, &p);
						if (err == TPBRT_ERROR_NONE) {
							out_element->as.option.param = *p;
							tpbrt_free_param(&p);
						}
					return err;
				}

			case TPBRT_DIRECTIVE_FILM:
			case TPBRT_DIRECTIVE_CAMERA:
			case TPBRT_DIRECTIVE_SAMPLER:
			case TPBRT_DIRECTIVE_INTEGRATOR:
			case TPBRT_DIRECTIVE_ACCELERATOR:
			case TPBRT_DIRECTIVE_LIGHT_SOURCE:
			case TPBRT_DIRECTIVE_AREA_LIGHT_SOURCE:
			case TPBRT_DIRECTIVE_MATERIAL:
			case TPBRT_DIRECTIVE_SHAPE:
			case TPBRT_DIRECTIVE_MAKE_NAMED_MEDIUM:
				err = read_str(parser, &out_element->as.generic_with_params.type_name);
					if (err != TPBRT_ERROR_NONE) { return err; }
				return read_param_list(parser, &out_element->as.generic_with_params.params);

			case TPBRT_DIRECTIVE_COLOR_SPACE:
			case TPBRT_DIRECTIVE_ACTIVE_TRANSFORM:	  return read_str(parser, &out_element->as.single_string.type_name);

			case TPBRT_DIRECTIVE_COORDINATE_SYSTEM:
			case TPBRT_DIRECTIVE_COORD_SYS_TRANSFORM:
			case TPBRT_DIRECTIVE_PIXEL_FILTER:
			case TPBRT_DIRECTIVE_NAMED_MATERIAL:
			case TPBRT_DIRECTIVE_OBJECT_BEGIN:
			case TPBRT_DIRECTIVE_OBJECT_INSTANCE:	  return read_str(parser, &out_element->as.named_entity.name);

			case TPBRT_DIRECTIVE_TRANSLATE:
			case TPBRT_DIRECTIVE_SCALE:				  return read_point(parser, out_element->as.vector3.v);

			case TPBRT_DIRECTIVE_ROTATE:
				err = read_float(parser, &out_element->as.rotate.angle);
					if (err != TPBRT_ERROR_NONE) { return err; }
				return read_point(parser, out_element->as.rotate.v);

			case TPBRT_DIRECTIVE_LOOK_AT:
				err = read_point(parser, out_element->as.look_at.eye);
					if (err != TPBRT_ERROR_NONE) { return err; }
				err = read_point(parser, out_element->as.look_at.look_at);
					if (err != TPBRT_ERROR_NONE) { return err; }
				return read_point(parser, out_element->as.look_at.up);

			case TPBRT_DIRECTIVE_TRANSFORM:
			case TPBRT_DIRECTIVE_CONCAT_TRANSFORM:
				err = skip_brace(parser, TPBRT_TRUE);
					if (err != TPBRT_ERROR_NONE) { return err; }
				err = read_matrix(parser, out_element->as.transform.m);
					if (err != TPBRT_ERROR_NONE) { return err; }
				return skip_brace(parser, TPBRT_FALSE);

			case TPBRT_DIRECTIVE_TRANSFORM_TIMES:
				err = read_float(parser, &out_element->as.transform_times.start);
					if (err != TPBRT_ERROR_NONE) { return err; }
				return read_float(parser, &out_element->as.transform_times.end);

			case TPBRT_DIRECTIVE_ATTRIBUTE:
				err = read_str(parser, &out_element->as.attribute.target);
					if (err != TPBRT_ERROR_NONE) { return err; }
				return read_param_list(parser, &out_element->as.attribute.params);

			case TPBRT_DIRECTIVE_MAKE_NAMED_MATERIAL:
				err = read_str(parser, &out_element->as.named_with_params.name);
					if (err != TPBRT_ERROR_NONE) { return err; }
				return read_param_list(parser, &out_element->as.named_with_params.params);

			case TPBRT_DIRECTIVE_TEXTURE:
				err = read_str(parser, &out_element->as.texture.name);
					if (err != TPBRT_ERROR_NONE) { return err; }
				err = read_str(parser, &out_element->as.texture.type_name);
					if (err != TPBRT_ERROR_NONE) { return err; }
				err = read_str(parser, &out_element->as.texture.class_name);
					if (err != TPBRT_ERROR_NONE) { return err; }
				return read_param_list(parser, &out_element->as.texture.params);

			case TPBRT_DIRECTIVE_MEDIUM_INTERFACE:
				err = read_str(parser, &out_element->as.medium_interface.interior);
					if (err != TPBRT_ERROR_NONE) { return err; }
				return read_str(parser, &out_element->as.medium_interface.exterior);

			case TPBRT_DIRECTIVE_IDENTITY:
			case TPBRT_DIRECTIVE_REVERSE_ORIENTATION:
			case TPBRT_DIRECTIVE_WORLD_BEGIN:
			case TPBRT_DIRECTIVE_ATTRIBUTE_BEGIN:
			case TPBRT_DIRECTIVE_ATTRIBUTE_END:
			case TPBRT_DIRECTIVE_OBJECT_END:		  return TPBRT_ERROR_NONE;

			default:								  return TPBRT_ERROR_UNKNOWN_DIRECTIVE;
			}
	}

	void tpbrt_free_element(tpbrt_element_t* const element) {
			if (!element) { return; }

			switch (element->type) {
			case TPBRT_DIRECTIVE_FILM:
			case TPBRT_DIRECTIVE_CAMERA:
			case TPBRT_DIRECTIVE_SAMPLER:
			case TPBRT_DIRECTIVE_INTEGRATOR:
			case TPBRT_DIRECTIVE_ACCELERATOR:
			case TPBRT_DIRECTIVE_LIGHT_SOURCE:
			case TPBRT_DIRECTIVE_AREA_LIGHT_SOURCE:
			case TPBRT_DIRECTIVE_MATERIAL:
			case TPBRT_DIRECTIVE_SHAPE:
			case TPBRT_DIRECTIVE_MAKE_NAMED_MEDIUM:
					if (element->as.generic_with_params.params) {
						tpbrt_free_params_list(&element->as.generic_with_params.params);
					}
				break;
			case TPBRT_DIRECTIVE_ATTRIBUTE:
					if (element->as.attribute.params) { tpbrt_free_params_list(&element->as.attribute.params); }
				break;
			case TPBRT_DIRECTIVE_MAKE_NAMED_MATERIAL:
					if (element->as.named_with_params.params) { tpbrt_free_params_list(&element->as.named_with_params.params); }
				break;
			case TPBRT_DIRECTIVE_TEXTURE:
					if (element->as.texture.params) { tpbrt_free_params_list(&element->as.texture.params); }
				break;
			default: break;
			}
	}

#ifdef __cplusplus
}
#endif