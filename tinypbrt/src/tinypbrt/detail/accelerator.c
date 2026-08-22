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
					if (type_str->size == TYPES_STRS[t].size &&
						strncmp(type_str->data, TYPES_STRS[t].data, TYPES_STRS[t].size) == 0) {
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
					if (method_str->size == METHODS_STRS[m].size &&
						strncmp(method_str->data, METHODS_STRS[m].data, METHODS_STRS[m].size) == 0) {
						*method = m;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_UNKNOWN_ACCELERATOR_BVH_SPLIT_METHOD;
	}

	tpbrt_error_t tpbrt_create_accelerator(const tpbrt_string_t* type_str, const tpbrt_params_list_t* params,
	  tpbrt_accelerator_t** accelerator) {
			if (type_str == TPBRT_NULL || type_str->data == TPBRT_NULL || params == TPBRT_NULL || accelerator == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

		*accelerator = malloc(sizeof(tpbrt_accelerator_t));
			if (*accelerator == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }

		tpbrt_error_t err = tpbrt_accelerator_type_from_string(type_str, &(*accelerator)->type);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_free_accelerator(accelerator);
				return err;
			}

			switch ((*accelerator)->type) {
			default:
				case TPBRT_ACCELERATOR_TYPE_BVH: {
					static const tpbrt_string_t MAX_NODES_PRIMS_STR = TPBRT_STRING("maxnodeprims");
					static const tpbrt_string_t SPLIT_METHOD_STR	= TPBRT_STRING("splitmethod");

					err =
					  tpbrt_params_list_get_uint(params, &MAX_NODES_PRIMS_STR, 4u, &(*accelerator)->as.bvh.max_node_prims);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_accelerator(accelerator);
							return err;
						}

					tpbrt_string_t method_str;
					err = tpbrt_params_list_get_string(params, &SPLIT_METHOD_STR, &method_str);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_accelerator(accelerator);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							(*accelerator)->as.bvh.split_method = TPBRT_ACCELERATOR_BVH_SPLIT_METHOD_SAH;
						}
						else {
							err = tpbrt_accelerator_bvh_split_method_from_string(&method_str,
							  &(*accelerator)->as.bvh.split_method);
							free(method_str.data);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_accelerator(accelerator);
									return err;
								}
						}
					break;
				}
				case TPBRT_ACCELERATOR_TYPE_KD_TREE: {
					static const tpbrt_string_t INTERSECT_COST_STR	 = TPBRT_STRING("intersectcost");
					static const tpbrt_string_t TRAVERSAL_COST_STR	 = TPBRT_STRING("traversalcost");
					static const tpbrt_string_t EMPTY_BONUS_STR		 = TPBRT_STRING("emptybonus");
					static const tpbrt_string_t MAX_PRIMS_STR		 = TPBRT_STRING("maxprims");
					static const tpbrt_string_t MAX_DEPTH_STR		 = TPBRT_STRING("maxdepth");

					static const tpbrt_uint_t INTERSECT_COST_DEFAULT = 5u;
					err = tpbrt_params_list_get_uint(params, &INTERSECT_COST_STR, INTERSECT_COST_DEFAULT,
					  &(*accelerator)->as.kd_tree.intersect_cost);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_accelerator(accelerator);
							return err;
						}

					err =
					  tpbrt_params_list_get_uint(params, &TRAVERSAL_COST_STR, 1u, &(*accelerator)->as.kd_tree.traversal_cost);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_accelerator(accelerator);
							return err;
						}

					static const tpbrt_float_t EMPTY_BONUS_DEFAULT = 0.5f;
					err = tpbrt_params_list_get_float(params, &EMPTY_BONUS_STR, EMPTY_BONUS_DEFAULT,
					  &(*accelerator)->as.kd_tree.empty_bonus);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_accelerator(accelerator);
							return err;
						}

					err = tpbrt_params_list_get_uint(params, &MAX_PRIMS_STR, 1u, &(*accelerator)->as.kd_tree.max_prims);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_accelerator(accelerator);
							return err;
						}

					err = tpbrt_params_list_get_int(params, &MAX_DEPTH_STR, -1, &(*accelerator)->as.kd_tree.max_depth);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_accelerator(accelerator);
							return err;
						}
					break;
				}
			}

		return TPBRT_ERROR_NONE;
	}

	void tpbrt_free_accelerator(tpbrt_accelerator_t** accelerator) {
			if (accelerator == TPBRT_NULL || *accelerator == TPBRT_NULL) { return; }

		free(*accelerator);
		*accelerator = TPBRT_NULL;
	}

#ifdef __cplusplus
}
#endif