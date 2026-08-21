#include <tinypbrt/pch.h>

#include <tinypbrt/detail/token_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

	static tpbrt_error_t tpbrt_directive_from_string(const tpbrt_string_t* const directive_str, tpbrt_directive_t* directive) {
		static const tpbrt_string_t TYPES_STRS[TPBRT_DIRECTIVE_MAX_NUM] = {
			TPBRT_STRING("Identity"),
			TPBRT_STRING("Translate"),
			TPBRT_STRING("Scale"),
			TPBRT_STRING("Rotate"),
			TPBRT_STRING("LookAt"),
			TPBRT_STRING("CoordinateSystem"),
			TPBRT_STRING("CoordSysTransform"),
			TPBRT_STRING("Transform"),
			TPBRT_STRING("ConcatTransform"),
			TPBRT_STRING("TransformTimes"),
			TPBRT_STRING("ActiveTransform"),

			TPBRT_STRING("Include"),
			TPBRT_STRING("Import"),

			TPBRT_STRING("Option"),

			TPBRT_STRING("Camera"),
			TPBRT_STRING("Sampler"),
			TPBRT_STRING("ColorSpace"),
			TPBRT_STRING("Film"),
			TPBRT_STRING("PixelFilter"),
			TPBRT_STRING("Integrator"),
			TPBRT_STRING("Accelerator"),

			TPBRT_STRING("WorldBegin"),

			TPBRT_STRING("AttributeBegin"),
			TPBRT_STRING("AttributeEnd"),
			TPBRT_STRING("Attribute"),

			TPBRT_STRING("ReverseOrientation"),
			TPBRT_STRING("Shape"),
			TPBRT_STRING("ObjectBegin"),
			TPBRT_STRING("ObjectEnd"),
			TPBRT_STRING("ObjectInstance"),

			TPBRT_STRING("LightSource"),
			TPBRT_STRING("AreaLightSource"),

			TPBRT_STRING("Material"),
			TPBRT_STRING("MakeNamedMaterial"),
			TPBRT_STRING("NamedMaterial"),

			TPBRT_STRING("Texture"),

			TPBRT_STRING("MakeNamedMedium"),
			TPBRT_STRING("MediumInterface"),
		};

			if (directive_str == TPBRT_NULL || directive_str->chars == TPBRT_NULL || directive == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

			for (tpbrt_directive_t d = 0; d < TPBRT_DIRECTIVE_MAX_NUM; ++d) {
					if (directive_str->size == TYPES_STRS[d].size &&
						strncmp(directive_str->chars, TYPES_STRS[d].chars, TYPES_STRS[d].size) == 0) {
						*directive = d;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_UNKNOWN_DIRECTIVE;
	}

	tpbrt_error_t tpbrt_create_token(const tpbrt_string_t* const value, tpbrt_token_t* const token) {
			if (value == TPBRT_NULL || value->chars == TPBRT_NULL || token == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		token->value = *value;
			if (tpbrt_directive_from_string(value, &token->directive) == TPBRT_ERROR_NONE) {
				token->type = TPBRT_TOKEN_TYPE_DIRECTIVE;
			}
			else if (value->size >= 2 && value->chars[0] == '\"' && value->chars[value->size - 1] == '\"') {
				token->type = TPBRT_TOKEN_TYPE_QUOTED_STRING;
			}
			else { token->type = TPBRT_TOKEN_TYPE_SINGLE; }
		return TPBRT_ERROR_NONE;
	}

	tpbrt_size_t tpbrt_token_size(const tpbrt_token_t* const token) { return token != TPBRT_NULL ? token->value.size : 0u; }

	tpbrt_bool_t tpbrt_token_is_quoted_string(const tpbrt_token_t* const token) {
		return token != TPBRT_NULL ? token->type == TPBRT_TOKEN_TYPE_QUOTED_STRING : TPBRT_FALSE;
	}

	tpbrt_bool_t tpbrt_token_is_directive(const tpbrt_token_t* const token) {
		return token != TPBRT_NULL ? token->type == TPBRT_TOKEN_TYPE_DIRECTIVE : TPBRT_FALSE;
	}

	tpbrt_bool_t tpbrt_token_is_open_brace(const tpbrt_token_t* const token) {
			if (token == TPBRT_NULL || token->type != TPBRT_TOKEN_TYPE_SINGLE) { return TPBRT_FALSE; }
		return token->value.size == 1u && token->value.chars[0] == '[';
	}

	tpbrt_bool_t tpbrt_token_is_close_brace(const tpbrt_token_t* const token) {
			if (token == TPBRT_NULL || token->type != TPBRT_TOKEN_TYPE_SINGLE) { return TPBRT_FALSE; }
		return token->value.size == 1u && token->value.chars[0] == ']';
	}

	tpbrt_bool_t tpbrt_token_is_valid(const tpbrt_token_t* const token) {
			if (token == TPBRT_NULL || token->value.size == 0u) { return TPBRT_FALSE; }

			if (token->value.size >= 1u) {
				const tpbrt_bool_t starts_with_quote = token->value.chars[0] == '\"';
				const tpbrt_bool_t ends_with_quote	 = token->value.chars[token->value.size - 1] == '\"';

					if (starts_with_quote || ends_with_quote) {
							if (starts_with_quote != ends_with_quote) { return TPBRT_FALSE; }

							if (token->value.size < 2u) { return TPBRT_FALSE; }
					}

					if (!starts_with_quote) {
							for (tpbrt_size_t i = 0u; i < token->value.size; ++i) {
									if (token->value.chars[i] == ' ') { return TPBRT_FALSE; }
							}
					}
			}

		return TPBRT_TRUE;
	}

#ifdef __cplusplus
}
#endif