// NOLINTBEGIN(readability-function-cognitive-complexity)
#include <pch.hpp>

#include <test_helpers.hpp>

#include <clocale>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <random>
#include <string>

using namespace tpbrt_test;

namespace {
	bool read_float(const std::string& text, float& out) { return tpbrt_read_float_value(str(text.c_str()), &out) == TPBRT_TRUE; }

	bool read_int(const std::string& text, tpbrt_int_t& out) {
		return tpbrt_read_int_value(str(text.c_str()), &out) == TPBRT_TRUE;
	}

	bool read_uint(const std::string& text, tpbrt_uint_t& out) {
		return tpbrt_read_uint_value(str(text.c_str()), &out) == TPBRT_TRUE;
	}
} // namespace

TEST(numbers, float_forms) {
	float v = 0.0f;
	ASSERT_TRUE(read_float("1.5", v));
	EXPECT_EQ(v, 1.5f);
	ASSERT_TRUE(read_float("-0.25e-2", v));
	EXPECT_EQ(v, -0.0025f);
	ASSERT_TRUE(read_float(".5", v));
	EXPECT_EQ(v, 0.5f);
	ASSERT_TRUE(read_float("5.", v));
	EXPECT_EQ(v, 5.0f);
	ASSERT_TRUE(read_float("+3", v));
	EXPECT_EQ(v, 3.0f);
	ASSERT_TRUE(read_float("1E3", v));
	EXPECT_EQ(v, 1000.0f);
	ASSERT_TRUE(read_float("0", v));
	EXPECT_EQ(v, 0.0f);
	ASSERT_TRUE(read_float("000123.4500", v));
	EXPECT_EQ(v, 123.45f);
	ASSERT_TRUE(read_float("3.14159265358979323846264338327950288", v));
	EXPECT_EQ(v, 3.14159265358979323846f);
	ASSERT_TRUE(read_float("1e-50", v));
	EXPECT_EQ(v, 0.0f);
	ASSERT_TRUE(read_float("123456789012345678901234567890", v));
	EXPECT_FLOAT_EQ(v, 1.23456789e29f);
}

TEST(numbers, float_rejects_non_numbers) {
	float v = 0.0f;
		for (const char* bad :
		  { "", "-", "+", ".", "e5", "1e", "1e+", "1.5abc", "1,5", "0x10", "inf", "nan", "1.2.3", "--1", "1e40" }) {
			EXPECT_FALSE(read_float(bad, v)) << '"' << bad << '"';
		}
}

TEST(numbers, float_matches_strtof) {
	// NOLINTBEGIN(readability-magic-numbers)
	std::mt19937 rng(12345u);
	std::uniform_real_distribution<float> unit(-1.0f, 1.0f);
	std::uniform_int_distribution<int> exponent(-30, 30);
	int mismatches = 0;
		for (int i = 0; i < 20000; ++i) {
			const float x = unit(rng) * std::pow(10.0f, static_cast<float>(exponent(rng)));
			char text[64];
			std::snprintf(text, sizeof(text), i % 2 == 0 ? "%.9g" : "%.6f", static_cast<double>(x));
			float ours = 0.0f;
			ASSERT_TRUE(read_float(text, ours)) << text;
			const float expected = std::strtof(text, nullptr);
				if (std::memcmp(&ours, &expected, sizeof(float)) != 0) { ++mismatches; }
		}
	EXPECT_EQ(mismatches, 0);
	// NOLINTEND(readability-magic-numbers)
}

TEST(numbers, float_does_not_depend_on_locale) {
	const char* const previous = std::setlocale(LC_NUMERIC, nullptr);
	const std::string saved	   = previous != nullptr ? previous : "C";
		if (std::setlocale(LC_NUMERIC, "de_DE.UTF-8") == nullptr && std::setlocale(LC_NUMERIC, "pl_PL.UTF-8") == nullptr) {
			GTEST_SKIP() << "no locale with a decimal comma installed";
		}
	float v = 0.0f;
	EXPECT_TRUE(read_float("0.5", v));
	EXPECT_EQ(v, 0.5f);
	const Scene s("WorldBegin\nShape \"sphere\" \"float radius\" 0.25\n");
	std::setlocale(LC_NUMERIC, saved.c_str());
	ASSERT_TRUE(s.ok()) << s.error();
	EXPECT_EQ(s->shapes.shapes[0].as.sphere.radius, 0.25f);
}

TEST(numbers, integers) {
	tpbrt_int_t i = 0;
	ASSERT_TRUE(read_int("-2147483648", i));
	EXPECT_EQ(i, INT32_MIN);
	ASSERT_TRUE(read_int("2147483647", i));
	EXPECT_EQ(i, INT32_MAX);
	ASSERT_TRUE(read_int("+12", i));
	EXPECT_EQ(i, 12);
		for (const char* bad : { "", "-", "2147483648", "-2147483649", "2.0", "1e3", "12abc", "99999999999999999999999" }) {
			EXPECT_FALSE(read_int(bad, i)) << '"' << bad << '"';
		}

	tpbrt_uint_t u = 0u;
	ASSERT_TRUE(read_uint("4294967295", u));
	EXPECT_EQ(u, UINT32_MAX);
		for (const char* bad : { "", "-1", "4294967296", "1.5", "+" }) { EXPECT_FALSE(read_uint(bad, u)) << '"' << bad << '"'; }
}

TEST(numbers, scene_errors) {
	EXPECT_EQ(Scene("WorldBegin\nShape \"sphere\" \"float radius\" 1.5abc\n").error(), TPBRT_ERROR_PARSE_FLOAT);
	EXPECT_EQ(Scene("Film \"rgb\" \"integer xresolution\" 640.5\nWorldBegin\n").error(), TPBRT_ERROR_PARSE_UINT);
}
// NOLINTEND(readability-function-cognitive-complexity)
