#pragma once
#ifndef _TINYPBRT_COMMON_HPP_
#define _TINYPBRT_COMMON_HPP_

#include <tinypbrt/detail/cpp/fwd.hpp>

#include <tinypbrt/detail/common.h>

#include <algorithm>
#include <compare>
#include <cstddef>
#include <iterator>
#include <optional>
#include <ranges>
#include <span>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <utility>
#include <variant>

#include <tinypbrt/detail/cpp/error.hpp>

namespace tpbrt {
	/**
	 * @defgroup common-cpp-public Common
	 * @ingroup tinypbrt-cpp-public
	 */

	namespace detail {
		/// @brief True when every type in the pack is different from all the others.
		template<class... Ts>
		inline constexpr bool are_distinct_v = true; // NOLINT

		/// @brief True when every type in the pack is different from all the others.
		template<class T, class... Ts>
		inline constexpr bool are_distinct_v<T, Ts...> = (!std::is_same_v<T, Ts> && ...) && are_distinct_v<Ts...>; // NOLINT

		/// @brief Optional variant (empty when the C tagged union holds an unknown type).
		template<class... Ts>
		using optional_variant = std::optional<std::variant<Ts...>>;

		/**
		 * @brief Visits an optional variant.
		 * @return Nothing for void visitors, otherwise std::optional with the visitor result (empty when variant is empty).
		 */
		template<class Visitor, class... Ts>
		constexpr auto visit_optional(Visitor&& visitor, const std::optional<std::variant<Ts...>>& variant) {
			using return_type = decltype(std::visit(std::forward<Visitor>(visitor), *variant));

				if constexpr (std::is_void_v<return_type>) {
						if (variant.has_value()) { std::visit(std::forward<Visitor>(visitor), *variant); }
				}
				else {
					using value_type = std::remove_cvref_t<return_type>;
						if (!variant.has_value()) { return std::optional<value_type>(std::nullopt); }
					return std::optional<value_type>(std::visit(std::forward<Visitor>(visitor), *variant));
				}
		}

		template<class Derived, class Native>
		class wrapper {
		public:
			/// @brief Native C library type for this struct.
			using native_type = Native;

		protected:
			/// @brief Pointer to the viewed C struct (never null for a constructed wrapper).
			const native_type* _native;

		public:
			/// @brief C to C++ wrapper conversion constructor (the wrapper is a view, the C struct must outlive it).
			constexpr wrapper(const native_type& native) noexcept : _native(&native) {} // NOLINT

			/// @brief Binding a view to a temporary C struct would leave it dangling.
			wrapper(const native_type&&) = delete; // NOLINT

			/// @brief Returns the viewed C struct.
			[[nodiscard]] constexpr const native_type& native() const noexcept { return *_native; }

			/// @brief Conversion operator to native type.
			[[nodiscard]] constexpr operator const native_type&() const noexcept { return *_native; } // NOLINT

			/// @brief Three-way comparison operator (compares the values, not the addresses).
			[[nodiscard]] constexpr auto operator<=>(const wrapper& rhs) const noexcept
			  requires requires (const Derived& d) { d.reflect(); }
			{
				static_assert(std::is_base_of_v<wrapper, Derived>, "Derived must be the type which inherits this wrapper (CRTP)");
				const auto& derived_lhs = static_cast<const Derived&>(*this);
				const auto& derived_rhs = static_cast<const Derived&>(rhs);
				return derived_lhs.reflect() <=> derived_rhs.reflect();
			}

			/// @brief Equality operator (compares the values, not the addresses).
			[[nodiscard]] constexpr bool_t operator==(const wrapper& rhs) const noexcept
			  requires requires (const Derived& d) { d.reflect(); }
			{
				static_assert(std::is_base_of_v<wrapper, Derived>, "Derived must be the type which inherits this wrapper (CRTP)");
				const auto& derived_lhs = static_cast<const Derived&>(*this);
				const auto& derived_rhs = static_cast<const Derived&>(rhs);
				return derived_lhs.reflect() == derived_rhs.reflect();
			}
		};

