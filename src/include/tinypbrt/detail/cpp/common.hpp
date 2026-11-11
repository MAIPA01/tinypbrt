#pragma once
#ifndef _TINYPBRT_COMMON_HPP_
#define _TINYPBRT_COMMON_HPP_

#include <tinypbrt/detail/cpp/fwd.hpp>

#include <tinypbrt/detail/common.h>

#include <algorithm>
#include <ranges>
#include <span>
#include <string_view>
#include <type_traits>

#include <tinypbrt/detail/cpp/error.hpp>

namespace tpbrt {
	/**
	 * @defgroup common-cpp-public Common
	 * @ingroup tinypbrt-cpp-public
	 */

	template<class Derived, class Native>
	struct wrapper {
		/// @brief Native C library type for this struct.
		using native_type = Native;

	private:
		/// @brief Derived C++ wrapper type.
		using derived_type = Derived;

	protected:
		/// @brief C native struct.
		const native_type& _native;

	public:
		/// @brief C to C++ wrapper conversion constructor.
		constexpr wrapper(const native_type& native) noexcept : _native(native) {} // NOLINT

		/// @brief Conversion operator to native type.
		[[nodiscard]] constexpr operator const native_type&() const noexcept { return _native; } // NOLINT

		/// @brief Conversion operator to pointer to native type.
		[[nodiscard]] constexpr operator const native_type*() const noexcept { return &_native; } // NOLINT

		/// @brief Space ship operator.
		[[nodiscard]] constexpr auto operator<=>(const wrapper& rhs) const noexcept
		  requires requires (const Derived& d) { d.reflect(); }
		{
			const auto& derived_lhs = static_cast<const Derived&>(*this);
			const auto& derived_rhs = static_cast<const Derived&>(rhs);
			return derived_lhs.reflect() <=> derived_rhs.reflect();
		}

		/// @brief Equal operator.
		[[nodiscard]] constexpr bool_t operator==(const wrapper& rhs) const noexcept
		  requires requires (const Derived& d) { d.reflect(); }
		{
			return operator<=>(rhs) == 0;
		}
	};

	template<class Derived, class... Ts>
	struct with_variant {
		/// @brief Type used for better reading tagged union of parameters.
		using variant_type = detail::optional_variant<Ts...>;

		/// @brief Visits parameters.
		constexpr auto visit(const auto& visitor) const noexcept requires requires (const Derived& d) { d.to_variant(); }
		{
			const variant_type variant = static_cast<const Derived&>(*this).to_variant();

			using return_type		   = decltype(std::visit(visitor, variant.value()));

				if constexpr (std::is_void_v<return_type>) {
						if (variant.has_value()) { return std::visit(visitor, variant.value()); }
				}
				else {
						if (variant.has_value()) { return std::optional<return_type>(std::visit(visitor, variant.value())); }
					return std::optional<return_type>(std::nullopt);
				}
		}
	};

	/**
	 * @brief C++ wrapper for tpbrt_string_t.
	 * @ingroup common-cpp-public
	 */
	struct string : wrapper<string, tpbrt_string_t> {
		/// @brief Type used for reflection.
		using reflect_type = std::tuple<const char_t*, size_t>;

		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns data value.
		[[nodiscard]] constexpr const char_t* data() const noexcept { return _native.data; }

		/// @brief Returns size value.
		[[nodiscard]] constexpr size_t size() const noexcept { return _native.size; }

		/// @brief Returns string length.
		[[nodiscard]] constexpr size_t length() const noexcept { return size(); }

		/// @brief Returns true if string is empty.
		[[nodiscard]] constexpr bool_t empty() const noexcept { return data() == nullptr || size() == 0; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept { return std::make_tuple(data(), size()); }

		/// @brief Conversion to std::string_view.
		[[nodiscard]] constexpr std::string_view to_string_view() const noexcept {
			const char_t* data_ptr = data();
				if (data_ptr == nullptr) { return {}; }
			return std::string_view(data_ptr, size());
		}

		/// @brief Conversion operator to std::string_view
		[[nodiscard]] constexpr operator std::string_view() const noexcept { return to_string_view(); } // NOLINT
	};

