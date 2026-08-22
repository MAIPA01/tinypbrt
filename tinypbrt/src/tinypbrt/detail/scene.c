#include <tinypbrt/pch.h>

#include <tinypbrt/detail/scene_internal.h>

#include <tinypbrt/detail/accelerator_internal.h>
#include <tinypbrt/detail/camera_internal.h>
#include <tinypbrt/detail/color_internal.h>
#include <tinypbrt/detail/coord_internal.h>
#include <tinypbrt/detail/integrator_internal.h>
#include <tinypbrt/detail/light_internal.h>
#include <tinypbrt/detail/material_internal.h>
#include <tinypbrt/detail/math_internal.h>
#include <tinypbrt/detail/media_internal.h>
#include <tinypbrt/detail/sampler_internal.h>
#include <tinypbrt/detail/shape_internal.h>
#include <tinypbrt/detail/texture_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

	tpbrt_error_t tpbrt_init_options(tpbrt_options_t* options) {
			if (options == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		options->disable_pixel_jitter		= TPBRT_FALSE;
		options->disable_texture_filtering	= TPBRT_FALSE;
		options->disable_wave_length_jitter = TPBRT_FALSE;
		options->displacement_edge_scale	= 1;
		options->mse_reference_image.data	= TPBRT_NULL;
		options->mse_reference_image.size	= 0;
		options->mse_reference_out.data		= TPBRT_NULL;
		options->mse_reference_out.size		= 0;
		options->render_coord_sys			= TPBRT_COORDINATE_SYSTEM_CAMERA_WORLD;
		options->seed						= 0;
		options->force_diffuse				= TPBRT_FALSE;
		options->pixel_stats				= TPBRT_FALSE;
		options->wavefront					= TPBRT_FALSE;
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_options_apply(tpbrt_options_t* options, const tpbrt_param_t* param) {
			if (options == TPBRT_NULL || param == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (tpbrt_string_equals_literal(&param->name, TPBRT_STRING("disablepixeljitter"))) {
				return tpbrt_param_as_bool(param, &options->disable_pixel_jitter);
			}
			if (tpbrt_string_equals_literal(&param->name, TPBRT_STRING("disabletexturefiltering"))) {
				return tpbrt_param_as_bool(param, &options->disable_texture_filtering);
			}
			if (tpbrt_string_equals_literal(&param->name, TPBRT_STRING("disablewavelengthjitter"))) {
				return tpbrt_param_as_bool(param, &options->disable_wave_length_jitter);
			}
			if (tpbrt_string_equals_literal(&param->name, TPBRT_STRING("displacementedgescale"))) {
				return tpbrt_param_as_float(param, &options->displacement_edge_scale);
			}
			if (tpbrt_string_equals_literal(&param->name, TPBRT_STRING("msereferenceimage"))) {
				return tpbrt_param_as_string(param, &options->mse_reference_image);
			}
			if (tpbrt_string_equals_literal(&param->name, TPBRT_STRING("msereferenceout"))) {
				return tpbrt_param_as_string(param, &options->mse_reference_out);
			}
			if (tpbrt_string_equals_literal(&param->name, TPBRT_STRING("rendercoordsys"))) {
				tpbrt_string_t coord_str;
				tpbrt_error_t err = tpbrt_param_as_string(param, &coord_str);
					if (err != TPBRT_ERROR_NONE) { return err; }

				err = tpbrt_coordinate_system_from_string(&coord_str, &options->render_coord_sys);
				tpbrt_free_string(&coord_str);
				return err;
			}
			if (tpbrt_string_equals_literal(&param->name, TPBRT_STRING("seed"))) {
				return tpbrt_param_as_int(param, &options->seed);
			}
			if (tpbrt_string_equals_literal(&param->name, TPBRT_STRING("forcediffuse"))) {
				return tpbrt_param_as_bool(param, &options->force_diffuse);
			}
			if (tpbrt_string_equals_literal(&param->name, TPBRT_STRING("pixelstats"))) {
				return tpbrt_param_as_bool(param, &options->pixel_stats);
			}
			if (tpbrt_string_equals_literal(&param->name, TPBRT_STRING("wavefront"))) {
				return tpbrt_param_as_bool(param, &options->wavefront);
			}

		return TPBRT_ERROR_NONE;
	}

	void tpbrt_free_options(tpbrt_options_t* options) {
			if (options == TPBRT_NULL) { return; }

		tpbrt_free_string(&options->mse_reference_image);
		tpbrt_free_string(&options->mse_reference_out);
	}

#define DYN_ARRAY_PUSH(arr, count, capacity, item, type)                           \
		do {                                                                       \
				if ((count) >= (capacity)) {                                       \
					(capacity) = (capacity) == 0 ? 8 : (capacity) * 2;             \
					(arr)	   = (type*)realloc((arr), (capacity) * sizeof(type)); \
				}                                                                  \
			(arr)[(count)++] = (item);                                             \
	} while (0)

	tpbrt_error_t tpbrt_init_state(tpbrt_state_t* const state) {
		memset(state, 0, sizeof(tpbrt_state_t));
		state->reverse_orientation = TPBRT_FALSE;
		tpbrt_mat4_identity(&state->ctm);
		state->current_color_space = TPBRT_COLOR_SPACE_SRGB;
		state->material_handle	   = ~(tpbrt_material_handle_t)0;
		state->active_object	   = TPBRT_NULL;

		tpbrt_error_t err		   = tpbrt_init_params_list(&state->shape_params);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_free_state(state);
				return err;
			}

		err = tpbrt_init_params_list(&state->light_params);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_free_state(state);
				return err;
			}

		err = tpbrt_init_params_list(&state->material_params);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_free_state(state);
				return err;
			}

		err = tpbrt_init_params_list(&state->medium_params);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_free_state(state);
				return err;
			}

		err = tpbrt_init_params_list(&state->texture_params);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_free_state(state);
				return err;
			}
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_state_clone(tpbrt_state_t* const dest, const tpbrt_state_t* const src) {
		*dest = *src;
		tpbrt_init_params_list(&dest->shape_params);
		tpbrt_params_list_extend(&dest->shape_params, &src->shape_params);
		tpbrt_init_params_list(&dest->light_params);
		tpbrt_params_list_extend(&dest->light_params, &src->light_params);
		tpbrt_init_params_list(&dest->material_params);
		tpbrt_params_list_extend(&dest->material_params, &src->material_params);
		tpbrt_init_params_list(&dest->medium_params);
		tpbrt_params_list_extend(&dest->medium_params, &src->medium_params);
		tpbrt_init_params_list(&dest->texture_params);
		tpbrt_params_list_extend(&dest->texture_params, &src->texture_params);
		return TPBRT_ERROR_NONE;
	}

	void tpbrt_free_state(tpbrt_state_t* const state) {
			if (state == TPBRT_NULL) { return; }

			if (state->active_object != TPBRT_NULL) {
				tpbrt_free_object(state->active_object);
				free(state->active_object);
				state->active_object = TPBRT_NULL;
			}

		tpbrt_free_params_list(&state->shape_params);
		tpbrt_free_params_list(&state->light_params);
		tpbrt_free_params_list(&state->material_params);
		tpbrt_free_params_list(&state->medium_params);
		tpbrt_free_params_list(&state->texture_params);
	}

	tpbrt_error_t tpbrt_init_coord_sys_map(tpbrt_coord_sys_map_t* const map) {
		map->entries  = TPBRT_NULL;
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
						strncmp(map->entries[i].name.data, name->data, name->size) == 0) {
						memcpy(out_transform, map->entries[i].transform, 16 * sizeof(tpbrt_float_t));
						return TPBRT_TRUE;
					}
			}
		return TPBRT_FALSE;
	}

	void tpbrt_free_coord_sys_map(const tpbrt_coord_sys_map_t* const map) {
			if (map->entries) { free(map->entries); }
	}

	tpbrt_error_t tpbrt_scene_load_from_file(const tpbrt_char_t* filepath, tpbrt_scene_t* out_scene) {
			if (filepath == TPBRT_NULL || out_scene == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		FILE* f;
		fopen_s(&f, filepath, "rb");
			if (f == TPBRT_NULL) { return TPBRT_ERROR_FILE_NOT_FOUND; }

		fseek(f, 0, SEEK_END);
		const tpbrt_size_t size = ftell(f);
		fseek(f, 0, SEEK_SET);

		tpbrt_char_t* file_data = malloc(size + 1);
			if (file_data == TPBRT_NULL) {
				fclose(f);
				return TPBRT_ERROR_OUT_OF_MEMORY;
			}

		fread(file_data, 1, size, f);
		file_data[size] = '\0';
		fclose(f);

		tpbrt_char_t working_directory[1024];
		working_directory[0]			= '\0';

		const tpbrt_char_t* last_slash	= strrchr(filepath, '/');
		const tpbrt_char_t* last_bslash = strrchr(filepath, '\\');
		const tpbrt_char_t* sep			= last_slash > last_bslash ? last_slash : last_bslash;

			if (sep != TPBRT_NULL) {
				const tpbrt_size_t dir_len = sep - filepath;
					if (dir_len < 1024) {
						strncpy_s(working_directory, 1024 * sizeof(tpbrt_char_t), filepath, dir_len);
						working_directory[dir_len] = '\0';
					}
			}

		const tpbrt_error_t err = tpbrt_scene_load_from_memory(file_data, working_directory, out_scene);

			if (err == TPBRT_ERROR_NONE) {
				const tpbrt_size_t cap = out_scene->include_buffers.count == 0 ? 8 : out_scene->include_buffers.count * 2;
				out_scene->include_buffers.data =
				  (tpbrt_string_t*)realloc(out_scene->include_buffers.data, cap * sizeof(tpbrt_string_t));
				out_scene->include_buffers.data[out_scene->include_buffers.count++].data = file_data;
			}
			else { free(file_data); }

		return err;
	}

	tpbrt_error_t tpbrt_scene_load_from_memory(const tpbrt_char_t* data, const tpbrt_char_t* working_directory,
	  tpbrt_scene_t* out_scene) {
		memset(out_scene, 0, sizeof(tpbrt_scene_t));

		tpbrt_init_options(&out_scene->options);

		tpbrt_init_textures_list(&out_scene->textures);
		tpbrt_init_materials_list(&out_scene->materials);
		tpbrt_init_lights_list(&out_scene->lights);
		tpbrt_init_medias_list(&out_scene->medias);
		tpbrt_init_objects_list(&out_scene->objects);
		tpbrt_init_instances_list(&out_scene->instances);

		out_scene->shapes.data		 = TPBRT_NULL;
		out_scene->shapes.count		 = 0;

		tpbrt_parser_node_t* parsers = TPBRT_NULL;
		tpbrt_size_t parsers_count = 0, parsers_cap = 0;

		tpbrt_parser_node_t root_node;
		root_node.file_data.data	   = TPBRT_NULL;
		const tpbrt_string_t file_view = { .data = (tpbrt_char_t*)data, .size = strlen(data) };
		tpbrt_parser_init(&root_node.parser, &file_view);
		DYN_ARRAY_PUSH(parsers, parsers_count, parsers_cap, root_node, tpbrt_parser_node_t);

		tpbrt_state_t current_state;
		tpbrt_init_state(&current_state);

		tpbrt_state_t* states_stack = TPBRT_NULL;
		tpbrt_size_t states_count = 0, states_cap = 0;
		tpbrt_bool_t is_world_block = TPBRT_FALSE;

		tpbrt_coord_sys_map_t named_coord_systems;
		tpbrt_init_coord_sys_map(&named_coord_systems);

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
							err = tpbrt_state_clone(&cloned, &current_state);
								if (err != TPBRT_ERROR_NONE) {
									last_error = err;
									break;
								}
							DYN_ARRAY_PUSH(states_stack, states_count, states_cap, cloned, tpbrt_state_t);
							break;
						}
						case TPBRT_DIRECTIVE_ATTRIBUTE_END: {
								if (states_count == 0) {
									last_error = TPBRT_ERROR_UNEXPECTED_TOKEN;
									break;
								}
							tpbrt_free_state(&current_state);
							current_state = states_stack[--states_count];
							break;
						}
						case TPBRT_DIRECTIVE_ATTRIBUTE: {
								if (tpbrt_string_equals_literal(&element.as.attribute.target, TPBRT_STRING("shape"))) {
									err = tpbrt_params_list_extend(&current_state.shape_params, &element.as.attribute.params);
								}
								else if (tpbrt_string_equals_literal(&element.as.attribute.target, TPBRT_STRING("light"))) {
									err = tpbrt_params_list_extend(&current_state.light_params, &element.as.attribute.params);
								}
								else if (tpbrt_string_equals_literal(&element.as.attribute.target, TPBRT_STRING("material"))) {
									err = tpbrt_params_list_extend(&current_state.material_params, &element.as.attribute.params);
								}
								else if (tpbrt_string_equals_literal(&element.as.attribute.target, TPBRT_STRING("texture"))) {
									err = tpbrt_params_list_extend(&current_state.texture_params, &element.as.attribute.params);
								}
								else if (tpbrt_string_equals_literal(&element.as.attribute.target, TPBRT_STRING("medium"))) {
									err = tpbrt_params_list_extend(&current_state.medium_params, &element.as.attribute.params);
								}
								if (err != TPBRT_ERROR_NONE) { last_error = err; }
							break;
						}
						case TPBRT_DIRECTIVE_OPTION: {
							err = tpbrt_options_apply(&out_scene->options, &element.as.option.param);
								if (err != TPBRT_ERROR_NONE) { last_error = err; }
							break;
						}

						case TPBRT_DIRECTIVE_COLOR_SPACE: {
							err = tpbrt_color_space_from_string(&element.as.single_string.type_name,
							  &current_state.current_color_space);
								if (err != TPBRT_ERROR_NONE) { last_error = err; }
							break;
						}

						case TPBRT_DIRECTIVE_REVERSE_ORIENTATION: {
							current_state.reverse_orientation = !current_state.reverse_orientation;
							break;
						}

						case TPBRT_DIRECTIVE_IDENTITY: {
							tpbrt_mat4_identity(&current_state.ctm);
							break;
						}

						case TPBRT_DIRECTIVE_TRANSFORM: {
							tpbrt_mat4_from_array(&current_state.ctm, element.as.transform.m);
							break;
						}

						case TPBRT_DIRECTIVE_CONCAT_TRANSFORM: {
							tpbrt_mat4_t right;
							tpbrt_mat4_from_array(&right, element.as.transform.m);
							tpbrt_mat4_t res;
							tpbrt_mat4_multiply(&res, &current_state.ctm, &right);
							current_state.ctm = res;
							break;
						}

						case TPBRT_DIRECTIVE_COORDINATE_SYSTEM: {
							err =
							  tpbrt_coord_sys_map_insert(&named_coord_systems, &element.as.named_entity.name, &current_state.ctm);
								if (err != TPBRT_ERROR_NONE) { last_error = err; }
							break;
						}

						case TPBRT_DIRECTIVE_COORD_SYS_TRANSFORM: {
							tpbrt_coord_sys_map_get(&named_coord_systems, &element.as.named_entity.name, &current_state.ctm);
							break;
						}

						case TPBRT_DIRECTIVE_CAMERA: {
							tpbrt_mat4_t camera_to_world;
							tpbrt_mat4_inverse((tpbrt_float_t*)&camera_to_world, (const tpbrt_float_t*)&current_state.ctm);

							static const tpbrt_string_t CAM_STR = TPBRT_STRING("camera");
							err = tpbrt_coord_sys_map_insert(&named_coord_systems, &CAM_STR, &camera_to_world);
								if (err != TPBRT_ERROR_NONE) {
									last_error = err;
									break;
								}

							err = tpbrt_create_camera(&element.as.generic_with_params.type_name,
							  &element.as.generic_with_params.params, &out_scene->film, &camera_to_world, &out_scene->camera);
								if (err != TPBRT_ERROR_NONE) { last_error = err; }
							break;
						}

						case TPBRT_DIRECTIVE_FILM: {
							err = tpbrt_create_film(&element.as.generic_with_params.type_name,
							  &element.as.generic_with_params.params, &out_scene->film);
								if (err != TPBRT_ERROR_NONE) { last_error = err; }
							break;
						}
						case TPBRT_DIRECTIVE_INTEGRATOR: {
							err = tpbrt_create_integrator(&element.as.generic_with_params.type_name,
							  &element.as.generic_with_params.params, &out_scene->integrator);
								if (err != TPBRT_ERROR_NONE) { last_error = err; }
							break;
						}
						case TPBRT_DIRECTIVE_ACCELERATOR: {
							err = tpbrt_create_accelerator(&element.as.generic_with_params.type_name,
							  &element.as.generic_with_params.params, &out_scene->accelerator);
								if (err != TPBRT_ERROR_NONE) { last_error = err; }
							break;
						}
						case TPBRT_DIRECTIVE_SAMPLER: {
							err = tpbrt_create_sampler(&element.as.generic_with_params.type_name,
							  &element.as.generic_with_params.params, &out_scene->sampler);
								if (err != TPBRT_ERROR_NONE) { last_error = err; }
							break;
						}

						case TPBRT_DIRECTIVE_TEXTURE: {
							err = tpbrt_params_list_extend(&element.as.texture.params, &current_state.texture_params);
								if (err != TPBRT_ERROR_NONE) {
									last_error = err;
									break;
								}

							tpbrt_texture_t tex;
							err = tpbrt_create_texture(&element.as.texture.name, &element.as.texture.type_name,
							  &element.as.texture.class_name, &element.as.texture.params, &out_scene->textures,
							  &current_state.ctm, &tex);

								if (err != TPBRT_ERROR_NONE) {
									last_error = err;
									break;
								}

							err = tpbrt_textures_list_add_texture(&out_scene->textures, &tex);
								if (err != TPBRT_ERROR_NONE) { last_error = err; }
							break;
						}
						case TPBRT_DIRECTIVE_MATERIAL: {
							err =
							  tpbrt_params_list_extend(&element.as.generic_with_params.params, &current_state.material_params);
								if (err != TPBRT_ERROR_NONE) {
									last_error = err;
									break;
								}

							tpbrt_material_t mat;
							err = tpbrt_create_material(&element.as.generic_with_params.type_name,
							  &element.as.generic_with_params.params, &out_scene->textures, &out_scene->materials, &mat);
								if (err != TPBRT_ERROR_NONE) {
									last_error = err;
									break;
								}

							err = tpbrt_materials_list_add_material(&out_scene->materials, &mat, &current_state.material_handle);
								if (err != TPBRT_ERROR_NONE) { last_error = err; }
							break;
						}
						case TPBRT_DIRECTIVE_MAKE_NAMED_MATERIAL: {
							err = tpbrt_params_list_extend(&element.as.named_with_params.params, &current_state.material_params);
								if (err != TPBRT_ERROR_NONE) {
									last_error = err;
									break;
								}

							tpbrt_material_t mat;
							err = tpbrt_create_named_material(&element.as.named_with_params.name,
							  &element.as.named_with_params.params, &out_scene->textures, &out_scene->materials, &mat);
								if (err != TPBRT_ERROR_NONE) {
									last_error = err;
									break;
								}

							err = tpbrt_materials_list_add_material(&out_scene->materials, &mat, &current_state.material_handle);
								if (err != TPBRT_ERROR_NONE) { last_error = err; }
							break;
						}
						case TPBRT_DIRECTIVE_NAMED_MATERIAL: {
							err = tpbrt_materials_list_get_material_handle(&out_scene->materials, &element.as.named_entity.name,
							  &current_state.material_handle);
								if (err != TPBRT_ERROR_NONE) { last_error = err; }
							break;
						}
						case TPBRT_DIRECTIVE_LIGHT_SOURCE: {
							err = tpbrt_params_list_extend(&element.as.generic_with_params.params, &current_state.light_params);
								if (err != TPBRT_ERROR_NONE) {
									last_error = err;
									break;
								}

							tpbrt_light_source_t light;
							err = tpbrt_create_light_source(&element.as.generic_with_params.type_name,
							  &element.as.generic_with_params.params, current_state.current_color_space, &light);
								if (err != TPBRT_ERROR_NONE) {
									last_error = err;
									break;
								}

							err = tpbrt_lights_list_add_light_source(&out_scene->lights, &light);
								if (err != TPBRT_ERROR_NONE) { last_error = err; }
							break;
						}
						case TPBRT_DIRECTIVE_AREA_LIGHT_SOURCE: {
							err = tpbrt_params_list_extend(&element.as.generic_with_params.params, &current_state.light_params);
								if (err != TPBRT_ERROR_NONE) {
									last_error = err;
									break;
								}

							tpbrt_area_light_t area_light;
							err = tpbrt_create_area_light(&element.as.generic_with_params.type_name,
							  &element.as.generic_with_params.params, current_state.current_color_space, &area_light);
								if (err != TPBRT_ERROR_NONE) {
									last_error = err;
									break;
								}

							err = tpbrt_lights_list_add_area_light(&out_scene->lights, &area_light);
								if (err != TPBRT_ERROR_NONE) { last_error = err; }
							break;
						}
						case TPBRT_DIRECTIVE_MAKE_NAMED_MEDIUM: {
							err = tpbrt_params_list_extend(&element.as.generic_with_params.params, &current_state.medium_params);
								if (err != TPBRT_ERROR_NONE) {
									last_error = err;
									break;
								}

							tpbrt_media_t media;
							err = tpbrt_create_media(&element.as.generic_with_params.type_name,
							  &element.as.generic_with_params.type_name, &element.as.generic_with_params.params, &media);
								if (err != TPBRT_ERROR_NONE) {
									last_error = err;
									break;
								}

							err = tpbrt_medias_list_add_media(&out_scene->medias, &media);
								if (err != TPBRT_ERROR_NONE) { last_error = err; }
							break;
						}
						case TPBRT_DIRECTIVE_MEDIUM_INTERFACE: {
							current_state.current_inside_medium	 = element.as.medium_interface.interior;
							current_state.current_outside_medium = element.as.medium_interface.exterior;
							break;
						}
						case TPBRT_DIRECTIVE_WORLD_BEGIN: {
							is_world_block = TPBRT_TRUE;
							tpbrt_mat4_identity(&current_state.ctm);
							break;
						}

						case TPBRT_DIRECTIVE_SHAPE: {
							err = tpbrt_params_list_extend(&element.as.generic_with_params.params, &current_state.shape_params);
								if (err != TPBRT_ERROR_NONE) {
									last_error = err;
									break;
								}

							tpbrt_shape_t shape;
							err = tpbrt_create_shape(&element.as.generic_with_params.type_name,
							  &element.as.generic_with_params.params, &out_scene->textures, &current_state.ctm,
							  &current_state.material_handle, &shape);
								if (err != TPBRT_ERROR_NONE) {
									last_error = err;
									break;
								}

								if (current_state.active_object != TPBRT_NULL) {
									err = tpbrt_object_add_shape(current_state.active_object, &shape);
								}
								else { err = tpbrt_shape_array_add_shape(&out_scene->shapes, &shape); }
								if (err != TPBRT_ERROR_NONE) { last_error = err; }
							break;
						}
						case TPBRT_DIRECTIVE_OBJECT_BEGIN: {
								if (current_state.active_object != TPBRT_NULL) {
									last_error = TPBRT_ERROR_INVALID_TOKEN;
									break;
								}

							tpbrt_state_t cloned;
							err = tpbrt_state_clone(&cloned, &current_state);
								if (err != TPBRT_ERROR_NONE) {
									last_error = err;
									break;
								}

							DYN_ARRAY_PUSH(states_stack, states_count, states_cap, cloned, tpbrt_state_t);

							current_state.active_object = malloc(sizeof(tpbrt_object_t));
							err = tpbrt_create_object(&element.as.named_entity.name, current_state.active_object);
								if (err != TPBRT_ERROR_NONE) { last_error = err; }
							break;
						}
						case TPBRT_DIRECTIVE_OBJECT_END: {
								if (current_state.active_object == TPBRT_NULL) {
									last_error = TPBRT_ERROR_UNEXPECTED_TOKEN;
									break;
								}

							err = tpbrt_objects_list_add_object(&out_scene->objects, current_state.active_object);
								if (err != TPBRT_ERROR_NONE) {
									last_error = err;
									break;
								}

							free(current_state.active_object);
							current_state.active_object = TPBRT_NULL;

							tpbrt_free_state(&current_state);
							current_state = states_stack[--states_count];
							break;
						}
						case TPBRT_DIRECTIVE_OBJECT_INSTANCE: {
							tpbrt_instance_t inst;
							err = tpbrt_create_instance(&element.as.named_entity.name, &out_scene->objects, &current_state.ctm,
							  &inst);
								if (err != TPBRT_ERROR_NONE) {
									last_error = err;
									break;
								}

							err = tpbrt_instances_list_add_instance(&out_scene->instances, &inst);
								if (err != TPBRT_ERROR_NONE) { last_error = err; }
							break;
						}
						default: {
							break;
						}
					}

				tpbrt_free_element(&element);
					if (last_error != TPBRT_ERROR_NONE) { break; }
			}

		tpbrt_free_state(&current_state);
			for (tpbrt_size_t i = 0; i < states_count; i++) { tpbrt_free_state(&states_stack[i]); }
			if (states_stack) { free(states_stack); }
			if (parsers) { free(parsers); }
		tpbrt_free_coord_sys_map(&named_coord_systems);

		return last_error;
	}

	void tpbrt_free_scene(tpbrt_scene_t* scene) {
			if (scene == TPBRT_NULL) { return; }

		tpbrt_free_options(&scene->options);

		tpbrt_free_camera(&scene->camera);
		tpbrt_free_film(&scene->film);

		tpbrt_free_textures_list(&scene->textures);
		tpbrt_free_materials_list(&scene->materials);
		tpbrt_free_lights_list(&scene->lights);
		tpbrt_free_medias_list(&scene->medias);
		tpbrt_free_objects_list(&scene->objects);
		tpbrt_free_instances_list(&scene->instances);

		tpbrt_free_shape_array(&scene->shapes);

		tpbrt_free_string_array(&scene->include_buffers);
	}

#ifdef __cplusplus
}
#endif