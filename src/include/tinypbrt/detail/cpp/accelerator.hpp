#pragma once
#ifndef _TINYPBRT_ACCELERATOR_HPP_
#define _TINYPBRT_ACCELERATOR_HPP_

#include <tinypbrt/detail/cpp/fwd.hpp>

#include <tinypbrt/detail/accelerator.h>

#include <string_view>
#include <tuple>
#include <type_traits>

namespace tpbrt {
	/**
	 * @defgroup accelerator-cpp-public Accelerator
	 * @ingroup tinypbrt-cpp-public
	 */

	/**
	 * @copybrief tpbrt_accelerator_type_t
	 * @ingroup accelerator-cpp-public
	 */
	enum class accelerator_type : char_t {
		/// @copybrief TPBRT_ACCELERATOR_TYPE_BVH
		BVH		= TPBRT_ACCELERATOR_TYPE_BVH,
		/// @copybrief TPBRT_ACCELERATOR_TYPE_KD_TREE
		KD_TREE = TPBRT_ACCELERATOR_TYPE_KD_TREE,
		/// @copybrief TPBRT_ACCELERATOR_TYPE_MAX_NUM
		MAX_NUM = TPBRT_ACCELERATOR_TYPE_MAX_NUM,
	};

	/**
	 * @brief Converts accelerator_type enum value to string.
	 * @param type Accelerator type.
	 * @return String value of accelerator type.
	 * @ingroup accelerator-cpp-public
	 */
	[[nodiscard]] constexpr std::string_view to_string(const accelerator_type type) noexcept {
		using enum accelerator_type;
			switch (type) {
			case BVH:	  return "bvh";
			case KD_TREE: return "kdtree";
			case MAX_NUM: return "maxnum";
			default:	  return "UNKNOWN";
			}
	}

	/**
	 * @copybrief tpbrt_accelerator_bvh_split_method_t
	 * @ingroup accelerator-cpp-public
	 */
	enum class accelerator_bvh_split_method : char_t {
		/// @copybrief TPBRT_ACCELERATOR_BVH_SPLIT_METHOD_SAH
		SAH		= TPBRT_ACCELERATOR_BVH_SPLIT_METHOD_SAH,
		/// @copybrief TPBRT_ACCELERATOR_BVH_SPLIT_METHOD_MIDDLE
		MIDDLE	= TPBRT_ACCELERATOR_BVH_SPLIT_METHOD_MIDDLE,
		/// @copybrief TPBRT_ACCELERATOR_BVH_SPLIT_METHOD_EQUAL
		EQUAL	= TPBRT_ACCELERATOR_BVH_SPLIT_METHOD_EQUAL,
		/// @copybrief TPBRT_ACCELERATOR_BVH_SPLIT_METHOD_HLBVH
		HLBVH	= TPBRT_ACCELERATOR_BVH_SPLIT_METHOD_HLBVH,
		/// @copybrief TPBRT_ACCELERATOR_BVH_SPLIT_METHOD_MAX_NUM
		MAX_NUM = TPBRT_ACCELERATOR_BVH_SPLIT_METHOD_MAX_NUM,
	};

	/**
	 * @brief Converts accelerator_bvh_split_method enum value to string.
	 * @param method Accelerator bvh split method.
	 * @return String value of accelerator bvh split method.
	 * @ingroup accelerator-cpp-public
	 */
	[[nodiscard]] constexpr std::string_view to_string(const accelerator_bvh_split_method method) noexcept {
		using enum accelerator_bvh_split_method;
			switch (method) {
			case SAH:	  return "sah";
			case MIDDLE:  return "middle";
			case EQUAL:	  return "equal";
			case HLBVH:	  return "hlbvh";
			case MAX_NUM: return "maxnum";
			default:	  return "UNKNOWN";
			}
	}

	/**
	 * @brief C++ wrapper for tpbrt_accelerator_bvh_params_t.
	 * @ingroup accelerator-cpp-public
	 */
	struct accelerator_bvh_params {
		/// @brief Native C library type for this struct.
		using native_type  = tpbrt_accelerator_bvh_params_t;

		/// @brief Type used for reflection.
		using reflect_type = std::tuple<uint_t, accelerator_bvh_split_method>;

	private:
		/// @brief C native struct.
		native_type _native;

	public:
		/// @brief C to C++ wrapper conversion constructor.
		constexpr accelerator_bvh_params(const native_type& native) noexcept : _native(native) {} // NOLINT

