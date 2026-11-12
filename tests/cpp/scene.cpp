// Loading scenes with the C++ API: errors, ownership, scene_view over a C scene.
#include <cpp_test_helpers.hpp>

#include <cstring>
#include <filesystem>
#include <fstream>
#include <string>
#include <utility>

using namespace tpbrt_cpp_test;

TEST(cpp_scene, load_from_memory) {
	const auto loaded = tpbrt::load_scene_from_memory("Film \"rgb\" \"integer xresolution\" 320\nWorldBegin\nShape \"sphere\"\n");
	ASSERT_TRUE(loaded.has_value()) << tpbrt::to_string(loaded.get_error());
	EXPECT_EQ(loaded->film().x_resolution(), 320u);
	EXPECT_EQ(loaded->shapes().size(), 1u);
	EXPECT_EQ(loaded->shapes()[0].type(), tpbrt::shape_type::SPHERE);
}

TEST(cpp_scene, load_from_c_string_and_static_functions) {
	const char* const text = "WorldBegin\nShape \"disk\"\n";
	const auto a		   = tpbrt::load_scene_from_memory(text);
	const auto b		   = tpbrt::scene::load_from_memory(std::string(text), ".");
	ASSERT_TRUE(a && b);
	EXPECT_EQ(a->shapes()[0].type(), tpbrt::shape_type::DISK);
	EXPECT_EQ(a->shapes(), b->shapes());
	EXPECT_EQ(tpbrt::load_scene_from_memory(static_cast<const char*>(nullptr)).get_error(), tpbrt::error::INVALID_POINTER);
}

TEST(cpp_scene, errors_are_returned_not_thrown) {
	EXPECT_EQ(load_error("WorldBegin\nShape \"sphere\" \"string radius\" \"2\"\n"), tpbrt::error::PARAM_TYPE_MISMATCH);
	EXPECT_EQ(load_error("WorldBegin\nShape \"cube\"\n"), tpbrt::error::INVALID_SHAPE_TYPE);
	EXPECT_EQ(load_error("WorldBegin\nWorldBegin\n"), tpbrt::error::WORLD_ALREADY_STARTED);
	EXPECT_EQ(tpbrt::load_scene_from_file("this/file/does/not/exist.pbrt").get_error(), tpbrt::error::FILE_NOT_FOUND);
}

TEST(cpp_scene, load_from_file_with_include) {
	const auto dir = std::filesystem::temp_directory_path() / "tinypbrt_cpp_scene_test";
	std::filesystem::create_directories(dir);
	std::ofstream(dir / "geometry.pbrt") << "Shape \"sphere\" \"float radius\" 4\n";
	std::ofstream(dir / "main.pbrt") << "WorldBegin\nInclude \"geometry.pbrt\"\n";

	const auto loaded = tpbrt::load_scene_from_file((dir / "main.pbrt").string());
	ASSERT_TRUE(loaded) << tpbrt::to_string(loaded.get_error());
	ASSERT_EQ(loaded->shapes().size(), 1u);
	EXPECT_FLOAT_EQ(loaded->shapes()[0].as_sphere()->radius(), 4.0f);

	// load_from_memory resolves Include relative to the working directory
	const auto from_memory = tpbrt::load_scene_from_memory("WorldBegin\nInclude \"geometry.pbrt\"\n", dir.string());
	ASSERT_TRUE(from_memory) << tpbrt::to_string(from_memory.get_error());
	EXPECT_EQ(from_memory->shapes(), loaded->shapes());
	std::filesystem::remove_all(dir);
}

TEST(cpp_scene, views_survive_moving_the_scene) {
	tpbrt::scene first				  = load("WorldBegin\nShape \"sphere\" \"float radius\" 7\n");
	const tpbrt::shapes_list shapes	  = first.shapes();
	const tpbrt_scene_t* const native = &first.native();

	tpbrt::scene second				  = std::move(first);
	EXPECT_EQ(&second.native(), native);
	EXPECT_FLOAT_EQ(shapes[0].as_sphere()->radius(), 7.0f);

	tpbrt::scene third = load("WorldBegin\n");
	third			   = std::move(second); // the empty scene held by third is freed (checked by the sanitizers)
	EXPECT_FLOAT_EQ(third.shapes()[0].as_sphere()->radius(), 7.0f);
}

TEST(cpp_scene, scene_view_over_a_c_scene) {
	tpbrt_scene_t c_scene;
	std::memset(&c_scene, 0, sizeof(c_scene));
	ASSERT_EQ(tpbrt_scene_load_from_memory("WorldBegin\nMakeNamedMaterial \"m\" \"string type\" \"diffuse\"\n", ".", &c_scene),
	  TPBRT_ERROR_NONE);
	{
		const tpbrt::scene_view view = c_scene;
		ASSERT_EQ(view.materials().size(), 1u);
		EXPECT_EQ(view.materials()[0].name(), "m");
		EXPECT_EQ(&view.native(), &c_scene);
	}
	tpbrt_scene_free(&c_scene);
}

TEST(cpp_scene, value_comparison) {
	const tpbrt::scene a	   = load("WorldBegin\nMaterial \"conductor\"\nShape \"sphere\"\n");
	const tpbrt::scene b	   = load("WorldBegin\nMaterial \"conductor\"\nShape \"sphere\"\n");
	const tpbrt::scene c	   = load("WorldBegin\nMaterial \"diffuse\"\nShape \"sphere\"\n");
	const tpbrt::scene_view va = a;
	const tpbrt::scene_view vb = b;
	const tpbrt::scene_view vc = c;
	EXPECT_EQ(va, vb);
	EXPECT_NE(va, vc);
	EXPECT_EQ(va.materials()[0], vb.materials()[0]);
	EXPECT_NE(va.materials()[0], vc.materials()[0]);
}
