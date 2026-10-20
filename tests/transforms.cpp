#include <pch.hpp>

#include <tinypbrt/detail/transform_internal.h>

#include <test_helpers.hpp>

using namespace tpbrt_test;

namespace {
	tpbrt_mat4_t translation(const float x, const float y, const float z) {
		tpbrt_vec3_t v;
		v.x = x;
		v.y = y;
		v.z = z;
		tpbrt_mat4_t m;
		tpbrt_mat4_from_translation(&m, v);
		return m;
	}

	struct Builder {
		tpbrt_transforms_list_t list;
		tpbrt_transforms_list_builder_t builder;

		Builder() {
			tpbrt_transforms_list_init(&list);
			EXPECT_EQ(tpbrt_transforms_list_builder_init(&builder, &list), TPBRT_ERROR_NONE);
		}

		~Builder() {
			tpbrt_transforms_list_builder_finish(&builder);
			tpbrt_transforms_list_free(&list);
		}

		tpbrt_transform_handle_t add(const tpbrt_mat4_t& m) {
			tpbrt_transform_handle_t h;
			EXPECT_EQ(tpbrt_transforms_list_builder_append(&builder, &m, &h), TPBRT_ERROR_NONE);
			return h;
		}
	};
} // namespace

TEST(transforms, list_init_empty_free) {
	tpbrt_transforms_list_t list;
	ASSERT_EQ(tpbrt_transforms_list_init(&list), TPBRT_ERROR_NONE);
	EXPECT_EQ(tpbrt_transforms_list_empty(list), TPBRT_TRUE);
	tpbrt_transforms_list_free(&list);
	tpbrt_transforms_list_free(&list);
	EXPECT_EQ(tpbrt_transforms_list_init(nullptr), TPBRT_ERROR_INVALID_POINTER);
}

TEST(transforms, get_rejects_invalid_handles) {
	Builder b;
	const tpbrt_transform_handle_t h = b.add(translation(1, 2, 3));

	const tpbrt_mat4_t* m			 = nullptr;
	ASSERT_EQ(tpbrt_get_transform(b.list, h, &m), TPBRT_ERROR_NONE);
	ASSERT_NE(m, nullptr);
	EXPECT_FLOAT_EQ(m->m[3], 1.0f);

	EXPECT_EQ(tpbrt_get_transform(b.list, TPBRT_TRANSFORM_HANDLE_INVALID, &m), TPBRT_ERROR_INVALID_HANDLE);
	EXPECT_EQ(m, nullptr);

	tpbrt_transform_handle_t out_of_range;
	out_of_range.transform_idx = 1u;
	EXPECT_EQ(tpbrt_get_transform(b.list, out_of_range, &m), TPBRT_ERROR_INVALID_HANDLE);
	EXPECT_EQ(tpbrt_get_transform(b.list, h, nullptr), TPBRT_ERROR_INVALID_POINTER);
}

TEST(transforms, builder_deduplicates) {
	Builder b;
	const tpbrt_transform_handle_t a1 = b.add(translation(1, 2, 3));
	const tpbrt_transform_handle_t c  = b.add(translation(4, 5, 6));
	const tpbrt_transform_handle_t a2 = b.add(translation(1, 2, 3));

	EXPECT_EQ(a1.transform_idx, a2.transform_idx);
	EXPECT_NE(a1.transform_idx, c.transform_idx);
	EXPECT_EQ(b.list.count, 2u);
}

TEST(transforms, builder_animated_static_ctm_uses_one_entry) {
	Builder b;
	tpbrt_mat4_animated_t ctm;
	ctm.start = translation(1, 0, 0);
	ctm.end	  = translation(1, 0, 0);

	tpbrt_transform_animated_t h;
	ASSERT_EQ(tpbrt_transforms_list_builder_append_animated(&b.builder, &ctm, &h), TPBRT_ERROR_NONE);
	EXPECT_EQ(h.start_transform.transform_idx, h.end_transform.transform_idx);
	EXPECT_EQ(b.list.count, 1u);

	ctm.end = translation(2, 0, 0);
	ASSERT_EQ(tpbrt_transforms_list_builder_append_animated(&b.builder, &ctm, &h), TPBRT_ERROR_NONE);
	EXPECT_NE(h.start_transform.transform_idx, h.end_transform.transform_idx);
	EXPECT_EQ(b.list.count, 2u);
}

