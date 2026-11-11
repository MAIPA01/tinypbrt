#pragma once
#ifndef _TINYPBRT_OBJECT_INSTANCE_HPP_
#define _TINYPBRT_OBJECT_INSTANCE_HPP_

#include <tinypbrt/detail/cpp/fwd.hpp>

#include <tinypbrt/detail/object_instance.h>

#include <tinypbrt/detail/cpp/object.hpp>

namespace tpbrt {
	/**
	 * @defgroup object-instance-cpp-public Object Instance
	 * @ingroup tinypbrt-cpp-public
	 */

	/**
	 * @brief C++ wrapper for tpbrt_object_instance_t.
	 * @ingroup object-instance-cpp-public
	 */
	struct object_instance : wrapper<object_instance, tpbrt_object_instance_t> {
		/// @brief Type used for reflection.
		using reflect_type = std::tuple<object_handle, transform_animated>;

		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns object value.
		[[nodiscard]] constexpr object_handle object() const noexcept { return _native.object; }

		/// @brief Returns transform value.
		[[nodiscard]] constexpr transform_animated transform() const noexcept { return _native.transform; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept { return std::make_tuple(object(), transform()); }
	};
} // namespace tpbrt
#endif // _TINYPBRT_OBJECT_INSTANCE_HPP_
