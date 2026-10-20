#include <pch.hpp>

#include <tinypbrt/detail/options_internal.h>
#include <tinypbrt/detail/scene.h>

#include <test_helpers.hpp>

using namespace tpbrt_test;

TEST(scene_options, sampler_pixelsamples_rounded_for_sobol) {
	const Scene s("Sampler \"zsobol\" \"integer pixelsamples\" 100\nWorldBegin\n");
	ASSERT_TRUE(s.ok()) << s.error();
	EXPECT_EQ(s->sampler.as.z_sobol.pixel_samples, 128u);
}

TEST(scene_options, sampler_types) {
	EXPECT_EQ(Scene("Sampler \"independent\"\nWorldBegin\n")->sampler.type, TPBRT_SAMPLER_TYPE_INDEPENDENT);
	EXPECT_EQ(Scene("Sampler \"stratified\"\nWorldBegin\n")->sampler.type, TPBRT_SAMPLER_TYPE_STRATIFIED);
	EXPECT_EQ(Scene("Sampler \"sobol\"\nWorldBegin\n")->sampler.type, TPBRT_SAMPLER_TYPE_SOBOL);
	EXPECT_EQ(Scene("Sampler \"paddedsobol\"\nWorldBegin\n")->sampler.type, TPBRT_SAMPLER_TYPE_PADDED_SOBOL);
	EXPECT_EQ(Scene("Sampler \"lowdiscrepancy\"\nWorldBegin\n").error(), TPBRT_ERROR_INVALID_SAMPLER_TYPE);
}

TEST(scene_options, integrators) {
	{
		const Scene s("Integrator \"volpath\" \"string lightsampler\" \"power\" \"integer maxdepth\" 7\nWorldBegin\n");
		ASSERT_TRUE(s.ok()) << s.error();
		EXPECT_EQ(s->integrator.as.vol_path.light_sampler, TPBRT_INTEGRATOR_LIGHT_SAMPLER_POWER);
		EXPECT_EQ(s->integrator.as.vol_path.max_depth, 7u);
	}
	{
		const Scene s("Integrator \"mlt\"\nWorldBegin\n");
		ASSERT_TRUE(s.ok()) << s.error();
		EXPECT_EQ(s->integrator.type, TPBRT_INTEGRATOR_TYPE_MLT);
		EXPECT_FLOAT_EQ(s->integrator.as.mlt.large_step_probability, 0.3f);
	}
	EXPECT_EQ(Scene("Integrator \"whitted\"\nWorldBegin\n").error(), TPBRT_ERROR_INVALID_INTEGRATOR_TYPE);
	EXPECT_EQ(Scene("Integrator \"path\" \"string lightsampler\" \"random\"\nWorldBegin\n").error(),
	  TPBRT_ERROR_INVALID_INTEGRATOR_LIGHT_SAMPLER);
}

TEST(scene_options, negative_value_for_unsigned_param) {
	EXPECT_EQ(Scene("Integrator \"path\" \"integer maxdepth\" -1\nWorldBegin\n").error(), TPBRT_ERROR_PARSE_UINT);
}

TEST(scene_options, film) {
	const Scene s(
	  "Film \"gbuffer\" \"integer xresolution\" 64 \"integer yresolution\" 32 \"string filename\" \"g.exr\"\nWorldBegin\n"
	);
	ASSERT_TRUE(s.ok()) << s.error();
	EXPECT_EQ(s->film.type, TPBRT_FILM_TYPE_GBUFFER);
	EXPECT_EQ(s->film.x_resolution, 64u);
	EXPECT_EQ(to_std(s->film.file_name), "g.exr");
	EXPECT_EQ(Scene("Film \"image\"\nWorldBegin\n").error(), TPBRT_ERROR_INVALID_FILM_TYPE);
}

TEST(scene_options, accelerator) {
	const Scene s("Accelerator \"kdtree\" \"integer maxprims\" 3\nWorldBegin\n");
	ASSERT_TRUE(s.ok()) << s.error();
	EXPECT_EQ(s->accelerator.type, TPBRT_ACCELERATOR_TYPE_KD_TREE);
	EXPECT_EQ(s->accelerator.as.kd_tree.max_prims, 3u);
	EXPECT_EQ(Scene("Accelerator \"grid\"\nWorldBegin\n").error(), TPBRT_ERROR_INVALID_ACCELERATOR_TYPE);
}

TEST(scene_options, pixel_filter) {
	EXPECT_TRUE(Scene("PixelFilter \"gaussian\" \"float sigma\" 0.5\nWorldBegin\n").ok());
	EXPECT_EQ(Scene("PixelFilter \"lanczos\"\nWorldBegin\n").error(), TPBRT_ERROR_INVALID_FILTER_TYPE);
}

TEST(scene_options, option_directive) {
	EXPECT_TRUE(Scene("Option \"bool disablepixeljitter\" true\nWorldBegin\n").ok());
	EXPECT_NE(Scene("Option \"bool nosuchoption\" true\nWorldBegin\n").error(), TPBRT_ERROR_NONE);
}
