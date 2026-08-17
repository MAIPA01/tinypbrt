#pragma once
#ifndef _TINYPBRT_ACCELERATOR_H_
#define _TINYPBRT_ACCELERATOR_H_

#include "../param.h"

#ifdef __cplusplus
extern "C" {
#endif

	typedef enum : uint8_t {
		TPBRT_ACCELERATOR_BVH	  = 0,
		TPBRT_ACCELERATOR_KD_TREE = 1,
	} tpbrt_accelerator_type_t;

	typedef enum : uint8_t {
		TPBRT_ACCELERATOR_BVH_SPLIT_METHOD_SAH	  = 0,
		TPBRT_ACCELERATOR_BVH_SPLIT_METHOD_MIDDLE = 1,
		TPBRT_ACCELERATOR_BVH_SPLIT_METHOD_EQUAL  = 2,
		TPBRT_ACCELERATOR_BVH_SPLIT_METHOD_HLBVH  = 3,
	} tpbrt_accelerator_bvh_split_method_t;

	typedef struct {
		uint32_t max_node_prims;
		tpbrt_accelerator_bvh_split_method_t split_method;
	} tpbrt_accelerator_bvh_params_t;

	typedef struct {
		uint32_t intersect_cost;
		uint32_t traversal_cost;
		float empty_bonus;
		uint32_t max_prims;
		int32_t max_depth;
	} tpbrt_accelerator_kd_tree_params_t;

	typedef struct {
		tpbrt_accelerator_type_t type;

		union {
			tpbrt_accelerator_bvh_params_t bvh_params;
			tpbrt_accelerator_kd_tree_params_t kd_tree_params;
		};
	} tpbrt_accelerator_t;

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_ACCELERATOR_H_