TEST(transforms, builder_many_distinct_transforms) {
	constexpr unsigned N = 10000u;
	Builder b;
	std::vector<tpbrt_transform_idx_t> handles;
	handles.reserve(N);
		for (unsigned i = 0u; i < N; ++i) { handles.push_back(b.add(translation(static_cast<float>(i), 0, 0)).transform_idx); }
	ASSERT_EQ(b.list.count, N);

		for (unsigned i = 0u; i < N; ++i) {
			ASSERT_LT(handles[i], N);
			EXPECT_FLOAT_EQ(b.list.transforms[handles[i]].m[3], static_cast<float>(i)) << "transform " << i;
		}
		for (unsigned i = 0u; i < N; i += 97u) {
			EXPECT_EQ(b.add(translation(static_cast<float>(i), 0, 0)).transform_idx, handles[i]) << "transform " << i;
		}
	EXPECT_EQ(b.list.count, N);
}

TEST(transforms, builder_finish_keeps_list) {
	tpbrt_transforms_list_t list;
	tpbrt_transforms_list_init(&list);
	tpbrt_transforms_list_builder_t builder;
	ASSERT_EQ(tpbrt_transforms_list_builder_init(&builder, &list), TPBRT_ERROR_NONE);

		for (int i = 0; i < 17; ++i) { // 17 > początkowa pojemność 16 -> lista jest przycinana w finish
			const tpbrt_mat4_t m = translation(static_cast<float>(i), 1, 1);
			tpbrt_transform_handle_t h;
			ASSERT_EQ(tpbrt_transforms_list_builder_append(&builder, &m, &h), TPBRT_ERROR_NONE);
		}
	tpbrt_transforms_list_builder_finish(&builder);

	ASSERT_EQ(list.count, 17u);
	EXPECT_FLOAT_EQ(list.transforms[16].m[3], 16.0f);
	tpbrt_transforms_list_free(&list);
}

TEST(transforms, builder_finish_on_empty_list) {
	tpbrt_transforms_list_t list;
	tpbrt_transforms_list_init(&list);
	tpbrt_transforms_list_builder_t builder;
	ASSERT_EQ(tpbrt_transforms_list_builder_init(&builder, &list), TPBRT_ERROR_NONE);
	tpbrt_transforms_list_builder_finish(&builder);
	EXPECT_EQ(tpbrt_transforms_list_empty(list), TPBRT_TRUE);
	tpbrt_transforms_list_free(&list);
}

TEST(transforms, builder_rejects_null) {
	Builder b;
	const tpbrt_mat4_t m = translation(0, 0, 0);
	tpbrt_transform_handle_t h;
	EXPECT_EQ(tpbrt_transforms_list_builder_append(nullptr, &m, &h), TPBRT_ERROR_INVALID_POINTER);
	EXPECT_EQ(tpbrt_transforms_list_builder_append(&b.builder, nullptr, &h), TPBRT_ERROR_INVALID_POINTER);
	EXPECT_EQ(tpbrt_transforms_list_builder_append(&b.builder, &m, nullptr), TPBRT_ERROR_INVALID_POINTER);
	tpbrt_transforms_list_builder_finish(nullptr);
}

TEST(transforms, scene_shares_identical_ctm) {
	/* clang-format off */
	const Scene s("WorldBegin\n"
			"AttributeBegin\nTranslate 1 2 3\nShape \"sphere\"\nAttributeEnd\n"
			"AttributeBegin\nTranslate 1 2 3\nShape \"disk\"\nAttributeEnd\n"
			"Shape \"cylinder\"\n");
	/* clang-format on */
	ASSERT_TRUE(s.ok()) << s.error();
	ASSERT_EQ(s->shapes.count, 3u);
	EXPECT_EQ(s->shapes.shapes[0].transform.start_transform.transform_idx,
	  s->shapes.shapes[1].transform.start_transform.transform_idx);
	EXPECT_NE(s->shapes.shapes[0].transform.start_transform.transform_idx,
	  s->shapes.shapes[2].transform.start_transform.transform_idx);
}

TEST(transforms, scene_with_many_distinct_transforms) {
	std::string src = "WorldBegin\n";
		for (int i = 0; i < 5000; ++i) {
			src += "AttributeBegin\nTranslate " + std::to_string(i) + " 0 0\nShape \"sphere\"\nAttributeEnd\n";
		}
	const Scene s(src);
	ASSERT_TRUE(s.ok()) << s.error();
	ASSERT_EQ(s->shapes.count, 5000u);
		for (int i = 0; i < 5000; i += 499) {
			TPBRT_EXPECT_VEC3_NEAR(origin(*s, s->shapes.shapes[i].transform.start_transform), static_cast<float>(i), 0.0f, 0.0f);
		}
}