		template<class Derived, class... Ts>
		struct with_variant {
			// clang-format off
			static_assert(
			  are_distinct_v<Ts...>,
			  "Variant alternatives must be distinct types, otherwise std::visit cannot tell them apart "
			  "(use a separate wrapper type, e.g. tagged_params, instead of a type alias)"
			);
			// clang-format on

			/// @brief Type used for better reading tagged union of parameters.
			using variant_type = optional_variant<Ts...>;

			/**
			 * @brief Visits parameters.
			 * @return Nothing for void visitors, otherwise std::optional with the visitor result (empty for an unknown type).
			 */
			template<class Visitor>
			constexpr auto visit(Visitor&& visitor) const requires requires (const Derived& d) { d.to_variant(); }
			{
				return detail::visit_optional(std::forward<Visitor>(visitor), static_cast<const Derived&>(*this).to_variant());
			}

			/// @brief Returns parameters as T when the tagged union currently holds T.
			template<class T>
			requires (std::is_same_v<T, Ts> || ...)
			[[nodiscard]] constexpr std::optional<T> as() const noexcept requires requires (const Derived& d) { d.to_variant(); }
			{
				const variant_type variant = static_cast<const Derived&>(*this).to_variant();
					if (!variant.has_value()) { return std::nullopt; }
					if (const T* const value = std::get_if<T>(&*variant); value != nullptr) { return *value; }
				return std::nullopt;
			}

			/// @brief Returns true when the tagged union currently holds T.
			template<class T>
			requires (std::is_same_v<T, Ts> || ...)
			[[nodiscard]] constexpr bool_t holds() const noexcept requires requires (const Derived& d) { d.to_variant(); }
			{
				const variant_type variant = static_cast<const Derived&>(*this).to_variant();
				return variant.has_value() && std::holds_alternative<T>(*variant);
			}
		};

		/**
		 * @brief Generic C++ array traits to all C style arrays.
		 * @tparam Native Type of C library native array type.
		 * @tparam DataField In this scenario nullptr.
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
		 */
		template<class Native, auto DataField>
		requires (!std::is_array_v<Native>) && std::is_same_v<decltype(DataField), std::nullptr_t> && requires (const Native& n) {
			n.data;
			n.count;
		}
		struct array_traits<Native, DataField> {
			/// @brief Native C library array element type.
			using native_element_type	   = std::remove_pointer_t<decltype(Native::data)>;

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
		 */
		template<class Native, auto DataField>
		requires (!std::is_array_v<Native>) && (!std::is_same_v<decltype(DataField), std::nullptr_t>) &&
				 requires (const Native& n) {
					 n.*DataField;
					 n.count;
				 } && std::is_pointer_v<std::remove_cvref_t<decltype(std::declval<const Native&>().*DataField)>>
		struct array_traits<Native, DataField> {
			/// @brief Native C library array element type.
			using native_element_type =
			  std::remove_pointer_t<std::remove_cvref_t<decltype(std::declval<const Native&>().*DataField)>>;

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
		 */
		template<class Native, auto DataField>
		requires (!std::is_array_v<Native>) && (!std::is_same_v<decltype(DataField), std::nullptr_t>) &&
				 std::is_array_v<std::remove_cvref_t<decltype(std::declval<const Native&>().*DataField)>>
		struct array_traits<Native, DataField> {
			/// @brief Type of the C array field.
			using field_type			   = std::remove_cvref_t<decltype(std::declval<const Native&>().*DataField)>;

			/// @brief Native C library array element type.
			using native_element_type	   = std::remove_extent_t<field_type>;

			/// @brief Extent of array.
			static constexpr size_t Extent = std::extent_v<field_type>;

			/// @brief std::span type.
			using span_type				   = std::span<const native_element_type, Extent>;

			/// @brief Returns std::span.
			[[nodiscard]] static constexpr span_type get_span(const Native& native) noexcept {
				return span_type(native.*DataField);
			}
		};

