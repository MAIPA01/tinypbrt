#pragma once
#ifndef _TINYPBRT_TESTS_FROM_STRING_HELPERS_HPP_
#define _TINYPBRT_TESTS_FROM_STRING_HELPERS_HPP_

#include <test_helpers.hpp>

#include <gtest/gtest.h>

#include <cctype>
#include <cstddef>
#include <cstring>
#include <memory>
#include <set>
#include <string>

namespace tpbrt_test {

	template<typename E>
	struct FromStringCase {
		const char* text;
		E value;
	};

	template<typename E>
	using from_string_fn_t = tpbrt_error_t (*)(tpbrt_string_t, E*);

	class UnterminatedString {
	public:
		explicit UnterminatedString(const std::string& text)
			: _size(text.size()), _data(new char[text.size() == 0u ? 1u : text.size()]) {
				if (_size != 0u) { std::memcpy(_data.get(), text.data(), _size); }
		}

		tpbrt_string_t get() const {
			tpbrt_string_t s;
			s.data = _data.get();
			s.size = _size;
			return s;
		}

	private:
		std::size_t _size;
		std::unique_ptr<char[]> _data;
	};

	template<typename E>
	tpbrt_error_t call_from_string(const from_string_fn_t<E> fn, const std::string& text, E* const out) {
		const UnterminatedString s(text);
		return fn(s.get(), out);
	}

	inline std::string toggle_case(std::string s) {
			for (char& c : s) {
				const unsigned char u = static_cast<unsigned char>(c);
				c					  = static_cast<char>(std::isupper(u) ? std::tolower(u) : std::toupper(u));
			}
		return s;
	}

	template<typename E, std::size_t N>
	void check_from_string(const from_string_fn_t<E> fn, const FromStringCase<E> (&cases)[N], const tpbrt_error_t invalid_error,
	  const E max_num) {
		ASSERT_EQ(N, static_cast<std::size_t>(max_num)) << "test nie obejmuje wszystkich wartosci enuma";
		std::set<int> values;
		std::set<std::string> texts;
			for (const FromStringCase<E>& c : cases) {
				EXPECT_TRUE(values.insert(static_cast<int>(c.value)).second) << "zdublowana wartosc dla \"" << c.text << "\"";
				EXPECT_TRUE(texts.insert(c.text).second) << "zdublowany napis \"" << c.text << "\"";
			}

		const E sentinel = max_num;

			for (const FromStringCase<E>& c : cases) {
				SCOPED_TRACE(c.text);
				const std::string text = c.text;

				E out				   = sentinel;
				EXPECT_EQ(call_from_string(fn, text, &out), TPBRT_ERROR_NONE);
				EXPECT_EQ(static_cast<int>(out), static_cast<int>(c.value));

				{
					const std::string buffer = text + "_tail";
					tpbrt_string_t s;
					s.data = buffer.c_str();
					s.size = text.size();
					out	   = sentinel;
					EXPECT_EQ(fn(s, &out), TPBRT_ERROR_NONE);
					EXPECT_EQ(static_cast<int>(out), static_cast<int>(c.value));
				}

				const std::string prefix = text.substr(0u, text.size() - 1u);
					if (!prefix.empty() && texts.count(prefix) == 0u) {
						out = sentinel;
						EXPECT_EQ(call_from_string(fn, prefix, &out), invalid_error) << "prefiks \"" << prefix << "\"";
						EXPECT_EQ(static_cast<int>(out), static_cast<int>(sentinel)) << "wartosc nadpisana przy bledzie";
					}

					for (const std::string& bad : { text + "x", text + " ", " " + text, toggle_case(text) }) {
							if (texts.count(bad) != 0u) { continue; }
						out = sentinel;
						EXPECT_EQ(call_from_string(fn, bad, &out), invalid_error) << "\"" << bad << "\"";
						EXPECT_EQ(static_cast<int>(out), static_cast<int>(sentinel)) << "wartosc nadpisana przy bledzie";
					}

				EXPECT_EQ(call_from_string<E>(fn, text, nullptr), TPBRT_ERROR_INVALID_POINTER);
			}

		E out = sentinel;
		EXPECT_EQ(call_from_string(fn, std::string(), &out), invalid_error);
		tpbrt_string_t null_str;
		null_str.data = nullptr;
		null_str.size = 0u;
		EXPECT_EQ(fn(null_str, &out), invalid_error);
		EXPECT_EQ(fn(null_str, nullptr), TPBRT_ERROR_INVALID_POINTER);

		EXPECT_EQ(call_from_string(fn, "definitely-not-a-pbrt-name", &out), invalid_error);
		EXPECT_EQ(static_cast<int>(out), static_cast<int>(sentinel));
	}

} // namespace tpbrt_test

#endif // _TINYPBRT_TESTS_FROM_STRING_HELPERS_HPP_
