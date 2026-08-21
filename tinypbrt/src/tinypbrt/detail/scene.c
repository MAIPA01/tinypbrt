#include <tinypbrt/pch.h>

#include <tinypbrt/detail/scene_internal.h>

#include <tinypbrt/detail/accelerator_internal.h>
#include <tinypbrt/detail/camera_internal.h>
#include <tinypbrt/detail/color_internal.h>
#include <tinypbrt/detail/coord_internal.h>
#include <tinypbrt/detail/integrator_internal.h>
#include <tinypbrt/detail/light_internal.h>
#include <tinypbrt/detail/material_internal.h>
#include <tinypbrt/detail/media_internal.h>
#include <tinypbrt/detail/sampler_internal.h>
#include <tinypbrt/detail/shape_internal.h>
#include <tinypbrt/detail/texture_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

	static tpbrt_bool_t str_equal(const tpbrt_string_t* s, const tpbrt_string_t literal) {
			if (s->size != literal.size) { return TPBRT_FALSE; }
		return strncmp(s->chars, literal.chars, literal.size) == 0 ? TPBRT_TRUE : TPBRT_FALSE;
	}

	tpbrt_error_t tpbrt_create_options(tpbrt_options_t** options) {
			if (options == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		*options = malloc(sizeof(tpbrt_options_t));
			if (*options == NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }

		(*options)->disable_pixel_jitter	   = TPBRT_FALSE;
		(*options)->disable_texture_filtering  = TPBRT_FALSE;
		(*options)->disable_wave_length_jitter = TPBRT_FALSE;
		(*options)->displacement_edge_scale	   = 1;
		(*options)->mse_reference_image.chars  = TPBRT_NULL;
		(*options)->mse_reference_image.size   = 0;
		(*options)->mse_reference_out.chars	   = TPBRT_NULL;
		(*options)->mse_reference_out.size	   = 0;
		(*options)->render_coord_sys		   = TPBRT_COORDINATE_SYSTEM_CAMERA_WORLD;
		(*options)->seed					   = 0;
		(*options)->force_diffuse			   = TPBRT_FALSE;
		(*options)->pixel_stats				   = TPBRT_FALSE;
		(*options)->wavefront				   = TPBRT_FALSE;
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_options_apply(tpbrt_options_t* options, const tpbrt_param_t* param) {
			if (options == TPBRT_NULL || param == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (str_equal(&param->name, TPBRT_STRING("disablepixeljitter"))) {
				return tpbrt_param_as_bool(param, &options->disable_pixel_jitter);
			}
			if (str_equal(&param->name, TPBRT_STRING("disabletexturefiltering"))) {
				return tpbrt_param_as_bool(param, &options->disable_texture_filtering);
			}
			if (str_equal(&param->name, TPBRT_STRING("disablewavelengthjitter"))) {
				return tpbrt_param_as_bool(param, &options->disable_wave_length_jitter);
			}
			if (str_equal(&param->name, TPBRT_STRING("displacementedgescale"))) {
				return tpbrt_param_as_float(param, &options->displacement_edge_scale);
			}
			if (str_equal(&param->name, TPBRT_STRING("msereferenceimage"))) {
				return tpbrt_param_as_string(param, &options->mse_reference_image);
			}
			if (str_equal(&param->name, TPBRT_STRING("msereferenceout"))) {
				return tpbrt_param_as_string(param, &options->mse_reference_out);
			}
			if (str_equal(&param->name, TPBRT_STRING("rendercoordsys"))) {
				tpbrt_string_t coord_str;
				const tpbrt_error_t err = tpbrt_param_as_string(param, &coord_str);
					if (err != TPBRT_ERROR_NONE) { return err; }

				return tpbrt_coordinate_system_from_string(&coord_str, &options->render_coord_sys);
			}
			if (str_equal(&param->name, TPBRT_STRING("seed"))) { return tpbrt_param_as_int(param, &options->seed); }
			if (str_equal(&param->name, TPBRT_STRING("forcediffuse"))) {
				return tpbrt_param_as_bool(param, &options->force_diffuse);
			}
			if (str_equal(&param->name, TPBRT_STRING("pixelstats"))) { return tpbrt_param_as_bool(param, &options->pixel_stats); }
			if (str_equal(&param->name, TPBRT_STRING("wavefront"))) { return tpbrt_param_as_bool(param, &options->wavefront); }

		return TPBRT_ERROR_NONE;
	}

	void tpbrt_free_options(tpbrt_options_t** options) {
			if (options == TPBRT_NULL || *options == TPBRT_NULL) { return; }

			if ((*options)->mse_reference_image.chars != TPBRT_NULL) { free((*options)->mse_reference_image.chars); }

			if ((*options)->mse_reference_out.chars != TPBRT_NULL) { free((*options)->mse_reference_out.chars); }

		free(*options);
		*options = TPBRT_NULL;
	}

#define DYN_ARRAY_PUSH(arr, count, capacity, item, type)                           \
		do {                                                                       \
				if ((count) >= (capacity)) {                                       \
					(capacity) = (capacity) == 0 ? 8 : (capacity) * 2;             \
					(arr)	   = (type*)realloc((arr), (capacity) * sizeof(type)); \
				}                                                                  \
			(arr)[(count)++] = (item);                                             \
	} while (0)

	static void mat4_identity(tpbrt_mat4_t* const m) {
		tpbrt_float_t* out = (tpbrt_float_t*)m;
			for (int i = 0; i < 16; i++) { out[i] = (i % 5 == 0) ? 1.0f : 0.0f; }
	}

	static void mat4_multiply(tpbrt_mat4_t* out, const tpbrt_mat4_t* a, const tpbrt_mat4_t* b) {
		tpbrt_float_t* o		 = (tpbrt_float_t*)out;
		const tpbrt_float_t* a_f = (const tpbrt_float_t*)a;
		const tpbrt_float_t* b_f = (const tpbrt_float_t*)b;
			for (int c = 0; c < 4; c++) {
					for (int r = 0; r < 4; r++) {
						o[c * 4 + r] = a_f[0 * 4 + r] * b_f[c * 4 + 0] + a_f[1 * 4 + r] * b_f[c * 4 + 1] +
									   a_f[2 * 4 + r] * b_f[c * 4 + 2] + a_f[3 * 4 + r] * b_f[c * 4 + 3];
					}
			}
	}

	static void mat4_from_array(tpbrt_mat4_t* m, const tpbrt_float_t arr[16]) { memcpy(m, arr, 16 * sizeof(tpbrt_float_t)); }

	static void mat4_inverse(float out_m[16], const float m[16]) {
		float inv[16];

		inv[0] = m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] + m[9] * m[7] * m[14] + m[13] * m[6] * m[11] -
				 m[13] * m[7] * m[10];
		inv[4] = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] + m[8] * m[6] * m[15] - m[8] * m[7] * m[14] - m[12] * m[6] * m[11] +
				 m[12] * m[7] * m[10];
		inv[8] = m[4] * m[9] * m[15] - m[4] * m[11] * m[13] - m[8] * m[5] * m[15] + m[8] * m[7] * m[13] + m[12] * m[5] * m[11] -
				 m[12] * m[7] * m[9];
		inv[12] = -m[4] * m[9] * m[14] + m[4] * m[10] * m[13] + m[8] * m[5] * m[14] - m[8] * m[6] * m[13] - m[12] * m[5] * m[10] +
				  m[12] * m[6] * m[9];

		inv[1] = -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] + m[9] * m[2] * m[15] - m[9] * m[3] * m[14] - m[13] * m[2] * m[11] +
				 m[13] * m[3] * m[10];
		inv[5] = m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15] + m[8] * m[3] * m[14] + m[12] * m[2] * m[11] -
				 m[12] * m[3] * m[10];
		inv[9] = -m[0] * m[9] * m[15] + m[0] * m[11] * m[13] + m[8] * m[1] * m[15] - m[8] * m[3] * m[13] - m[12] * m[1] * m[11] +
				 m[12] * m[3] * m[9];
		inv[13] = m[0] * m[9] * m[14] - m[0] * m[10] * m[13] - m[8] * m[1] * m[14] + m[8] * m[2] * m[13] + m[12] * m[1] * m[10] -
				  m[12] * m[2] * m[9];

		inv[2]	= m[1] * m[6] * m[15] - m[1] * m[7] * m[14] - m[5] * m[2] * m[15] + m[5] * m[3] * m[14] + m[13] * m[2] * m[7] -
				  m[13] * m[3] * m[6];
		inv[6]	= -m[0] * m[6] * m[15] + m[0] * m[7] * m[14] + m[4] * m[2] * m[15] - m[4] * m[3] * m[14] - m[12] * m[2] * m[7] +
				  m[12] * m[3] * m[6];
		inv[10] = m[0] * m[5] * m[15] - m[0] * m[7] * m[13] - m[4] * m[1] * m[15] + m[4] * m[3] * m[13] + m[12] * m[1] * m[7] -
				  m[12] * m[3] * m[5];
		inv[14] = -m[0] * m[5] * m[14] + m[0] * m[6] * m[13] + m[4] * m[1] * m[14] - m[4] * m[2] * m[13] - m[12] * m[1] * m[6] +
				  m[12] * m[2] * m[5];

		inv[3]	= -m[1] * m[6] * m[11] + m[1] * m[7] * m[10] + m[5] * m[2] * m[11] - m[5] * m[3] * m[10] - m[9] * m[2] * m[7] +
				  m[9] * m[3] * m[6];
		inv[7]	= m[0] * m[6] * m[11] - m[0] * m[7] * m[10] - m[4] * m[2] * m[11] + m[4] * m[3] * m[10] + m[8] * m[2] * m[7] -
				  m[8] * m[3] * m[6];
		inv[11] = -m[0] * m[5] * m[11] + m[0] * m[7] * m[9] + m[4] * m[1] * m[11] - m[4] * m[3] * m[9] - m[8] * m[1] * m[7] +
				  m[8] * m[3] * m[5];
		inv[15] = m[0] * m[5] * m[10] - m[0] * m[6] * m[9] - m[4] * m[1] * m[10] + m[4] * m[2] * m[9] + m[8] * m[1] * m[6] -
				  m[8] * m[2] * m[5];

		const float det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

			if (fabsf(det) < 1e-6f) {
					for (int i = 0; i < 16; i++) { out_m[i] = (i % 5 == 0) ? 1.0f : 0.0f; }
				return;
			}

		const float inv_det = 1.0f / det;
			for (int i = 0; i < 16; i++) { out_m[i] = inv[i] * inv_det; }
	}

	tpbrt_error_t tpbrt_state_init_default(tpbrt_state_t* const state) {
		memset(state, 0, sizeof(tpbrt_state_t));
		state->reverse_orientation = TPBRT_FALSE;
		mat4_identity(&state->ctm);
		state->current_color_space = TPBRT_COLOR_SPACE_SRGB;
		state->material_handle	   = ~(tpbrt_material_handle_t)0;
		state->active_object	   = TPBRT_NULL;

		tpbrt_create_empty_params_list(&state->shape_params);
		tpbrt_create_empty_params_list(&state->light_params);
		tpbrt_create_empty_params_list(&state->material_params);
		tpbrt_create_empty_params_list(&state->medium_params);
		tpbrt_create_empty_params_list(&state->texture_params);
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_state_clone(tpbrt_state_t* const dest, const tpbrt_state_t* const src) {
		*dest = *src;
		tpbrt_create_empty_params_list(&dest->shape_params);
		tpbrt_params_list_extend(dest->shape_params, src->shape_params);
		tpbrt_create_empty_params_list(&dest->light_params);
		tpbrt_params_list_extend(dest->light_params, src->light_params);
		tpbrt_create_empty_params_list(&dest->material_params);
		tpbrt_params_list_extend(dest->material_params, src->material_params);
		tpbrt_create_empty_params_list(&dest->medium_params);
		tpbrt_params_list_extend(dest->medium_params, src->medium_params);
		tpbrt_create_empty_params_list(&dest->texture_params);
		tpbrt_params_list_extend(dest->texture_params, src->texture_params);
		return TPBRT_ERROR_NONE;
	}

	void tpbrt_state_free(tpbrt_state_t* const state) {
		tpbrt_free_params_list(&state->shape_params);
		tpbrt_free_params_list(&state->light_params);
		tpbrt_free_params_list(&state->material_params);
		tpbrt_free_params_list(&state->medium_params);
		tpbrt_free_params_list(&state->texture_params);
	}

	tpbrt_error_t tpbrt_coord_sys_map_init(tpbrt_coord_sys_map_t* const map) {
		map->entries  = NULL;
		map->count	  = 0;
		map->capacity = 0;
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_coord_sys_map_insert(tpbrt_coord_sys_map_t* const map, const tpbrt_string_t* const name,
	  const tpbrt_mat4_t* const transform) {
		tpbrt_coord_sys_entry_t entry;
		entry.name = *name;
		memcpy(entry.transform, transform, 16 * sizeof(tpbrt_float_t));
		DYN_ARRAY_PUSH(map->entries, map->count, map->capacity, entry, tpbrt_coord_sys_entry_t);
		return TPBRT_ERROR_NONE;
	}

	tpbrt_bool_t tpbrt_coord_sys_map_get(const tpbrt_coord_sys_map_t* const map, const tpbrt_string_t* const name,
	  tpbrt_mat4_t* const out_transform) {
			for (tpbrt_size_t i = 0; i < map->count; i++) {
					if (map->entries[i].name.size == name->size &&
						strncmp(map->entries[i].name.chars, name->chars, name->size) == 0) {
						memcpy(out_transform, map->entries[i].transform, 16 * sizeof(float));
						return TPBRT_TRUE;
					}
			}
		return TPBRT_FALSE;
	}

	void tpbrt_coord_sys_map_free(const tpbrt_coord_sys_map_t* const map) {
			if (map->entries) { free(map->entries); }
	}

	tpbrt_error_t tpbrt_scene_load_from_file(const char* filepath, tpbrt_scene_t* out_scene) {
			if (filepath == TPBRT_NULL || out_scene == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		FILE* f;
		fopen_s(&f, filepath, "rb");
			if (f == TPBRT_NULL) { return TPBRT_ERROR_FILE_NOT_FOUND; }

		fseek(f, 0, SEEK_END);
		const long size = ftell(f);
		fseek(f, 0, SEEK_SET);

		char* file_data = (char*)malloc(size + 1);
			if (file_data == TPBRT_NULL) {
				fclose(f);
				return TPBRT_ERROR_OUT_OF_MEMORY;
			}

		fread(file_data, 1, size, f);
		file_data[size] = '\0';
		fclose(f);

		char working_directory[1024];
		working_directory[0]	= '\0';

		const char* last_slash	= strrchr(filepath, '/');
		const char* last_bslash = strrchr(filepath, '\\');
		const char* sep			= last_slash > last_bslash ? last_slash : last_bslash;

			if (sep != TPBRT_NULL) {
				const tpbrt_size_t dir_len = sep - filepath;
					if (dir_len < 1024) {
						strncpy_s(working_directory, 1024 * sizeof(char), filepath, dir_len);
						working_directory[dir_len] = '\0';
					}
			}

		const tpbrt_error_t err = tpbrt_scene_load_from_memory(file_data, working_directory, out_scene);

			if (err == TPBRT_ERROR_NONE) {
				const tpbrt_size_t cap	   = out_scene->include_buffers_count == 0 ? 8 : out_scene->include_buffers_count * 2;
				out_scene->include_buffers = (tpbrt_string_t*)realloc(out_scene->include_buffers, cap * sizeof(tpbrt_string_t));
				out_scene->include_buffers[out_scene->include_buffers_count++].chars = file_data;
			}
			else { free(file_data); }

		return err;
	}

	tpbrt_error_t tpbrt_scene_load_from_memory(const char* data, const char* working_directory, tpbrt_scene_t* out_scene) {
		memset(out_scene, 0, sizeof(tpbrt_scene_t));

		tpbrt_create_options(&out_scene->options);

		tpbrt_create_empty_textures_list(&out_scene->textures);
		tpbrt_create_empty_materials_list(&out_scene->materials);
		tpbrt_create_empty_lights_list(&out_scene->lights);
		tpbrt_create_empty_medias_list(&out_scene->medias);
		tpbrt_create_empty_objects_list(&out_scene->objects);

		tpbrt_size_t cap_world_shapes  = 0;
		out_scene->world_shapes.values = NULL;
		out_scene->world_shapes.count  = 0;
		tpbrt_size_t cap_instances	   = 0;
		out_scene->instances.instances = NULL;
		out_scene->instances.count	   = 0;

		tpbrt_parser_node_t* parsers   = NULL;
		tpbrt_size_t parsers_count = 0, parsers_cap = 0;

		tpbrt_parser_node_t root_node;
		root_node.file_data = NULL;
		tpbrt_parser_init(&root_node.parser, data, strlen(data));
		DYN_ARRAY_PUSH(parsers, parsers_count, parsers_cap, root_node, tpbrt_parser_node_t);

		tpbrt_state_t current_state;
		tpbrt_state_init_default(&current_state);

		tpbrt_state_t* states_stack = NULL;
		tpbrt_size_t states_count = 0, states_cap = 0;
		tpbrt_bool_t is_world_block = TPBRT_FALSE;

		tpbrt_coord_sys_map_t named_coord_systems;
		tpbrt_coord_sys_map_init(&named_coord_systems);

		tpbrt_error_t last_error = TPBRT_ERROR_NONE;

			while (parsers_count > 0) {
				tpbrt_parser_node_t* current_node = &parsers[parsers_count - 1];
				tpbrt_element_t element;

				tpbrt_error_t err = tpbrt_parser_parse_next(&current_node->parser, &element);

					if (err == TPBRT_ERROR_END_OF_FILE) {
						parsers_count--;
						continue;
					}
					if (err != TPBRT_ERROR_NONE) {
						last_error = err;
						break;
					}

					switch (element.type) {
						case TPBRT_DIRECTIVE_ATTRIBUTE_BEGIN: {
							tpbrt_state_t cloned;
							tpbrt_state_clone(&cloned, &current_state);
							DYN_ARRAY_PUSH(states_stack, states_count, states_cap, cloned, tpbrt_state_t);
							break;
						}
						case TPBRT_DIRECTIVE_ATTRIBUTE_END: {
								if (states_count == 0) {
									last_error = TPBRT_ERROR_UNEXPECTED_TOKEN;
									break;
								}
							tpbrt_state_free(&current_state);
							current_state = states_stack[--states_count];
							break;
						}
						case TPBRT_DIRECTIVE_ATTRIBUTE: {
								if (strncmp(element.as.attribute.target.chars, "shape", 5) == 0) {
									tpbrt_params_list_extend(current_state.shape_params, element.as.attribute.params);
								}
								else if (strncmp(element.as.attribute.target.chars, "light", 5) == 0) {
									tpbrt_params_list_extend(current_state.light_params, element.as.attribute.params);
								}
								else if (strncmp(element.as.attribute.target.chars, "material", 8) == 0) {
									tpbrt_params_list_extend(current_state.material_params, element.as.attribute.params);
								}
								else if (strncmp(element.as.attribute.target.chars, "texture", 7) == 0) {
									tpbrt_params_list_extend(current_state.texture_params, element.as.attribute.params);
								}
								else if (strncmp(element.as.attribute.target.chars, "medium", 6) == 0) {
									tpbrt_params_list_extend(current_state.medium_params, element.as.attribute.params);
								}
							break;
						}
					case TPBRT_DIRECTIVE_OPTION: tpbrt_options_apply(out_scene->options, &element.as.option.param); break;

					case TPBRT_DIRECTIVE_COLOR_SPACE:
						tpbrt_color_space_from_string(&element.as.single_string.type_name, &current_state.current_color_space);
						break;

					case TPBRT_DIRECTIVE_REVERSE_ORIENTATION:
						current_state.reverse_orientation = !current_state.reverse_orientation;
						break;

					case TPBRT_DIRECTIVE_IDENTITY:			   mat4_identity(&current_state.ctm); break;

					case TPBRT_DIRECTIVE_TRANSFORM:			   mat4_from_array(&current_state.ctm, element.as.transform.m); break;

						case TPBRT_DIRECTIVE_CONCAT_TRANSFORM: {
							tpbrt_mat4_t right;
							mat4_from_array(&right, element.as.transform.m);
							tpbrt_mat4_t res;
							mat4_multiply(&res, &current_state.ctm, &right);
							current_state.ctm = res;
							break;
						}

					case TPBRT_DIRECTIVE_COORDINATE_SYSTEM:
						tpbrt_coord_sys_map_insert(&named_coord_systems, &element.as.named_entity.name, &current_state.ctm);
						break;

					case TPBRT_DIRECTIVE_COORD_SYS_TRANSFORM:
						tpbrt_coord_sys_map_get(&named_coord_systems, &element.as.named_entity.name, &current_state.ctm);
						break;

						case TPBRT_DIRECTIVE_CAMERA: {
							tpbrt_mat4_t camera_to_world;
							mat4_inverse((float*)&camera_to_world, (const float*)&current_state.ctm);

							tpbrt_string_t cam_str = TPBRT_STRING("camera");
							tpbrt_coord_sys_map_insert(&named_coord_systems, &cam_str, &camera_to_world);

							tpbrt_create_camera(&element.as.generic_with_params.type_name, element.as.generic_with_params.params,
							  out_scene->film, &camera_to_world, &out_scene->camera);
							break;
						}

					case TPBRT_DIRECTIVE_FILM:
						tpbrt_create_film(&element.as.generic_with_params.type_name, element.as.generic_with_params.params,
						  &out_scene->film);
						break;
					case TPBRT_DIRECTIVE_INTEGRATOR:
						tpbrt_create_integrator(&element.as.generic_with_params.type_name, element.as.generic_with_params.params,
						  &out_scene->integrator);
						break;
					case TPBRT_DIRECTIVE_ACCELERATOR:
						tpbrt_create_accelerator(&element.as.generic_with_params.type_name, element.as.generic_with_params.params,
						  &out_scene->accelerator);
						break;
					case TPBRT_DIRECTIVE_SAMPLER:
						tpbrt_create_sampler(&element.as.generic_with_params.type_name, element.as.generic_with_params.params,
						  &out_scene->sampler);
						break;

						case TPBRT_DIRECTIVE_TEXTURE: {
							tpbrt_params_list_extend(element.as.texture.params, current_state.texture_params);
							tpbrt_texture_t* tex;
							tpbrt_create_texture(&element.as.texture.name, &element.as.texture.type_name,
							  &element.as.texture.class_name, element.as.texture.params, out_scene->textures, &current_state.ctm,
							  &tex);

							tpbrt_textures_list_add_texture(out_scene->textures, tex);
							free(tex); // Uwalniamy outer pointer, bo list_add zrobiło kopię danych do tablicy!
							break;
						}
						case TPBRT_DIRECTIVE_MATERIAL: {
							tpbrt_params_list_extend(element.as.generic_with_params.params, current_state.material_params);
							tpbrt_material_t* mat;
							tpbrt_create_material(&element.as.generic_with_params.type_name,
							  element.as.generic_with_params.params, out_scene->textures, out_scene->materials, &mat);

							tpbrt_materials_list_add_material(out_scene->materials, mat, &current_state.material_handle);
							free(mat);
							break;
						}
						case TPBRT_DIRECTIVE_MAKE_NAMED_MATERIAL: {
							tpbrt_params_list_extend(element.as.named_with_params.params, current_state.material_params);
							tpbrt_material_t* mat;
							tpbrt_create_named_material(&element.as.named_with_params.name, element.as.named_with_params.params,
							  out_scene->textures, out_scene->materials, &mat);

							tpbrt_materials_list_add_material(out_scene->materials, mat, &current_state.material_handle);
							free(mat);
							break;
						}
						case TPBRT_DIRECTIVE_NAMED_MATERIAL: {
							tpbrt_materials_list_get_material_handle(out_scene->materials, &element.as.named_entity.name,
							  &current_state.material_handle);
							break;
						}
						case TPBRT_DIRECTIVE_LIGHT_SOURCE: {
							tpbrt_params_list_extend(element.as.generic_with_params.params, current_state.light_params);
							tpbrt_light_source_t* light;
							tpbrt_create_light_source(&element.as.generic_with_params.type_name,
							  element.as.generic_with_params.params, current_state.current_color_space, &light);

							tpbrt_lights_list_add_light_source(out_scene->lights, light);
							free(light);
							break;
						}
						case TPBRT_DIRECTIVE_AREA_LIGHT_SOURCE: {
							tpbrt_params_list_extend(element.as.generic_with_params.params, current_state.light_params);
							tpbrt_area_light_t* area_light;
							tpbrt_create_area_light(&element.as.generic_with_params.type_name,
							  element.as.generic_with_params.params, current_state.current_color_space, &area_light);

							tpbrt_lights_list_add_area_light(out_scene->lights, area_light);
							free(area_light);
							break;
						}
						case TPBRT_DIRECTIVE_MAKE_NAMED_MEDIUM: {
							tpbrt_params_list_extend(element.as.generic_with_params.params, current_state.medium_params);
							tpbrt_media_t* media;
							tpbrt_create_media(&element.as.generic_with_params.type_name,
							  &element.as.generic_with_params.type_name, element.as.generic_with_params.params, &media);

							tpbrt_medias_list_add_media(out_scene->medias, media);
							free(media);
							break;
						}
						case TPBRT_DIRECTIVE_MEDIUM_INTERFACE: {
							current_state.current_inside_medium	 = element.as.medium_interface.interior;
							current_state.current_outside_medium = element.as.medium_interface.exterior;
							break;
						}
					case TPBRT_DIRECTIVE_WORLD_BEGIN:
						is_world_block = TPBRT_TRUE;
						mat4_identity(&current_state.ctm);
						break;

						case TPBRT_DIRECTIVE_SHAPE: {
							tpbrt_params_list_extend(element.as.generic_with_params.params, current_state.shape_params);

							tpbrt_shape_t* shape;
							tpbrt_create_shape(&element.as.generic_with_params.type_name, element.as.generic_with_params.params,
							  out_scene->textures, &current_state.ctm, &current_state.material_handle, &shape);

								if (current_state.active_object != TPBRT_NULL) {
									tpbrt_object_add_shape(current_state.active_object, shape);
								}
								else {
									DYN_ARRAY_PUSH(out_scene->world_shapes.values, out_scene->world_shapes.count,
									  cap_world_shapes, *shape, tpbrt_shape_t);
								}

							free(shape);
							break;
						}
						case TPBRT_DIRECTIVE_OBJECT_BEGIN: {
								if (current_state.active_object != TPBRT_NULL) {
									last_error = TPBRT_ERROR_INVALID_TOKEN;
									break;
								}

							tpbrt_state_t cloned;
							tpbrt_state_clone(&cloned, &current_state);
							DYN_ARRAY_PUSH(states_stack, states_count, states_cap, cloned, tpbrt_state_t);

							tpbrt_object_t* new_obj;
							tpbrt_create_object(&element.as.named_entity.name, &new_obj);
							current_state.active_object = new_obj;
							break;
						}
						case TPBRT_DIRECTIVE_OBJECT_END: {
								if (current_state.active_object == TPBRT_NULL) {
									last_error = TPBRT_ERROR_UNEXPECTED_TOKEN;
									break;
								}

							tpbrt_objects_list_add_object(out_scene->objects, current_state.active_object);
							free(current_state.active_object);

							current_state.active_object = TPBRT_NULL;
							tpbrt_state_free(&current_state);
							current_state = states_stack[--states_count];
							break;
						}
						case TPBRT_DIRECTIVE_OBJECT_INSTANCE: {
							tpbrt_object_instance_t* inst;
							tpbrt_create_object_instance(&element.as.named_entity.name, out_scene->objects, &current_state.ctm,
							  &inst);

							DYN_ARRAY_PUSH(out_scene->instances.instances, out_scene->instances.count, cap_instances, *inst,
							  tpbrt_object_instance_t);
							free(inst);
							break;
						}
					default: break;
					}

				tpbrt_free_element(&element);
					if (last_error != TPBRT_ERROR_NONE) { break; }
			}

		tpbrt_state_free(&current_state);
			for (tpbrt_size_t i = 0; i < states_count; i++) { tpbrt_state_free(&states_stack[i]); }
			if (states_stack) { free(states_stack); }
			if (parsers) { free(parsers); }
		tpbrt_coord_sys_map_free(&named_coord_systems);

		return last_error;
	}

	void tpbrt_free_scene(tpbrt_scene_t* scene) {
			if (scene == TPBRT_NULL) { return; }

		tpbrt_free_options(&scene->options);

			if (scene->camera) { tpbrt_free_camera(&scene->camera); }
			if (scene->film) { tpbrt_free_film(&scene->film); }
			if (scene->integrator) { tpbrt_free_integrator(&scene->integrator); }
			if (scene->accelerator) { tpbrt_free_accelerator(&scene->accelerator); }
			if (scene->sampler) { tpbrt_free_sampler(&scene->sampler); }

		tpbrt_free_textures_list(&scene->textures);
		tpbrt_free_materials_list(&scene->materials);
		tpbrt_free_lights_list(&scene->lights);
		tpbrt_free_medias_list(&scene->medias);
		tpbrt_free_objects_list(&scene->objects);

			if (scene->world_shapes.values != TPBRT_NULL) { free(scene->world_shapes.values); }

			if (scene->instances.instances != TPBRT_NULL) { free(scene->instances.instances); }

			if (scene->include_buffers != TPBRT_NULL) {
					for (tpbrt_size_t i = 0; i < scene->include_buffers_count; ++i) { free(scene->include_buffers[i].chars); }
				free(scene->include_buffers);
			}
	}

#ifdef __cplusplus
}
#endif