		/**
		 * @brief Random access iterator which converts C elements to C++ wrappers on the fly.
		 * @tparam NativeElement C element type.
		 * @tparam Element C++ element type (constructed from `const NativeElement&`).
		 */
		template<class NativeElement, class Element>
		class converting_iterator {
		public:
			/// @brief C++20 iterator concept.
			using iterator_concept	= std::random_access_iterator_tag;
			/// @brief Legacy iterator category (the reference is a value, so only an input iterator in C++17 terms).
			using iterator_category = std::input_iterator_tag;
			/// @brief Value type.
			using value_type		= Element;
			/// @brief Difference type.
			using difference_type	= std::ptrdiff_t;

		private:
			/// @brief Current element.
			const NativeElement* _ptr = nullptr;

		public:
			/// @brief Default constructor (singular iterator).
			constexpr converting_iterator() noexcept = default;

			/// @brief Creates iterator pointing to given element.
			constexpr explicit converting_iterator(const NativeElement* const ptr) noexcept : _ptr(ptr) {}

			/// @brief Returns current element.
			[[nodiscard]] constexpr value_type operator*() const noexcept { return static_cast<value_type>(*_ptr); }

			/// @brief Returns element with given offset.
			[[nodiscard]] constexpr value_type operator[](const difference_type n) const noexcept {
				return static_cast<value_type>(_ptr[n]);
			}

			/// @brief Pre-increment.
			constexpr converting_iterator& operator++() noexcept {
				++_ptr;
				return *this;
			}

			/// @brief Post-increment.
			constexpr converting_iterator operator++(int) noexcept {
				converting_iterator copy = *this;
				++_ptr;
				return copy;
			}

			/// @brief Pre-decrement.
			constexpr converting_iterator& operator--() noexcept {
				--_ptr;
				return *this;
			}

			/// @brief Post-decrement.
			constexpr converting_iterator operator--(int) noexcept {
				converting_iterator copy = *this;
				--_ptr;
				return copy;
			}

			/// @brief Advance.
			constexpr converting_iterator& operator+=(const difference_type n) noexcept {
				_ptr += n;
				return *this;
			}

			/// @brief Advance backwards.
			constexpr converting_iterator& operator-=(const difference_type n) noexcept {
				_ptr -= n;
				return *this;
			}

			/// @brief Returns advanced iterator.
			[[nodiscard]] friend constexpr converting_iterator operator+(converting_iterator it,
			  const difference_type n) noexcept {
				return it += n;
			}

			/// @brief Returns advanced iterator.
			[[nodiscard]] friend constexpr converting_iterator operator+(const difference_type n,
			  converting_iterator it) noexcept {
				return it += n;
			}

			/// @brief Returns iterator moved backwards.
			[[nodiscard]] friend constexpr converting_iterator operator-(converting_iterator it,
			  const difference_type n) noexcept {
				return it -= n;
			}

			/// @brief Distance between iterators.
			[[nodiscard]] friend constexpr difference_type operator-(const converting_iterator& lhs,
			  const converting_iterator& rhs) noexcept {
				return lhs._ptr - rhs._ptr;
			}

			/// @brief Equality operator.
			[[nodiscard]] friend constexpr bool operator==(const converting_iterator&,
			  const converting_iterator&) noexcept = default;

			/// @brief Three-way comparison operator.
			[[nodiscard]] friend constexpr auto operator<=>(const converting_iterator&,
			  const converting_iterator&) noexcept = default;
		};

		/// @brief Creates C string view on given characters (no copy, the C lookups do not keep it).
		[[nodiscard]] inline tpbrt_string_t to_native_string(const std::string_view str) noexcept {
			return tpbrt_string_t { .data = str.data(), .size = str.size() };
		}