	/**
	 * @brief Base to all generic C++ array traits.
	 * @tparam Native Type of C library native array type.
	 * @tparam DataField Pointer to data field of array.
	 * @ingroup common-cpp-public
	 */
	template<class Native, auto DataField = nullptr>
	struct array_traits;

	/**
	 * @brief Generic C++ array traits to all C style arrays.
	 * @tparam Native Type of C library native array type.
	 * @tparam DataField In this scenario nullptr.
	 * @ingroup common-cpp-public
	 */
	template<class Native, auto DataField>
	requires std::is_array_v<Native>
	struct array_traits<Native, DataField> {
		/// @brief Native C library array element type.
		using native_element_type	   = std::remove_extent_t<Native>;

		/// @brief Extent of array.
		static constexpr size_t Extent = std::extent_v<Native>;

		/// @brief std::span type.
		using span_type				   = std::span<const native_element_type, Extent>;

		/// @brief Returns std::span.
		[[nodiscard]] static constexpr span_type get_span(const Native& native) noexcept { return span_type(native); }
	};

	/**
	 * @brief Generic C++ array traits to all tpbrt_*_array_t.
	 * @tparam Native Type of C library native array type.
	 * @tparam DataField In this scenario nullptr.
	 * @ingroup common-cpp-public
	 */
	template<class Native, auto DataField>
	requires (!std::is_array_v<Native>) && std::is_same_v<decltype(DataField), std::nullptr_t> && requires (const Native& n) {
		n.data;
		n.count;
	}
	struct array_traits<Native, DataField> {
		/// @brief Native C library array element type.
		using native_element_type	   = std::remove_pointer_t<decltype(std::declval<Native>().data)>;

		/// @brief Extent of array.
		static constexpr size_t Extent = std::dynamic_extent;

		/// @brief std::span type.
		using span_type				   = std::span<const native_element_type>;

		/// @brief Returns std::span.
		[[nodiscard]] static constexpr span_type get_span(const Native& native) noexcept {
			const native_element_type* ptr = native.data;
			return (ptr == nullptr || native.count == 0) ? span_type {} : span_type { ptr, native.count };
		}
	};

	/**
	 * @brief Generic C++ array traits to all basic tpbrt_*_list_t.
	 * @tparam Native Type of C library native array type.
	 * @tparam DataField Pointer to field with data.
	 * @ingroup common-cpp-public
	 */
	template<class Native, auto DataField>
	requires (!std::is_array_v<Native>) && (!std::is_same_v<decltype(DataField), std::nullptr_t>) &&
			 requires (const Native& n) {
				 n.*DataField;
				 n.count;
			 } && (!std::is_array_v<std::remove_reference_t<decltype(std::declval<Native>().*DataField)>>)
	struct array_traits<Native, DataField> {
		/// @brief Native C library array element type.
		using native_element_type = std::remove_reference_t<std::remove_pointer_t<decltype(std::declval<Native>().*DataField)>>;

		/// @brief Extent of array.
		static constexpr size_t Extent = std::dynamic_extent;

		/// @brief std::span type.
		using span_type				   = std::span<const native_element_type>;

		/// @brief Returns std::span.
		[[nodiscard]] static constexpr span_type get_span(const Native& native) noexcept {
			const native_element_type* ptr = native.*DataField;
			return (ptr == nullptr || native.count == 0) ? span_type {} : span_type { ptr, native.count };
		}
	};

	/**
	 * @brief Generic C++ array traits to all basic tpbrt_*_t which can be read as array but have data field as C array.
	 * @tparam Native Type of C library native array type.
	 * @tparam DataField Pointer to field with data.
	 * @ingroup common-cpp-public
	 */
	template<class Native, auto DataField>
	requires (!std::is_array_v<Native>) && (!std::is_same_v<decltype(DataField), std::nullptr_t>) &&
			 requires (const Native& n) { n.*DataField; } &&
			 std::is_array_v<std::remove_reference_t<decltype(std::declval<Native>().*DataField)>>
	struct array_traits<Native, DataField> {
		using field_type		  = std::remove_reference_t<std::remove_pointer_t<decltype(std::declval<Native>().*DataField)>>;

