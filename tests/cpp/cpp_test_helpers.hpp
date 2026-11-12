#pragma once
#ifndef _TINYPBRT_TESTS_CPP_TEST_HELPERS_HPP_
#define _TINYPBRT_TESTS_CPP_TEST_HELPERS_HPP_

#include <tinypbrt/tinypbrt.hpp>

#include <gtest/gtest.h>

#include <ostream>
#include <string>

namespace tpbrt {
	/// @brief Readable error codes in gtest messages.
	inline void print_to(const error err, std::ostream* const os) {
		*os << static_cast<int>(err) << " (" << to_string(err) << ")";
	}
} // namespace tpbrt

namespace tpbrt_cpp_test {
	/// @brief Loads scene from text and fails the current test (fatal) when it cannot be loaded.
	inline tpbrt::scene load(const std::string& text) {
		auto loaded = tpbrt::load_scene_from_memory(text);
			if (!loaded) {
				ADD_FAILURE() << "scene failed to load: " << tpbrt::to_string(loaded.get_error()) << "\n" << text;
				// an empty but valid scene, so the caller can continue without crashing
				auto empty = tpbrt::load_scene_from_memory("WorldBegin\n");
				return std::move(*empty);
			}
		return std::move(*loaded);
	}

	/// @brief Error returned when loading given scene text.
	inline tpbrt::error load_error(const std::string& text) { return tpbrt::load_scene_from_memory(text).get_error(); }
} // namespace tpbrt_cpp_test

#endif // _TINYPBRT_TESTS_CPP_TEST_HELPERS_HPP_
