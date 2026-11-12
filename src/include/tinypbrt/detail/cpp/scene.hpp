#pragma once
#ifndef _TINYPBRT_SCENE_HPP_
#define _TINYPBRT_SCENE_HPP_

#include <tinypbrt/detail/cpp/fwd.hpp>

#include <tinypbrt/detail/scene.h>

#include <memory>
#include <string>
#include <tuple>
#include <utility>

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
	 * @brief C++ wrapper for tpbrt_scene_t (a non owning view, e.g. for a scene loaded with the C API).
	 * @details Every value returned from it is a view into the scene memory as well, so it is valid as long as the
	 * scene is alive. To load and own a scene use tpbrt::scene.
	 * @ingroup scene-cpp-public
	 */
	struct scene_view : detail::wrapper<scene_view, tpbrt_scene_t> {
		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns options value.
		[[nodiscard]] constexpr tpbrt::options options() const noexcept { return _native->options; }

		/// @brief Returns camera value.
		[[nodiscard]] constexpr tpbrt::camera camera() const noexcept { return _native->camera; }

		/// @brief Returns film value.
		[[nodiscard]] constexpr tpbrt::film film() const noexcept { return _native->film; }

		/// @brief Returns integrator value.
		[[nodiscard]] constexpr tpbrt::integrator integrator() const noexcept { return _native->integrator; }

		/// @brief Returns accelerator value.
		[[nodiscard]] constexpr tpbrt::accelerator accelerator() const noexcept { return _native->accelerator; }

		/// @brief Returns sampler value.
		[[nodiscard]] constexpr tpbrt::sampler sampler() const noexcept { return _native->sampler; }

		/// @brief Returns start_time value.
		[[nodiscard]] constexpr float_t start_time() const noexcept { return _native->start_time; }

		/// @brief Returns filter value.
		[[nodiscard]] constexpr tpbrt::filter filter() const noexcept { return _native->filter; }

		/// @brief Returns end_time value.
		[[nodiscard]] constexpr float_t end_time() const noexcept { return _native->end_time; }

		/// @brief Returns textures value.
		[[nodiscard]] constexpr tpbrt::textures_list textures() const noexcept { return _native->textures; }

		/// @brief Returns materials value.
		[[nodiscard]] constexpr tpbrt::materials_list materials() const noexcept { return _native->materials; }

		/// @brief Returns medias value.
		[[nodiscard]] constexpr tpbrt::medias_list medias() const noexcept { return _native->medias; }

		/// @brief Returns lights value.
		[[nodiscard]] constexpr tpbrt::lights_list lights() const noexcept { return _native->lights; }

		/// @brief Returns shapes value.
		[[nodiscard]] constexpr tpbrt::shapes_list shapes() const noexcept { return _native->shapes; }

		/// @brief Returns objects value.
		[[nodiscard]] constexpr tpbrt::objects_list objects() const noexcept { return _native->objects; }

		/// @brief Returns instances value.
		[[nodiscard]] constexpr tpbrt::objects_instances_list instances() const noexcept { return _native->instances; }

		/// @brief Returns transforms value.
		[[nodiscard]] constexpr tpbrt::transforms_list transforms() const noexcept { return _native->transforms; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr auto reflect() const noexcept {
			return std::make_tuple(options(), camera(), film(), integrator(), accelerator(), sampler(), start_time(), filter(),
			  end_time(), textures(), materials(), medias(), lights(), shapes(), objects(), instances(), transforms());
		}
	};

	/**
	 * @brief Scene loaded from a pbrt-v4 file or string, owning all its memory (move only, freed in the destructor).
	 * @details All accessors from tpbrt::scene_view return views into the scene memory: they stay valid while the scene
	 * lives, also after the scene object is moved (the C scene lives on the heap and does not move).
	 * @ingroup scene-cpp-public
	 */
	class scene : public scene_view {
		/// @brief Deleter calling tpbrt_scene_free.
		struct deleter {
			/// @brief Frees all scene resources and the scene itself.
			void operator()(tpbrt_scene_t* const c_scene) const noexcept {
				tpbrt_scene_free(c_scene);
				delete c_scene; // NOLINT(cppcoreguidelines-owning-memory)
			}
		};

		/// @brief Owned C scene.
		std::unique_ptr<tpbrt_scene_t, deleter> _owned;

		/// @brief Takes ownership of a loaded C scene.
		explicit scene(std::unique_ptr<tpbrt_scene_t, deleter> owned) noexcept : scene_view(*owned), _owned(std::move(owned)) {}

		/// @brief Loads scene with given C loader.
		template<class Loader>
		[[nodiscard]] static result<scene> _load(const Loader& loader) {
			auto owned = std::unique_ptr<tpbrt_scene_t, deleter>(new tpbrt_scene_t {}); // NOLINT
				// on error the C loader already freed everything it allocated, only the struct itself is left
				if (const auto err = static_cast<error>(loader(owned.get())); err != error::NONE) {
					delete owned.release(); // NOLINT(cppcoreguidelines-owning-memory)
					return err;
				}
			return scene(std::move(owned));
		}

	public:
		/// @brief Moves scene (views created from other stay valid, they point to the same heap memory).
		scene(scene&& other) noexcept			 = default;

		/// @brief Moves scene (the scene held before is freed).
		scene& operator=(scene&& other) noexcept = default;

		/// @brief Scene owns C memory, so it cannot be copied.
		scene(const scene&)						 = delete;

		/// @brief Scene owns C memory, so it cannot be copied.
		scene& operator=(const scene&)			 = delete;

		/// @brief Frees the scene.
		~scene()								 = default;

		/**
		 * @brief Loads scene from given file.
		 * @param filepath Path to scene file.
		 * @return Result with error code or with scene.
		 */
		[[nodiscard]] static result<scene> load_from_file(const std::string& filepath) {
			return _load([&](tpbrt_scene_t* const out) { return tpbrt_scene_load_from_file(filepath.c_str(), out); });
		}

		/**
		 * @brief Loads scene from memory (provided string).
		 * @param data Scene text (std::string is always null-terminated, so it is passed without a copy).
		 * @param working_directory Directory used to resolve Include / Import and other relative paths.
		 * @return Result with error code or with scene.
		 */
		[[nodiscard]] static result<scene> load_from_memory(const std::string& data, const std::string& working_directory = ".") {
			return _load([&](tpbrt_scene_t* const out) {
				return tpbrt_scene_load_from_memory(data.c_str(), working_directory.c_str(), out);
			});
		}

		/**
		 * @brief Loads scene from memory (provided null-terminated string).
		 * @param data Scene text (must end with '\0').
		 * @param working_directory Directory used to resolve Include / Import and other relative paths.
		 * @return Result with error code or with scene.
		 */
		[[nodiscard]] static result<scene> load_from_memory(const char_t* const data,
		  const std::string& working_directory = ".") {
				if (data == nullptr) { return error::INVALID_POINTER; }
			return _load([&](tpbrt_scene_t* const out) {
				return tpbrt_scene_load_from_memory(data, working_directory.c_str(), out);
			});
		}
	};

	/**
	 * @brief Loads scene from given file.
	 * @param filepath Path to scene file.
	 * @return Result with error code or with scene.
	 * @ingroup scene-cpp-public
	 */
	[[nodiscard]] inline result<scene> load_scene_from_file(const std::string& filepath) {
		return scene::load_from_file(filepath);
	}

	/**
	 * @brief Loads scene from memory (provided string).
	 * @param data Scene text.
	 * @param working_directory Directory used to resolve Include / Import and other relative paths.
	 * @return Result with error code or with scene.
	 * @ingroup scene-cpp-public
	 */
	[[nodiscard]] inline result<scene> load_scene_from_memory(const std::string& data,
	  const std::string& working_directory = ".") {
		return scene::load_from_memory(data, working_directory);
	}

	/**
	 * @brief Loads scene from memory (provided null-terminated string).
	 * @param data Scene text (must end with '\0').
	 * @param working_directory Directory used to resolve Include / Import and other relative paths.
	 * @return Result with error code or with scene.
	 * @ingroup scene-cpp-public
	 */
	[[nodiscard]] inline result<scene> load_scene_from_memory(const char_t* const data,
	  const std::string& working_directory = ".") {
		return scene::load_from_memory(data, working_directory);
	}
} // namespace tpbrt
#endif // _TINYPBRT_SCENE_HPP_
