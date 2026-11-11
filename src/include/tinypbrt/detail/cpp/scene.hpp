#pragma once
#ifndef _TINYPBRT_SCENE_HPP_
#define _TINYPBRT_SCENE_HPP_

#include <tinypbrt/detail/cpp/fwd.hpp>

#include <tinypbrt/detail/scene.h>

#include <tinypbrt/detail/cpp/accelerator.hpp>
#include <tinypbrt/detail/cpp/camera.hpp>
#include <tinypbrt/detail/cpp/common.hpp>
#include <tinypbrt/detail/cpp/error.hpp>
#include <tinypbrt/detail/cpp/film.hpp>
#include <tinypbrt/detail/cpp/filter.hpp>
#include <tinypbrt/detail/cpp/integrator.hpp>
#include <tinypbrt/detail/cpp/light.hpp>
#include <tinypbrt/detail/cpp/material.hpp>
#include <tinypbrt/detail/cpp/media.hpp>
#include <tinypbrt/detail/cpp/object.hpp>
#include <tinypbrt/detail/cpp/object_instance.hpp>
#include <tinypbrt/detail/cpp/options.hpp>
#include <tinypbrt/detail/cpp/sampler.hpp>
#include <tinypbrt/detail/cpp/shape.hpp>
#include <tinypbrt/detail/cpp/texture.hpp>
#include <tinypbrt/detail/cpp/transform.hpp>

namespace tpbrt {
	/**
	 * @defgroup scene-cpp-public Scene
	 * @ingroup tinypbrt-cpp-public
	 */

	/**
	 * @brief C++ wrapper for tpbrt_scene_t.
	 * @ingroup scene-cpp-public
	 */
	struct scene : wrapper<scene, tpbrt_scene_t> {
		/// @brief Type used for reflection.
		using reflect_type =
		  std::tuple<options, camera, film, integrator, accelerator, sampler, float_t, filter, float_t, textures_list,
			materials_list, medias_list, lights_list, shapes_list, objects_list, objects_instances_list, transforms_list>;

		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns options value.
		[[nodiscard]] constexpr options options() const noexcept { return _native.options; }

		/// @brief Returns camera value.
		[[nodiscard]] constexpr camera camera() const noexcept { return _native.camera; }

		/// @brief Returns film value.
		[[nodiscard]] constexpr film film() const noexcept { return _native.film; }

		/// @brief Returns integrator value.
		[[nodiscard]] constexpr integrator integrator() const noexcept { return _native.integrator; }

		/// @brief Returns accelerator value.
		[[nodiscard]] constexpr accelerator accelerator() const noexcept { return _native.accelerator; }

		/// @brief Returns sampler value.
		[[nodiscard]] constexpr sampler sampler() const noexcept { return _native.sampler; }

		/// @brief Returns start_time value.
		[[nodiscard]] constexpr float_t start_time() const noexcept { return _native.start_time; }

		/// @brief Returns filter value.
		[[nodiscard]] constexpr filter filter() const noexcept { return _native.filter; }

		/// @brief Returns end_time value.
		[[nodiscard]] constexpr float_t end_time() const noexcept { return _native.end_time; }

		/// @brief Returns textures value.
		[[nodiscard]] constexpr textures_list textures() const noexcept { return _native.textures; }

		/// @brief Returns materials value.
		[[nodiscard]] constexpr materials_list materials() const noexcept { return _native.materials; }

		/// @brief Returns medias value.
		[[nodiscard]] constexpr medias_list medias() const noexcept { return _native.medias; }

		/// @brief Returns lights value.
		[[nodiscard]] constexpr lights_list lights() const noexcept { return _native.lights; }

		/// @brief Returns shapes value.
		[[nodiscard]] constexpr shapes_list shapes() const noexcept { return _native.shapes; }

		/// @brief Returns objects value.
		[[nodiscard]] constexpr objects_list objects() const noexcept { return _native.objects; }

		/// @brief Returns instances value.
		[[nodiscard]] constexpr objects_instances_list instances() const noexcept { return _native.instances; }

		/// @brief Returns transforms value.
		[[nodiscard]] constexpr transforms_list transforms() const noexcept { return _native.transforms; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept {
			return std::make_tuple(options(), camera(), film(), integrator(), accelerator(), sampler(), start_time(), filter(),
			  end_time(), textures(), materials(), medias(), lights(), shapes(), objects(), instances(), transforms());
		}
	};

	/**
	 * @brief Loads scene from given file.
	 * @param filepath Filepath to scene file.
	 * @return Result with error code or with scene.
	 * @ingroup scene-cpp-public
	 */
	[[nodiscard]] constexpr result<scene> load_scene_from_file(const std::string_view filepath) {
		auto* const c_scene = new tpbrt_scene_t {};

		const auto err		= static_cast<error>(tpbrt_scene_load_from_file(filepath.data(), c_scene));

			if (err == error::NONE) { return scene(*c_scene); }

		return err;
	}

	/**
	 * @brief Loads scene from memory (provided string).
	 * @param data Scene string (must end with '\0').
	 * @param working_directory Directory which will be used if Include or Import will be used.
	 * @return Result with error code or with scene.
	 * @ingroup scene-cpp-public
	 */
	[[nodiscard]] constexpr result<scene> load_scene_from_memory(const std::string_view data,
	  const std::string_view working_directory) {
		auto* const c_scene = new tpbrt_scene_t {};

		const auto err		= static_cast<error>(tpbrt_scene_load_from_memory(data.data(), working_directory.data(), c_scene));

			if (err == error::NONE) { return scene(*c_scene); }

		return err;
	}

	/**
	 * @brief Free all scene resources. Nothing happens if nullptr is provided.
	 * @param scene Scene resources to free.
	 * @ingroup scene-cpp-public
	 */
	void free(const scene scene) {
		const tpbrt_scene_t* const c_scene = scene;
		tpbrt_scene_free(c_scene);
		delete c_scene;
	}
} // namespace tpbrt
#endif // _TINYPBRT_SCENE_HPP_
