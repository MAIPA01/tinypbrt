#pragma once
#ifndef _TINYPBRT_OBJECT_HPP_
#define _TINYPBRT_OBJECT_HPP_

#include <tinypbrt/detail/cpp/fwd.hpp>

#include <tinypbrt/detail/object.h>

#include <tinypbrt/detail/cpp/shape.hpp>

namespace tpbrt {
	/**
	 * @defgroup object-cpp-public Object
	 * @ingroup tinypbrt-cpp-public
	 */

	/**
	 * @brief C++ wrapper for tpbrt_object_t.
	 * @ingroup object-cpp-public
	 */
	struct object : detail::wrapper<object, tpbrt_object_t> {
		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns name value.
		[[nodiscard]] constexpr string name() const noexcept { return _native->name; }

		/// @brief Returns children value.
		[[nodiscard]] constexpr shapes_list children() const noexcept { return _native->children; }

		/// @brief Returns begin iterator for children.
		[[nodiscard]] constexpr auto begin() const noexcept { return children().begin(); }

		/// @brief Returns end iterator for children.
		[[nodiscard]] constexpr auto end() const noexcept { return children().end(); }

		/// @brief Returns idx value.
		[[nodiscard]] constexpr object_idx_t idx() const noexcept { return _native->idx; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr auto reflect() const noexcept { return std::make_tuple(name(), children(), idx()); }
	};

	/**
	 * @brief C++ wrapper for tpbrt_object_handle_t.
	 * @ingroup object-cpp-public
	 */
	struct object_handle : detail::wrapper<object_handle, tpbrt_object_handle_t> {
		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns object_idx value.
		[[nodiscard]] constexpr object_idx_t object_idx() const noexcept { return _native->object_idx; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr auto reflect() const noexcept { return std::make_tuple(object_idx()); }
	};

	/**
	 * @brief C++ wrapper for tpbrt_objects_list_t.
	 * @details Index access (operator[], at(size_t), iteration) works as in tpbrt::array, at(handle) and at(name) use
	 * the C lookups and return error::NOT_FOUND (or the C error code) when there is no such object.
	 * @ingroup object-cpp-public
	 */
	struct objects_list : array<tpbrt_objects_list_t, object, &tpbrt_objects_list_t::objects> {
		/// @brief Base list constructor.
		using array::array;

		/// @brief Index based access from tpbrt::array.
		using array::at;

		/// @brief Operator for index based access from tpbrt::array.
		using array::operator[];

		/// @brief Returns object pointed by given handle.
		[[nodiscard]] result<element_type> at(const object_handle handle) const noexcept {
			return detail::find_in<element_type, tpbrt_object_t>(tpbrt_get_object_by_handle, _native, handle.native());
		}

		/// @brief Returns object with given name.
		[[nodiscard]] result<element_type> at(const std::string_view name) const noexcept {
			return detail::find_in<element_type, tpbrt_object_t>(tpbrt_get_object_by_name, _native,
			  detail::to_native_string(name));
		}

		/// @brief Returns object pointed by given handle.
		[[nodiscard]] result<element_type> operator[](const object_handle handle) const noexcept { return at(handle); }

		/// @brief Returns object with given name.
		[[nodiscard]] result<element_type> operator[](const std::string_view name) const noexcept { return at(name); }
	};

	/**
	 * @brief Returns object to which handle is provided.
	 * @param list List of objects in which object given by handle should be.
	 * @param handle Handle to object.
	 * @return Result with the object or with error code (most likely error::NOT_FOUND).
	 * @ingroup object-cpp-public
	 */
	[[nodiscard]] inline result<object> get_object(const objects_list list, const object_handle handle) noexcept {
		return list.at(handle);
	}

	/**
	 * @brief Returns object with given name.
	 * @param list List of objects in which searched object should be.
	 * @param name Name of the searched object.
	 * @return Result with the object or with error code (most likely error::NOT_FOUND).
	 * @ingroup object-cpp-public
	 */
	[[nodiscard]] inline result<object> get_object(const objects_list list, const std::string_view name) noexcept {
		return list.at(name);
	}
} // namespace tpbrt
#endif // _TINYPBRT_OBJECT_HPP_
