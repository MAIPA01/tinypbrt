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
	struct object : wrapper<object, tpbrt_object_t> {
		/// @brief Type used for reflection.
		using reflect_type = std::tuple<string, shapes_list, object_idx_t>;

		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns name value.
		[[nodiscard]] constexpr string name() const noexcept { return _native.name; }

		/// @brief Returns children value.
		[[nodiscard]] constexpr shapes_list children() const noexcept { return _native.children; }

		/// @brief Returns begin iterator for children.
		[[nodiscard]] constexpr auto begin() const noexcept { return children().begin(); }

		/// @brief Returns end iterator for children.
		[[nodiscard]] constexpr auto end() const noexcept { return children().end(); }

		/// @brief Returns idx value.
		[[nodiscard]] constexpr object_idx_t idx() const noexcept { return _native.idx; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept { return std::make_tuple(name(), children(), idx()); }
	};

	/**
	 * @brief C++ wrapper for tpbrt_object_handle_t.
	 * @ingroup object-cpp-public
	 */
	struct object_handle : wrapper<object_handle, tpbrt_object_handle_t> {
		/// @brief Type used for reflection.
		using reflect_type = std::tuple<object_idx_t>;

		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns object_idx value.
		[[nodiscard]] constexpr media_idx_t object_idx() const noexcept { return _native.object_idx; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept { return std::make_tuple(object_idx()); }
	};

	/**
	 * @brief Returns object to which handle is provided.
	 * @param objects_list List of objects in which object given by handle should be.
	 * @param handle Handle to object.
	 * @return Returns result with error code (most possible TPBRT_ERROR_NOT_FOUND, TPBRT_ERROR_NONE) or with object.
	 * @ingroup object-cpp-public
	 */
	[[nodiscard]] constexpr result<object> get_object(objects_list objects_list, object_handle handle) noexcept;

	/**
	 * @brief Returns object with given name.
	 * @param objects_list List of objects in which searched object should be.
	 * @param name Name of the searched object.
	 * @return Returns result with error code (most possible TPBRT_ERROR_NOT_FOUND, TPBRT_ERROR_NONE) or with object.
	 * @ingroup object-cpp-public
	 */
	[[nodiscard]] constexpr result<object> get_object(objects_list objects_list, string name) noexcept;

	/**
	 * @brief C++ wrapper for tpbrt_objects_list_t.
	 * @ingroup object-cpp-public
	 */
	struct objects_list : array<tpbrt_objects_list_t, object, &tpbrt_objects_list_t::objects> {
		/// @brief Base array constructor.
		using array::array;

		/// @brief Returns element pointed by given handle.
		[[nodiscard]] constexpr result<element_type> at(const object_handle handle) const noexcept {
			return get_object(*this, handle);
		}

		/// @brief Returns element pointed by given name.
		[[nodiscard]] constexpr result<element_type> at(const string name) const noexcept { return get_object(*this, name); }

		/// @brief Returns element pointed by given handle.
		[[nodiscard]] constexpr result<element_type> operator[](const object_handle handle) const noexcept { return at(handle); }

		/// @brief Returns element pointed by given name.
		[[nodiscard]] constexpr result<element_type> operator[](const string name) const noexcept { return at(name); }
	};

	constexpr result<object> get_object(const objects_list objects_list, const object_handle handle) noexcept {
		const tpbrt_object_t* c_object = nullptr;

		const auto err				   = static_cast<error>(tpbrt_get_object_by_handle(objects_list, handle, &c_object));

			if (err == error::NONE && c_object != nullptr) { return object(*c_object); }

		return err;
	}

	constexpr result<object> get_object(const objects_list objects_list, const string name) noexcept {
		const tpbrt_object_t* c_object = nullptr;

		const auto err				   = static_cast<error>(tpbrt_get_object_by_name(objects_list, name, &c_object));

			if (err == error::NONE && c_object != nullptr) { return object(*c_object); }

		return err;
	}
} // namespace tpbrt
#endif // _TINYPBRT_OBJECT_HPP_
