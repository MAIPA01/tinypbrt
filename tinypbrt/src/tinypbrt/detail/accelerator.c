#include <tinypbrt/pch.h>

#include <tinypbrt/detail/accelerator_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

	static tpbrt_error_t tpbrt_accelerator_type_from_string(const tpbrt_string_t* const type_str,
	  tpbrt_accelerator_type_t* const type) {
		static const tpbrt_string_t TYPES_STRS[TPBRT_ACCELERATOR_TYPE_MAX_NUM] = {
			{ .chars = "bvh",	  .size = 3 },
			{ .chars = "kdtree", .size = 6 },
		};

			if (type_str == TPBRT_NULL || type_str->chars == TPBRT_NULL || type == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

			for (tpbrt_accelerator_type_t t = 0; t < TPBRT_ACCELERATOR_TYPE_MAX_NUM; ++t) {
					if (type_str->size == TYPES_STRS[t].size &&
						strncmp(type_str->chars, TYPES_STRS[t].chars, TYPES_STRS[t].size) == 0) {
						*type = t;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_UNKNOWN_ACCELERATOR_TYPE;
	}

	static tpbrt_error_t tpbrt_accelerator_bvh_split_method_from_string(const tpbrt_string_t* const method_str,
	  tpbrt_accelerator_bvh_split_method_t* const method) {
		static const tpbrt_string_t METHODS_STRS[TPBRT_ACCELERATOR_BVH_SPLIT_METHOD_MAX_NUM] = {
			{ .chars = "sah",	  .size = 3 },
			{ .chars = "middle", .size = 6 },
			{ .chars = "equal",	.size = 5 },
			{ .chars = "hlbvh",	.size = 5 },
		};

			if (method_str == TPBRT_NULL || method_str->chars == TPBRT_NULL || method == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

			for (tpbrt_accelerator_bvh_split_method_t m = 0; m < TPBRT_ACCELERATOR_BVH_SPLIT_METHOD_MAX_NUM; ++m) {
					if (method_str->size == METHODS_STRS[m].size &&
						strncmp(method_str->chars, METHODS_STRS[m].chars, METHODS_STRS[m].size) == 0) {
						*method = m;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_UNKNOWN_ACCELERATOR_BVH_SPLIT_METHOD;
	}

	tpbrt_error_t tpbrt_create_accelerator(const tpbrt_string_t* type_str, const tpbrt_params_list_t* params,
	  tpbrt_accelerator_t** accelerator) {
			if (type_str == TPBRT_NULL || type_str->chars == TPBRT_NULL || params == TPBRT_NULL || accelerator == TPBRT_NULL) {
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
					static const tpbrt_string_t MAX_NODES_PRIMS_STR = { .chars = "maxnodeprims", .size = 12 };
					static const tpbrt_string_t SPLIT_METHOD_STR	= { .chars = "splitmethod", .size = 11 };

					err =
					  tpbrt_params_list_get_uint(params, &MAX_NODES_PRIMS_STR, 4u, &(*accelerator)->bvh_params.max_node_prims);
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
							(*accelerator)->bvh_params.split_method = TPBRT_ACCELERATOR_BVH_SPLIT_METHOD_SAH;
						}
						else {
							err = tpbrt_accelerator_bvh_split_method_from_string(&method_str,
							  &(*accelerator)->bvh_params.split_method);
							free(method_str.chars);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_accelerator(accelerator);
									return err;
								}
						}
					break;
				}
				case TPBRT_ACCELERATOR_TYPE_KD_TREE: {
					static const tpbrt_string_t INTERSECT_COST_STR	 = { .chars = "intersectcost", .size = 13 };
					static const tpbrt_string_t TRAVERSAL_COST_STR	 = { .chars = "traversalcost", .size = 13 };
					static const tpbrt_string_t EMPTY_BONUS_STR		 = { .chars = "emptybonus", .size = 10 };
					static const tpbrt_string_t MAX_PRIMS_STR		 = { .chars = "maxprims", .size = 8 };
					static const tpbrt_string_t MAX_DEPTH_STR		 = { .chars = "maxdepth", .size = 8 };

					static const tpbrt_uint_t INTERSECT_COST_DEFAULT = 5u;
					err = tpbrt_params_list_get_uint(params, &INTERSECT_COST_STR, INTERSECT_COST_DEFAULT,
					  &(*accelerator)->kd_tree_params.intersect_cost);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_accelerator(accelerator);
							return err;
						}

					err =
					  tpbrt_params_list_get_uint(params, &TRAVERSAL_COST_STR, 1u, &(*accelerator)->kd_tree_params.traversal_cost);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_accelerator(accelerator);
							return err;
						}

					static const tpbrt_float_t EMPTY_BONUS_DEFAULT = 0.5f;
					err = tpbrt_params_list_get_float(params, &EMPTY_BONUS_STR, EMPTY_BONUS_DEFAULT,
					  &(*accelerator)->kd_tree_params.empty_bonus);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_accelerator(accelerator);
							return err;
						}

					err = tpbrt_params_list_get_uint(params, &MAX_PRIMS_STR, 1u, &(*accelerator)->kd_tree_params.max_prims);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_accelerator(accelerator);
							return err;
						}

					err = tpbrt_params_list_get_int(params, &MAX_DEPTH_STR, -1, &(*accelerator)->kd_tree_params.max_depth);
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