		/// @brief C to C++ wrapper convertion assign operator.
		constexpr accelerator_bvh_params& operator=(const native_type& native) noexcept {
			_native = native;
			return *this;
		}

		/// @brief Returns max_node_prims value.
		[[nodiscard]] constexpr uint_t max_node_prims() const noexcept { return _native.max_node_prims; }

		/// @brief Returns split_method value.
		[[nodiscard]] constexpr accelerator_bvh_split_method split_method() const noexcept {
			return static_cast<accelerator_bvh_split_method>(_native.split_method);
		}

		/// @brief Conversion operator to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept {
			return std::make_tuple(max_node_prims(), split_method());
		}

		/// @brief Conversion operator to native type.
		[[nodiscard]] constexpr operator const native_type&() const noexcept { return _native; } // NOLINT

		/// @brief Conversion operator to native type.
		[[nodiscard]] constexpr operator native_type&() noexcept { return _native; } // NOLINT

		/// @brief Conversion operator to native type.
		[[nodiscard]] constexpr operator const native_type*() const noexcept { return &_native; } // NOLINT

		/// @brief Conversion operator to native type.
		[[nodiscard]] constexpr operator native_type*() noexcept { return &_native; } // NOLINT

		/// @brief Space ship operator.
		[[nodiscard]] constexpr auto operator<=>(const accelerator_bvh_params& rhs) const noexcept {
			return reflect() <=> rhs.reflect();
		}

