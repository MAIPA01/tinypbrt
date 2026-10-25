// NOLINTBEGIN(readability-function-cognitive-complexity)
#pragma once
#ifndef _TINYPBRT_TESTS_TEST_HELPERS_HPP_
#define _TINYPBRT_TESTS_TEST_HELPERS_HPP_

#include <tinypbrt/tinypbrt.h>

#include <array>
#include <atomic>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

inline void print_to(const tpbrt_error_t err, std::ostream* const os) {
	char msg[128] = {};
	tpbrt_get_error_message(err, msg, sizeof(msg));
	*os << err << " (" << msg << ")";
}

namespace tpbrt_test {

	inline tpbrt_string_t str(const char* const s) {
		tpbrt_string_t out;
		out.data = s;
		out.size = std::strlen(s);
		return out;
	}

	inline tpbrt_spectrum_t constant_spectrum(const float value) {
		tpbrt_spectrum_t s = {};
		s.type			   = TPBRT_SPECTRUM_TYPE_CONSTANT;
		s.as.constant	   = value;
		return s;
	}

	inline tpbrt_texture_handle_t constant_handle(const float value) {
		tpbrt_texture_handle_t h = {};
		h.value_type			 = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_SPECTRUM;
		h.as.spectrum			 = constant_spectrum(value);
		return h;
	}

	inline std::string to_std(const tpbrt_string_t s) { return s.data == nullptr ? std::string() : std::string(s.data, s.size); }

	class Scene {
	public:
		explicit Scene(const std::string& source, const char* const working_directory = ".") : _source(source) {
			std::memset(&_scene, 0, sizeof(_scene));
			_err = tpbrt_scene_load_from_memory(_source.c_str(), working_directory, &_scene);
		}

		static Scene from_file(const std::string& path) {
			Scene s;
			s._err = tpbrt_scene_load_from_file(path.c_str(), &s._scene);
			return s;
		}

		Scene(const Scene&)			   = delete;
		Scene& operator=(const Scene&) = delete;

		Scene(Scene&& other) noexcept : _scene(other._scene), _err(other._err), _source(std::move(other._source)) {
			other._err = TPBRT_ERROR_INVALID_POINTER;
		}

		Scene& operator=(Scene&&) = delete;

		~Scene() {
				if (_err == TPBRT_ERROR_NONE) { tpbrt_scene_free(&_scene); }
		}

		tpbrt_error_t error() const { return _err; }

		bool ok() const { return _err == TPBRT_ERROR_NONE; }

		const tpbrt_scene_t& operator*() const { return _scene; }

		const tpbrt_scene_t* operator->() const { return &_scene; }

	private:
		Scene() : _err(TPBRT_ERROR_NONE) { std::memset(&_scene, 0, sizeof(_scene)); }

		tpbrt_scene_t _scene;
		tpbrt_error_t _err;
		std::string _source;
	};

	inline const tpbrt_mat4_t& matrix(const tpbrt_scene_t& scene, const tpbrt_transform_handle_t handle) {
		static tpbrt_mat4_t invalid;
		const tpbrt_mat4_t* m	= nullptr;
		const tpbrt_error_t err = tpbrt_get_transform(&scene.transforms, handle, &m);
		EXPECT_EQ(err, TPBRT_ERROR_NONE) << "invalid transform handle " << handle.transform_idx;
			if (err != TPBRT_ERROR_NONE || m == nullptr) {
				tpbrt_mat4_identity(&invalid);
				return invalid;
			}
		return *m;
	}

	inline std::array<float, 3> transform_point(const tpbrt_mat4_t& m, const float x, const float y, const float z) {
		std::array<float, 3> out {};
			for (int r = 0; r < 3; ++r) {
				out[r] = m.m[r * 4 + 0] * x + m.m[r * 4 + 1] * y + m.m[r * 4 + 2] * z + m.m[r * 4 + 3];
			}
		return out;
	}

	inline std::array<float, 3> origin(const tpbrt_scene_t& scene, const tpbrt_transform_handle_t handle) {
		return transform_point(matrix(scene, handle), 0.0f, 0.0f, 0.0f);
	}

	constexpr float EPS = 1e-4f;

#define TPBRT_EXPECT_VEC3_NEAR(actual, ex, ey, ez)              \
		do {                                                    \
			const std::array<float, 3> _v = (actual);           \
			EXPECT_NEAR(_v[0], (ex), ::tpbrt_test::EPS) << "x"; \
			EXPECT_NEAR(_v[1], (ey), ::tpbrt_test::EPS) << "y"; \
			EXPECT_NEAR(_v[2], (ez), ::tpbrt_test::EPS) << "z"; \
	} while (0)

	class Params {
	public:
		explicit Params(const std::string& params_text) : _text("Shape \"sphere\" " + params_text + "\n") {
			std::memset(&_element, 0, sizeof(_element));
			tpbrt_parser_t parser;
			_err = tpbrt_parser_init(&parser, str(_text.c_str()));
				if (_err == TPBRT_ERROR_NONE) { _err = tpbrt_parser_next(&parser, &_element); }
		}

		Params(const Params&)			 = delete;
		Params& operator=(const Params&) = delete;

		~Params() {
				if (_err == TPBRT_ERROR_NONE) { tpbrt_element_free(&_element); }
		}

		tpbrt_error_t error() const { return _err; }

		const tpbrt_params_list_t& list() const { return _element.as.generic_with_params.params; }

	private:
		std::string _text;
		tpbrt_element_t _element;
		tpbrt_error_t _err;
	};

	class TempDir {
	public:
		TempDir() {
			static std::atomic<unsigned> counter { 0 };
			const auto base = std::filesystem::temp_directory_path();
				for (;;) {
					_path = base / ("tinypbrt-tests-" + std::to_string(counter++) + "-" +
									 std::to_string(reinterpret_cast<std::uintptr_t>(this) & 0xFFFFu));
					std::error_code ec;
						if (std::filesystem::create_directories(_path, ec)) { break; }
				}
		}

		TempDir(const TempDir&)			   = delete;
		TempDir& operator=(const TempDir&) = delete;

		~TempDir() {
			std::error_code ec;
			std::filesystem::remove_all(_path, ec);
		}

		std::string write(const std::string& relative, const std::string& content) const {
			const auto p = _path / relative;
			std::filesystem::create_directories(p.parent_path());
			std::ofstream(p, std::ios::binary) << content;
			return p.string();
		}

		std::string path() const { return _path.string(); }

	private:
		std::filesystem::path _path;
	};

} // namespace tpbrt_test

#endif // _TINYPBRT_TESTS_TEST_HELPERS_HPP_
// NOLINTEND(readability-function-cognitive-complexity)
