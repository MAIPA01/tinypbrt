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
#include <tinypbrt/detail/transform_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TPBRT_MAX_INCLUDE_DEPTH 32u

#if defined(_MSC_VER) || defined(__STDC_LIB_EXT1__)
#define TPBRT_HAS_SECURE_CRT TPBRT_TRUE
#else
#define TPBRT_HAS_SECURE_CRT TPBRT_FALSE
#endif

	static FILE* tpbrt_fopen(const tpbrt_char_t* const path, const tpbrt_char_t* const mode) {
#if TPBRT_HAS_SECURE_CRT
		FILE* f = TPBRT_NULL;
		return fopen_s(&f, path, mode) == 0u ? f : TPBRT_NULL;
#else
	return fopen(path, mode);
#endif
	}

	static tpbrt_error_t tpbrt_read_file(const tpbrt_char_t* const path, tpbrt_string_t* const out_data) {
		FILE* f = tpbrt_fopen(path, "rb");
			if (f == TPBRT_NULL) { return TPBRT_ERROR_FILE_NOT_FOUND; }

		fseek(f, 0, SEEK_END);
		const tpbrt_long_t size = ftell(f);
		fseek(f, 0, SEEK_SET);
			if (size < 0) {
				fclose(f);
				return TPBRT_ERROR_FILE_NOT_FOUND;
			}

		tpbrt_char_t* buffer = malloc((tpbrt_size_t)size + 1u);
			if (buffer == TPBRT_NULL) {
				fclose(f);
				return TPBRT_ERROR_OUT_OF_MEMORY;
			}
		const tpbrt_size_t read = fread(buffer, 1u, (tpbrt_size_t)size, f);
		buffer[read]			= '\0';
		fclose(f);

		*out_data = (tpbrt_string_t) { .data = buffer, .size = read };
		return TPBRT_ERROR_NONE;
	}

	static tpbrt_error_t tpbrt_resolve_include_path(const tpbrt_char_t* const working_directory, const tpbrt_string_t path,
	  tpbrt_char_t* const out, const tpbrt_size_t out_size) {
		const tpbrt_bool_t is_absolute =
		  path.size > 0u && (path.data[0] == '/' || path.data[0] == '\\' || (path.size > 1u && path.data[1] == ':')) ?
			TPBRT_TRUE :
			TPBRT_FALSE;
		const tpbrt_size_t dir_len =
		  (is_absolute == TPBRT_TRUE || working_directory == TPBRT_NULL) ? 0u : strlen(working_directory);
			if (dir_len + 1u + path.size + 1u > out_size) { return TPBRT_ERROR_OUT_OF_MEMORY; }

		tpbrt_size_t pos = 0u;
			if (dir_len > 0u) {
				const tpbrt_error_t err = tpbrt_copy_chars(out, out_size, working_directory, dir_len);
					if (err != TPBRT_ERROR_NONE) { return err; }

				pos		   = dir_len;
				out[pos++] = '/';
			}
		return tpbrt_copy_chars(out + pos, out_size - (pos * sizeof(tpbrt_char_t)), path.data, path.size);
	}

	static tpbrt_error_t tpbrt_scene_keep_buffer(tpbrt_string_array_t* const includes, const tpbrt_string_t buffer) {
		tpbrt_string_t* const tmp = realloc(includes->data, (includes->count + 1u) * sizeof(tpbrt_string_t));
			if (tmp == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }
		includes->data					  = tmp;
		includes->data[includes->count++] = buffer;
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_scene_load_from_file(const tpbrt_char_t* const filepath, tpbrt_scene_t* const out_scene) {
			if (filepath == TPBRT_NULL || out_scene == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		tpbrt_string_t file_data;
		tpbrt_error_t err = tpbrt_read_file(filepath, &file_data);
			if (err != TPBRT_ERROR_NONE) { return err; }

		enum { WORKING_DIRECTORY_MAX_LENGTH = 1024 };

		tpbrt_char_t working_directory[WORKING_DIRECTORY_MAX_LENGTH];
		working_directory[0]				  = '\0';

		const tpbrt_char_t* const last_slash  = strrchr(filepath, '/');
		const tpbrt_char_t* const last_bslash = strrchr(filepath, '\\');
		const tpbrt_char_t* sep				  = last_slash;
			if (last_bslash != TPBRT_NULL && (sep == TPBRT_NULL || last_bslash > sep)) { sep = last_bslash; }

			if (sep != TPBRT_NULL) {
				const tpbrt_size_t dir_len = sep - filepath;
					if (dir_len < WORKING_DIRECTORY_MAX_LENGTH) {
						err = tpbrt_copy_chars(working_directory, WORKING_DIRECTORY_MAX_LENGTH * sizeof(tpbrt_char_t), filepath,
						  dir_len);
							if (err != TPBRT_ERROR_NONE) { return err; }
					}
			}

		err = tpbrt_scene_load_from_memory(file_data.data, working_directory, out_scene);
		tpbrt_string_free(&file_data);

		return err;
	}

	static tpbrt_error_t tpbrt_scene_create_camera(tpbrt_camera_create_info_t* const info,
	  tpbrt_transforms_list_builder_t* const transforms_list_builder, tpbrt_scene_t* const scene) {
			if (info->camera_transform.start_transform.transform_idx == TPBRT_TRANSFORM_IDX_INVALID) {
				tpbrt_mat4_animated_t identity;
				tpbrt_mat4_identity(&identity.start);
				tpbrt_mat4_identity(&identity.end);
				const tpbrt_error_t err =
				  tpbrt_transforms_list_builder_append_animated(transforms_list_builder, &identity, &info->camera_transform);
					if (err != TPBRT_ERROR_NONE) { return err; }
			}
		tpbrt_camera_free(&scene->camera);
		return tpbrt_camera_init(info, &scene->film, &scene->camera);
	}

	static tpbrt_error_t tpbrt_scene_init_defaults(tpbrt_scene_t* const out_scene) {
		out_scene->start_time = 0.0f;
		out_scene->end_time	  = 1.0f;

		tpbrt_error_t err	  = tpbrt_options_init(&out_scene->options);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_textures_list_init(&out_scene->textures);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_materials_list_init(&out_scene->materials);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_lights_list_init(&out_scene->lights);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_medias_list_init(&out_scene->medias);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_objects_list_init(&out_scene->objects);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_objects_instances_list_init(&out_scene->instances);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_accelerator_init_default(&out_scene->accelerator);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_integrator_init_default(&out_scene->integrator);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_sampler_init_default(&out_scene->sampler);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_film_init_default(&out_scene->film);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_filter_init_default(&out_scene->filter);
			if (err != TPBRT_ERROR_NONE) { return err; }

		return tpbrt_shapes_list_init(&out_scene->shapes);
	}

	static tpbrt_error_t tpbrt_scene_ctm_set(tpbrt_state_t* const current_state, const tpbrt_mat4_t* const transform) {
			if (current_state == TPBRT_NULL || transform == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (current_state->active_ctm == TPBRT_ACTIVE_CTM_ALL || current_state->active_ctm == TPBRT_ACTIVE_CTM_START_TIME) {
				current_state->ctm.start = *transform;
			}

			if (current_state->active_ctm == TPBRT_ACTIVE_CTM_ALL || current_state->active_ctm == TPBRT_ACTIVE_CTM_END_TIME) {
				current_state->ctm.end = *transform;
			}
		return TPBRT_ERROR_NONE;
	}

	static tpbrt_error_t tpbrt_scene_ctm_set_animated(tpbrt_state_t* const current_state,
	  const tpbrt_mat4_animated_t* const transform) {
			if (current_state == TPBRT_NULL || transform == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (current_state->active_ctm == TPBRT_ACTIVE_CTM_ALL || current_state->active_ctm == TPBRT_ACTIVE_CTM_START_TIME) {
				current_state->ctm.start = transform->start;
			}

			if (current_state->active_ctm == TPBRT_ACTIVE_CTM_ALL || current_state->active_ctm == TPBRT_ACTIVE_CTM_END_TIME) {
				current_state->ctm.end = transform->end;
			}

		return TPBRT_ERROR_NONE;
	}

	static tpbrt_error_t tpbrt_scene_ctm_multiply(tpbrt_state_t* const current_state, const tpbrt_mat4_t* const transform) {
			if (current_state == TPBRT_NULL || transform == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		tpbrt_mat4_t res;

			if (current_state->active_ctm == TPBRT_ACTIVE_CTM_ALL || current_state->active_ctm == TPBRT_ACTIVE_CTM_START_TIME) {
				tpbrt_mat4_multiply(&res, &current_state->ctm.start, transform);
				current_state->ctm.start = res;
			}

			if (current_state->active_ctm == TPBRT_ACTIVE_CTM_ALL || current_state->active_ctm == TPBRT_ACTIVE_CTM_END_TIME) {
				tpbrt_mat4_multiply(&res, &current_state->ctm.end, transform);
				current_state->ctm.end = res;
			}

		return TPBRT_ERROR_NONE;
	}

	static tpbrt_error_t tpbrt_scene_directive_identity(tpbrt_state_t* const current_state) {
			if (current_state == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		tpbrt_mat4_t identity;
		tpbrt_mat4_identity(&identity);

		return tpbrt_scene_ctm_set(current_state, &identity);
	}

	static tpbrt_error_t tpbrt_scene_directive_translate(tpbrt_state_t* const current_state,
	  const tpbrt_element_t* const element) {
			if (current_state == TPBRT_NULL || element == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		tpbrt_mat4_t translation;
		tpbrt_mat4_from_translation(&translation, element->as.vector3.v);

		return tpbrt_scene_ctm_multiply(current_state, &translation);
	}

	static tpbrt_error_t tpbrt_scene_directive_scale(tpbrt_state_t* const current_state, const tpbrt_element_t* const element) {
			if (current_state == TPBRT_NULL || element == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		tpbrt_mat4_t scale;
		tpbrt_mat4_from_scale(&scale, element->as.vector3.v);

		return tpbrt_scene_ctm_multiply(current_state, &scale);
	}

	static tpbrt_error_t tpbrt_scene_directive_rotate(tpbrt_state_t* const current_state, const tpbrt_element_t* const element) {
			if (current_state == TPBRT_NULL || element == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		tpbrt_mat4_t rotation;
		tpbrt_mat4_from_axis_angle(&rotation, element->as.rotate.v, element->as.rotate.angle);

		return tpbrt_scene_ctm_multiply(current_state, &rotation);
	}

	static tpbrt_error_t tpbrt_scene_directive_look_at(tpbrt_state_t* const current_state, const tpbrt_element_t* const element) {
			if (current_state == TPBRT_NULL || element == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		tpbrt_mat4_t look_at;
		tpbrt_mat4_look_at_lh(&look_at, element->as.look_at.eye, element->as.look_at.look_at, element->as.look_at.up);

		return tpbrt_scene_ctm_multiply(current_state, &look_at);
	}

	static tpbrt_error_t tpbrt_scene_directive_coordintate_system(const tpbrt_state_t* const current_state,
	  const tpbrt_element_t* const element, tpbrt_coord_system_map_t* const named_coord_systems) {
			if (current_state == TPBRT_NULL || element == TPBRT_NULL || named_coord_systems == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

		return tpbrt_coord_system_map_insert(named_coord_systems, element->as.named_entity.name, &current_state->ctm);
	}

	static tpbrt_error_t tpbrt_scene_directive_coord_sys_transform(tpbrt_state_t* const current_state,
	  const tpbrt_element_t* const element, const tpbrt_coord_system_map_t* const named_coord_systems) {
			if (current_state == TPBRT_NULL || element == TPBRT_NULL || named_coord_systems == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

		tpbrt_mat4_animated_t mat;
			if (tpbrt_coord_system_map_get(named_coord_systems, element->as.named_entity.name, &mat) == TPBRT_FALSE) {
				return TPBRT_ERROR_NONE;
			}

		return tpbrt_scene_ctm_set_animated(current_state, &mat);
	}

	static tpbrt_error_t tpbrt_scene_directive_transform(tpbrt_state_t* const current_state,
	  const tpbrt_element_t* const element) {
			if (current_state == TPBRT_NULL || element == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		tpbrt_mat4_t transform;
		tpbrt_mat4_transpose(&transform, &element->as.transform.m);

		return tpbrt_scene_ctm_set(current_state, &transform);
	}

	static tpbrt_error_t tpbrt_scene_directive_concat_transform(tpbrt_state_t* const current_state,
	  const tpbrt_element_t* const element) {
			if (current_state == TPBRT_NULL || element == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		tpbrt_mat4_t right;
		tpbrt_mat4_transpose(&right, &element->as.transform.m);

		return tpbrt_scene_ctm_multiply(current_state, &right);
	}

	static tpbrt_error_t tpbrt_scene_directive_transform_times(const tpbrt_element_t* const element,
	  const tpbrt_bool_t is_world_block, tpbrt_scene_t* const out_scene) {
			if (element == TPBRT_NULL || out_scene == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (is_world_block == TPBRT_TRUE) { return TPBRT_ERROR_UNEXPECTED_TOKEN; }

		out_scene->start_time = element->as.transform_times.start;
		out_scene->end_time	  = element->as.transform_times.end;
		return TPBRT_ERROR_NONE;
	}

	static tpbrt_error_t tpbrt_scene_directive_active_transform(tpbrt_state_t* const current_state,
	  const tpbrt_element_t* const element) {
			if (current_state == TPBRT_NULL || element == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t START_TIME_STR = TPBRT_STRING("StartTime");
		static const tpbrt_string_t END_TIME_STR   = TPBRT_STRING("EndTime");
		static const tpbrt_string_t ALL_STR		   = TPBRT_STRING("All");

			if (tpbrt_string_equals(element->as.single_string.type_name, START_TIME_STR)) {
				current_state->active_ctm = TPBRT_ACTIVE_CTM_START_TIME;
				return TPBRT_ERROR_NONE;
			}

			if (tpbrt_string_equals(element->as.single_string.type_name, END_TIME_STR)) {
				current_state->active_ctm = TPBRT_ACTIVE_CTM_END_TIME;
				return TPBRT_ERROR_NONE;
			}

			if (tpbrt_string_equals(element->as.single_string.type_name, ALL_STR)) {
				current_state->active_ctm = TPBRT_ACTIVE_CTM_ALL;
				return TPBRT_ERROR_NONE;
			}

		return TPBRT_ERROR_UNEXPECTED_TOKEN;
	}

	static tpbrt_error_t tpbrt_scene_directive_include(const tpbrt_char_t* const working_directory,
	  tpbrt_string_array_t* const includes, tpbrt_parser_t parsers[TPBRT_MAX_INCLUDE_DEPTH], tpbrt_size_t* const parsers_count,
	  const tpbrt_element_t* const element) {
			if (working_directory == TPBRT_NULL || includes == TPBRT_NULL || parsers == TPBRT_NULL || element == TPBRT_NULL ||
				parsers_count == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

			if (*parsers_count >= TPBRT_MAX_INCLUDE_DEPTH) { return TPBRT_ERROR_INCLUDES_STACK_OVERFLOW; }

		enum { MAX_PATH_LENGTH = 4096 };

		tpbrt_char_t path[MAX_PATH_LENGTH];
		tpbrt_error_t err = tpbrt_resolve_include_path(working_directory, element->as.include_import.path, path, MAX_PATH_LENGTH);
			if (err != TPBRT_ERROR_NONE) { return err; }

		tpbrt_string_t file_data;
		err = tpbrt_read_file(path, &file_data);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_scene_keep_buffer(includes, file_data);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_string_free(&file_data);
				return err;
			}

		err = tpbrt_parser_init(&parsers[*parsers_count], file_data);
			if (err != TPBRT_ERROR_NONE) { return err; }
		++(*parsers_count);

		return TPBRT_ERROR_NONE;
	}

	static tpbrt_error_t tpbrt_scene_directive_import(const tpbrt_char_t* const working_directory,
	  tpbrt_string_array_t* const includes, tpbrt_parser_t parsers[TPBRT_MAX_INCLUDE_DEPTH], tpbrt_size_t* const parsers_count,
	  const tpbrt_element_t* const element) {
		return tpbrt_scene_directive_include(working_directory, includes, parsers, parsers_count, element);
	}

	static tpbrt_error_t tpbrt_scene_directive_option(tpbrt_options_t* const options, const tpbrt_bool_t is_world_block,
	  const tpbrt_element_t* const element) {
			if (options == TPBRT_NULL || element == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (is_world_block == TPBRT_TRUE) { return TPBRT_ERROR_UNEXPECTED_TOKEN; }

		return tpbrt_options_apply(options, &element->as.option.param);
	}

	static tpbrt_error_t tpbrt_scene_directive_camera(const tpbrt_state_t* const current_state,
	  tpbrt_transforms_list_builder_t* const transforms_list_builder, tpbrt_camera_create_info_t* const camera_create_info,
	  const tpbrt_bool_t is_world_block, const tpbrt_element_t* const element,
	  tpbrt_coord_system_map_t* const named_coord_systems) {
			if (current_state == TPBRT_NULL || transforms_list_builder == TPBRT_NULL || camera_create_info == TPBRT_NULL ||
				element == TPBRT_NULL || named_coord_systems == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

			if (is_world_block == TPBRT_TRUE) { return TPBRT_ERROR_UNEXPECTED_TOKEN; }

		tpbrt_mat4_animated_t camera_to_world;
		tpbrt_mat4_inverse(&camera_to_world.start, &current_state->ctm.start);
		tpbrt_mat4_inverse(&camera_to_world.end, &current_state->ctm.end);

		tpbrt_transform_animated_t transform;
		tpbrt_error_t err = tpbrt_transforms_list_builder_append_animated(transforms_list_builder, &camera_to_world, &transform);
			if (err != TPBRT_ERROR_NONE) { return err; }

		tpbrt_camera_create_info_free(camera_create_info);
		err = tpbrt_camera_create_info_init(element->as.generic_with_params.type_name, element->as.generic_with_params.params,
		  transform, current_state->current_inside_medium, current_state->current_outside_medium, camera_create_info);
			if (err != TPBRT_ERROR_NONE) { return err; }

		static const tpbrt_string_t CAM_STR = TPBRT_STRING("camera");
		return tpbrt_coord_system_map_insert(named_coord_systems, CAM_STR, &camera_to_world);
	}

	static tpbrt_error_t tpbrt_scene_directive_sampler(const tpbrt_bool_t is_world_block, const tpbrt_element_t* const element,
	  tpbrt_sampler_t* const sampler) {
			if (element == TPBRT_NULL || sampler == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (is_world_block == TPBRT_TRUE) { return TPBRT_ERROR_UNEXPECTED_TOKEN; }

		return tpbrt_sampler_init(element->as.generic_with_params.type_name, element->as.generic_with_params.params, sampler);
	}

	static tpbrt_error_t tpbrt_scene_directive_color_space(tpbrt_state_t* const current_state,
	  const tpbrt_element_t* const element) {
			if (current_state == TPBRT_NULL || element == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		tpbrt_color_space_t color_space	   = TPBRT_COLOR_SPACE_MAX_NUM;
		const tpbrt_error_t err			   = tpbrt_color_space_from_string(element->as.single_string.type_name, &color_space);
		current_state->current_color_space = color_space;
		return err;
	}

	static tpbrt_error_t tpbrt_scene_directive_film(const tpbrt_bool_t is_world_block, const tpbrt_element_t* const element,
	  tpbrt_film_t* const film) {
			if (element == TPBRT_NULL || film == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (is_world_block == TPBRT_TRUE) { return TPBRT_ERROR_UNEXPECTED_TOKEN; }

		tpbrt_film_free(film);

		return tpbrt_film_init(element->as.generic_with_params.type_name, element->as.generic_with_params.params, film);
	}

	static tpbrt_error_t tpbrt_scene_directive_pixel_filter(const tpbrt_bool_t is_world_block,
	  const tpbrt_element_t* const element, tpbrt_filter_t* const filter) {
			if (element == TPBRT_NULL || filter == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (is_world_block == TPBRT_TRUE) { return TPBRT_ERROR_UNEXPECTED_TOKEN; }

		return tpbrt_filter_init(element->as.generic_with_params.type_name, element->as.generic_with_params.params, filter);
	}

	static tpbrt_error_t tpbrt_scene_directive_integrator(const tpbrt_bool_t is_world_block, const tpbrt_element_t* const element,
	  tpbrt_integrator_t* const integrator) {
			if (element == TPBRT_NULL || integrator == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (is_world_block == TPBRT_TRUE) { return TPBRT_ERROR_UNEXPECTED_TOKEN; }

		return tpbrt_integrator_init(element->as.generic_with_params.type_name, element->as.generic_with_params.params,
		  integrator);
	}

	static tpbrt_error_t tpbrt_scene_directive_accelerator(const tpbrt_bool_t is_world_block,
	  const tpbrt_element_t* const element, tpbrt_accelerator_t* const accelerator) {
			if (element == TPBRT_NULL || accelerator == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (is_world_block == TPBRT_TRUE) { return TPBRT_ERROR_UNEXPECTED_TOKEN; }

		return tpbrt_accelerator_init(element->as.generic_with_params.type_name, element->as.generic_with_params.params,
		  accelerator);
	}

	static tpbrt_error_t tpbrt_scene_directive_world_begin(tpbrt_state_t* const current_state,
	  tpbrt_camera_create_info_t* const camera_create_info, tpbrt_transforms_list_builder_t* const transforms_list_builder,
	  tpbrt_bool_t* const is_world_block, tpbrt_bool_t* const camera_created, tpbrt_scene_t* const out_scene) {
			if (current_state == TPBRT_NULL || camera_create_info == TPBRT_NULL || transforms_list_builder == TPBRT_NULL ||
				is_world_block == TPBRT_NULL || camera_created == TPBRT_NULL || out_scene == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

			if (*is_world_block == TPBRT_TRUE) { return TPBRT_ERROR_WORLD_ALREADY_STARTED; }

		const tpbrt_error_t err = tpbrt_scene_create_camera(camera_create_info, transforms_list_builder, out_scene);
			if (err != TPBRT_ERROR_NONE) { return err; }
		*camera_created = TPBRT_TRUE;

		*is_world_block = TPBRT_TRUE;
		tpbrt_mat4_identity(&current_state->ctm.start);
		tpbrt_mat4_identity(&current_state->ctm.end);
		return TPBRT_ERROR_NONE;
	}

	static tpbrt_error_t tpbrt_scene_directive_attribute_begin(const tpbrt_state_t* const current_state,
	  tpbrt_states_stack_t* const states) {
			if (current_state == TPBRT_NULL || states == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		tpbrt_state_t cloned;

		const tpbrt_error_t err = tpbrt_state_clone(&cloned, current_state);
			if (err != TPBRT_ERROR_NONE) { return err; }

		cloned.type = TPBRT_STATE_TYPE_ATTRIBUTE;
		return tpbrt_states_stack_push(states, &cloned);
	}

	static tpbrt_error_t tpbrt_scene_directive_transform_begin(const tpbrt_state_t* const current_state,
	  tpbrt_states_stack_t* const states) {
		return tpbrt_scene_directive_attribute_begin(current_state, states);
	}

	static tpbrt_error_t tpbrt_scene_directive_attribute_end(tpbrt_state_t* const current_state,
	  tpbrt_states_stack_t* const states) {
			if (current_state == TPBRT_NULL || states == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (states->count == 0) { return TPBRT_ERROR_TOO_MANY_END_ATTRIBUTES; }

			if (states->states[states->count - 1u].type != TPBRT_STATE_TYPE_ATTRIBUTE) { return TPBRT_ERROR_UNEXPECTED_TOKEN; }

		tpbrt_state_free(current_state);
		return tpbrt_states_stack_pop(states, current_state);
	}

	static tpbrt_error_t tpbrt_scene_directive_transform_end(tpbrt_state_t* const current_state,
	  tpbrt_states_stack_t* const states) {
		return tpbrt_scene_directive_attribute_end(current_state, states);
	}

	static tpbrt_error_t tpbrt_scene_directive_attribute(tpbrt_state_t* const current_state,
	  const tpbrt_element_t* const element) {
			if (current_state == TPBRT_NULL || element == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t SHAPE_STR	 = TPBRT_STRING("shape");
		static const tpbrt_string_t LIGHT_STR	 = TPBRT_STRING("light");
		static const tpbrt_string_t MATERIAL_STR = TPBRT_STRING("material");
		static const tpbrt_string_t TEXTURE_STR	 = TPBRT_STRING("texture");
		static const tpbrt_string_t MEDIUM_STR	 = TPBRT_STRING("medium");

			if (tpbrt_string_equals(element->as.attribute.target, SHAPE_STR)) {
				return tpbrt_params_list_extend(&current_state->shape_params, element->as.attribute.params);
			}

			if (tpbrt_string_equals(element->as.attribute.target, LIGHT_STR)) {
				return tpbrt_params_list_extend(&current_state->light_params, element->as.attribute.params);
			}

			if (tpbrt_string_equals(element->as.attribute.target, MATERIAL_STR)) {
				return tpbrt_params_list_extend(&current_state->material_params, element->as.attribute.params);
			}

			if (tpbrt_string_equals(element->as.attribute.target, TEXTURE_STR)) {
				return tpbrt_params_list_extend(&current_state->texture_params, element->as.attribute.params);
			}

			if (tpbrt_string_equals(element->as.attribute.target, MEDIUM_STR)) {
				return tpbrt_params_list_extend(&current_state->medium_params, element->as.attribute.params);
			}

		return TPBRT_ERROR_NONE;
	}

	static tpbrt_error_t tpbrt_scene_directive_reverse_orientation(tpbrt_state_t* const current_state) {
			if (current_state == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		current_state->reverse_orientation = current_state->reverse_orientation == TPBRT_TRUE ? TPBRT_FALSE : TPBRT_TRUE;
		return TPBRT_ERROR_NONE;
	}

	static tpbrt_error_t tpbrt_scene_directive_shape(const tpbrt_state_t* const current_state, const tpbrt_bool_t is_world_block,
	  tpbrt_transforms_list_builder_t* const transforms_list_builder, tpbrt_element_t* const element,
	  tpbrt_scene_t* const out_scene) {
			if (current_state == TPBRT_NULL || transforms_list_builder == TPBRT_NULL || element == TPBRT_NULL ||
				out_scene == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

			if (is_world_block == TPBRT_FALSE) { return TPBRT_ERROR_UNEXPECTED_TOKEN; }

		tpbrt_error_t err =
		  tpbrt_params_list_append_missing(&element->as.generic_with_params.params, current_state->shape_params);
			if (err != TPBRT_ERROR_NONE) { return err; }

		tpbrt_transform_animated_t transform;
		err = tpbrt_transforms_list_builder_append_animated(transforms_list_builder, &current_state->ctm, &transform);
			if (err != TPBRT_ERROR_NONE) { return err; }

		tpbrt_shape_t shape;
		err =
		  tpbrt_shape_init(element->as.generic_with_params.type_name, element->as.generic_with_params.params, out_scene->textures,
			transform, current_state->material_handle, current_state->current_area_light, current_state->current_inside_medium,
			current_state->current_outside_medium, current_state->reverse_orientation, &shape);
			if (err != TPBRT_ERROR_NONE) { return err; }

			if (current_state->active_object != TPBRT_NULL) {
				return tpbrt_object_add_shape(current_state->active_object, &shape);
			}
		return tpbrt_shapes_list_append(&out_scene->shapes, &shape);
	}

	static tpbrt_error_t tpbrt_scene_directive_object_begin(tpbrt_state_t* const current_state, const tpbrt_bool_t is_world_block,
	  const tpbrt_element_t* const element, tpbrt_states_stack_t* const states) {
			if (current_state == TPBRT_NULL || element == TPBRT_NULL || states == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

			if (is_world_block == TPBRT_FALSE) { return TPBRT_ERROR_UNEXPECTED_TOKEN; }

			if (current_state->active_object != TPBRT_NULL) { return TPBRT_ERROR_INVALID_TOKEN; }

		tpbrt_state_t cloned;
		tpbrt_error_t err = tpbrt_state_clone(&cloned, current_state);
			if (err != TPBRT_ERROR_NONE) { return err; }
		cloned.type = TPBRT_STATE_TYPE_OBJECT;

		err			= tpbrt_states_stack_push(states, &cloned);
			if (err != TPBRT_ERROR_NONE) { return err; }

		current_state->active_object = malloc(sizeof(tpbrt_object_t));
		return tpbrt_object_init(element->as.named_entity.name, current_state->active_object);
	}

	static tpbrt_error_t tpbrt_scene_directive_object_end(tpbrt_state_t* const current_state, const tpbrt_bool_t is_world_block,
	  tpbrt_states_stack_t* const states, tpbrt_objects_list_t* const objects) {
			if (current_state == TPBRT_NULL || states == TPBRT_NULL || objects == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

			if (is_world_block == TPBRT_FALSE) { return TPBRT_ERROR_UNEXPECTED_TOKEN; }

			if (current_state->active_object == TPBRT_NULL || states->count == 0u ||
				states->states[states->count - 1u].type != TPBRT_STATE_TYPE_OBJECT) {
				return TPBRT_ERROR_UNEXPECTED_TOKEN;
			}

		const tpbrt_error_t err = tpbrt_objects_list_append(objects, current_state->active_object);
			if (err != TPBRT_ERROR_NONE) { return err; }

		free(current_state->active_object);
		current_state->active_object = TPBRT_NULL;

		tpbrt_state_free(current_state);
		return tpbrt_states_stack_pop(states, current_state);
	}

	static tpbrt_error_t tpbrt_scene_directive_object_instance(const tpbrt_state_t* const current_state,
	  tpbrt_transforms_list_builder_t* const transforms_list_builder, const tpbrt_bool_t is_world_block,
	  const tpbrt_element_t* const element, tpbrt_scene_t* const out_scene) {
			if (current_state == TPBRT_NULL || transforms_list_builder == TPBRT_NULL || element == TPBRT_NULL ||
				out_scene == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

			if (is_world_block == TPBRT_FALSE) { return TPBRT_ERROR_UNEXPECTED_TOKEN; }

		tpbrt_transform_animated_t transform;
		tpbrt_error_t err =
		  tpbrt_transforms_list_builder_append_animated(transforms_list_builder, &current_state->ctm, &transform);
			if (err != TPBRT_ERROR_NONE) { return err; }

		tpbrt_object_instance_t inst;
		err = tpbrt_object_instance_init(element->as.named_entity.name, out_scene->objects, transform, &inst);
			if (err != TPBRT_ERROR_NONE) { return err; }

		return tpbrt_objects_instances_list_append(&out_scene->instances, &inst);
	}

	static tpbrt_error_t tpbrt_scene_directive_light_source(const tpbrt_state_t* const current_state,
	  tpbrt_transforms_list_builder_t* const transforms_list_builder, const tpbrt_bool_t is_world_block,
	  tpbrt_element_t* const element, tpbrt_lights_list_t* const lights) {
			if (current_state == TPBRT_NULL || transforms_list_builder == TPBRT_NULL || element == TPBRT_NULL ||
				lights == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

			if (is_world_block == TPBRT_FALSE) { return TPBRT_ERROR_UNEXPECTED_TOKEN; }

		tpbrt_error_t err =
		  tpbrt_params_list_append_missing(&element->as.generic_with_params.params, current_state->light_params);
			if (err != TPBRT_ERROR_NONE) { return err; }

		tpbrt_transform_handle_t transform;
		err = tpbrt_transforms_list_builder_append(transforms_list_builder, &current_state->ctm.start, &transform);
			if (err != TPBRT_ERROR_NONE) { return err; }

		tpbrt_light_source_t light;
		err = tpbrt_light_source_init(element->as.generic_with_params.type_name, element->as.generic_with_params.params,
		  current_state->current_color_space, transform, current_state->current_inside_medium,
		  current_state->current_outside_medium, &light);
			if (err != TPBRT_ERROR_NONE) { return err; }

		return tpbrt_lights_list_append_light_source(lights, &light);
	}

	static tpbrt_error_t tpbrt_scene_directive_area_light_source(tpbrt_state_t* const current_state,
	  const tpbrt_bool_t is_world_block, tpbrt_element_t* const element, tpbrt_lights_list_t* const lights) {
			if (current_state == TPBRT_NULL || element == TPBRT_NULL || lights == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

			if (is_world_block == TPBRT_FALSE) { return TPBRT_ERROR_UNEXPECTED_TOKEN; }

		tpbrt_error_t err =
		  tpbrt_params_list_append_missing(&element->as.generic_with_params.params, current_state->light_params);
			if (err != TPBRT_ERROR_NONE) { return err; }

		tpbrt_area_light_t area_light;
		err = tpbrt_area_light_init(element->as.generic_with_params.type_name, element->as.generic_with_params.params,
		  current_state->current_color_space, &area_light);
			if (err != TPBRT_ERROR_NONE) { return err; }

		return tpbrt_lights_list_append_area_light(lights, &current_state->current_area_light, &area_light);
	}

	static tpbrt_error_t tpbrt_scene_directive_material(tpbrt_state_t* const current_state, const tpbrt_bool_t is_world_block,
	  tpbrt_element_t* const element, tpbrt_scene_t* const out_scene) {
			if (current_state == TPBRT_NULL || element == TPBRT_NULL || out_scene == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

			if (is_world_block == TPBRT_FALSE) { return TPBRT_ERROR_UNEXPECTED_TOKEN; }

		tpbrt_error_t err =
		  tpbrt_params_list_append_missing(&element->as.generic_with_params.params, current_state->material_params);
			if (err != TPBRT_ERROR_NONE) { return err; }

		tpbrt_material_t mat;
		err = tpbrt_material_init(element->as.generic_with_params.type_name, element->as.generic_with_params.params,
		  out_scene->textures, out_scene->materials, &mat);
			if (err != TPBRT_ERROR_NONE) { return err; }

		return tpbrt_materials_list_append(&out_scene->materials, &mat, &current_state->material_handle);
	}

	static tpbrt_error_t tpbrt_scene_directive_make_named_material(const tpbrt_state_t* const current_state,
	  const tpbrt_bool_t is_world_block, tpbrt_element_t* const element, tpbrt_scene_t* const out_scene) {
			if (current_state == TPBRT_NULL || element == TPBRT_NULL || out_scene == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

			if (is_world_block == TPBRT_FALSE) { return TPBRT_ERROR_UNEXPECTED_TOKEN; }

		tpbrt_error_t err =
		  tpbrt_params_list_append_missing(&element->as.named_with_params.params, current_state->material_params);
			if (err != TPBRT_ERROR_NONE) { return err; }

		tpbrt_material_t mat;
		err = tpbrt_material_init_named(element->as.named_with_params.name, element->as.named_with_params.params,
		  out_scene->textures, out_scene->materials, &mat);
			if (err != TPBRT_ERROR_NONE) { return err; }

		tpbrt_material_handle_t mat_handle;
		return tpbrt_materials_list_append(&out_scene->materials, &mat, &mat_handle);
	}

	static tpbrt_error_t tpbrt_scene_directive_named_material(tpbrt_state_t* const current_state,
	  const tpbrt_bool_t is_world_block, const tpbrt_element_t* const element, const tpbrt_materials_list_t materials) {
			if (current_state == TPBRT_NULL || element == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (is_world_block == TPBRT_FALSE) { return TPBRT_ERROR_UNEXPECTED_TOKEN; }

		return tpbrt_materials_list_get_handle(materials, element->as.named_entity.name, &current_state->material_handle);
	}

	static tpbrt_error_t tpbrt_scene_directive_texture(const tpbrt_state_t* const current_state,
	  tpbrt_transforms_list_builder_t* const transforms_list_builder, const tpbrt_bool_t is_world_block,
	  tpbrt_element_t* const element, tpbrt_textures_list_t* const textures) {
			if (current_state == TPBRT_NULL || transforms_list_builder == TPBRT_NULL || element == TPBRT_NULL ||
				textures == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

			if (is_world_block == TPBRT_FALSE) { return TPBRT_ERROR_UNEXPECTED_TOKEN; }

		tpbrt_error_t err = tpbrt_params_list_append_missing(&element->as.texture.params, current_state->texture_params);
			if (err != TPBRT_ERROR_NONE) { return err; }

		tpbrt_transform_handle_t transform;
		err = tpbrt_transforms_list_builder_append(transforms_list_builder, &current_state->ctm.start, &transform);
			if (err != TPBRT_ERROR_NONE) { return err; }

		tpbrt_texture_t tex;
		err = tpbrt_texture_init(element->as.texture.name, element->as.texture.type_name, element->as.texture.class_name,
		  element->as.texture.params, *textures, transform, &tex);
			if (err != TPBRT_ERROR_NONE) { return err; }

		return tpbrt_textures_list_append(textures, &tex);
	}

	static tpbrt_error_t tpbrt_scene_directive_make_named_medium(const tpbrt_state_t* const current_state,
	  tpbrt_transforms_list_builder_t* const transforms_list_builder, tpbrt_element_t* const element,
	  tpbrt_medias_list_t* const medias) {
			if (current_state == TPBRT_NULL || transforms_list_builder == TPBRT_NULL || element == TPBRT_NULL ||
				medias == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

		tpbrt_error_t err = tpbrt_params_list_append_missing(&element->as.named_medium.params, current_state->medium_params);
			if (err != TPBRT_ERROR_NONE) { return err; }

		tpbrt_transform_handle_t transform;
		err = tpbrt_transforms_list_builder_append(transforms_list_builder, &current_state->ctm.start, &transform);
			if (err != TPBRT_ERROR_NONE) { return err; }

		tpbrt_media_t media;
		err = tpbrt_media_init(element->as.named_medium.name, element->as.named_medium.type, element->as.named_medium.params,
		  transform, &media);
			if (err != TPBRT_ERROR_NONE) { return err; }

		return tpbrt_medias_list_append(medias, &media);
	}

	static tpbrt_error_t tpbrt_scene_directive_medium_interface(tpbrt_state_t* const current_state,
	  const tpbrt_element_t* const element, const tpbrt_medias_list_t medias) {
			if (current_state == TPBRT_NULL || element == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (tpbrt_string_empty(element->as.medium_interface.interior) == TPBRT_TRUE) {
				current_state->current_inside_medium = TPBRT_MEDIA_HANDLE_INVALID;
			}
			else {
				const tpbrt_error_t err = tpbrt_medias_list_get_handle(medias, element->as.medium_interface.interior,
				  &current_state->current_inside_medium);
					if (err != TPBRT_ERROR_NONE) { return err; }
			}

			if (tpbrt_string_empty(element->as.medium_interface.exterior) == TPBRT_TRUE) {
				current_state->current_outside_medium = TPBRT_MEDIA_HANDLE_INVALID;
			}
			else {
				const tpbrt_error_t err = tpbrt_medias_list_get_handle(medias, element->as.medium_interface.exterior,
				  &current_state->current_outside_medium);
					if (err != TPBRT_ERROR_NONE) { return err; }
			}

		return TPBRT_ERROR_NONE;
	}

	static tpbrt_error_t tpbrt_scene_process_directive(tpbrt_state_t* const current_state, tpbrt_element_t* const element,
	  tpbrt_coord_system_map_t* const named_coord_systems, tpbrt_transforms_list_builder_t* const transforms_list_builder,
	  tpbrt_states_stack_t* const states, tpbrt_camera_create_info_t* const camera_create_info,
	  tpbrt_bool_t* const camera_created, tpbrt_bool_t* const is_world_block, const tpbrt_char_t* const working_directory,
	  tpbrt_string_array_t* const includes, tpbrt_parser_t parsers[TPBRT_MAX_INCLUDE_DEPTH], tpbrt_size_t* const parsers_count,
	  tpbrt_scene_t* out_scene) {
			if (current_state == TPBRT_NULL || element == TPBRT_NULL || named_coord_systems == TPBRT_NULL ||
				transforms_list_builder == TPBRT_NULL || states == TPBRT_NULL || camera_create_info == TPBRT_NULL ||
				camera_created == TPBRT_NULL || is_world_block == TPBRT_NULL || working_directory == TPBRT_NULL ||
				includes == TPBRT_NULL || parsers == TPBRT_NULL || parsers_count == TPBRT_NULL || out_scene == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

			switch (element->type) {
				case TPBRT_DIRECTIVE_IDENTITY: {
					return tpbrt_scene_directive_identity(current_state);
				}
				case TPBRT_DIRECTIVE_TRANSLATE: {
					return tpbrt_scene_directive_translate(current_state, element);
				}
				case TPBRT_DIRECTIVE_SCALE: {
					return tpbrt_scene_directive_scale(current_state, element);
				}
				case TPBRT_DIRECTIVE_ROTATE: {
					return tpbrt_scene_directive_rotate(current_state, element);
				}
				case TPBRT_DIRECTIVE_LOOK_AT: {
					return tpbrt_scene_directive_look_at(current_state, element);
				}
				case TPBRT_DIRECTIVE_COORDINATE_SYSTEM: {
					return tpbrt_scene_directive_coordintate_system(current_state, element, named_coord_systems);
				}
				case TPBRT_DIRECTIVE_COORD_SYS_TRANSFORM: {
					return tpbrt_scene_directive_coord_sys_transform(current_state, element, named_coord_systems);
				}
				case TPBRT_DIRECTIVE_TRANSFORM: {
					return tpbrt_scene_directive_transform(current_state, element);
				}
				case TPBRT_DIRECTIVE_CONCAT_TRANSFORM: {
					return tpbrt_scene_directive_concat_transform(current_state, element);
				}
				case TPBRT_DIRECTIVE_TRANSFORM_TIMES: {
					return tpbrt_scene_directive_transform_times(element, *is_world_block, out_scene);
				}
				case TPBRT_DIRECTIVE_ACTIVE_TRANSFORM: {
					return tpbrt_scene_directive_active_transform(current_state, element);
				}

				case TPBRT_DIRECTIVE_INCLUDE: {
					return tpbrt_scene_directive_include(working_directory, includes, parsers, parsers_count, element);
				}
				case TPBRT_DIRECTIVE_IMPORT: {
					return tpbrt_scene_directive_import(working_directory, includes, parsers, parsers_count, element);
				}

				case TPBRT_DIRECTIVE_OPTION: {
					return tpbrt_scene_directive_option(&out_scene->options, *is_world_block, element);
				}

				case TPBRT_DIRECTIVE_CAMERA: {
					return tpbrt_scene_directive_camera(current_state, transforms_list_builder, camera_create_info,
					  *is_world_block, element, named_coord_systems);
				}
				case TPBRT_DIRECTIVE_SAMPLER: {
					return tpbrt_scene_directive_sampler(*is_world_block, element, &out_scene->sampler);
				}
				case TPBRT_DIRECTIVE_COLOR_SPACE: {
					return tpbrt_scene_directive_color_space(current_state, element);
				}
				case TPBRT_DIRECTIVE_FILM: {
					return tpbrt_scene_directive_film(*is_world_block, element, &out_scene->film);
				}
				case TPBRT_DIRECTIVE_PIXEL_FILTER: {
					return tpbrt_scene_directive_pixel_filter(*is_world_block, element, &out_scene->filter);
				}
				case TPBRT_DIRECTIVE_INTEGRATOR: {
					return tpbrt_scene_directive_integrator(*is_world_block, element, &out_scene->integrator);
				}
				case TPBRT_DIRECTIVE_ACCELERATOR: {
					return tpbrt_scene_directive_accelerator(*is_world_block, element, &out_scene->accelerator);
				}

				case TPBRT_DIRECTIVE_WORLD_BEGIN: {
					return tpbrt_scene_directive_world_begin(current_state, camera_create_info, transforms_list_builder,
					  is_world_block, camera_created, out_scene);
				}
				case TPBRT_DIRECTIVE_TRANSFORM_BEGIN: {
					return tpbrt_scene_directive_transform_begin(current_state, states);
				}
				case TPBRT_DIRECTIVE_ATTRIBUTE_BEGIN: {
					return tpbrt_scene_directive_attribute_begin(current_state, states);
				}
				case TPBRT_DIRECTIVE_TRANSFORM_END: {
					return tpbrt_scene_directive_transform_end(current_state, states);
				}
				case TPBRT_DIRECTIVE_ATTRIBUTE_END: {
					return tpbrt_scene_directive_attribute_end(current_state, states);
				}
				case TPBRT_DIRECTIVE_ATTRIBUTE: {
					return tpbrt_scene_directive_attribute(current_state, element);
				}

				case TPBRT_DIRECTIVE_REVERSE_ORIENTATION: {
					return tpbrt_scene_directive_reverse_orientation(current_state);
				}

				case TPBRT_DIRECTIVE_SHAPE: {
					return tpbrt_scene_directive_shape(current_state, *is_world_block, transforms_list_builder, element,
					  out_scene);
				}
				case TPBRT_DIRECTIVE_OBJECT_BEGIN: {
					return tpbrt_scene_directive_object_begin(current_state, *is_world_block, element, states);
				}
				case TPBRT_DIRECTIVE_OBJECT_END: {
					return tpbrt_scene_directive_object_end(current_state, *is_world_block, states, &out_scene->objects);
				}
				case TPBRT_DIRECTIVE_OBJECT_INSTANCE: {
					return tpbrt_scene_directive_object_instance(current_state, transforms_list_builder, *is_world_block, element,
					  out_scene);
				}

				case TPBRT_DIRECTIVE_LIGHT_SOURCE: {
					return tpbrt_scene_directive_light_source(current_state, transforms_list_builder, *is_world_block, element,
					  &out_scene->lights);
				}
				case TPBRT_DIRECTIVE_AREA_LIGHT_SOURCE: {
					return tpbrt_scene_directive_area_light_source(current_state, *is_world_block, element, &out_scene->lights);
				}

				case TPBRT_DIRECTIVE_MATERIAL: {
					return tpbrt_scene_directive_material(current_state, *is_world_block, element, out_scene);
				}
				case TPBRT_DIRECTIVE_MAKE_NAMED_MATERIAL: {
					return tpbrt_scene_directive_make_named_material(current_state, *is_world_block, element, out_scene);
				}
				case TPBRT_DIRECTIVE_NAMED_MATERIAL: {
					return tpbrt_scene_directive_named_material(current_state, *is_world_block, element, out_scene->materials);
				}

				case TPBRT_DIRECTIVE_TEXTURE: {
					return tpbrt_scene_directive_texture(current_state, transforms_list_builder, *is_world_block, element,
					  &out_scene->textures);
				}

				case TPBRT_DIRECTIVE_MAKE_NAMED_MEDIUM: {
					return tpbrt_scene_directive_make_named_medium(current_state, transforms_list_builder, element,
					  &out_scene->medias);
				}
				case TPBRT_DIRECTIVE_MEDIUM_INTERFACE: {
					return tpbrt_scene_directive_medium_interface(current_state, element, out_scene->medias);
				}
				default: {
					return TPBRT_ERROR_INVALID_DIRECTIVE;
				}
			}
	}

	tpbrt_error_t tpbrt_scene_load_from_memory(const tpbrt_char_t* const data, const tpbrt_char_t* const working_directory,
	  tpbrt_scene_t* const out_scene) {
			if (data == TPBRT_NULL || working_directory == TPBRT_NULL || out_scene == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

			if (memset(out_scene, 0, sizeof(tpbrt_scene_t)) == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		tpbrt_error_t err = tpbrt_scene_init_defaults(out_scene);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_scene_free(out_scene);
				return err;
			}

		tpbrt_camera_create_info_t camera_create_info;
		tpbrt_bool_t camera_created = TPBRT_FALSE;
		err							= tpbrt_camera_create_info_init_default(&camera_create_info);
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

		tpbrt_coord_system_map_t named_coord_systems;
		err = tpbrt_coord_system_map_init(&named_coord_systems);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_scene_free(out_scene);
				return err;
			}

		tpbrt_transforms_list_builder_t transforms_list_builder;
		err = tpbrt_transforms_list_builder_init(&transforms_list_builder, &out_scene->transforms);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_scene_free(out_scene);
				return err;
			}

		tpbrt_parser_t parsers[TPBRT_MAX_INCLUDE_DEPTH];
		tpbrt_size_t parsers_count = 1u;
		err						   = tpbrt_parser_init(&parsers[0], file_view);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_scene_free(out_scene);
				return err;
			}

		tpbrt_string_array_t includes = TPBRT_EMPTY_STRING_ARRAY;

		tpbrt_bool_t is_world_block	  = TPBRT_FALSE;
		tpbrt_error_t last_error	  = TPBRT_ERROR_NONE;

			while (parsers_count > 0u) {
				tpbrt_element_t element;
				err = tpbrt_parser_next(&parsers[parsers_count - 1u], &element);

					if (err == TPBRT_ERROR_END_OF_FILE) {
						--parsers_count;
						continue;
					}

					if (err != TPBRT_ERROR_NONE) {
						last_error = err;
						break;
					}

				err = tpbrt_scene_process_directive(&current_state, &element, &named_coord_systems, &transforms_list_builder,
				  &states, &camera_create_info, &camera_created, &is_world_block, working_directory, &includes, parsers,
				  &parsers_count, out_scene);

					if (err != TPBRT_ERROR_NONE) { last_error = err; }

				tpbrt_element_free(&element);
					if (last_error != TPBRT_ERROR_NONE) { break; }
			}

			if (last_error == TPBRT_ERROR_NONE && camera_created == TPBRT_FALSE) {
				last_error = tpbrt_scene_create_camera(&camera_create_info, &transforms_list_builder, out_scene);
			}
		tpbrt_camera_create_info_free(&camera_create_info);

			if (current_state.active_object != TPBRT_NULL) {
				tpbrt_object_free(current_state.active_object);
				free(current_state.active_object);
			}

		tpbrt_string_array_free(&includes);
		tpbrt_transforms_list_builder_finish(&transforms_list_builder);
		tpbrt_state_free(&current_state);
		tpbrt_states_stack_free(&states);
		tpbrt_coord_system_map_free(&named_coord_systems);

			if (last_error != TPBRT_ERROR_NONE) { tpbrt_scene_free(out_scene); }

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
		tpbrt_transforms_list_free(&scene->transforms);

		tpbrt_shapes_list_free(&scene->shapes);
	}

#ifdef __cplusplus
}
#endif