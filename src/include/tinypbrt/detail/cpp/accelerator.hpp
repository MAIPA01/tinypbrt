#pragma once
#ifndef _TINYPBRT_ACCELERATOR_HPP_
#define _TINYPBRT_ACCELERATOR_HPP_

#include <tinypbrt/detail/cpp/fwd.hpp>

#include <tinypbrt/detail/accelerator.h>

#include <string_view>
#include <tuple>

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
			switch (type) {
			case accelerator_type::BVH:		return "bvh";
			case accelerator_type::KD_TREE: return "kdtree";
			case accelerator_type::MAX_NUM: return "maxnum";
			default:						return "UNKNOWN";
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
			switch (method) {
			case accelerator_bvh_split_method::SAH:		return "sah";
			case accelerator_bvh_split_method::MIDDLE:	return "middle";
			case accelerator_bvh_split_method::EQUAL:	return "equal";
			case accelerator_bvh_split_method::HLBVH:	return "hlbvh";
			case accelerator_bvh_split_method::MAX_NUM: return "maxnum";
			default:									return "UNKNOWN";
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
		/// @brief C bvh parameters struct.
		native_type _native;

	public:
		/// @brief C to C++ wrapper conversion constructor.
		constexpr accelerator_bvh_params(const native_type& native) noexcept : _native(native) {} // NOLINT

		/// @brief Default copy constructor.
		constexpr accelerator_bvh_params(const accelerator_bvh_params&) noexcept = default;
		/// @brief Default move constructor.
		constexpr accelerator_bvh_params(accelerator_bvh_params&&) noexcept		 = default;

		/// @brief Default destructor.
		constexpr ~accelerator_bvh_params() noexcept							 = default;

		/// @brief Convertion assign operator.
		constexpr accelerator_bvh_params& operator=(const native_type& native) noexcept {
			_native = native;
			return *this;
		}

		/// @brief Default copy assign operator.
		constexpr accelerator_bvh_params& operator=(const accelerator_bvh_params&) noexcept = default;
		/// @brief Default move assign operator.
		constexpr accelerator_bvh_params& operator=(accelerator_bvh_params&&) noexcept		= default;

		/// @brief Returns max node prims value.
		[[nodiscard]] constexpr uint_t max_node_prims() const noexcept { return _native.max_node_prims; }

		/// @brief Returns split method value.
		[[nodiscard]] constexpr accelerator_bvh_split_method split_method() const noexcept {
			return static_cast<accelerator_bvh_split_method>(_native.split_method);
		}

		/// @brief Conversion operator to native type.
		[[nodiscard]] constexpr operator const native_type&() const noexcept { return _native; } // NOLINT

		/// @brief Conversion operator to native type.
		[[nodiscard]] constexpr operator native_type&() noexcept { return _native; } // NOLINT

		/// @brief Conversion operator to native type.
		[[nodiscard]] constexpr operator const native_type*() const noexcept { return &_native; } // NOLINT

		/// @brief Conversion operator to native type.
		[[nodiscard]] constexpr operator native_type*() noexcept { return &_native; } // NOLINT

		/// @brief Conversion operator to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept {
			return std::make_tuple(max_node_prims(), split_method());
		}

		/// @brief Space ship operator.
		[[nodiscard]] constexpr auto operator<=>(const accelerator_bvh_params& rhs) const noexcept {
			return reflect() <=> rhs.reflect();
		}

		/// @brief Equal operator.
		[[nodiscard]] constexpr bool operator==(const accelerator_bvh_params& rhs) const noexcept {
			return operator<=>(rhs) == 0;
		}

		/// @brief Not equal operator.
		[[nodiscard]] constexpr bool operator!=(const accelerator_bvh_params& rhs) const noexcept { return !operator==(rhs); }
	};

	/**
	 * @brief C++ wrapper for tpbrt_accelerator_kd_tree_params_t
	 * @ingroup accelerator-cpp-public
	 */
	struct accelerator_kd_tree_params {
		/// @brief Native C library type for this struct.
		using native_type  = tpbrt_accelerator_kd_tree_params_t;

		/// @brief Type used for reflection.
		using reflect_type = std::tuple<uint_t, uint_t, float_t, uint_t, int_t>;

	private:
		/// @brief C bvh parameters struct.
		native_type _native;

	public:
		/// @brief C to C++ wrapper conversion constructor.
		constexpr accelerator_kd_tree_params(const native_type& native) noexcept // NOLINT
			: _native(native) {}

		/// @brief Default copy constructor.
		constexpr accelerator_kd_tree_params(const accelerator_kd_tree_params&) noexcept = default;
		/// @brief Default move constructor.
		constexpr accelerator_kd_tree_params(accelerator_kd_tree_params&&) noexcept		 = default;

		/// @brief Default destructor.
		constexpr ~accelerator_kd_tree_params() noexcept								 = default;

		/// @brief Convertion assign operator.
		constexpr accelerator_kd_tree_params& operator=(const native_type& native) noexcept {
			_native = native;
			return *this;
		}

		/// @brief Default copy assign operator.
		constexpr accelerator_kd_tree_params& operator=(const accelerator_kd_tree_params&) noexcept = default;
		/// @brief Default move assign operator.
		constexpr accelerator_kd_tree_params& operator=(accelerator_kd_tree_params&&) noexcept		= default;

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

		/// @brief Conversion operator to native type.
		[[nodiscard]] operator const native_type&() const noexcept { return _native; } // NOLINT

		/// @brief Conversion operator to native type.
		[[nodiscard]] operator native_type&() noexcept { return _native; } // NOLINT

		/// @brief Conversion operator to native type.
		[[nodiscard]] operator const native_type*() const noexcept { return &_native; } // NOLINT

		/// @brief Conversion operator to native type.
		[[nodiscard]] operator native_type*() noexcept { return &_native; } // NOLINT

		/// @brief Conversion operator to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept {
			return std::make_tuple(_native.intersect_cost, _native.traversal_cost, _native.empty_bonus, _native.max_prims,
			  _native.max_depth);
		}

		/// @brief Space ship operator.
		[[nodiscard]] constexpr auto operator<=>(const accelerator_kd_tree_params& rhs) const noexcept {
			return reflect() <=> rhs.reflect();
		}

		/// @brief Equal operator.
		[[nodiscard]] constexpr bool operator==(const accelerator_kd_tree_params& rhs) const noexcept {
			return operator<=>(rhs) == 0;
		}

		/// @brief Not equal operator.
		[[nodiscard]] constexpr bool operator!=(const accelerator_kd_tree_params& rhs) const noexcept { return !operator==(rhs); }
	};

	/**
	 * @brief C++ wrapper for tpbrt_accelerator_t
	 * @ingroup accelerator-cpp-public
	 */
	struct accelerator {
		/// @brief Native C library type for this struct.
		using native_type  = tpbrt_accelerator_t;

		/// @brief Type used for reflection.
		using reflect_type = optional_variant<accelerator_bvh_params, accelerator_kd_tree_params>;

	private:
		/// @brief C bvh parameters struct.
		native_type _native;

	public:
		/// @brief C to C++ wrapper conversion constructor.
		constexpr accelerator(const native_type& native) noexcept : _native(native) {} // NOLINT

		/// @brief Default copy constructor.
		constexpr accelerator(const accelerator&) noexcept = default;
		/// @brief Default move constructor.
		constexpr accelerator(accelerator&&) noexcept	   = default;

		/// @brief Default destructor.
		constexpr ~accelerator() noexcept				   = default;

		/// @brief Convertion assign operator.
		constexpr accelerator& operator=(const native_type& native) noexcept {
			_native = native;
			return *this;
		}

		/// @brief Default copy assign operator.
		constexpr accelerator& operator=(const accelerator& rhs) noexcept = default;
		/// @brief Default move assign operator.
		constexpr accelerator& operator=(accelerator&&) noexcept		  = default;

		/// @brief Returns type value.
		[[nodiscard]] constexpr accelerator_type type() const noexcept { return static_cast<accelerator_type>(_native.type); }

		/// @brief Returns if accelerator type is bvh.
		[[nodiscard]] constexpr bool is_bvh() const noexcept { return type() == accelerator_type::BVH; }

		/// @brief Returns if accelerator type is kd tree.
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

		/// @brief Visits accelerator parameters.
		constexpr auto visit(const auto visitor) const noexcept {
			const reflect_type res = reflect();
				if (res.has_value()) { return; }
			return std::visit(visitor, res);
		}

		/// @brief Conversion operator to native type.
		[[nodiscard]] constexpr operator const native_type&() const noexcept { return _native; } // NOLINT

		/// @brief Conversion operator to native type.
		[[nodiscard]] constexpr operator native_type&() noexcept { return _native; } // NOLINT

		/// @brief Conversion operator to native type.
		[[nodiscard]] constexpr operator const native_type*() const noexcept { return &_native; } // NOLINT

		/// @brief Conversion operator to native type.
		[[nodiscard]] constexpr operator native_type*() noexcept { return &_native; } // NOLINT

		/// @brief Conversion operator to variant.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept {
				switch (_native.type) {
				case TPBRT_ACCELERATOR_TYPE_BVH:	 return _native.as.bvh;
				case TPBRT_ACCELERATOR_TYPE_KD_TREE: return _native.as.kd_tree;
				case TPBRT_ACCELERATOR_TYPE_MAX_NUM: [[fallthrough]];
				default:							 break;
				}
			return std::nullopt;
		}

		/// @brief Space ship operator.
		[[nodiscard]] constexpr auto operator<=>(const accelerator& rhs) const noexcept { return reflect() <=> rhs.reflect(); }

		/// @brief Equal operator.
		[[nodiscard]] constexpr bool operator==(const accelerator& rhs) const noexcept { return operator<=>(rhs) == 0; }

		/// @brief Not equal operator.
		[[nodiscard]] constexpr bool operator!=(const accelerator& rhs) const noexcept { return !operator==(rhs); }
	};
} // namespace tpbrt
#endif // _TINYPBRT_ACCELERATOR_HPP_
