#include <tinypbrt/pch.h>

#include <tinypbrt/detail/scene.h>

#include <tinypbrt/detail/accelerator_internal.h>
#include <tinypbrt/detail/camera_internal.h>
#include <tinypbrt/detail/color_internal.h>
#include <tinypbrt/detail/common_internal.h>
#include <tinypbrt/detail/coord_internal.h>
#include <tinypbrt/detail/film_internal.h>
#include <tinypbrt/detail/filter_internal.h>
#include <tinypbrt/detail/integrator_internal.h>
#include <tinypbrt/detail/light_internal.h>
#include <tinypbrt/detail/material_internal.h>
#include <tinypbrt/detail/math_internal.h>
#include <tinypbrt/detail/media_internal.h>
#include <tinypbrt/detail/object_instance_internal.h>
#include <tinypbrt/detail/object_internal.h>
#include <tinypbrt/detail/options_internal.h>
#include <tinypbrt/detail/param_internal.h>
#include <tinypbrt/detail/parser_internal.h>
#include <tinypbrt/detail/sampler_internal.h>
#include <tinypbrt/detail/shape_internal.h>
#include <tinypbrt/detail/state_internal.h>
#include <tinypbrt/detail/texture_internal.h>
#include <tinypbrt/detail/token_internal.h>
#include <tinypbrt/detail/tokenizer_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

	tpbrt_error_t tpbrt_scene_load_from_file(const tpbrt_char_t* const filepath, tpbrt_scene_t* const out_scene) {
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

		static const tpbrt_char_t NULL_TERMINATOR = '\0';
		fread((tpbrt_char_t*)file_data.data, 1, file_data.size, f);
		memcpy((tpbrt_char_t*)(file_data.data + file_data.size), &NULL_TERMINATOR, sizeof(tpbrt_char_t));
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
						tpbrt_string_free(&file_data);
						return TPBRT_ERROR_OUT_OF_MEMORY;
					}
				out_scene->includes.data							  = tempPtr;

				out_scene->includes.data[out_scene->includes.count++] = file_data;
			}
			else { tpbrt_string_free(&file_data); }

		return err;
	}

	tpbrt_error_t tpbrt_scene_load_from_memory(const tpbrt_char_t* const data, const tpbrt_char_t* const working_directory,
	  tpbrt_scene_t* const out_scene) {
			if (data == TPBRT_NULL || working_directory == TPBRT_NULL || out_scene == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

		memset(out_scene, 0, sizeof(tpbrt_scene_t));

		out_scene->start_time = 0.0f;
		out_scene->end_time	  = 1.0f;

		tpbrt_error_t err	  = tpbrt_options_init(&out_scene->options);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_scene_free(out_scene);
				return err;
			}

		err = tpbrt_textures_list_init(&out_scene->textures);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_scene_free(out_scene);
				return err;
			}

		err = tpbrt_materials_list_init(&out_scene->materials);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_scene_free(out_scene);
				return err;
			}

		err = tpbrt_lights_list_init(&out_scene->lights);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_scene_free(out_scene);
				return err;
			}

		err = tpbrt_medias_list_init(&out_scene->medias);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_scene_free(out_scene);
				return err;
			}

		err = tpbrt_objects_list_init(&out_scene->objects);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_scene_free(out_scene);
				return err;
			}

		err = tpbrt_objects_instances_list_init(&out_scene->instances);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_scene_free(out_scene);
				return err;
			}

		err = tpbrt_accelerator_init_default(&out_scene->accelerator);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_scene_free(out_scene);
				return err;
			}

		err = tpbrt_integrator_init_default(&out_scene->integrator);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_scene_free(out_scene);
				return err;
			}

		err = tpbrt_sampler_init_default(&out_scene->sampler);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_scene_free(out_scene);
				return err;
			}

		err = tpbrt_film_init_default(&out_scene->film);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_scene_free(out_scene);
				return err;
			}

		err = tpbrt_filter_init_default(&out_scene->filter);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_scene_free(out_scene);
				return err;
			}

		err = tpbrt_camera_init_default(&out_scene->film, &out_scene->camera);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_scene_free(out_scene);
				return err;
			}

		err = tpbrt_shapes_list_init(&out_scene->shapes);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_scene_free(out_scene);
				return err;
			}

		tpbrt_string_t file_view;
		err = tpbrt_string_from_cstr(data, &file_view);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_scene_free(out_scene);
				return err;
			}

		tpbrt_parser_t parser;
		err = tpbrt_parser_init(&parser, file_view);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_scene_free(out_scene);
				return err;
			}

		tpbrt_state_t current_state;
		err = tpbrt_state_init(&current_state);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_scene_free(out_scene);
				return err;
			}

		tpbrt_states_stack_t states;
		err = tpbrt_states_stack_init(&states);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_scene_free(out_scene);
				return err;
			}

		tpbrt_bool_t is_world_block = TPBRT_FALSE;

		tpbrt_coord_system_map_t named_coord_systems;
		err = tpbrt_coord_system_map_init(&named_coord_systems);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_scene_free(out_scene);
				return err;
			}

		tpbrt_error_t last_error = TPBRT_ERROR_NONE;

			while (TPBRT_TRUE) {
				tpbrt_element_t element;

				err = tpbrt_parser_next(&parser, &element);

					if (err == TPBRT_ERROR_END_OF_FILE) { break; }

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
							last_error = tpbrt_coord_system_map_insert(&named_coord_systems, element.as.named_entity.name,
							  &current_state.ctm);
							break;
						}
						case TPBRT_DIRECTIVE_COORD_SYS_TRANSFORM: {
							tpbrt_mat4_animated_t mat;
								if (tpbrt_coord_system_map_get(&named_coord_systems, element.as.named_entity.name, &mat) ==
									TPBRT_FALSE) {
									last_error = TPBRT_ERROR_INVALID_COORD_SYSTEM_NAME;
									break;
								}

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
							tpbrt_mat4_t transform = element.as.transform.m;

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
							tpbrt_mat4_t right = element.as.transform.m;

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

								if (tpbrt_string_equals(element.as.single_string.type_name, START_TIME_STR)) {
									current_state.active_ctm = TPBRT_ACTIVE_CTM_START_TIME;
								}
								else if (tpbrt_string_equals(element.as.single_string.type_name, END_TIME_STR)) {
									current_state.active_ctm = TPBRT_ACTIVE_CTM_END_TIME;
								}
								else if (tpbrt_string_equals(element.as.single_string.type_name, ALL_STR)) {
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

							last_error				 = tpbrt_string_copy(&out_scene->includes.data[out_scene->includes.count++],
							  element.as.include_import.path);
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

							last_error				 = tpbrt_string_copy(&out_scene->includes.data[out_scene->includes.count++],
							  element.as.include_import.path);
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

							tpbrt_camera_free(&out_scene->camera);

							tpbrt_mat4_animated_t camera_to_world;
							tpbrt_mat4_inverse(&camera_to_world.start, &current_state.ctm.start);
							tpbrt_mat4_inverse(&camera_to_world.end, &current_state.ctm.end);

							static const tpbrt_string_t CAM_STR = TPBRT_STRING("camera");
							last_error = tpbrt_coord_system_map_insert(&named_coord_systems, CAM_STR, &camera_to_world);
								if (last_error != TPBRT_ERROR_NONE) { break; }

							last_error = tpbrt_camera_init(element.as.generic_with_params.type_name,
							  element.as.generic_with_params.params, &out_scene->film, &camera_to_world,
							  current_state.current_inside_medium, current_state.current_outside_medium, &out_scene->camera);
							break;
						}
						case TPBRT_DIRECTIVE_SAMPLER: {
								if (is_world_block) {
									last_error = TPBRT_ERROR_UNEXPECTED_TOKEN;
									break;
								}

							last_error = tpbrt_sampler_init(element.as.generic_with_params.type_name,
							  element.as.generic_with_params.params, &out_scene->sampler);
							break;
						}
						case TPBRT_DIRECTIVE_COLOR_SPACE: {
								if (is_world_block) {
									last_error = TPBRT_ERROR_UNEXPECTED_TOKEN;
									break;
								}

							tpbrt_color_space_t color_space;
							last_error = tpbrt_color_space_from_string(element.as.single_string.type_name, &color_space);
							current_state.current_color_space = color_space;
							break;
						}
						case TPBRT_DIRECTIVE_FILM: {
								if (is_world_block) {
									last_error = TPBRT_ERROR_UNEXPECTED_TOKEN;
									break;
								}

							tpbrt_film_free(&out_scene->film);

							last_error = tpbrt_film_init(element.as.generic_with_params.type_name,
							  element.as.generic_with_params.params, &out_scene->film);
							break;
						}
						case TPBRT_DIRECTIVE_PIXEL_FILTER: {
								if (is_world_block) {
									last_error = TPBRT_ERROR_UNEXPECTED_TOKEN;
									break;
								}

							last_error = tpbrt_filter_init(element.as.generic_with_params.type_name,
							  element.as.generic_with_params.params, &out_scene->filter);
							break;
						}
						case TPBRT_DIRECTIVE_INTEGRATOR: {
								if (is_world_block) {
									last_error = TPBRT_ERROR_UNEXPECTED_TOKEN;
									break;
								}

							last_error = tpbrt_integrator_init(element.as.generic_with_params.type_name,
							  element.as.generic_with_params.params, &out_scene->integrator);
							break;
						}
						case TPBRT_DIRECTIVE_ACCELERATOR: {
								if (is_world_block) {
									last_error = TPBRT_ERROR_UNEXPECTED_TOKEN;
									break;
								}

							last_error = tpbrt_accelerator_init(element.as.generic_with_params.type_name,
							  element.as.generic_with_params.params, &out_scene->accelerator);
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
							last_error = tpbrt_states_stack_push(&states, &cloned);
							break;
						}
						case TPBRT_DIRECTIVE_ATTRIBUTE_END: {
								if (states.count == 0) {
									last_error = TPBRT_ERROR_TOO_MANY_END_ATTRIBUTES;
									break;
								}
							tpbrt_state_free(&current_state);
							last_error = tpbrt_states_stack_pop(&states, &current_state);
							break;
						}
						case TPBRT_DIRECTIVE_ATTRIBUTE: {
							static const tpbrt_string_t SHAPE_STR	 = TPBRT_STRING("shape");
							static const tpbrt_string_t LIGHT_STR	 = TPBRT_STRING("light");
							static const tpbrt_string_t MATERIAL_STR = TPBRT_STRING("material");
							static const tpbrt_string_t TEXTURE_STR	 = TPBRT_STRING("texture");
							static const tpbrt_string_t MEDIUM_STR	 = TPBRT_STRING("medium");

								if (tpbrt_string_equals(element.as.attribute.target, SHAPE_STR)) {
									last_error =
									  tpbrt_params_list_extend(&current_state.shape_params, element.as.attribute.params);
								}
								else if (tpbrt_string_equals(element.as.attribute.target, LIGHT_STR)) {
									last_error =
									  tpbrt_params_list_extend(&current_state.light_params, element.as.attribute.params);
								}
								else if (tpbrt_string_equals(element.as.attribute.target, MATERIAL_STR)) {
									last_error =
									  tpbrt_params_list_extend(&current_state.material_params, element.as.attribute.params);
								}
								else if (tpbrt_string_equals(element.as.attribute.target, TEXTURE_STR)) {
									last_error =
									  tpbrt_params_list_extend(&current_state.texture_params, element.as.attribute.params);
								}
								else if (tpbrt_string_equals(element.as.attribute.target, MEDIUM_STR)) {
									last_error =
									  tpbrt_params_list_extend(&current_state.medium_params, element.as.attribute.params);
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
							  tpbrt_params_list_extend(&element.as.generic_with_params.params, current_state.shape_params);
								if (last_error != TPBRT_ERROR_NONE) { break; }

							tpbrt_shape_t shape;
							last_error =
							  tpbrt_shape_init(element.as.generic_with_params.type_name, element.as.generic_with_params.params,
								out_scene->textures, &current_state.ctm, current_state.material_handle,
								current_state.current_area_light, current_state.current_inside_medium,
								current_state.current_outside_medium, current_state.reverse_orientation, &shape);
								if (last_error != TPBRT_ERROR_NONE) { break; }

								if (current_state.active_object != TPBRT_NULL) {
									last_error = tpbrt_object_add_shape(current_state.active_object, &shape);
								}
								else { last_error = tpbrt_shapes_list_append(&out_scene->shapes, &shape); }
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

							last_error = tpbrt_states_stack_push(&states, &cloned);
								if (last_error != TPBRT_ERROR_NONE) { break; }

							current_state.active_object = malloc(sizeof(tpbrt_object_t));
							last_error = tpbrt_object_init(element.as.named_entity.name, current_state.active_object);
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

							last_error = tpbrt_objects_list_append(&out_scene->objects, current_state.active_object);
								if (last_error != TPBRT_ERROR_NONE) { break; }

							free(current_state.active_object);
							current_state.active_object = TPBRT_NULL;

							tpbrt_state_free(&current_state);
							last_error = tpbrt_states_stack_pop(&states, &current_state);
								if (last_error != TPBRT_ERROR_NONE) { break; }
							break;
						}
						case TPBRT_DIRECTIVE_OBJECT_INSTANCE: {
								if (!is_world_block) {
									last_error = TPBRT_ERROR_UNEXPECTED_TOKEN;
									break;
								}

							tpbrt_object_instance_t inst;
							last_error = tpbrt_object_instance_init(element.as.named_entity.name, out_scene->objects,
							  &current_state.ctm, &inst);
								if (last_error != TPBRT_ERROR_NONE) { break; }

							last_error = tpbrt_objects_instances_list_append(&out_scene->instances, &inst);
							break;
						}

						case TPBRT_DIRECTIVE_LIGHT_SOURCE: {
								if (!is_world_block) {
									last_error = TPBRT_ERROR_UNEXPECTED_TOKEN;
									break;
								}

							last_error =
							  tpbrt_params_list_extend(&element.as.generic_with_params.params, current_state.light_params);
								if (last_error != TPBRT_ERROR_NONE) { break; }

							tpbrt_light_source_t light;
							last_error = tpbrt_light_source_init(element.as.generic_with_params.type_name,
							  element.as.generic_with_params.params, current_state.current_color_space, &current_state.ctm,
							  current_state.current_inside_medium, current_state.current_outside_medium, &light);
								if (last_error != TPBRT_ERROR_NONE) { break; }

							last_error = tpbrt_lights_list_append_light_source(&out_scene->lights, &light);
							break;
						}
						case TPBRT_DIRECTIVE_AREA_LIGHT_SOURCE: {
								if (!is_world_block) {
									last_error = TPBRT_ERROR_UNEXPECTED_TOKEN;
									break;
								}

							last_error =
							  tpbrt_params_list_extend(&element.as.generic_with_params.params, current_state.light_params);
								if (last_error != TPBRT_ERROR_NONE) { break; }

							tpbrt_area_light_t area_light;
							last_error = tpbrt_area_light_init(element.as.generic_with_params.type_name,
							  element.as.generic_with_params.params, current_state.current_color_space, &area_light);
								if (last_error != TPBRT_ERROR_NONE) { break; }

							last_error = tpbrt_lights_list_append_area_light(&out_scene->lights,
							  &current_state.current_area_light, &area_light);
							break;
						}

						case TPBRT_DIRECTIVE_MATERIAL: {
								if (!is_world_block) {
									last_error = TPBRT_ERROR_UNEXPECTED_TOKEN;
									break;
								}

							last_error =
							  tpbrt_params_list_extend(&element.as.generic_with_params.params, current_state.material_params);
								if (last_error != TPBRT_ERROR_NONE) { break; }

							tpbrt_material_t mat;
							last_error = tpbrt_material_init(element.as.generic_with_params.type_name,
							  element.as.generic_with_params.params, out_scene->textures, out_scene->materials, &mat);
								if (last_error != TPBRT_ERROR_NONE) { break; }

							last_error = tpbrt_materials_list_append(&out_scene->materials, &mat, &current_state.material_handle);
							break;
						}
						case TPBRT_DIRECTIVE_MAKE_NAMED_MATERIAL: {
								if (!is_world_block) {
									last_error = TPBRT_ERROR_UNEXPECTED_TOKEN;
									break;
								}

							last_error =
							  tpbrt_params_list_extend(&element.as.named_with_params.params, current_state.material_params);
								if (last_error != TPBRT_ERROR_NONE) { break; }

							tpbrt_material_t mat;
							last_error = tpbrt_material_init_named(element.as.named_with_params.name,
							  element.as.named_with_params.params, out_scene->textures, out_scene->materials, &mat);
							// if (last_error != TPBRT_ERROR_NONE) { break; }

							// last_error = tpbrt_materials_list_append(&out_scene->materials, &mat, &current_state.material_handle);
							break;
						}
						case TPBRT_DIRECTIVE_NAMED_MATERIAL: {
								if (!is_world_block) {
									last_error = TPBRT_ERROR_UNEXPECTED_TOKEN;
									break;
								}

							last_error = tpbrt_materials_list_get_handle(out_scene->materials, element.as.named_entity.name,
							  &current_state.material_handle);
							break;
						}

						case TPBRT_DIRECTIVE_TEXTURE: {
								if (!is_world_block) {
									last_error = TPBRT_ERROR_UNEXPECTED_TOKEN;
									break;
								}

							last_error = tpbrt_params_list_extend(&element.as.texture.params, current_state.texture_params);
								if (last_error != TPBRT_ERROR_NONE) { break; }

							tpbrt_texture_t tex;
							last_error = tpbrt_texture_init(element.as.texture.name, element.as.texture.type_name,
							  element.as.texture.class_name, element.as.texture.params, out_scene->textures, &current_state.ctm,
							  &tex);

								if (last_error != TPBRT_ERROR_NONE) { break; }

							last_error = tpbrt_textures_list_append(&out_scene->textures, &tex);
							break;
						}

						case TPBRT_DIRECTIVE_MAKE_NAMED_MEDIUM: {
							last_error = tpbrt_params_list_extend(&element.as.named_medium.params, current_state.medium_params);
								if (last_error != TPBRT_ERROR_NONE) { break; }

							tpbrt_media_t media;
							last_error = tpbrt_media_init(element.as.named_medium.name, element.as.named_medium.type,
							  element.as.named_medium.params, &media);
								if (last_error != TPBRT_ERROR_NONE) { break; }

							last_error = tpbrt_medias_list_append(&out_scene->medias, &media);
							break;
						}
						case TPBRT_DIRECTIVE_MEDIUM_INTERFACE: {
								if (element.as.medium_interface.interior.size == 0) {
									current_state.current_inside_medium = TPBRT_MEDIA_HANDLE_INVALID;
								}
								else {
									last_error = tpbrt_medias_list_get_handle(out_scene->medias,
									  element.as.medium_interface.interior, &current_state.current_inside_medium);
										if (last_error != TPBRT_ERROR_NONE) { break; }
								}

								if (element.as.medium_interface.exterior.size == 0) {
									current_state.current_outside_medium = TPBRT_MEDIA_HANDLE_INVALID;
								}
								else {
									last_error = tpbrt_medias_list_get_handle(out_scene->medias,
									  element.as.medium_interface.exterior, &current_state.current_outside_medium);
										if (last_error != TPBRT_ERROR_NONE) { break; }
								}
							break;
						}
						default: {
							break;
						}
					}

				tpbrt_element_free(&element);
					if (last_error != TPBRT_ERROR_NONE) { break; }
			}

		tpbrt_state_free(&current_state);
		tpbrt_states_stack_free(&states);
		tpbrt_coord_system_map_free(&named_coord_systems);

		return last_error;
	}

	void tpbrt_scene_free(tpbrt_scene_t* scene) {
			if (scene == TPBRT_NULL) { return; }

		tpbrt_options_free(&scene->options);

		tpbrt_camera_free(&scene->camera);
		tpbrt_film_free(&scene->film);

		tpbrt_textures_list_free(&scene->textures);
		tpbrt_materials_list_free(&scene->materials);
		tpbrt_lights_list_free(&scene->lights);
		tpbrt_medias_list_free(&scene->medias);
		tpbrt_objects_list_free(&scene->objects);
		tpbrt_objects_instances_list_free(&scene->instances);

		tpbrt_shapes_list_free(&scene->shapes);

		tpbrt_string_array_free(&scene->includes);
	}

#ifdef __cplusplus
}
#endif