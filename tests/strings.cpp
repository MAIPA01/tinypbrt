#include <pch.hpp>

#include <tinypbrt/detail/common_internal.h>

#include <test_helpers.hpp>

using namespace tpbrt_test;

TEST(strings, copy_unescaped_decodes_pbrt_escapes) {
	struct Case {
		const char* input;
		const char* expected;
	};

	const Case cases[] = {
		{ .input = "plain",			.expected = "plain"		},
		{ .input = "a\\\"b.png",	 .expected = "a\"b.png"	},
		{ .input = "C:\\\\tex\\\\a", .expected = "C:\\tex\\a" },
		{ .input = "x\\ty\\nz",		.expected = "x\ty\nz"	  },
		{ .input = "\\'q\\'",		  .expected = "'q'"		},
		{ .input = "\\b\\f\\r",		.expected = "\b\f\r"	 },
	};

		for (const Case& c : cases) {
			tpbrt_string_t out;
			ASSERT_EQ(tpbrt_string_copy_unescaped(&out, str(c.input)), TPBRT_ERROR_NONE) << c.input;
			EXPECT_EQ(to_std(out), c.expected) << c.input;
			tpbrt_string_free(&out);
		}
}

TEST(strings, copy_unescaped_rejects_unknown_escape) {
	tpbrt_string_t out;
	EXPECT_EQ(tpbrt_string_copy_unescaped(&out, str("bad\\q")), TPBRT_ERROR_INVALID_STRING);
}

TEST(strings, copy_unescaped_empty) {
	tpbrt_string_t out;
	ASSERT_EQ(tpbrt_string_copy_unescaped(&out, TPBRT_EMPTY_STRING), TPBRT_ERROR_NONE);
	EXPECT_EQ(tpbrt_string_empty(out), TPBRT_TRUE);
	EXPECT_EQ(tpbrt_string_copy_unescaped(nullptr, str("x")), TPBRT_ERROR_INVALID_POINTER);
}

TEST(strings, copy_chars) {
	char buf[8];
	ASSERT_EQ(tpbrt_copy_chars(buf, sizeof(buf), "abcdef", 3u), TPBRT_ERROR_NONE);
	EXPECT_STREQ(buf, "abc");

	EXPECT_EQ(tpbrt_copy_chars(buf, sizeof(buf), "abcdefgh", 8u), TPBRT_ERROR_INVALID_STRING);
	EXPECT_EQ(tpbrt_copy_chars(buf, 0u, "a", 0u), TPBRT_ERROR_INVALID_STRING);
	EXPECT_EQ(tpbrt_copy_chars(nullptr, 4u, "a", 1u), TPBRT_ERROR_INVALID_STRING);
	EXPECT_EQ(tpbrt_copy_chars(buf, sizeof(buf), nullptr, 1u), TPBRT_ERROR_INVALID_STRING);

	ASSERT_EQ(tpbrt_copy_chars(buf, sizeof(buf), nullptr, 0u), TPBRT_ERROR_NONE);
	EXPECT_STREQ(buf, "");
}

TEST(strings, copy_to_truncates) {
	char buf[4];
	tpbrt_size_t written = 0u;
	ASSERT_EQ(tpbrt_string_copy_to(str("abcdef"), buf, sizeof(buf), &written), TPBRT_ERROR_NONE);
	EXPECT_STREQ(buf, "abc");
	EXPECT_EQ(written, 3u);
}

TEST(strings, equals) {
	EXPECT_EQ(tpbrt_string_equals(str("abc"), str("abc")), TPBRT_TRUE);
	EXPECT_EQ(tpbrt_string_equals(str("abc"), str("abd")), TPBRT_FALSE);
	EXPECT_EQ(tpbrt_string_equals(str("abc"), str("ab")), TPBRT_FALSE);
	EXPECT_EQ(tpbrt_string_equals_cstr(str("abc"), "abc"), TPBRT_TRUE);
}

TEST(strings, error_message) {
	char buf[128];
	EXPECT_GT(tpbrt_get_error_message(TPBRT_ERROR_NONE, buf, sizeof(buf)), 0u);
	EXPECT_STREQ(buf, "No error");

	EXPECT_GT(tpbrt_get_error_message(static_cast<tpbrt_error_t>(TPBRT_ERROR_MAX_NUM + 5), buf, sizeof(buf)), 0u);
	EXPECT_STREQ(buf, "Undefined error");

	char tiny[1] = { 'x' };
	EXPECT_EQ(tpbrt_get_error_message(TPBRT_ERROR_NONE, tiny, sizeof(tiny)), 0u);
	EXPECT_EQ(tiny[0], '\0');

	EXPECT_EQ(tpbrt_get_error_message(TPBRT_ERROR_NONE, nullptr, 16u), 0u);
}

TEST(strings, every_error_has_message) {
	char buf[128];
		for (int e = 0; e < TPBRT_ERROR_MAX_NUM; ++e) {
			ASSERT_GT(tpbrt_get_error_message(static_cast<tpbrt_error_t>(e), buf, sizeof(buf)), 0u) << "error " << e;
			EXPECT_STRNE(buf, "Undefined error") << "error " << e;
		}
}
