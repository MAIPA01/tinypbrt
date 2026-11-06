#pragma once
#ifndef _TINYPBRT_ACCELERATOR_H_
#define _TINYPBRT_ACCELERATOR_H_

#include <tinypbrt/detail/fwd.h>

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * @defgroup accelerator-public Accelerator
	 * @ingroup tinypbrt-public
	 */

	/**
	 * @brief Used to declare accelerator type.
	 * @ingroup accelerator-public
	 */
	typedef enum tpbrt_accelerator_type_t {
		/// @brief BVH acceleration.
		TPBRT_ACCELERATOR_TYPE_BVH	   = 0,
		/// @brief KD Tree acceleration.
		TPBRT_ACCELERATOR_TYPE_KD_TREE = 1,
		/// @brief Count of available accelerator types.
		TPBRT_ACCELERATOR_TYPE_MAX_NUM = 2,
	} tpbrt_accelerator_type_t;

	/**
	 * @brief Used to declare BVH split method.
	 * @ingroup accelerator-public
	 */
	typedef enum tpbrt_accelerator_bvh_split_method_t {
		/// @brief SAH split method. Denotes the surface area heuristic.
		TPBRT_ACCELERATOR_BVH_SPLIT_METHOD_SAH	   = 0,
		/// @brief Middle split method. Splits each node at its midpoint along the split axis.
		TPBRT_ACCELERATOR_BVH_SPLIT_METHOD_MIDDLE  = 1,
		/// @brief Equal split method. Splits the current group of primitives into two equal-sized sets.
		TPBRT_ACCELERATOR_BVH_SPLIT_METHOD_EQUAL   = 2,
		/// @brief HLBVH split method. Selects the HLBVH algorithm, which parallelizes well.
		TPBRT_ACCELERATOR_BVH_SPLIT_METHOD_HLBVH   = 3,
		/// @brief Count of available BVH split methods.
		TPBRT_ACCELERATOR_BVH_SPLIT_METHOD_MAX_NUM = 4,
	} tpbrt_accelerator_bvh_split_method_t;

	/**
	 * @brief BVH accelerator parameters.
	 * @ingroup accelerator-public
	 */
	typedef struct tpbrt_accelerator_bvh_params_t {
		/// @brief Maximum number of primitives to allow in a node in the tree.
		tpbrt_uint_t max_node_prims;
		/// @brief Method to use to partition the primitives when building the tree.
		tpbrt_accelerator_bvh_split_method_t split_method : 4;
	} tpbrt_accelerator_bvh_params_t;

	/**
	 * @brief KD Tree accelerator parameters.
	 * @ingroup accelerator-public
	 */
	typedef struct tpbrt_accelerator_kd_tree_params_t {
		/// @brief The value of the cost function that estimates the expected cost of performing a ray-object intersection, for use in building the kd-tree.
		tpbrt_uint_t intersect_cost;
		/// @brief Estimated cost for traversing a ray through a kd-tree node.
		tpbrt_uint_t traversal_cost;
		/// @brief "Bonus" factor for kd-tree nodes that represent empty space.
		tpbrt_float_t empty_bonus;
		/// @brief Maximum number of primitives to store in kd-tree node. (Not a hard limit; more may be stored if the kd-tree can't find splitting planes that reduce the number of primitives when refining a node.)
		tpbrt_uint_t max_prims;
		/// @brief Maximum depth of the kd-tree. If negative, the kd-tree chooses a maximum depth based on the number of primitives to be stored in it.
		tpbrt_int_t max_depth;
	} tpbrt_accelerator_kd_tree_params_t;

	/**
	 * @brief PBRT Accelerator representation.
	 * @ingroup accelerator-public
	 */
	typedef struct tpbrt_accelerator_t {
		/// @brief Initialized value dependent on current type.
		union {
			/// @brief BVH accelerator parameters.
			tpbrt_accelerator_bvh_params_t bvh;
			/// @brief KD Tree accelerator parameters.
			tpbrt_accelerator_kd_tree_params_t kd_tree;
		} as;

		/// @brief Accelerator type.
		tpbrt_accelerator_type_t type : 3;
	} tpbrt_accelerator_t;

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_ACCELERATOR_H_
