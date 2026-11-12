// Every C++ enum has the same values as the C enum and to_string() returns the name the C parser accepts.
#include <cpp_test_helpers.hpp>

extern "C" {
#include <tinypbrt/detail/accelerator_internal.h>
#include <tinypbrt/detail/camera_internal.h>
#include <tinypbrt/detail/color_internal.h>
#include <tinypbrt/detail/coord_internal.h>
#include <tinypbrt/detail/film_internal.h>
#include <tinypbrt/detail/filter_internal.h>
#include <tinypbrt/detail/integrator_internal.h>
#include <tinypbrt/detail/light_internal.h>
#include <tinypbrt/detail/material_internal.h>
#include <tinypbrt/detail/media_internal.h>
#include <tinypbrt/detail/sampler_internal.h>
#include <tinypbrt/detail/scattering_internal.h>
#include <tinypbrt/detail/shape_internal.h>
#include <tinypbrt/detail/texture_internal.h>
}

#include <string_view>

namespace {
	template<class CppEnum, class CEnum, class Parse>
	void expect_round_trip(const Parse parse) {
			for (int i = 0; i < static_cast<int>(CppEnum::MAX_NUM); ++i) {
				const auto value			= static_cast<CppEnum>(i);
				const std::string_view name = tpbrt::to_string(value);
				CEnum parsed {};
				EXPECT_EQ(parse(tpbrt_string_t { .data = name.data(), .size = name.size() }, &parsed), TPBRT_ERROR_NONE) << name;
				EXPECT_EQ(static_cast<int>(parsed), i) << name;
			}
		EXPECT_EQ(tpbrt::to_string(CppEnum::MAX_NUM), "maxnum");
	}
} // namespace

#define ROUND_TRIP(CPP, C, PARSE)                                                                             \
	TEST(cpp_enums, CPP) {                                                                                    \
		expect_round_trip<tpbrt::CPP, C>([](const tpbrt_string_t s, C* const out) { return PARSE(s, out); }); \
	}

ROUND_TRIP(accelerator_type, tpbrt_accelerator_type_t, tpbrt_accelerator_type_from_string)
ROUND_TRIP(accelerator_bvh_split_method, tpbrt_accelerator_bvh_split_method_t, tpbrt_accelerator_bvh_split_method_from_string)
ROUND_TRIP(camera_type, tpbrt_camera_type_t, tpbrt_camera_type_from_string)
ROUND_TRIP(camera_spherical_mapping, tpbrt_camera_spherical_mapping_t, tpbrt_camera_spherical_mapping_from_string)
ROUND_TRIP(camera_realistic_aperture_builtin, tpbrt_camera_realistic_aperture_builtin_t,
  tpbrt_camera_realistic_aperture_builtin_from_string)
ROUND_TRIP(color_space, tpbrt_color_space_t, tpbrt_color_space_from_string)
ROUND_TRIP(spectrum_builtin, tpbrt_spectrum_builtin_t, tpbrt_spectrum_builtin_from_string)
ROUND_TRIP(coord_system_type, tpbrt_coord_system_type_t, tpbrt_coord_system_type_from_string)
ROUND_TRIP(film_type, tpbrt_film_type_t, tpbrt_film_type_from_string)
ROUND_TRIP(film_sensor, tpbrt_film_sensor_t, tpbrt_film_sensor_from_string)
ROUND_TRIP(filter_type, tpbrt_filter_type_t, tpbrt_filter_type_from_string)
ROUND_TRIP(integrator_type, tpbrt_integrator_type_t, tpbrt_integrator_type_from_string)
ROUND_TRIP(integrator_light_sampler, tpbrt_integrator_light_sampler_t, tpbrt_integrator_light_sampler_from_string)
ROUND_TRIP(light_source_type, tpbrt_light_source_type_t, tpbrt_light_source_type_from_string)
ROUND_TRIP(area_light_type, tpbrt_area_light_type_t, tpbrt_area_light_type_from_string)
ROUND_TRIP(material_type, tpbrt_material_type_t, tpbrt_material_type_from_string)
ROUND_TRIP(media_type, tpbrt_media_type_t, tpbrt_media_type_from_string)
ROUND_TRIP(sampler_type, tpbrt_sampler_type_t, tpbrt_sampler_type_from_string)
ROUND_TRIP(sampler_random, tpbrt_sampler_random_t, tpbrt_sampler_random_from_string)
ROUND_TRIP(scattering_builtin, tpbrt_scattering_builtin_t, tpbrt_scattering_builtin_from_string)
ROUND_TRIP(shape_type, tpbrt_shape_type_t, tpbrt_shape_type_from_string)
ROUND_TRIP(shape_curve_basis, tpbrt_shape_curve_basis_t, tpbrt_shape_curve_basis_from_string)
ROUND_TRIP(shape_curve_type, tpbrt_shape_curve_type_t, tpbrt_shape_curve_type_from_string)
ROUND_TRIP(texture_type, tpbrt_texture_type_t, tpbrt_texture_type_from_string)
ROUND_TRIP(texture_class, tpbrt_texture_class_t, tpbrt_texture_class_from_string)
ROUND_TRIP(texture_wrap, tpbrt_texture_wrap_t, tpbrt_texture_wrap_from_string)
ROUND_TRIP(texture_filter, tpbrt_texture_filter_t, tpbrt_texture_filter_from_string)
ROUND_TRIP(texture_mapping, tpbrt_texture_mapping_t, tpbrt_texture_mapping_from_string)

TEST(cpp_enums, unknown_values) {
	EXPECT_EQ(tpbrt::to_string(static_cast<tpbrt::shape_type>(200)), "UNKNOWN");
	EXPECT_EQ(tpbrt::to_string(tpbrt::texture_encoding::SRGB), "sRGB");
	EXPECT_EQ(tpbrt::to_string(tpbrt::shape_curve_degree::DEGREE_3), "3");
}
