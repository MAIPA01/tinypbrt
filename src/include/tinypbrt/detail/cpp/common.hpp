#pragma once
#ifndef _TINYPBRT_COMMON_HPP_
#define _TINYPBRT_COMMON_HPP_

#include <tinypbrt/detail/cpp/fwd.hpp>

#include <tinypbrt/detail/common.h>

#include <tinypbrt/detail/cpp/error.hpp>

#include <algorithm>
#include <ranges>
#include <span>
#include <string_view>
#include <type_traits>

namespace tpbrt {
	/**
	 * @defgroup common-cpp-public Common
	 * @ingroup tinypbrt-cpp-public
	 */

	/**
	 * @brief C++ wrapper for tpbrt_string_t.
	 * @ingroup common-cpp-public
	 */
	struct string {
		/// @brief Native C library type for this struct.
		using native_type  = tpbrt_string_t;

		/// @brief Type used for reflection.
		using reflect_type = std::tuple<const char_t*, size_t>;

	private:
		/// @brief C native struct.
		const native_type& _native;

	public:
		/// @brief C to C++ wrapper conversion constructor.
		constexpr string(const native_type& native) noexcept : _native(native) {} // NOLINT

		/// @brief Returns data value.
		[[nodiscard]] constexpr const char_t* data() const noexcept { return _native.data; }

		/// @brief Returns size value.
		[[nodiscard]] constexpr size_t size() const noexcept { return _native.size; }

		/// @brief Returns string length.
		[[nodiscard]] constexpr size_t length() const noexcept { return size(); }

		/// @brief Returns true if string is empty.
		[[nodiscard]] constexpr bool empty() const noexcept { return data() == nullptr || size() == 0; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept { return std::make_tuple(data(), size()); }

		/// @brief Conversion to std::string_view.
		[[nodiscard]] constexpr std::string_view to_string_view() const noexcept {
			const char_t* data_ptr = data();
				if (data_ptr == nullptr) { return {}; }
			return std::string_view(data_ptr, size());
		}

		/// @brief Conversion operator to native type.
		[[nodiscard]] constexpr operator const native_type&() const noexcept { return _native; } // NOLINT

		/// @brief Conversion operator to pointer to native type.
		[[nodiscard]] constexpr operator const native_type*() const noexcept { return &_native; } // NOLINT

		/// @brief Conversion operator to std::string_view
		[[nodiscard]] constexpr operator std::string_view() const noexcept { return to_string_view(); } // NOLINT

		/// @brief Space ship operator.
		[[nodiscard]] constexpr auto operator<=>(const string& rhs) const noexcept {
			return to_string_view() <=> rhs.to_string_view();
		}

		/// @brief Equal operator.
		[[nodiscard]] constexpr bool operator==(const string& rhs) const noexcept {
			return to_string_view() == rhs.to_string_view();
		}
	};

	/**
	 * @brief Generic C++ array view wrapper to all tpbrt_*_array_t.
	 * @tparam NativeArray Type of C library native array type.
	 * @tparam Element Desired array element. (If not the same as NativeArrayElement transformation will be performed).
	 */
	template<class NativeArray, class Element>
	struct array {
		/// @brief Native C library type for this struct.
		using native_type		  = NativeArray;

		/// @brief Native C library array element type.
		using native_element_type = std::remove_pointer_t<decltype(native_type::data)>;

		/// @brief C++ array view element type.
		using element_type		  = Element;

	private:
		/// @brief C to C++ element type conversion functor.
		struct element_transformer {
			/// @brief Call operator.
			[[nodiscard]] constexpr element_type operator()(const native_element_type& element) const noexcept {
				return static_cast<element_type>(element);
			}
		};

	public:
		/// @brief Type used for viewing array data.
		using view_type =
		  std::conditional_t<std::is_same_v<native_element_type, element_type>, std::span<const native_element_type>,
			std::ranges::transform_view<std::span<const native_element_type>, element_transformer>>;

		/// @brief Type used for reflection.
		using reflect_type = std::tuple<view_type>;

	private:
		/// @brief C native struct.
		const native_type& _native;

	public:
		/// @brief C to C++ wrapper conversion constructor.
		constexpr array(const native_type& native) noexcept : _native(native) {} // NOLINT

		/// @brief Returns count value.
		[[nodiscard]] constexpr size_t size() const noexcept { return _native.count; }

		/// @brief Returns true if array is empty.
		[[nodiscard]] constexpr bool empty() const noexcept { return _native.data == nullptr || size() == 0; }

		/// @brief Returns element on given index.
		[[nodiscard]] constexpr result<element_type> at(const size_t index) const noexcept {
				if (index >= size()) { return error::INDEX_OUT_OF_RANGE; }
			return static_cast<element_type>(_native.data[index]);
		}

		/// @brief Returns view on array data.
		[[nodiscard]] constexpr view_type to_view() const noexcept {
			const native_element_type* const ptr = _native.data;

			const auto raw_span =
			  empty() ? std::span<const native_element_type> {} : std::span<const native_element_type> { ptr, size() };

				if constexpr (std::is_same_v<native_element_type, element_type>) { return raw_span; }
				else { return raw_span | std::views::transform(element_transformer {}); }
		}

