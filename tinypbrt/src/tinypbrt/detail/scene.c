#include <tinypbrt/pch.h>

#include <tinypbrt/detail/scene_internal.h>

#include <tinypbrt/detail/accelerator_internal.h>
#include <tinypbrt/detail/camera_internal.h>
#include <tinypbrt/detail/color_internal.h>
#include <tinypbrt/detail/coord_internal.h>
#include <tinypbrt/detail/filter_internal.h>
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

		static const tpbrt_string_t DISABLE_PIXEL_JITTER = TPBRT_STRING("disablepixeljitter");
			if (tpbrt_string_equals(&param->name, &DISABLE_PIXEL_JITTER)) {
				return tpbrt_param_as_bool(param, &options->disable_pixel_jitter);
			}

		static const tpbrt_string_t DISABLE_TEXTURE_FILTERING = TPBRT_STRING("disabletexturefiltering");
			if (tpbrt_string_equals(&param->name, &DISABLE_TEXTURE_FILTERING)) {
				return tpbrt_param_as_bool(param, &options->disable_texture_filtering);
			}

		static const tpbrt_string_t DISABLE_WAVE_LENGTH_JITTER = TPBRT_STRING("disablewavelengthjitter");
			if (tpbrt_string_equals(&param->name, &DISABLE_WAVE_LENGTH_JITTER)) {
				return tpbrt_param_as_bool(param, &options->disable_wave_length_jitter);
			}

		static const tpbrt_string_t DISPLACEMENT_EDGE_SCALE = TPBRT_STRING("displacementedgescale");
			if (tpbrt_string_equals(&param->name, &DISPLACEMENT_EDGE_SCALE)) {
				return tpbrt_param_as_float(param, &options->displacement_edge_scale);
			}

		static const tpbrt_string_t MSE_REFERENCE_IMAGE = TPBRT_STRING("msereferenceimage");
			if (tpbrt_string_equals(&param->name, &MSE_REFERENCE_IMAGE)) {
				return tpbrt_param_as_string(param, &options->mse_reference_image);
			}

		static const tpbrt_string_t MSE_REFERENCE_OUT = TPBRT_STRING("msereferenceout");
			if (tpbrt_string_equals(&param->name, &MSE_REFERENCE_OUT)) {
				return tpbrt_param_as_string(param, &options->mse_reference_out);
			}

		static const tpbrt_string_t RENDER_COORD_SYS = TPBRT_STRING("rendercoordsys");
			if (tpbrt_string_equals(&param->name, &RENDER_COORD_SYS)) {
				tpbrt_string_t coord_str;
				tpbrt_error_t err = tpbrt_param_as_string(param, &coord_str);
					if (err != TPBRT_ERROR_NONE) { return err; }

				err = tpbrt_coordinate_system_from_string(&coord_str, &options->render_coord_sys);
				tpbrt_free_string(&coord_str);
				return err;
			}

		static const tpbrt_string_t SEED = TPBRT_STRING("seed");
			if (tpbrt_string_equals(&param->name, &SEED)) { return tpbrt_param_as_int(param, &options->seed); }

		static const tpbrt_string_t FORCE_DIFFUSE = TPBRT_STRING("forcediffuse");
			if (tpbrt_string_equals(&param->name, &FORCE_DIFFUSE)) { return tpbrt_param_as_bool(param, &options->force_diffuse); }

		static const tpbrt_string_t PIXEL_STATS = TPBRT_STRING("pixelstats");
			if (tpbrt_string_equals(&param->name, &PIXEL_STATS)) { return tpbrt_param_as_bool(param, &options->pixel_stats); }

		static const tpbrt_string_t WAVEFRONT = TPBRT_STRING("wavefront");
			if (tpbrt_string_equals(&param->name, &WAVEFRONT)) { return tpbrt_param_as_bool(param, &options->wavefront); }

		return TPBRT_ERROR_UNKNOWN_OPTION;
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
		state->active_ctm		   = TPBRT_ACTIVE_CTM_ALL;
		tpbrt_mat4_identity(&state->ctm.start);
		tpbrt_mat4_identity(&state->ctm.end);
		state->current_color_space	  = TPBRT_COLOR_SPACE_SRGB;
		state->current_inside_medium  = TPBRT_MEDIA_HANDLE_INVALID;
		state->current_outside_medium = TPBRT_MEDIA_HANDLE_INVALID;
		state->current_area_light	  = TPBRT_MEDIA_HANDLE_INVALID;
		state->material_handle		  = TPBRT_MATERIAL_HANDLE_INVALID;
		state->active_object		  = TPBRT_NULL;

		tpbrt_error_t err			  = tpbrt_init_params_list(&state->shape_params);
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
	  const tpbrt_mat4_animated_t* const transform) {
		tpbrt_coord_sys_entry_t entry;
		entry.name = *name;
		memcpy(&entry.transform, transform, sizeof(tpbrt_mat4_animated_t));
		DYN_ARRAY_PUSH(map->entries, map->count, map->capacity, entry, tpbrt_coord_sys_entry_t);
		return TPBRT_ERROR_NONE;
	}

	tpbrt_bool_t tpbrt_coord_sys_map_get(const tpbrt_coord_sys_map_t* const map, const tpbrt_string_t* const name,
	  tpbrt_mat4_animated_t* const out_transform) {
			for (tpbrt_size_t i = 0; i < map->count; i++) {
					if (map->entries[i].name.size == name->size &&
						strncmp(map->entries[i].name.data, name->data, name->size) == 0) {
						memcpy(out_transform, &map->entries[i].transform, sizeof(tpbrt_mat4_animated_t));
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

		tpbrt_string_t file_data;

		fseek(f, 0, SEEK_END);
		file_data.size = ftell(f);
		fseek(f, 0, SEEK_SET);

		file_data.data = malloc(file_data.size + 1);
			if (file_data.data == TPBRT_NULL) {
				fclose(f);
				return TPBRT_ERROR_OUT_OF_MEMORY;
			}

		fread(file_data.data, 1, file_data.size, f);
		file_data.data[file_data.size] = '\0';
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

		const tpbrt_error_t err = tpbrt_scene_load_from_memory(file_data.data, working_directory, out_scene);

			if (err == TPBRT_ERROR_NONE) {
				const tpbrt_size_t cap	= out_scene->includes.count == 0 ? 8 : out_scene->includes.count * 2;

				tpbrt_string_t* tempPtr = realloc(out_scene->includes.data, cap * sizeof(tpbrt_string_t));
					if (tempPtr == TPBRT_NULL) {
						tpbrt_free_string(&file_data);
						return TPBRT_ERROR_OUT_OF_MEMORY;
					}
				out_scene->includes.data							  = tempPtr;

				out_scene->includes.data[out_scene->includes.count++] = file_data;
			}
			else { tpbrt_free_string(&file_data); }

		return err;
	}

	tpbrt_error_t tpbrt_scene_load_from_memory(const tpbrt_char_t* data, const tpbrt_char_t* working_directory,
	  tpbrt_scene_t* out_scene) {
		memset(out_scene, 0, sizeof(tpbrt_scene_t));

		tpbrt_error_t err = tpbrt_init_options(&out_scene->options);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_free_scene(out_scene);
				return err;
			}

		err = tpbrt_init_textures_list(&out_scene->textures);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_free_scene(out_scene);
				return err;
			}

		err = tpbrt_init_materials_list(&out_scene->materials);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_free_scene(out_scene);
				return err;
			}

		err = tpbrt_init_lights_list(&out_scene->lights);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_free_scene(out_scene);
				return err;
			}

		err = tpbrt_init_medias_list(&out_scene->medias);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_free_scene(out_scene);
				return err;
			}

		err = tpbrt_init_objects_list(&out_scene->objects);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_free_scene(out_scene);
				return err;
			}

		err = tpbrt_init_instances_list(&out_scene->instances);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_free_scene(out_scene);
				return err;
			}

		err = tpbrt_create_default_film(&out_scene->film);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_free_scene(out_scene);
				return err;
			}

		err = tpbrt_create_default_filter(&out_scene->filter);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_free_scene(out_scene);
				return err;
			}

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
		err = tpbrt_init_state(&current_state);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_free_scene(out_scene);
				return err;
			}

		tpbrt_state_t* states_stack = TPBRT_NULL;
		tpbrt_size_t states_count = 0, states_cap = 0;
		tpbrt_bool_t is_world_block = TPBRT_FALSE;

		tpbrt_coord_sys_map_t named_coord_systems;
		err = tpbrt_init_coord_sys_map(&named_coord_systems);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_free_scene(out_scene);
				return err;
			}

		tpbrt_error_t last_error = TPBRT_ERROR_NONE;

			while (parsers_count > 0) {
				tpbrt_parser_node_t* current_node = &parsers[parsers_count - 1];
				tpbrt_element_t element;

				err = tpbrt_parser_parse_next(&current_node->parser, &element);

					if (err == TPBRT_ERROR_END_OF_FILE) {
						parsers_count--;
						continue;
					}
					if (err != TPBRT_ERROR_NONE) {
						last_error = err;
						break;
					}

					switch (element.type) {
						case TPBRT_DIRECTIVE_IDENTITY: {
								if (current_state.active_ctm == TPBRT_ACTIVE_CTM_ALL ||
									current_state.active_ctm == TPBRT_ACTIVE_CTM_START_TIME) {
									tpbrt_mat4_identity(&current_state.ctm.start);
								}

								if (current_state.active_ctm == TPBRT_ACTIVE_CTM_ALL ||
									current_state.active_ctm == TPBRT_ACTIVE_CTM_END_TIME) {
									tpbrt_mat4_identity(&current_state.ctm.end);
								}
							break;
						}
						case TPBRT_DIRECTIVE_TRANSLATE: {
							tpbrt_mat4_t translation;
							tpbrt_mat4_from_translation(&translation, element.as.vector3.v);

							tpbrt_mat4_t res;

								if (current_state.active_ctm == TPBRT_ACTIVE_CTM_ALL ||
									current_state.active_ctm == TPBRT_ACTIVE_CTM_START_TIME) {
									tpbrt_mat4_multiply(&res, &current_state.ctm.start, &translation);
									current_state.ctm.start = res;
								}

								if (current_state.active_ctm == TPBRT_ACTIVE_CTM_ALL ||
									current_state.active_ctm == TPBRT_ACTIVE_CTM_END_TIME) {
									tpbrt_mat4_multiply(&res, &current_state.ctm.end, &translation);
									current_state.ctm.end = res;
								}
							break;
						}
						case TPBRT_DIRECTIVE_SCALE: {
							tpbrt_mat4_t scale;
							tpbrt_mat4_from_scale(&scale, element.as.vector3.v);

							tpbrt_mat4_t res;

								if (current_state.active_ctm == TPBRT_ACTIVE_CTM_ALL ||
									current_state.active_ctm == TPBRT_ACTIVE_CTM_START_TIME) {
									tpbrt_mat4_multiply(&res, &current_state.ctm.start, &scale);
									current_state.ctm.start = res;
								}

								if (current_state.active_ctm == TPBRT_ACTIVE_CTM_ALL ||
									current_state.active_ctm == TPBRT_ACTIVE_CTM_END_TIME) {
									tpbrt_mat4_multiply(&res, &current_state.ctm.end, &scale);
									current_state.ctm.end = res;
								}
							break;
						}
						case TPBRT_DIRECTIVE_ROTATE: {
							tpbrt_mat4_t rotation;
							tpbrt_mat4_from_axis_angle(&rotation, element.as.rotate.v, element.as.rotate.angle);

							tpbrt_mat4_t res;

								if (current_state.active_ctm == TPBRT_ACTIVE_CTM_ALL ||
									current_state.active_ctm == TPBRT_ACTIVE_CTM_START_TIME) {
									tpbrt_mat4_multiply(&res, &current_state.ctm.start, &rotation);
									current_state.ctm.start = res;
								}

								if (current_state.active_ctm == TPBRT_ACTIVE_CTM_ALL ||
									current_state.active_ctm == TPBRT_ACTIVE_CTM_END_TIME) {
									tpbrt_mat4_multiply(&res, &current_state.ctm.end, &rotation);
									current_state.ctm.end = res;
								}
							break;
						}
						case TPBRT_DIRECTIVE_LOOK_AT: {
							tpbrt_mat4_t look_at;
							tpbrt_mat4_look_at_lh(&look_at, element.as.look_at.eye, element.as.look_at.look_at,
							  element.as.look_at.up);

								if (current_state.active_ctm == TPBRT_ACTIVE_CTM_ALL ||
									current_state.active_ctm == TPBRT_ACTIVE_CTM_START_TIME) {
									current_state.ctm.start = look_at;
								}

								if (current_state.active_ctm == TPBRT_ACTIVE_CTM_ALL ||
									current_state.active_ctm == TPBRT_ACTIVE_CTM_END_TIME) {
									current_state.ctm.end = look_at;
								}
							break;
						}
						case TPBRT_DIRECTIVE_COORDINATE_SYSTEM: {
							last_error =
							  tpbrt_coord_sys_map_insert(&named_coord_systems, &element.as.named_entity.name, &current_state.ctm);
							break;
						}
						case TPBRT_DIRECTIVE_COORD_SYS_TRANSFORM: {
							tpbrt_mat4_animated_t mat;
							tpbrt_coord_sys_map_get(&named_coord_systems, &element.as.named_entity.name, &mat);

								if (current_state.active_ctm == TPBRT_ACTIVE_CTM_ALL ||
									current_state.active_ctm == TPBRT_ACTIVE_CTM_START_TIME) {
									current_state.ctm.start = mat.start;
								}

								if (current_state.active_ctm == TPBRT_ACTIVE_CTM_ALL ||
									current_state.active_ctm == TPBRT_ACTIVE_CTM_END_TIME) {
									current_state.ctm.end = mat.end;
								}
							break;
						}
						case TPBRT_DIRECTIVE_TRANSFORM: {
							tpbrt_mat4_t transform;
							tpbrt_mat4_from_array(&transform, element.as.transform.m);

								if (current_state.active_ctm == TPBRT_ACTIVE_CTM_ALL ||
									current_state.active_ctm == TPBRT_ACTIVE_CTM_START_TIME) {
									current_state.ctm.start = transform;
								}

								if (current_state.active_ctm == TPBRT_ACTIVE_CTM_ALL ||
									current_state.active_ctm == TPBRT_ACTIVE_CTM_END_TIME) {
									current_state.ctm.end = transform;
								}
							break;
						}
						case TPBRT_DIRECTIVE_CONCAT_TRANSFORM: {
							tpbrt_mat4_t right;
							tpbrt_mat4_from_array(&right, element.as.transform.m);

							tpbrt_mat4_t res;

								if (current_state.active_ctm == TPBRT_ACTIVE_CTM_ALL ||
									current_state.active_ctm == TPBRT_ACTIVE_CTM_START_TIME) {
									tpbrt_mat4_multiply(&res, &current_state.ctm.start, &right);
									current_state.ctm.start = res;
								}

								if (current_state.active_ctm == TPBRT_ACTIVE_CTM_ALL ||
									current_state.active_ctm == TPBRT_ACTIVE_CTM_END_TIME) {
									tpbrt_mat4_multiply(&res, &current_state.ctm.end, &right);
									current_state.ctm.end = res;
								}
							break;
						}
						case TPBRT_DIRECTIVE_TRANSFORM_TIMES: {
								if (is_world_block) {
									last_error = TPBRT_ERROR_UNEXPECTED_TOKEN;
									break;
								}

							out_scene->start_time = element.as.transform_times.start;
							out_scene->end_time	  = element.as.transform_times.end;
							break;
						}
						case TPBRT_DIRECTIVE_ACTIVE_TRANSFORM: {
							static const tpbrt_string_t START_TIME_STR = TPBRT_STRING("StartTime");
							static const tpbrt_string_t END_TIME_STR   = TPBRT_STRING("EndTime");
							static const tpbrt_string_t ALL_STR		   = TPBRT_STRING("All");

								if (tpbrt_string_equals(&element.as.single_string.type_name, &START_TIME_STR)) {
									current_state.active_ctm = TPBRT_ACTIVE_CTM_START_TIME;
								}
								else if (tpbrt_string_equals(&element.as.single_string.type_name, &END_TIME_STR)) {
									current_state.active_ctm = TPBRT_ACTIVE_CTM_END_TIME;
								}
								else if (tpbrt_string_equals(&element.as.single_string.type_name, &ALL_STR)) {
									current_state.active_ctm = TPBRT_ACTIVE_CTM_ALL;
								}
								else { last_error = TPBRT_ERROR_UNEXPECTED_TOKEN; }
							break;
						}

						case TPBRT_DIRECTIVE_INCLUDE: {
							const tpbrt_size_t cap	= out_scene->includes.count == 0 ? 8 : out_scene->includes.count * 2;

							tpbrt_string_t* tempPtr = realloc(out_scene->includes.data, cap * sizeof(tpbrt_string_t));
								if (tempPtr == TPBRT_NULL) {
									last_error = TPBRT_ERROR_OUT_OF_MEMORY;
									break;
								}
							out_scene->includes.data = tempPtr;

							last_error				 = tpbrt_copy_string(&out_scene->includes.data[out_scene->includes.count++],
							  &element.as.include_import.path);
							break;
						}
						case TPBRT_DIRECTIVE_IMPORT: {
							const tpbrt_size_t cap	= out_scene->includes.count == 0 ? 8 : out_scene->includes.count * 2;

							tpbrt_string_t* tempPtr = realloc(out_scene->includes.data, cap * sizeof(tpbrt_string_t));
								if (tempPtr == TPBRT_NULL) {
									last_error = TPBRT_ERROR_OUT_OF_MEMORY;
									break;
								}
							out_scene->includes.data = tempPtr;

							last_error				 = tpbrt_copy_string(&out_scene->includes.data[out_scene->includes.count++],
							  &element.as.include_import.path);
							break;
						}

						case TPBRT_DIRECTIVE_OPTION: {
								if (is_world_block) {
									last_error = TPBRT_ERROR_UNEXPECTED_TOKEN;
									break;
								}

							last_error = tpbrt_options_apply(&out_scene->options, &element.as.option.param);
							break;
						}

						case TPBRT_DIRECTIVE_CAMERA: {
								if (is_world_block) {
									last_error = TPBRT_ERROR_UNEXPECTED_TOKEN;
									break;
								}

							tpbrt_mat4_animated_t camera_to_world;
							tpbrt_mat4_inverse(&camera_to_world.start, &current_state.ctm.start);
							tpbrt_mat4_inverse(&camera_to_world.end, &current_state.ctm.end);

							static const tpbrt_string_t CAM_STR = TPBRT_STRING("camera");
							last_error = tpbrt_coord_sys_map_insert(&named_coord_systems, &CAM_STR, &camera_to_world);
								if (last_error != TPBRT_ERROR_NONE) { break; }

							last_error = tpbrt_create_camera(&element.as.generic_with_params.type_name,
							  &element.as.generic_with_params.params, &out_scene->film, &camera_to_world,
							  current_state.current_inside_medium, current_state.current_outside_medium, &out_scene->camera);
							break;
						}
						case TPBRT_DIRECTIVE_SAMPLER: {
								if (is_world_block) {
									last_error = TPBRT_ERROR_UNEXPECTED_TOKEN;
									break;
								}

							last_error = tpbrt_create_sampler(&element.as.generic_with_params.type_name,
							  &element.as.generic_with_params.params, &out_scene->sampler);
							break;
						}
						case TPBRT_DIRECTIVE_COLOR_SPACE: {
								if (is_world_block) {
									last_error = TPBRT_ERROR_UNEXPECTED_TOKEN;
									break;
								}

							last_error = tpbrt_color_space_from_string(&element.as.single_string.type_name,
							  &current_state.current_color_space);
							break;
						}
						case TPBRT_DIRECTIVE_FILM: {
								if (is_world_block) {
									last_error = TPBRT_ERROR_UNEXPECTED_TOKEN;
									break;
								}

							last_error = tpbrt_create_film(&element.as.generic_with_params.type_name,
							  &element.as.generic_with_params.params, &out_scene->film);
							break;
						}
						case TPBRT_DIRECTIVE_PIXEL_FILTER: {
								if (is_world_block) {
									last_error = TPBRT_ERROR_UNEXPECTED_TOKEN;
									break;
								}

							last_error = tpbrt_create_filter(&element.as.generic_with_params.type_name,
							  &element.as.generic_with_params.params, &out_scene->filter);
							break;
						}
						case TPBRT_DIRECTIVE_INTEGRATOR: {
								if (is_world_block) {
									last_error = TPBRT_ERROR_UNEXPECTED_TOKEN;
									break;
								}

							last_error = tpbrt_create_integrator(&element.as.generic_with_params.type_name,
							  &element.as.generic_with_params.params, &out_scene->integrator);
							break;
						}
						case TPBRT_DIRECTIVE_ACCELERATOR: {
								if (is_world_block) {
									last_error = TPBRT_ERROR_UNEXPECTED_TOKEN;
									break;
								}

							last_error = tpbrt_create_accelerator(&element.as.generic_with_params.type_name,
							  &element.as.generic_with_params.params, &out_scene->accelerator);
							break;
						}

						case TPBRT_DIRECTIVE_WORLD_BEGIN: {
								if (is_world_block) {
									last_error = TPBRT_ERROR_WORLD_ALREADY_STARTED;
									break;
								}

							is_world_block = TPBRT_TRUE;
							tpbrt_mat4_identity(&current_state.ctm.start);
							tpbrt_mat4_identity(&current_state.ctm.end);
							break;
						}

						case TPBRT_DIRECTIVE_ATTRIBUTE_BEGIN: {
							tpbrt_state_t cloned;
							last_error = tpbrt_state_clone(&cloned, &current_state);
								if (last_error != TPBRT_ERROR_NONE) { break; }
							DYN_ARRAY_PUSH(states_stack, states_count, states_cap, cloned, tpbrt_state_t);
							break;
						}
						case TPBRT_DIRECTIVE_ATTRIBUTE_END: {
								if (states_count == 0) {
									last_error = TPBRT_ERROR_TOO_MANY_END_ATTRIBUTES;
									break;
								}
							tpbrt_free_state(&current_state);
							current_state = states_stack[--states_count];
							break;
						}
						case TPBRT_DIRECTIVE_ATTRIBUTE: {
							static const tpbrt_string_t SHAPE_STR	 = TPBRT_STRING("shape");
							static const tpbrt_string_t LIGHT_STR	 = TPBRT_STRING("light");
							static const tpbrt_string_t MATERIAL_STR = TPBRT_STRING("material");
							static const tpbrt_string_t TEXTURE_STR	 = TPBRT_STRING("texture");
							static const tpbrt_string_t MEDIUM_STR	 = TPBRT_STRING("medium");

								if (tpbrt_string_equals(&element.as.attribute.target, &SHAPE_STR)) {
									last_error =
									  tpbrt_params_list_extend(&current_state.shape_params, &element.as.attribute.params);
								}
								else if (tpbrt_string_equals(&element.as.attribute.target, &LIGHT_STR)) {
									last_error =
									  tpbrt_params_list_extend(&current_state.light_params, &element.as.attribute.params);
								}
								else if (tpbrt_string_equals(&element.as.attribute.target, &MATERIAL_STR)) {
									last_error =
									  tpbrt_params_list_extend(&current_state.material_params, &element.as.attribute.params);
								}
								else if (tpbrt_string_equals(&element.as.attribute.target, &TEXTURE_STR)) {
									last_error =
									  tpbrt_params_list_extend(&current_state.texture_params, &element.as.attribute.params);
								}
								else if (tpbrt_string_equals(&element.as.attribute.target, &MEDIUM_STR)) {
									last_error =
									  tpbrt_params_list_extend(&current_state.medium_params, &element.as.attribute.params);
								}
							break;
						}

						case TPBRT_DIRECTIVE_REVERSE_ORIENTATION: {
							current_state.reverse_orientation = !current_state.reverse_orientation;
							break;
						}

						case TPBRT_DIRECTIVE_SHAPE: {
								if (!is_world_block) {
									last_error = TPBRT_ERROR_UNEXPECTED_TOKEN;
									break;
								}

							last_error =
							  tpbrt_params_list_extend(&element.as.generic_with_params.params, &current_state.shape_params);
								if (last_error != TPBRT_ERROR_NONE) { break; }

							tpbrt_shape_t shape;
							last_error = tpbrt_create_shape(&element.as.generic_with_params.type_name,
							  &element.as.generic_with_params.params, &out_scene->textures, &current_state.ctm,
							  current_state.material_handle, current_state.current_area_light,
							  current_state.current_inside_medium, current_state.current_outside_medium,
							  current_state.reverse_orientation, &shape);
								if (last_error != TPBRT_ERROR_NONE) { break; }

								if (current_state.active_object != TPBRT_NULL) {
									last_error = tpbrt_object_add_shape(current_state.active_object, &shape);
								}
								else { last_error = tpbrt_shape_array_add_shape(&out_scene->shapes, &shape); }
							break;
						}
						case TPBRT_DIRECTIVE_OBJECT_BEGIN: {
								if (!is_world_block) {
									last_error = TPBRT_ERROR_UNEXPECTED_TOKEN;
									break;
								}

								if (current_state.active_object != TPBRT_NULL) {
									last_error = TPBRT_ERROR_INVALID_TOKEN;
									break;
								}

							tpbrt_state_t cloned;
							last_error = tpbrt_state_clone(&cloned, &current_state);
								if (last_error != TPBRT_ERROR_NONE) { break; }

							DYN_ARRAY_PUSH(states_stack, states_count, states_cap, cloned, tpbrt_state_t);

							current_state.active_object = malloc(sizeof(tpbrt_object_t));
							last_error = tpbrt_create_object(&element.as.named_entity.name, current_state.active_object);
							break;
						}
						case TPBRT_DIRECTIVE_OBJECT_END: {
								if (!is_world_block) {
									last_error = TPBRT_ERROR_UNEXPECTED_TOKEN;
									break;
								}

								if (current_state.active_object == TPBRT_NULL) {
									last_error = TPBRT_ERROR_UNEXPECTED_TOKEN;
									break;
								}

							last_error = tpbrt_objects_list_add_object(&out_scene->objects, current_state.active_object);
								if (last_error != TPBRT_ERROR_NONE) { break; }

							free(current_state.active_object);
							current_state.active_object = TPBRT_NULL;

							tpbrt_free_state(&current_state);
							current_state = states_stack[--states_count];
							break;
						}
						case TPBRT_DIRECTIVE_OBJECT_INSTANCE: {
								if (!is_world_block) {
									last_error = TPBRT_ERROR_UNEXPECTED_TOKEN;
									break;
								}

							tpbrt_instance_t inst;
							last_error = tpbrt_create_instance(&element.as.named_entity.name, &out_scene->objects,
							  &current_state.ctm, &inst);
								if (last_error != TPBRT_ERROR_NONE) { break; }

							last_error = tpbrt_instances_list_add_instance(&out_scene->instances, &inst);
							break;
						}

						case TPBRT_DIRECTIVE_LIGHT_SOURCE: {
								if (!is_world_block) {
									last_error = TPBRT_ERROR_UNEXPECTED_TOKEN;
									break;
								}

							last_error =
							  tpbrt_params_list_extend(&element.as.generic_with_params.params, &current_state.light_params);
								if (last_error != TPBRT_ERROR_NONE) { break; }

							tpbrt_light_source_t light;
							last_error = tpbrt_create_light_source(&element.as.generic_with_params.type_name,
							  &element.as.generic_with_params.params, current_state.current_color_space, &current_state.ctm,
							  current_state.current_inside_medium, current_state.current_outside_medium, &light);
								if (last_error != TPBRT_ERROR_NONE) { break; }

							last_error = tpbrt_lights_list_add_light_source(&out_scene->lights, &light);
							break;
						}
						case TPBRT_DIRECTIVE_AREA_LIGHT_SOURCE: {
								if (!is_world_block) {
									last_error = TPBRT_ERROR_UNEXPECTED_TOKEN;
									break;
								}

							last_error =
							  tpbrt_params_list_extend(&element.as.generic_with_params.params, &current_state.light_params);
								if (last_error != TPBRT_ERROR_NONE) { break; }

							tpbrt_area_light_t area_light;
							last_error = tpbrt_create_area_light(&element.as.generic_with_params.type_name,
							  &element.as.generic_with_params.params, current_state.current_color_space, &area_light);
								if (last_error != TPBRT_ERROR_NONE) { break; }

							last_error = tpbrt_lights_list_add_area_light(&out_scene->lights, &current_state.current_area_light,
							  &area_light);
							break;
						}

						case TPBRT_DIRECTIVE_MATERIAL: {
								if (!is_world_block) {
									last_error = TPBRT_ERROR_UNEXPECTED_TOKEN;
									break;
								}

							last_error =
							  tpbrt_params_list_extend(&element.as.generic_with_params.params, &current_state.material_params);
								if (last_error != TPBRT_ERROR_NONE) { break; }

							tpbrt_material_t mat;
							last_error = tpbrt_create_material(&element.as.generic_with_params.type_name,
							  &element.as.generic_with_params.params, &out_scene->textures, &out_scene->materials, &mat);
								if (last_error != TPBRT_ERROR_NONE) { break; }

							last_error =
							  tpbrt_materials_list_add_material(&out_scene->materials, &mat, &current_state.material_handle);
							break;
						}
						case TPBRT_DIRECTIVE_MAKE_NAMED_MATERIAL: {
								if (!is_world_block) {
									last_error = TPBRT_ERROR_UNEXPECTED_TOKEN;
									break;
								}

							last_error =
							  tpbrt_params_list_extend(&element.as.named_with_params.params, &current_state.material_params);
								if (last_error != TPBRT_ERROR_NONE) { break; }

							tpbrt_material_t mat;
							last_error = tpbrt_create_named_material(&element.as.named_with_params.name,
							  &element.as.named_with_params.params, &out_scene->textures, &out_scene->materials, &mat);
								if (last_error != TPBRT_ERROR_NONE) { break; }

							last_error =
							  tpbrt_materials_list_add_material(&out_scene->materials, &mat, &current_state.material_handle);
							break;
						}
						case TPBRT_DIRECTIVE_NAMED_MATERIAL: {
								if (!is_world_block) {
									last_error = TPBRT_ERROR_UNEXPECTED_TOKEN;
									break;
								}

							last_error = tpbrt_materials_list_get_material_handle(&out_scene->materials,
							  &element.as.named_entity.name, &current_state.material_handle);
							break;
						}

						case TPBRT_DIRECTIVE_TEXTURE: {
								if (!is_world_block) {
									last_error = TPBRT_ERROR_UNEXPECTED_TOKEN;
									break;
								}

							last_error = tpbrt_params_list_extend(&element.as.texture.params, &current_state.texture_params);
								if (last_error != TPBRT_ERROR_NONE) { break; }

							tpbrt_texture_t tex;
							last_error = tpbrt_create_texture(&element.as.texture.name, &element.as.texture.type_name,
							  &element.as.texture.class_name, &element.as.texture.params, &out_scene->textures,
							  &current_state.ctm, &tex);

								if (last_error != TPBRT_ERROR_NONE) { break; }

							last_error = tpbrt_textures_list_add_texture(&out_scene->textures, &tex);
							break;
						}

						case TPBRT_DIRECTIVE_MAKE_NAMED_MEDIUM: {
							last_error =
							  tpbrt_params_list_extend(&element.as.generic_with_params.params, &current_state.medium_params);
								if (last_error != TPBRT_ERROR_NONE) { break; }

							tpbrt_media_t media;
							last_error = tpbrt_create_media(&element.as.generic_with_params.type_name,
							  &element.as.generic_with_params.type_name, &element.as.generic_with_params.params, &media);
								if (last_error != TPBRT_ERROR_NONE) { break; }

							last_error = tpbrt_medias_list_add_media(&out_scene->medias, &media);
							break;
						}
						case TPBRT_DIRECTIVE_MEDIUM_INTERFACE: {
								if (element.as.medium_interface.interior.size == 0) {
									current_state.current_inside_medium = TPBRT_MEDIA_HANDLE_INVALID;
								}
								else {
									last_error = tpbrt_medias_list_get_media_handle(&out_scene->medias,
									  &element.as.medium_interface.interior, &current_state.current_inside_medium);
										if (last_error != TPBRT_ERROR_NONE) { break; }
								}

								if (element.as.medium_interface.exterior.size == 0) {
									current_state.current_outside_medium = TPBRT_MEDIA_HANDLE_INVALID;
								}
								else {
									last_error = tpbrt_medias_list_get_media_handle(&out_scene->medias,
									  &element.as.medium_interface.exterior, &current_state.current_outside_medium);
										if (last_error != TPBRT_ERROR_NONE) { break; }
								}
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

		tpbrt_free_string_array(&scene->includes);
	}

#ifdef __cplusplus
}
#endif