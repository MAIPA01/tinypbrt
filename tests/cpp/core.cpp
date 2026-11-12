// Core building blocks of the C++ wrapper: wrapper views, string, array, result.
#include <cpp_test_helpers.hpp>

#include <algorithm>
#include <concepts>
#include <iterator>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

using namespace tpbrt_cpp_test;

// views are regular, pointer sized values
static_assert(std::copyable<tpbrt::shape>);
static_assert(std::copyable<tpbrt::material>);
static_assert(std::copyable<tpbrt::spectrum>);
static_assert(sizeof(tpbrt::shape) == sizeof(void*));
static_assert(sizeof(tpbrt::shapes_list) == sizeof(void*));
static_assert(!std::is_default_constructible_v<tpbrt::shape>, "a view must always point to a C struct");

// a view cannot be created from a temporary C struct (it would dangle)
static_assert(!std::is_constructible_v<tpbrt::string, tpbrt_string_t&&>);
static_assert(std::is_constructible_v<tpbrt::string, const tpbrt_string_t&>);

// arrays are standard ranges whose iterators outlive the array object
static_assert(std::ranges::random_access_range<tpbrt::shapes_list>);
static_assert(std::ranges::sized_range<tpbrt::shapes_list>);
static_assert(std::ranges::borrowed_range<tpbrt::shapes_list>);
static_assert(std::ranges::borrowed_range<tpbrt::materials_list>);
static_assert(std::ranges::random_access_range<tpbrt::float_array>);
static_assert(std::ranges::contiguous_range<tpbrt::float_array>, "identity arrays iterate the C data directly");
static_assert(std::random_access_iterator<tpbrt::shapes_list::iterator>);
static_assert(std::same_as<std::ranges::range_value_t<tpbrt::shapes_list>, tpbrt::shape>);

// the scene owns the C memory
static_assert(!std::copyable<tpbrt::scene>);
static_assert(std::movable<tpbrt::scene>);
static_assert(std::derived_from<tpbrt::scene, tpbrt::scene_view>);

// every variant alternative is a distinct type, so std::visit can tell them apart
static_assert(!std::same_as<tpbrt::texture_fbm_params, tpbrt::texture_wrinkled_params>);
static_assert(!std::same_as<tpbrt::texture_fbm_params, tpbrt::texture_windy_params>);
static_assert(!std::same_as<tpbrt::texture_mapping_spherical_params, tpbrt::texture_mapping_cylindrical_params>);
static_assert(!std::same_as<tpbrt::blackbody, tpbrt::float_t>);

TEST(cpp_string, compares_characters_not_pointers) {
	constexpr char a_text[] = "glass";
	constexpr char b_text[] = "glass";
	const tpbrt_string_t a_native { .data = a_text, .size = 5 };
	const tpbrt_string_t b_native { .data = b_text, .size = 5 };
	const tpbrt_string_t c_native { .data = "gold", .size = 4 };
	const tpbrt::string a = a_native;
	const tpbrt::string b = b_native;
	const tpbrt::string c = c_native;

	EXPECT_EQ(a, b);
	EXPECT_NE(a, c);
	EXPECT_TRUE(a < c); // "glass" < "gold"
	EXPECT_EQ(a, std::string_view("glass"));
	EXPECT_EQ(a.to_string_view(), "glass");
	EXPECT_EQ(std::string(a.begin(), a.end()), "glass");
	EXPECT_EQ(a.size(), 5u);
	EXPECT_FALSE(a.empty());

	const tpbrt_string_t empty_native { .data = nullptr, .size = 0 };
	const tpbrt::string empty = empty_native;
	EXPECT_TRUE(empty.empty());
	EXPECT_EQ(empty.to_string_view(), std::string_view());
}

