#pragma once
#ifndef _TINYPBRT_SCENE_H_
#define _TINYPBRT_SCENE_H_

#include <tinypbrt/detail/fwd.h>

#include <tinypbrt/detail/accelerator.h>
#include <tinypbrt/detail/camera.h>
#include <tinypbrt/detail/common.h>
#include <tinypbrt/detail/error.h>
#include <tinypbrt/detail/film.h>
#include <tinypbrt/detail/filter.h>
#include <tinypbrt/detail/integrator.h>
#include <tinypbrt/detail/light.h>
#include <tinypbrt/detail/material.h>
#include <tinypbrt/detail/media.h>
#include <tinypbrt/detail/object.h>
#include <tinypbrt/detail/object_instance.h>
#include <tinypbrt/detail/options.h>
#include <tinypbrt/detail/sampler.h>
#include <tinypbrt/detail/shape.h>
#include <tinypbrt/detail/texture.h>
#include <tinypbrt/detail/transform.h>

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * @defgroup scene-public Scene
	 * @ingroup tinypbrt-public
	 */

	/**
	 * @brief PBRT scene representation.
	 * @ingroup scene-public
	 */
	typedef struct tpbrt_scene_t {
		/// @brief Scene general options.
		tpbrt_options_t options;

		/// @brief Scene camera.
		tpbrt_camera_t camera;
		/// @brief Scene film.
		tpbrt_film_t film;
		/// @brief Scene integrator.
		tpbrt_integrator_t integrator;
		/// @brief Scene accelerator.
		tpbrt_accelerator_t accelerator;
		/// @brief Scene sampler.
		tpbrt_sampler_t sampler;
		/// @brief Scene animation start time.
		tpbrt_float_t start_time;
		/// @brief Scene filter.
		tpbrt_filter_t filter;
		/// @brief Scene animation end time.
		tpbrt_float_t end_time;

		/// @brief List with all scene textures.
		tpbrt_textures_list_t textures;
		/// @brief List with all scene materials.
		tpbrt_materials_list_t materials;
		/// @brief List with all scene medias.
		tpbrt_medias_list_t medias;
		/// @brief List with all scene lights.
		tpbrt_lights_list_t lights;
		/// @brief List with all scene shapes.
		tpbrt_shapes_list_t shapes;
		/// @brief List with all scene objects.
		tpbrt_objects_list_t objects;
		/// @brief List with all scene objects instances.
		tpbrt_objects_instances_list_t instances;
		/// @brief List with all unique transforms.
		tpbrt_transforms_list_t transforms;
	} tpbrt_scene_t;

	/**
	 * @brief Loads scene from given file.
	 * @param filepath Filepath to scene file.
	 * @param out_scene Pointer to result scene structure.
	 * @return Error code. If not TPBRT_ERROR_NONE then out_scene is not initialized.
	 * @ingroup scene-public
	 */
	TPBRT_API tpbrt_error_t tpbrt_scene_load_from_file(const tpbrt_char_t* filepath, tpbrt_scene_t* out_scene);
	/**
	 * @brief Loads scene from memory (provided string).
	 * @param data Scene string (must end with '\0').
	 * @param working_directory Directory which will be used if Include or Export will be used.
	 * @param out_scene Pointer to result scene structure.
	 * @return Error code. If not TPBRT_ERROR_NONE then out_scene is not initialized.
	 * @ingroup scene-public
	 */
	TPBRT_API tpbrt_error_t tpbrt_scene_load_from_memory(const tpbrt_char_t* data, const tpbrt_char_t* working_directory,
	  tpbrt_scene_t* out_scene);
	/**
	 * @brief Free all scene resources. Nothing happens if TPBRT_NULL is provided.
	 * @ingroup scene-public
	 */
	TPBRT_API void tpbrt_scene_free(tpbrt_scene_t* scene);

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_SCENE_H_
