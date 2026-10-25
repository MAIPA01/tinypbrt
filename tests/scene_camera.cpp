#include <pch.hpp>

#include <tinypbrt/detail/camera_internal.h>
#include <tinypbrt/detail/scene.h>

#include <test_helpers.hpp>

using namespace tpbrt_test;

namespace {
	void expect_screen_window(const tpbrt_camera_t& camera, const float x0, const float x1, const float y0, const float y1) {
		ASSERT_EQ(camera.type, TPBRT_CAMERA_TYPE_PERSPECTIVE);
		EXPECT_NEAR(camera.as.perspective.screen_window[0], x0, EPS);
		EXPECT_NEAR(camera.as.perspective.screen_window[1], x1, EPS);
		EXPECT_NEAR(camera.as.perspective.screen_window[2], y0, EPS);
		EXPECT_NEAR(camera.as.perspective.screen_window[3], y1, EPS);
	}
} // namespace

TEST(scene_camera, default_screen_window_landscape) {
	const Scene s("Film \"rgb\" \"integer xresolution\" 1280 \"integer yresolution\" 720\nCamera \"perspective\"\nWorldBegin\n");
	ASSERT_TRUE(s.ok()) << s.error();
	EXPECT_NEAR(s->camera.as.perspective.frame_aspect_ratio, 1280.0f / 720.0f, EPS);
	// NOLINTNEXTLINE(readability-magic-numbers)
	expect_screen_window(s->camera, -1280.0f / 720.0f, 1280.0f / 720.0f, -1.0f, 1.0f);
}

// Kamera powstaje w WorldBegin, więc Film podany po Camera też jest brany pod uwagę
TEST(scene_camera, camera_before_film) {
	/* clang-format off */
	const Scene s("Camera \"perspective\" \"float fov\" 45\nFilm \"rgb\" \"integer xresolution\" 400 \"integer yresolution\" 800\n"
			"WorldBegin\n");
	/* clang-format on */
	ASSERT_TRUE(s.ok()) << s.error();
	EXPECT_FLOAT_EQ(s->camera.as.perspective.fov, 45.0f);
	// NOLINTNEXTLINE(readability-magic-numbers)
	expect_screen_window(s->camera, -1.0f, 1.0f, -2.0f, 2.0f);
}

TEST(scene_camera, only_film) {
	const Scene s("Film \"rgb\" \"integer xresolution\" 400 \"integer yresolution\" 800\nWorldBegin\n");
	ASSERT_TRUE(s.ok()) << s.error();
	EXPECT_FLOAT_EQ(s->camera.as.perspective.fov, 90.0f);
	// NOLINTNEXTLINE(readability-magic-numbers)
	expect_screen_window(s->camera, -1.0f, 1.0f, -2.0f, 2.0f);
}

TEST(scene_camera, explicit_screen_window) {
	const Scene s("Camera \"perspective\" \"float screenwindow\" [ -2 2 -1 1 ]\nWorldBegin\n");
	ASSERT_TRUE(s.ok()) << s.error();
	// NOLINTNEXTLINE(readability-magic-numbers)
	expect_screen_window(s->camera, -2.0f, 2.0f, -1.0f, 1.0f);
}

TEST(scene_camera, file_without_world_begin) {
	const Scene s(
	  "Film \"rgb\" \"integer xresolution\" 400 \"integer yresolution\" 800\nCamera \"perspective\" \"float fov\" 30\n"
	);
	ASSERT_TRUE(s.ok()) << s.error();
	EXPECT_FLOAT_EQ(s->camera.as.perspective.fov, 30.0f);
	// NOLINTNEXTLINE(readability-magic-numbers)
	expect_screen_window(s->camera, -1.0f, 1.0f, -2.0f, 2.0f);
	TPBRT_EXPECT_VEC3_NEAR(origin(*s, s->camera.transform.start_transform), 0.0f, 0.0f, 0.0f);
}

TEST(scene_camera, last_camera_wins) {
	const Scene s("Camera \"perspective\" \"float fov\" 30\nCamera \"orthographic\"\nWorldBegin\n");
	ASSERT_TRUE(s.ok()) << s.error();
	EXPECT_EQ(s->camera.type, TPBRT_CAMERA_TYPE_ORTHOGRAPHIC);
}

TEST(scene_camera, default_camera_is_at_origin) {
	const Scene s("WorldBegin\nShape \"sphere\"\n");
	ASSERT_TRUE(s.ok()) << s.error();
	TPBRT_EXPECT_VEC3_NEAR(origin(*s, s->camera.transform.start_transform), 0.0f, 0.0f, 0.0f);
	TPBRT_EXPECT_VEC3_NEAR(origin(*s, s->camera.transform.end_transform), 0.0f, 0.0f, 0.0f);
}

TEST(scene_camera, camera_to_world_is_inverse_ctm) {
	const Scene s("LookAt 0 0 -5  0 0 0  0 1 0\nCamera \"perspective\"\nWorldBegin\n");
	ASSERT_TRUE(s.ok()) << s.error();
	TPBRT_EXPECT_VEC3_NEAR(origin(*s, s->camera.transform.start_transform), 0.0f, 0.0f, -5.0f);
}

TEST(scene_camera, transforms_after_camera_do_not_move_it) {
	const Scene s("Translate 0 0 5\nCamera \"perspective\"\nTranslate 100 0 0\nWorldBegin\n");
	ASSERT_TRUE(s.ok()) << s.error();
	TPBRT_EXPECT_VEC3_NEAR(origin(*s, s->camera.transform.start_transform), 0.0f, 0.0f, -5.0f);
}

TEST(scene_camera, animated_camera) {
	const Scene s("ActiveTransform EndTime\nTranslate 0 0 5\nActiveTransform All\nCamera \"perspective\"\nWorldBegin\n");
	ASSERT_TRUE(s.ok()) << s.error();
	TPBRT_EXPECT_VEC3_NEAR(origin(*s, s->camera.transform.start_transform), 0.0f, 0.0f, 0.0f);
	TPBRT_EXPECT_VEC3_NEAR(origin(*s, s->camera.transform.end_transform), 0.0f, 0.0f, -5.0f);
}

TEST(scene_camera, camera_medium) {
	const Scene s(
	  "MakeNamedMedium \"fog\" \"string type\" \"homogeneous\"\nMediumInterface \"fog\"\nCamera \"perspective\"\nWorldBegin\n"
	);
	ASSERT_TRUE(s.ok()) << s.error();
	ASSERT_EQ(s->medias.count, 1u);
	EXPECT_EQ(s->camera.interior_media_handle.media_idx, s->medias.medias[0].idx);
}

TEST(scene_camera, invalid_type) {
	EXPECT_EQ(Scene("Camera \"fisheye\"\nWorldBegin\n").error(), TPBRT_ERROR_INVALID_CAMERA_TYPE);
	EXPECT_EQ(Scene("Translate 1 0 0\nCamera \"perspective\"\nCamera \"fisheye\"\nWorldBegin\n").error(),
	  TPBRT_ERROR_INVALID_CAMERA_TYPE);
}

TEST(scene_camera, all_types) {
	EXPECT_EQ(Scene("Camera \"orthographic\"\nWorldBegin\n")->camera.type, TPBRT_CAMERA_TYPE_ORTHOGRAPHIC);
	EXPECT_EQ(Scene("Camera \"realistic\"\nWorldBegin\n")->camera.type, TPBRT_CAMERA_TYPE_REALISTIC);
	EXPECT_EQ(Scene("Camera \"spherical\"\nWorldBegin\n")->camera.type, TPBRT_CAMERA_TYPE_SPHERICAL);
}