		/**
		 * @brief Calls a C lookup function `tpbrt_error_t fn(args..., const NativeElement** out)` and wraps the result.
		 * @tparam Element C++ wrapper of found element.
		 * @tparam NativeElement C element type.
		 */
		template<class Element, class NativeElement, class Fn, class... Args>
		[[nodiscard]] result<Element> find_in(Fn fn, const Args&... args) noexcept {
			const NativeElement* found = nullptr;
				if (const auto err = static_cast<error>(fn(args..., &found)); err != error::NONE) { return err; }
				if (found == nullptr) { return error::NOT_FOUND; }
			return Element(*found);
		}

		/// @brief Marker base of tpbrt::array (used to mark every array as a borrowed range).
		struct array_base {};
	} // namespace detail

	/**
	 * @brief C++ wrapper for tpbrt_string_t (a view on characters owned by the C library).
	 * @ingroup common-cpp-public
	 */
	struct string : detail::wrapper<string, tpbrt_string_t> {
		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns data value.
		[[nodiscard]] constexpr const char_t* data() const noexcept { return _native->data; }

		/// @brief Returns size value.
		[[nodiscard]] constexpr size_t size() const noexcept { return _native->size; }

		/// @brief Returns string length.
		[[nodiscard]] constexpr size_t length() const noexcept { return size(); }

		/// @brief Returns true if string is empty.
		[[nodiscard]] constexpr bool_t empty() const noexcept { return data() == nullptr || size() == 0; }

		/// @brief Conversion to std::string_view.
		[[nodiscard]] constexpr std::string_view to_string_view() const noexcept {
			const char_t* data_ptr = data();
				if (data_ptr == nullptr) { return {}; }
			return std::string_view(data_ptr, size());
		}

		/// @brief Conversion operator to std::string_view.
		[[nodiscard]] constexpr operator std::string_view() const noexcept { return to_string_view(); } // NOLINT

		/// @brief Returns begin iterator.
		[[nodiscard]] constexpr auto begin() const noexcept { return to_string_view().begin(); }

		/// @brief Returns end iterator.
		[[nodiscard]] constexpr auto end() const noexcept { return to_string_view().end(); }

		/// @brief Three-way comparison operator (compares characters).
		[[nodiscard]] constexpr std::strong_ordering operator<=>(const string& rhs) const noexcept {
			return to_string_view() <=> rhs.to_string_view();
		}

		/// @brief Equality operator (compares characters).
		[[nodiscard]] constexpr bool_t operator==(const string& rhs) const noexcept {
			return to_string_view() == rhs.to_string_view();
		}

		/// @brief Three-way comparison operator (compares characters).
		[[nodiscard]] constexpr std::strong_ordering operator<=>(const std::string_view rhs) const noexcept {
			return to_string_view() <=> rhs;
		}

		/// @brief Equality operator (compares characters).
		[[nodiscard]] constexpr bool_t operator==(const std::string_view rhs) const noexcept { return to_string_view() == rhs; }
	};

