#include <pch.hpp>

#include <tinypbrt/detail/math_internal.h>

#include <test_helpers.hpp>

using namespace tpbrt_test;

namespace {
	tpbrt_vec3_t vec3(const float x, const float y, const float z) {
		tpbrt_vec3_t v;
		v.x = x;
		v.y = y;
		v.z = z;
		return v;
	}

	void expect_mat_near(const tpbrt_mat4_t& a, const tpbrt_mat4_t& b) {
			for (int i = 0; i < 16; ++i) { EXPECT_NEAR(a.m[i], b.m[i], EPS) << "element " << i; }
	}
} // namespace

TEST(math, identity) {
	tpbrt_mat4_t m;
	tpbrt_mat4_identity(&m);
		for (int r = 0; r < 4; ++r) {
				for (int c = 0; c < 4; ++c) { EXPECT_EQ(m.rows[r].v[c], r == c ? 1.0f : 0.0f); }
		}
}

TEST(math, translation_is_row_major) {
	tpbrt_mat4_t t;
	tpbrt_mat4_from_translation(&t, vec3(5.0f, 6.0f, 7.0f));
	EXPECT_EQ(t.m[3], 5.0f);
	EXPECT_EQ(t.m[7], 6.0f);
	EXPECT_EQ(t.m[11], 7.0f);
	TPBRT_EXPECT_VEC3_NEAR(transform_point(t, 1.0f, 1.0f, 1.0f), 6.0f, 7.0f, 8.0f);
}

TEST(math, multiply_applies_right_operand_first) {
	tpbrt_mat4_t translate, scale, ts;
	tpbrt_mat4_from_translation(&translate, vec3(5.0f, 0.0f, 0.0f));
	tpbrt_mat4_from_scale(&scale, vec3(2.0f, 2.0f, 2.0f));
	tpbrt_mat4_multiply(&ts, &translate, &scale);
	TPBRT_EXPECT_VEC3_NEAR(transform_point(ts, 1.0f, 0.0f, 0.0f), 7.0f, 0.0f, 0.0f);

	tpbrt_mat4_t st;
	tpbrt_mat4_multiply(&st, &scale, &translate);
	TPBRT_EXPECT_VEC3_NEAR(transform_point(st, 1.0f, 0.0f, 0.0f), 12.0f, 0.0f, 0.0f);
}

TEST(math, rotation_uses_degrees) {
	tpbrt_mat4_t r;
	tpbrt_mat4_from_axis_angle(&r, vec3(0.0f, 0.0f, 1.0f), 90.0f);
	TPBRT_EXPECT_VEC3_NEAR(transform_point(r, 1.0f, 0.0f, 0.0f), 0.0f, 1.0f, 0.0f);
}

TEST(math, inverse) {
	tpbrt_mat4_t a, b, ab, inv, product, identity;
	tpbrt_mat4_from_translation(&a, vec3(1.0f, -2.0f, 3.0f));
	tpbrt_mat4_from_axis_angle(&b, vec3(0.0f, 1.0f, 0.0f), 30.0f);
	tpbrt_mat4_multiply(&ab, &a, &b);
	tpbrt_mat4_inverse(&inv, &ab);
	tpbrt_mat4_multiply(&product, &ab, &inv);
	tpbrt_mat4_identity(&identity);
	expect_mat_near(product, identity);
}

TEST(math, transpose) {
	tpbrt_mat4_t m, t;
		for (int i = 0; i < 16; ++i) { m.m[i] = static_cast<float>(i); }
	tpbrt_mat4_transpose(&t, &m);
		for (int r = 0; r < 4; ++r) {
				for (int c = 0; c < 4; ++c) { EXPECT_EQ(t.m[r * 4 + c], m.m[c * 4 + r]); }
		}
}

TEST(math, equals_and_hash) {
	tpbrt_mat4_t a, b, c;
	tpbrt_mat4_from_translation(&a, vec3(1.0f, 2.0f, 3.0f));
	tpbrt_mat4_from_translation(&b, vec3(1.0f, 2.0f, 3.0f));
	tpbrt_mat4_from_translation(&c, vec3(1.0f, 2.0f, 3.5f));

	EXPECT_EQ(tpbrt_mat4_equals(&a, &b), TPBRT_TRUE);
	EXPECT_EQ(tpbrt_mat4_hash(&a), tpbrt_mat4_hash(&b));
	EXPECT_EQ(tpbrt_mat4_equals(&a, &c), TPBRT_FALSE);
	EXPECT_NE(tpbrt_mat4_hash(&a), tpbrt_mat4_hash(&c));

	EXPECT_EQ(tpbrt_mat4_equals(&a, nullptr), TPBRT_FALSE);
	EXPECT_EQ(tpbrt_mat4_hash(nullptr), 0u);
}

TEST(math, next_power_of_two) {
	EXPECT_EQ(tpbrt_next_power_of_two(0u), 1u);
	EXPECT_EQ(tpbrt_next_power_of_two(1u), 1u);
	EXPECT_EQ(tpbrt_next_power_of_two(3u), 4u);
	EXPECT_EQ(tpbrt_next_power_of_two(100u), 128u);
	EXPECT_EQ(tpbrt_next_power_of_two(128u), 128u);
	EXPECT_EQ(tpbrt_next_power_of_two(129u), 256u);
}