		/// @brief Returns begin iterator.
		[[nodiscard]] constexpr auto begin() const noexcept { return to_view().begin(); }

		/// @brief Returns end iterator.
		[[nodiscard]] constexpr auto end() const noexcept { return to_view().end(); }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept { return std::make_tuple(to_view()); }

		/// @brief Conversion operator to native type.
		[[nodiscard]] constexpr operator const native_type&() const noexcept { return _native; } // NOLINT

		/// @brief Conversion operator to pointer to native type.
		[[nodiscard]] constexpr operator const native_type*() const noexcept { return &_native; } // NOLINT

		/// @brief Returns element on given index.
		[[nodiscard]] constexpr result<element_type> operator[](const size_t index) const noexcept { return at(index); }

		/// @brief Space ship operator.
		[[nodiscard]] constexpr auto operator<=>(const array& rhs) const noexcept {
			const auto v1 = to_view();
			const auto v2 = rhs.to_view();
			return std::lexicographical_compare_three_way(v1.begin(), v1.end(), v2.begin(), v2.end());
		}

		/// @brief Equal operator.
		[[nodiscard]] constexpr bool operator==(const array& rhs) const noexcept {
			return std::ranges::equal(to_view(), rhs.to_view());
		}
	};

	/**
	 * @brief C++ wrapper for tpbrt_string_array_t.
	 * @ingroup common-cpp-public
	 */
	using string_array = array<tpbrt_string_array_t, string>;

	/**
	 * @brief Generic C++ list view wrapper to all basic tpbrt_*_list_t.
	 * @tparam NativeList Type of C library native array type.
	 * @tparam Element Desired array element. (If not the same as NativeArrayElement transformation will be performed).
	 * @tparam DataField Pointer to field with data.
	 */
	template<class NativeList, class Element, auto NativeList::* DataField>
	struct list {
		/// @brief Native C library type for this struct.
		using native_type		  = NativeList;

		/// @brief Native C library array element type.
		using native_element_type = std::remove_pointer_t<decltype(DataField)>;

		/// @brief C++ array view element type.
		using element_type		  = Element;

	private:
		/// @brief C to C++ element type conversion functor.
		struct element_transformer {
			/// @brief Call operator.
			[[nodiscard]] constexpr element_type operator()(const native_element_type& element) const noexcept {
				return static_cast<element_type>(element);
			}
		};

	public:
		/// @brief Type used for viewing array data.
		using view_type	   = std::conditional_t<std::is_same_v<native_element_type, element_type>, std::span<const element_type>,
		  std::ranges::transform_view<std::span<const native_element_type>, element_transformer>>;

		/// @brief Type used for reflection.
		using reflect_type = std::tuple<view_type>;

	protected:
		/// @brief C native struct.
		const native_type& _native;

		[[nodiscard]] constexpr native_element_type* _get_data_ptr() noexcept { return _native.*DataField; }

		[[nodiscard]] constexpr const native_element_type* _get_data_ptr() const noexcept { return _native.*DataField; }

	public:
		/// @brief C to C++ wrapper conversion constructor.
		constexpr list(const NativeList& native) noexcept : _native(native) {} // NOLINT

		/// @brief Returns count value.
		[[nodiscard]] constexpr size_t size() const noexcept { return _native.count; }

		/// @brief Returns true if list is empty.
		[[nodiscard]] constexpr bool empty() const noexcept { return _get_data_ptr() == nullptr || size() == 0; }

		/// @brief Returns view on list data.
		[[nodiscard]] constexpr view_type to_view() const noexcept {
			const native_element_type* const ptr = _get_data_ptr();

			const auto raw_span =
			  empty() ? std::span<const native_element_type> {} : std::span<const native_element_type> { ptr, size() };

				if constexpr (std::is_same_v<native_element_type, element_type>) { return raw_span; }
				else { return raw_span | std::views::transform(element_transformer {}); }
		}

		/// @brief Returns begin iterator.
		[[nodiscard]] constexpr auto begin() const noexcept { return to_view().begin(); }

		/// @brief Returns end iterator.
		[[nodiscard]] constexpr auto end() const noexcept { return to_view().end(); }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept { return std::make_tuple(to_view()); }

		/// @brief Conversion operator to native type.
		[[nodiscard]] constexpr operator const native_type&() const noexcept { return _native; } // NOLINT

		/// @brief Conversion operator to pointer to native type.
		[[nodiscard]] constexpr operator const native_type*() const noexcept { return &_native; } // NOLINT

		/// @brief Space ship operator.
		[[nodiscard]] constexpr auto operator<=>(const list& rhs) const noexcept {
			const auto v1 = to_view();
			const auto v2 = rhs.to_view();
			return std::lexicographical_compare_three_way(v1.begin(), v1.end(), v2.begin(), v2.end());
		}

		/// @brief Equal operator.
		[[nodiscard]] constexpr bool operator==(const list& rhs) const noexcept {
			return std::ranges::equal(to_view(), rhs.to_view());
		}
	};
} // namespace tpbrt
#endif // _TINYPBRT_TYPES_HPP_
