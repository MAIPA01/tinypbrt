#pragma once
#ifndef _TINYPBRT_ACCELERATOR_H_
#define _TINYPBRT_ACCELERATOR_H_

#include <tinypbrt/detail/fwd.h>

#ifdef __cplusplus
extern "C" {
#endif

	typedef enum tpbrt_accelerator_type_t : uint8_t {
		TPBRT_ACCELERATOR_TYPE_BVH	   = 0,
		TPBRT_ACCELERATOR_TYPE_KD_TREE = 1,
		TPBRT_ACCELERATOR_TYPE_MAX_NUM = 2,
	} tpbrt_accelerator_type_t;

	typedef enum tpbrt_accelerator_bvh_split_method_t : uint8_t {
		TPBRT_ACCELERATOR_BVH_SPLIT_METHOD_SAH	   = 0,
		TPBRT_ACCELERATOR_BVH_SPLIT_METHOD_MIDDLE  = 1,
		TPBRT_ACCELERATOR_BVH_SPLIT_METHOD_EQUAL   = 2,
		TPBRT_ACCELERATOR_BVH_SPLIT_METHOD_HLBVH   = 3,
		TPBRT_ACCELERATOR_BVH_SPLIT_METHOD_MAX_NUM = 4,
	} tpbrt_accelerator_bvh_split_method_t;

	typedef struct tpbrt_accelerator_bvh_params_t {
		tpbrt_uint_t max_node_prims;
		tpbrt_accelerator_bvh_split_method_t split_method;
	} tpbrt_accelerator_bvh_params_t;

	typedef struct tpbrt_accelerator_kd_tree_params_t {
		tpbrt_uint_t intersect_cost;
		tpbrt_uint_t traversal_cost;
		tpbrt_float_t empty_bonus;
		tpbrt_uint_t max_prims;
		tpbrt_int_t max_depth;
	} tpbrt_accelerator_kd_tree_params_t;

	typedef struct tpbrt_accelerator_t {
		tpbrt_accelerator_type_t type;

		union {
			tpbrt_accelerator_bvh_params_t bvh;
			tpbrt_accelerator_kd_tree_params_t kd_tree;
		} as;
	} tpbrt_accelerator_t;

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_ACCELERATOR_H_