		/// @brief Native C library array element type.
		using native_element_type = std::remove_extent_t<field_type>;

		/// @brief Extent of array.
		static constexpr size_t Extent = std::extent_v<field_type>;

		/// @brief std::span type.
		using span_type				   = std::span<const native_element_type, Extent>;

		/// @brief Returns std::span.
		[[nodiscard]] static constexpr span_type get_span(const Native& native) noexcept { return span_type(native.*DataField); }
	};

	template<class Native, class Element, auto DataField>
	struct array : wrapper<array<Native, Element, DataField>, Native> {
		/// @brief Default wrapper constructor.
		using wrapper<array, Native>::wrapper;

		/// @brief Array traits.
		using traits				   = array_traits<Native, DataField>;

		/// @brief Native C library array element type.
		using native_element_type	   = traits::native_element_type;

		/// @brief C++ array view element type.
		using element_type			   = std::conditional_t<std::is_void_v<Element>, native_element_type, Element>;

		/// @brief std::span type of this array.
		using span_type				   = traits::span_type;

		/// @brief Array static size.
		static constexpr size_t Extent = traits::Extent;

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
		using view_type	   = std::conditional_t<std::is_same_v<native_element_type, element_type>, span_type,
		  std::ranges::transform_view<span_type, element_transformer>>;

		/// @brief Type used for reflection.
		using reflect_type = std::tuple<view_type>;

	protected:
		/// @brief C native struct.
		using wrapper<array, Native>::_native;

	public:
		/// @brief Returns size of this array.
		[[nodiscard]] constexpr size_t size() const noexcept { return traits::get_span(_native).size(); }

		/// @brief Returns true if array is empty.
		[[nodiscard]] constexpr bool_t empty() const noexcept { return traits::get_span(_native).empty(); }

		[[nodiscard]] constexpr result<element_type> at(const size_t index) const noexcept {
				if (index >= size()) { return error::INDEX_OUT_OF_RANGE; }
			return static_cast<element_type>(traits::get_span(_native)[index]);
		}

		/// @brief Returns view on array data.
		[[nodiscard]] constexpr view_type to_view() const noexcept {
			const auto raw_span = traits::get_span(_native);
				if constexpr (std::is_same_v<native_element_type, element_type>) { return raw_span; }
				else { return raw_span | std::views::transform(element_transformer {}); }
		}

		/// @brief Returns begin iterator.
		[[nodiscard]] constexpr auto begin() const noexcept { return to_view().begin(); }

		/// @brief Returns end iterator.
		[[nodiscard]] constexpr auto end() const noexcept { return to_view().end(); }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept { return std::make_tuple(to_view()); }

		/// @brief Returns element on given index.
		[[nodiscard]] constexpr operator const native_element_type*() const noexcept { // NOLINT
			return traits::get_span(_native).data();
		}

		/// @brief Returns element on given index.
		[[nodiscard]] constexpr result<element_type> operator[](const size_t index) const noexcept { return at(index); }

		/// @brief Space ship operator.
		[[nodiscard]] constexpr auto operator<=>(const array& rhs) const noexcept {
			const auto v1 = to_view();
			const auto v2 = rhs.to_view();
			return std::lexicographical_compare_three_way(v1.begin(), v1.end(), v2.begin(), v2.end());
		}

		/// @brief Equal operator.
		[[nodiscard]] constexpr bool_t operator==(const array& rhs) const noexcept {
			return std::ranges::equal(to_view(), rhs.to_view());
		}
	};

	/**
	 * @brief C++ wrapper for tpbrt_string_array_t.
	 * @ingroup common-cpp-public
	 */
	using string_array = array<tpbrt_string_array_t, string>;
} // namespace tpbrt
#endif // _TINYPBRT_TYPES_HPP_