		/// @brief Equal operator.
		[[nodiscard]] constexpr bool operator==(const accelerator_bvh_params& rhs) const noexcept {
			return max_node_prims() == rhs.max_node_prims() && split_method() == rhs.split_method();
		}
	};

	/**
	 * @brief C++ wrapper for tpbrt_accelerator_kd_tree_params_t.
	 * @ingroup accelerator-cpp-public
	 */
	struct accelerator_kd_tree_params {
		/// @brief Native C library type for this struct.
		using native_type  = tpbrt_accelerator_kd_tree_params_t;

		/// @brief Type used for reflection.
		using reflect_type = std::tuple<uint_t, uint_t, float_t, uint_t, int_t>;

	private:
		/// @brief C native struct.
		native_type _native;

	public:
		/// @brief C to C++ wrapper conversion constructor.
		constexpr accelerator_kd_tree_params(const native_type& native) noexcept // NOLINT
			: _native(native) {}

		/// @brief C to C++ wrapper convertion assign operator.
		constexpr accelerator_kd_tree_params& operator=(const native_type& native) noexcept {
			_native = native;
			return *this;
		}

		/// @brief Returns intersect_cost value.
		[[nodiscard]] constexpr uint_t intersect_cost() const noexcept { return _native.intersect_cost; }

		/// @brief Returns traversal_cost value.
		[[nodiscard]] constexpr uint_t traversal_cost() const noexcept { return _native.traversal_cost; }

		/// @brief Returns empty_bonus value.
		[[nodiscard]] constexpr float_t empty_bonus() const noexcept { return _native.empty_bonus; }

		/// @brief Returns max_prims value.
		[[nodiscard]] constexpr uint_t max_prims() const noexcept { return _native.max_prims; }

		/// @brief Returns max_depth value.
		[[nodiscard]] constexpr int_t max_depth() const noexcept { return _native.max_depth; }

		/// @brief Conversion operator to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept {
			return std::make_tuple(intersect_cost(), traversal_cost(), empty_bonus(), max_prims(), max_depth());
		}

		/// @brief Conversion operator to native type.
		[[nodiscard]] operator const native_type&() const noexcept { return _native; } // NOLINT

		/// @brief Conversion operator to native type.
		[[nodiscard]] operator native_type&() noexcept { return _native; } // NOLINT

		/// @brief Conversion operator to native type.
		[[nodiscard]] operator const native_type*() const noexcept { return &_native; } // NOLINT

		/// @brief Conversion operator to native type.
		[[nodiscard]] operator native_type*() noexcept { return &_native; } // NOLINT

		/// @brief Space ship operator.
		[[nodiscard]] constexpr auto operator<=>(const accelerator_kd_tree_params& rhs) const noexcept {
			return reflect() <=> rhs.reflect();
		}

		/// @brief Equal operator.
		[[nodiscard]] constexpr bool operator==(const accelerator_kd_tree_params& rhs) const noexcept {
			return intersect_cost() == rhs.intersect_cost() && traversal_cost() == rhs.traversal_cost() &&
				   empty_bonus() == rhs.empty_bonus() && max_prims() == rhs.max_prims() && max_depth() == rhs.max_depth();
		}
	};

	/**
	 * @brief C++ wrapper for tpbrt_accelerator_t.
	 * @ingroup accelerator-cpp-public
	 */
	struct accelerator {
		/// @brief Native C library type for this struct.
		using native_type  = tpbrt_accelerator_t;

		/// @brief Type used for better reading tagged union of parameters.
		using variant_type = optional_variant<accelerator_bvh_params, accelerator_kd_tree_params>;

		/// @brief Type used for reflection.
		using reflect_type = std::tuple<variant_type, accelerator_type>;

	private:
		/// @brief C native struct.
		native_type _native;

	public:
		/// @brief C to C++ wrapper conversion constructor.
		constexpr accelerator(const native_type& native) noexcept : _native(native) {} // NOLINT

		/// @brief C to C++ wrapper convertion assign operator.
		constexpr accelerator& operator=(const native_type& native) noexcept {
			_native = native;
			return *this;
		}

		/// @brief Returns type value.
		[[nodiscard]] constexpr accelerator_type type() const noexcept { return static_cast<accelerator_type>(_native.type); }

		/// @brief Returns if type is bvh.
		[[nodiscard]] constexpr bool is_bvh() const noexcept { return type() == accelerator_type::BVH; }

		/// @brief Returns if type is kd tree.
		[[nodiscard]] constexpr bool is_kd_tree() const noexcept { return type() == accelerator_type::KD_TREE; }

		/// @brief Returns accelerator parameters as bvh.
		[[nodiscard]] constexpr std::optional<accelerator_bvh_params> as_bvh() const noexcept {
				if (!is_bvh()) { return std::nullopt; }
			return _native.as.bvh;
		}

		/// @brief Returns accelerator parameters as kd tree.
		[[nodiscard]] constexpr std::optional<accelerator_kd_tree_params> as_kd_tree() const noexcept {
				if (!is_kd_tree()) { return std::nullopt; }
			return _native.as.kd_tree;
		}

		/// @brief Conversion operator to variant.
		[[nodiscard]] constexpr variant_type to_variant() const noexcept {
			using enum accelerator_type;
				switch (type()) {
				case BVH:	  return _native.as.bvh;
				case KD_TREE: return _native.as.kd_tree;
				case MAX_NUM: [[fallthrough]];
				default:	  break;
				}
			return std::nullopt;
		}

		/// @brief Visits accelerator parameters.
		constexpr auto visit(const auto& visitor) const noexcept {
			const variant_type res = to_variant();

			using return_type	   = decltype(std::visit(visitor, res.value()));

				if constexpr (std::is_void_v<return_type>) {
						if (res.has_value()) { return std::visit(visitor, res.value()); }
				}
				else {
						if (res.has_value()) { return std::optional<return_type>(std::visit(visitor, res.value())); }
					return std::optional<return_type>(std::nullopt);
				}
		}

		/// @brief Conversion operator to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept { return std::make_tuple(to_variant(), type()); }

		/// @brief Conversion operator to native type.
		[[nodiscard]] constexpr operator const native_type&() const noexcept { return _native; } // NOLINT

		/// @brief Conversion operator to native type.
		[[nodiscard]] constexpr operator native_type&() noexcept { return _native; } // NOLINT

		/// @brief Conversion operator to native type.
		[[nodiscard]] constexpr operator const native_type*() const noexcept { return &_native; } // NOLINT

		/// @brief Conversion operator to native type.
		[[nodiscard]] constexpr operator native_type*() noexcept { return &_native; } // NOLINT

		/// @brief Space ship operator.
		[[nodiscard]] constexpr auto operator<=>(const accelerator& rhs) const noexcept {
			return to_variant() <=> rhs.to_variant();
		}

		/// @brief Equal operator.
		[[nodiscard]] constexpr bool operator==(const accelerator& rhs) const noexcept {
				if (type() != rhs.type()) { return false; }
			return to_variant() == rhs.to_variant();
		}
	};
} // namespace tpbrt
#endif // _TINYPBRT_ACCELERATOR_HPP_
