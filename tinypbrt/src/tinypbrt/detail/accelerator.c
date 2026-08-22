#include <tinypbrt/pch.h>

#include <tinypbrt/detail/accelerator_internal.h>

#include <tinypbrt/detail/common_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

	static tpbrt_error_t tpbrt_accelerator_type_from_string(const tpbrt_string_t* const type_str,
	  tpbrt_accelerator_type_t* const type) {
		static const tpbrt_string_t TYPES_STRS[TPBRT_ACCELERATOR_TYPE_MAX_NUM] = {
			TPBRT_STRING("bvh"),
			TPBRT_STRING("kdtree"),
		};

			if (type_str == TPBRT_NULL || type_str->data == TPBRT_NULL || type == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

			for (tpbrt_accelerator_type_t t = 0; t < TPBRT_ACCELERATOR_TYPE_MAX_NUM; ++t) {
					if (tpbrt_string_equals(type_str, TYPES_STRS + t)) {
						*type = t;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_UNKNOWN_ACCELERATOR_TYPE;
	}

	static tpbrt_error_t tpbrt_accelerator_bvh_split_method_from_string(const tpbrt_string_t* const method_str,
	  tpbrt_accelerator_bvh_split_method_t* const method) {
		static const tpbrt_string_t METHODS_STRS[TPBRT_ACCELERATOR_BVH_SPLIT_METHOD_MAX_NUM] = {
			TPBRT_STRING("sah"),
			TPBRT_STRING("middle"),
			TPBRT_STRING("equal"),
			TPBRT_STRING("hlbvh"),
		};

			if (method_str == TPBRT_NULL || method_str->data == TPBRT_NULL || method == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

			for (tpbrt_accelerator_bvh_split_method_t m = 0; m < TPBRT_ACCELERATOR_BVH_SPLIT_METHOD_MAX_NUM; ++m) {
					if (tpbrt_string_equals(method_str, METHODS_STRS + m)) {
						*method = m;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_UNKNOWN_ACCELERATOR_BVH_SPLIT_METHOD;
	}

	tpbrt_error_t tpbrt_create_accelerator(const tpbrt_string_t* type_str, const tpbrt_params_list_t* params,
	  tpbrt_accelerator_t* accelerator) {
			if (type_str == TPBRT_NULL || type_str->data == TPBRT_NULL || params == TPBRT_NULL || accelerator == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

		tpbrt_error_t err = tpbrt_accelerator_type_from_string(type_str, &accelerator->type);
			if (err != TPBRT_ERROR_NONE) { return err; }

			switch (accelerator->type) {
			default:
				case TPBRT_ACCELERATOR_TYPE_BVH: {
					static const tpbrt_string_t MAX_NODES_PRIMS_STR = TPBRT_STRING("maxnodeprims");
					static const tpbrt_string_t SPLIT_METHOD_STR	= TPBRT_STRING("splitmethod");

					tpbrt_accelerator_bvh_params_t* acc_params		= &accelerator->as.bvh;

					err = tpbrt_params_list_get_uint(params, &MAX_NODES_PRIMS_STR, 4u, &acc_params->max_node_prims);
						if (err != TPBRT_ERROR_NONE) { return err; }

					tpbrt_string_t method_str;
					err = tpbrt_params_list_get_string(params, &SPLIT_METHOD_STR, &method_str);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

						if (err == TPBRT_ERROR_NOT_FOUND) { acc_params->split_method = TPBRT_ACCELERATOR_BVH_SPLIT_METHOD_SAH; }
						else {
							err = tpbrt_accelerator_bvh_split_method_from_string(&method_str, &acc_params->split_method);
							tpbrt_free_string(&method_str);
								if (err != TPBRT_ERROR_NONE) { return err; }
						}
					break;
				}
				case TPBRT_ACCELERATOR_TYPE_KD_TREE: {
					static const tpbrt_string_t INTERSECT_COST_STR	 = TPBRT_STRING("intersectcost");
					static const tpbrt_string_t TRAVERSAL_COST_STR	 = TPBRT_STRING("traversalcost");
					static const tpbrt_string_t EMPTY_BONUS_STR		 = TPBRT_STRING("emptybonus");
					static const tpbrt_string_t MAX_PRIMS_STR		 = TPBRT_STRING("maxprims");
					static const tpbrt_string_t MAX_DEPTH_STR		 = TPBRT_STRING("maxdepth");

					tpbrt_accelerator_kd_tree_params_t* acc_params	 = &accelerator->as.kd_tree;

					static const tpbrt_uint_t INTERSECT_COST_DEFAULT = 5u;
					err = tpbrt_params_list_get_uint(params, &INTERSECT_COST_STR, INTERSECT_COST_DEFAULT,
					  &acc_params->intersect_cost);
						if (err != TPBRT_ERROR_NONE) { return err; }

					err = tpbrt_params_list_get_uint(params, &TRAVERSAL_COST_STR, 1u, &acc_params->traversal_cost);
						if (err != TPBRT_ERROR_NONE) { return err; }

					static const tpbrt_float_t EMPTY_BONUS_DEFAULT = 0.5f;
					err = tpbrt_params_list_get_float(params, &EMPTY_BONUS_STR, EMPTY_BONUS_DEFAULT, &acc_params->empty_bonus);
						if (err != TPBRT_ERROR_NONE) { return err; }

					err = tpbrt_params_list_get_uint(params, &MAX_PRIMS_STR, 1u, &acc_params->max_prims);
						if (err != TPBRT_ERROR_NONE) { return err; }

					err = tpbrt_params_list_get_int(params, &MAX_DEPTH_STR, -1, &acc_params->max_depth);
						if (err != TPBRT_ERROR_NONE) { return err; }
					break;
				}
			}

		return TPBRT_ERROR_NONE;
	}

#ifdef __cplusplus
}
#endif