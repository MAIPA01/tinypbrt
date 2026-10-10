#include <tinypbrt/pch.h>

#include <tinypbrt/detail/accelerator_internal.h>

#include <tinypbrt/detail/error.h>

#include <tinypbrt/detail/common_internal.h>
#include <tinypbrt/detail/param_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

	tpbrt_error_t tpbrt_accelerator_type_from_string(const tpbrt_string_t type_str, tpbrt_accelerator_type_t* const type) {
		static const tpbrt_string_t TYPES_STRS[TPBRT_ACCELERATOR_TYPE_MAX_NUM] = {
			TPBRT_STRING("bvh"),
			TPBRT_STRING("kdtree"),
		};

			if (type == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (tpbrt_string_empty(type_str)) { return TPBRT_ERROR_INVALID_ACCELERATOR_TYPE; }

			for (tpbrt_accelerator_type_t t = 0; t < TPBRT_ACCELERATOR_TYPE_MAX_NUM; ++t) {
					if (tpbrt_string_equals(type_str, TYPES_STRS[t])) {
						*type = t;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_INVALID_ACCELERATOR_TYPE;
	}

	tpbrt_error_t tpbrt_accelerator_bvh_split_method_from_string(const tpbrt_string_t method_str,
	  tpbrt_accelerator_bvh_split_method_t* const method) {
		static const tpbrt_string_t METHODS_STRS[TPBRT_ACCELERATOR_BVH_SPLIT_METHOD_MAX_NUM] = {
			TPBRT_STRING("sah"),
			TPBRT_STRING("middle"),
			TPBRT_STRING("equal"),
			TPBRT_STRING("hlbvh"),
		};

			if (method == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (tpbrt_string_empty(method_str)) { return TPBRT_ERROR_INVALID_ACCELERATOR_BVH_SPLIT_METHOD; }

			for (tpbrt_accelerator_bvh_split_method_t m = 0; m < TPBRT_ACCELERATOR_BVH_SPLIT_METHOD_MAX_NUM; ++m) {
					if (tpbrt_string_equals(method_str, METHODS_STRS[m])) {
						*method = m;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_INVALID_ACCELERATOR_BVH_SPLIT_METHOD;
	}

	static tpbrt_error_t tpbrt_accelerator_read_bvh(tpbrt_accelerator_bvh_params_t* const bvh_params,
	  const tpbrt_params_list_t params) {
			if (bvh_params == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t MAX_NODES_PRIMS_STR = TPBRT_STRING("maxnodeprims");
		static const tpbrt_string_t SPLIT_METHOD_STR	= TPBRT_STRING("splitmethod");

		tpbrt_error_t err = tpbrt_params_list_get_uint(params, MAX_NODES_PRIMS_STR, 4u, &bvh_params->max_node_prims);
			if (err != TPBRT_ERROR_NONE) { return err; }

		tpbrt_string_t method_str;
		err = tpbrt_params_list_get_string(params, SPLIT_METHOD_STR, &method_str);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

			if (err == TPBRT_ERROR_NOT_FOUND) {
				bvh_params->split_method = TPBRT_ACCELERATOR_BVH_SPLIT_METHOD_SAH;
				return TPBRT_ERROR_NONE;
			}

		tpbrt_accelerator_bvh_split_method_t split_method;
		err = tpbrt_accelerator_bvh_split_method_from_string(method_str, &split_method);
		tpbrt_string_free(&method_str);
			if (err != TPBRT_ERROR_NONE) { return err; }

		bvh_params->split_method = split_method;
		return TPBRT_ERROR_NONE;
	}

	static tpbrt_error_t tpbrt_accelerator_read_kd_tree(tpbrt_accelerator_kd_tree_params_t* const kd_tree_params,
	  const tpbrt_params_list_t params) {
			if (kd_tree_params == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t INTERSECT_COST_STR	 = TPBRT_STRING("intersectcost");
		static const tpbrt_string_t TRAVERSAL_COST_STR	 = TPBRT_STRING("traversalcost");
		static const tpbrt_string_t EMPTY_BONUS_STR		 = TPBRT_STRING("emptybonus");
		static const tpbrt_string_t MAX_PRIMS_STR		 = TPBRT_STRING("maxprims");
		static const tpbrt_string_t MAX_DEPTH_STR		 = TPBRT_STRING("maxdepth");

		static const tpbrt_uint_t INTERSECT_COST_DEFAULT = 5u;
		tpbrt_error_t err =
		  tpbrt_params_list_get_uint(params, INTERSECT_COST_STR, INTERSECT_COST_DEFAULT, &kd_tree_params->intersect_cost);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_uint(params, TRAVERSAL_COST_STR, 1u, &kd_tree_params->traversal_cost);
			if (err != TPBRT_ERROR_NONE) { return err; }

		static const tpbrt_float_t EMPTY_BONUS_DEFAULT = 0.5f;
		err = tpbrt_params_list_get_float(params, EMPTY_BONUS_STR, EMPTY_BONUS_DEFAULT, &kd_tree_params->empty_bonus);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_uint(params, MAX_PRIMS_STR, 1u, &kd_tree_params->max_prims);
			if (err != TPBRT_ERROR_NONE) { return err; }

		return tpbrt_params_list_get_int(params, MAX_DEPTH_STR, -1, &kd_tree_params->max_depth);
	}

	tpbrt_error_t tpbrt_accelerator_init(const tpbrt_string_t type_str, const tpbrt_params_list_t params,
	  tpbrt_accelerator_t* const accelerator) {
			if (accelerator == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (memset(accelerator, 0, sizeof(tpbrt_accelerator_t)) == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		tpbrt_accelerator_type_t type;
		tpbrt_error_t err = tpbrt_accelerator_type_from_string(type_str, &type);
			if (err != TPBRT_ERROR_NONE) { return err; }
		accelerator->type = type;

			switch (accelerator->type) {
			default:
				case TPBRT_ACCELERATOR_TYPE_BVH: {
					err = tpbrt_accelerator_read_bvh(&accelerator->as.bvh, params);
					break;
				}
				case TPBRT_ACCELERATOR_TYPE_KD_TREE: {
					err = tpbrt_accelerator_read_kd_tree(&accelerator->as.kd_tree, params);
					break;
				}
			}

		return err;
	}

#ifdef __cplusplus
}
#endif