	/**
	 * @brief Generic C++ array view with type conversion (a non owning view on memory owned by the C library).
	 * @details Behaves like std::span: operator[] is unchecked, at() returns tpbrt::result with INDEX_OUT_OF_RANGE.
	 * Iterators stay valid after the array object itself is destroyed (borrowed range), as long as the scene lives.
	 * @tparam Native Type of C library native array type.
	 * @tparam Element Desired array element type (if different from the native element type, every element is converted).
	 * @tparam DataField Pointer to field with data.
	 * @ingroup common-cpp-public
	 */
	template<class Native, class Element, auto DataField>
	struct array : detail::wrapper<array<Native, Element, DataField>, Native>,
				   detail::array_base {
		/// @brief Default wrapper constructor.
		using detail::wrapper<array, Native>::wrapper;

		/// @brief Array traits.
		using traits					   = detail::array_traits<Native, DataField>;

		/// @brief Native C library array element type.
		using native_element_type		   = traits::native_element_type;

		/// @brief C++ array view element type.
		using element_type				   = std::conditional_t<std::is_void_v<Element>, native_element_type, Element>;

		/// @brief Value type (as in std::span).
		using value_type				   = element_type;

		/// @brief std::span type of this array.
		using span_type					   = traits::span_type;

		/// @brief Array static size.
		static constexpr size_t Extent	   = traits::Extent;

		/// @brief True when elements are returned without conversion (as references to the C data).
		static constexpr bool_t IsIdentity = std::is_same_v<native_element_type, element_type>;

		/// @brief Iterator type.
		using iterator					   = std::conditional_t<IsIdentity, typename span_type::iterator,
		  detail::converting_iterator<native_element_type, element_type>>;

		/// @brief Type returned by operator[] (a reference for identity arrays, a converted value otherwise).
		using reference					   = std::conditional_t<IsIdentity, const native_element_type&, element_type>;

	protected:
		/// @brief C native struct.
		using detail::wrapper<array, Native>::_native;

		/// @brief Returns std::span over the C data.
		[[nodiscard]] constexpr span_type _raw_span() const noexcept { return traits::get_span(*_native); }

	public:
		/// @brief Returns size of this array.
		[[nodiscard]] constexpr size_t size() const noexcept { return _raw_span().size(); }

		/// @brief Returns true if array is empty.
		[[nodiscard]] constexpr bool_t empty() const noexcept { return _raw_span().empty(); }

		/// @brief Returns pointer to the C data.
		[[nodiscard]] constexpr const native_element_type* data() const noexcept { return _raw_span().data(); }

		/// @brief Returns std::span over the C data.
		[[nodiscard]] constexpr span_type to_span() const noexcept { return _raw_span(); }

		/// @brief Returns begin iterator.
		[[nodiscard]] constexpr iterator begin() const noexcept {
				if constexpr (IsIdentity) { return _raw_span().begin(); }
				else { return iterator(_raw_span().data()); }
		}

		/// @brief Returns end iterator.
		[[nodiscard]] constexpr iterator end() const noexcept {
				if constexpr (IsIdentity) { return _raw_span().end(); }
				else { return iterator(_raw_span().data() + _raw_span().size()); }
		}

		/// @brief Returns element on given index (unchecked, like std::span).
		[[nodiscard]] constexpr reference operator[](const size_t index) const noexcept {
				if constexpr (IsIdentity) { return _raw_span()[index]; }
				else { return static_cast<element_type>(_raw_span()[index]); }
		}

		/// @brief Returns element on given index or error::INDEX_OUT_OF_RANGE.
		[[nodiscard]] constexpr result<element_type> at(const size_t index) const noexcept {
				if (index >= size()) { return error::INDEX_OUT_OF_RANGE; }
			return element_type((*this)[index]);
		}

		/// @brief Returns first element (unchecked).
		[[nodiscard]] constexpr reference front() const noexcept { return (*this)[0]; }

		/// @brief Returns last element (unchecked).
		[[nodiscard]] constexpr reference back() const noexcept { return (*this)[size() - 1]; }

		/// @brief Three-way comparison operator (compares elements).
		[[nodiscard]] constexpr auto operator<=>(const array& rhs) const noexcept {
			return std::lexicographical_compare_three_way(begin(), end(), rhs.begin(), rhs.end());
		}

		/// @brief Equality operator (compares elements).
		[[nodiscard]] constexpr bool_t operator==(const array& rhs) const noexcept {
			return std::ranges::equal(begin(), end(), rhs.begin(), rhs.end());
		}
	};

	/**
	 * @brief C++ wrapper for tpbrt_string_array_t.
	 * @ingroup common-cpp-public
	 */
	struct string_array : array<tpbrt_string_array_t, string> {
		/// @brief Default array constructor.
		using array::array;
	};
} // namespace tpbrt

// Every tpbrt::array (and every type derived from it) is a view whose iterators do not depend on the view object.
/// @cond
template<class T>
requires std::derived_from<T, tpbrt::detail::array_base>
inline constexpr bool std::ranges::enable_borrowed_range<T> = true; // NOLINT
/// @endcond

#endif // _TINYPBRT_COMMON_HPP_
