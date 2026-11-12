// The C++ views show exactly the data of the C scene (same memory, same values).
#include <cpp_test_helpers.hpp>

#include <cstring>
#include <string>

namespace {
	struct c_scene {
		tpbrt_scene_t native {};
		tpbrt_error_t error = TPBRT_ERROR_NONE;

		explicit c_scene(const std::string& path) { error = tpbrt_scene_load_from_file(path.c_str(), &native); }

		c_scene(const c_scene&)			   = delete;
		c_scene& operator=(const c_scene&) = delete;

		~c_scene() {
				if (error == TPBRT_ERROR_NONE) { tpbrt_scene_free(&native); }
		}
	};

	class cpp_parity : public ::testing::TestWithParam<const char*> {};
} // namespace

INSTANTIATE_TEST_SUITE_P(example_scenes, cpp_parity, ::testing::Values("cornell_box.pbrt", "forest.pbrt"));

TEST_P(cpp_parity, same_values_as_the_c_api) {
	const std::string path = std::string(TINYPBRT_EXAMPLE_SCENES_DIR) + "/" + GetParam();
	const c_scene c(path);
	ASSERT_EQ(c.error, TPBRT_ERROR_NONE);
	const auto loaded = tpbrt::load_scene_from_file(path);
	ASSERT_TRUE(loaded) << tpbrt::to_string(loaded.get_error());
	const tpbrt::scene& s = *loaded;

	ASSERT_EQ(s.shapes().size(), c.native.shapes.count);
		for (tpbrt::size_t i = 0; i < c.native.shapes.count; ++i) {
			const tpbrt_shape_t& cs	 = c.native.shapes.shapes[i];
			const tpbrt::shape shape = s.shapes()[i];
			EXPECT_EQ(static_cast<int>(shape.type()), static_cast<int>(cs.type));
			EXPECT_EQ(shape.material().material_idx(), cs.material.material_idx);
			EXPECT_EQ(shape.transform().start_transform().transform_idx(), cs.transform.start_transform.transform_idx);
			EXPECT_EQ(shape.reverse_orientation(), cs.reverse_orientation == TPBRT_TRUE);
		}

	ASSERT_EQ(s.materials().size(), c.native.materials.count);
		for (tpbrt::size_t i = 0; i < c.native.materials.count; ++i) {
			EXPECT_EQ(s.materials()[i].name().to_string_view(),
			  std::string(c.native.materials.materials[i].name.data, c.native.materials.materials[i].name.size));
			EXPECT_EQ(static_cast<int>(s.materials()[i].type()), static_cast<int>(c.native.materials.materials[i].type));
		}

	ASSERT_EQ(s.transforms().size(), c.native.transforms.count);
		for (tpbrt::size_t i = 0; i < c.native.transforms.count; ++i) {
				for (tpbrt::size_t v = 0; v < tpbrt::mat4::ValuesCount; ++v) {
					EXPECT_FLOAT_EQ(s.transforms()[i].value_at(v), c.native.transforms.transforms[i].m[v]);
				}
		}

	EXPECT_EQ(s.film().x_resolution(), c.native.film.x_resolution);
	EXPECT_EQ(s.film().y_resolution(), c.native.film.y_resolution);
	EXPECT_EQ(s.film().file_name().to_string_view(), std::string(c.native.film.file_name.data, c.native.film.file_name.size));
	EXPECT_EQ(static_cast<int>(s.camera().type()), static_cast<int>(c.native.camera.type));
	EXPECT_EQ(s.lights().lights_sources().size(), c.native.lights.lights_sources.count);
	EXPECT_EQ(s.lights().area_lights().size(), c.native.lights.area_lights.count);
}