TEST(cpp_array, indexing_iteration_and_bounds) {
	const tpbrt::
	  scene
		s = load("WorldBegin\nShape \"sphere\" \"float radius\" 1\nShape \"sphere\" \"float radius\" 2\n" "Shape \"sphere\" "
																										  "\"float " "radiu" "s\""
																															 " " "3\n");
	const tpbrt::shapes_list shapes = s.shapes();
	ASSERT_EQ(shapes.size(), 3u);
	EXPECT_FALSE(shapes.empty());
	EXPECT_EQ(shapes.data(), s.native().shapes.shapes);

	EXPECT_FLOAT_EQ(shapes[0].as_sphere()->radius(), 1.0f);
	EXPECT_FLOAT_EQ(shapes.front().as_sphere()->radius(), 1.0f);
	EXPECT_FLOAT_EQ(shapes.back().as_sphere()->radius(), 3.0f);
	ASSERT_TRUE(shapes.at(2).has_value());
	EXPECT_FLOAT_EQ(shapes.at(2)->as_sphere()->radius(), 3.0f);
	EXPECT_EQ(shapes.at(3).get_error(), tpbrt::error::INDEX_OUT_OF_RANGE);

	std::vector<float> radii;
		for (const tpbrt::shape shape : shapes) { radii.push_back(shape.as_sphere()->radius()); }
	const std::vector<float> expected_radii { 1.0f, 2.0f, 3.0f };
	EXPECT_EQ(radii, expected_radii);

	// iterators work with the standard algorithms, also when the array object is a temporary (borrowed range)
	const auto it = std::ranges::find_if(s.shapes(), [](const tpbrt::shape shape) { return shape.as_sphere()->radius() > 1.5f; });
	ASSERT_NE(it, s.shapes().end());
	EXPECT_FLOAT_EQ((*it).as_sphere()->radius(), 2.0f);
	EXPECT_EQ(std::ranges::distance(s.shapes()), 3);
	EXPECT_EQ(s.shapes().end() - s.shapes().begin(), 3);

	auto radii_view = s.shapes() | std::views::transform([](const tpbrt::shape shape) { return shape.as_sphere()->radius(); });
	EXPECT_FLOAT_EQ(*std::ranges::max_element(radii_view), 3.0f);
}

TEST(cpp_array, compares_elements) {
	const tpbrt::scene a = load("WorldBegin\nShape \"sphere\" \"float radius\" 1\n");
	const tpbrt::scene b = load("WorldBegin\nShape \"sphere\" \"float radius\" 1\n");
	const tpbrt::scene c = load("WorldBegin\nShape \"sphere\" \"float radius\" 2\n");
	EXPECT_EQ(a.shapes(), b.shapes());
	EXPECT_NE(a.shapes(), c.shapes());
	EXPECT_TRUE(a.shapes() < c.shapes());
	EXPECT_EQ(a.shapes()[0], b.shapes()[0]);
}

TEST(cpp_array, fixed_size_c_arrays) {
	const tpbrt::scene s = load("Camera \"perspective\" \"float screenwindow\" [ -2 2 -1 1 ]\nWorldBegin\n");
	const auto window	 = s.camera().as_perspective()->screen_window();
	static_assert(decltype(window)::Extent == 4);
	ASSERT_EQ(window.size(), 4u);
	EXPECT_FLOAT_EQ(window[0], -2.0f);
	EXPECT_FLOAT_EQ(window[3], 1.0f);
}

TEST(cpp_result, value_and_error) {
	const tpbrt::result<int> ok = 5;
	EXPECT_TRUE(ok.has_value());
	EXPECT_TRUE(static_cast<bool>(ok));
	EXPECT_EQ(*ok, 5);
	EXPECT_EQ(ok.get_value(), 5);
	EXPECT_EQ(ok.get_error(), tpbrt::error::NONE);
	EXPECT_EQ(ok.value_or(7), 5);

	const tpbrt::result<int> failed = tpbrt::error::NOT_FOUND;
	EXPECT_FALSE(failed.has_value());
	EXPECT_FALSE(static_cast<bool>(failed));
	EXPECT_EQ(failed.get_error(), tpbrt::error::NOT_FOUND);
	EXPECT_EQ(failed, tpbrt::error::NOT_FOUND);
	EXPECT_EQ(failed.value_or(7), 7);
	EXPECT_THROW((void)failed.get_value(), std::bad_variant_access);

	const auto described = failed.visit([](const auto& v) -> std::string {
		if constexpr (std::is_same_v<std::decay_t<decltype(v)>, tpbrt::error>) { return "error"; }
		else { return "value"; }
	});
	EXPECT_EQ(described, "error");
}

TEST(cpp_error, messages) {
	EXPECT_EQ(tpbrt::to_string(tpbrt::error::NONE), "No error");
	EXPECT_EQ(tpbrt::to_string(tpbrt::error::PARAM_TYPE_MISMATCH),
	  "Parameter is declared with a different type than expected (e.g. \"string radius\" instead of \"float radius\")");
		for (int code = 0; code < static_cast<int>(tpbrt::error::MAX_NUM); ++code) {
			EXPECT_FALSE(tpbrt::to_string(static_cast<tpbrt::error>(code)).empty()) << code;
		}
	// C and C++ error codes are the same values
	EXPECT_EQ(static_cast<int>(tpbrt::error::INVALID_SCATTERING_BUILTIN), TPBRT_ERROR_INVALID_SCATTERING_BUILTIN);
	EXPECT_EQ(static_cast<int>(tpbrt::error::MAX_NUM), TPBRT_ERROR_MAX_NUM);
}
